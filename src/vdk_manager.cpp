#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <string>
#include <vector>

namespace {
const wchar_t* kClassName = L"VDaniKexManagerWindow";
HWND g_list = nullptr;
HWND g_status = nullptr;
std::wstring g_qemu;
struct Machine { std::wstring name, iso; int memory; };
std::vector<Machine> g_machines;

std::wstring chooseFile(HWND owner, const wchar_t* filter, const wchar_t* title) {
    wchar_t path[MAX_PATH] = {};
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    return GetOpenFileNameW(&ofn) ? std::wstring(path) : std::wstring();
}
void setStatus(const std::wstring& value) {
    if (g_status) SetWindowTextW(g_status, value.c_str());
}
void refreshList() {
    SendMessageW(g_list, LB_RESETCONTENT, 0, 0);
    for (const auto& machine : g_machines)
        SendMessageW(g_list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(machine.name.c_str()));
}
void addMachine(HWND hwnd) {
    if (g_qemu.empty()) {
        g_qemu = chooseFile(hwnd, L"QEMU executable\0qemu-system-*.exe\0Executable files\0*.exe\0All files\0*.*\0\0", L"Select QEMU executable");
        if (g_qemu.empty()) return;
    }
    std::wstring iso = chooseFile(hwnd, L"ISO images\0*.iso\0All files\0*.*\0\0", L"Select operating system ISO");
    if (iso.empty()) return;
    Machine m;
    m.name = L"Virtual Machine " + std::to_wstring(g_machines.size() + 1);
    m.iso = iso;
    m.memory = 512;
    g_machines.push_back(m);
    refreshList();
    SendMessageW(g_list, LB_SETCURSEL, g_machines.size() - 1, 0);
    setStatus(L"Machine added to this session. ISO: " + iso);
}
void launchSelected(HWND hwnd) {
    LRESULT index = SendMessageW(g_list, LB_GETCURSEL, 0, 0);
    if (index == LB_ERR || static_cast<size_t>(index) >= g_machines.size()) {
        MessageBoxW(hwnd, L"Select a virtual machine first.", L"VDaniKex", MB_OK | MB_ICONINFORMATION);
        return;
    }
    if (g_qemu.empty()) {
        g_qemu = chooseFile(hwnd, L"QEMU executable\0qemu-system-*.exe\0Executable files\0*.exe\0All files\0*.*\0\0", L"Select QEMU executable");
        if (g_qemu.empty()) return;
    }
    const Machine& m = g_machines[static_cast<size_t>(index)];
    std::wstring cmd = L"\"" + g_qemu + L"\" -machine pc -m " + std::to_wstring(m.memory) +
        L" -vga std -net none -boot order=d -cdrom \"" + m.iso + L"\"";
    STARTUPINFOW si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {};
    std::vector<wchar_t> mutableCmd(cmd.begin(), cmd.end());
    mutableCmd.push_back(L'\0');
    if (!CreateProcessW(g_qemu.c_str(), mutableCmd.data(), nullptr, nullptr, FALSE,
                        CREATE_NEW_PROCESS_GROUP, nullptr, nullptr, &si, &pi)) {
        MessageBoxW(hwnd, L"Could not start QEMU. Check the selected executable.", L"VDaniKex", MB_OK | MB_ICONERROR);
        return;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    setStatus(L"QEMU launched. Guest networking is disabled by default.");
}
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"VDaniKex Universal", WS_CHILD | WS_VISIBLE, 20, 14, 360, 30, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"STATIC", L"Virtual machines", WS_CHILD | WS_VISIBLE, 20, 55, 180, 22, hwnd, nullptr, nullptr, nullptr);
        g_list = CreateWindowW(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
                               20, 80, 310, 250, hwnd, reinterpret_cast<HMENU>(100), nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"New", WS_CHILD | WS_VISIBLE, 350, 80, 120, 32, hwnd, reinterpret_cast<HMENU>(101), nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"Settings / QEMU", WS_CHILD | WS_VISIBLE, 350, 122, 120, 32, hwnd, reinterpret_cast<HMENU>(102), nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE, 350, 164, 120, 32, hwnd, reinterpret_cast<HMENU>(103), nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"About", WS_CHILD | WS_VISIBLE, 350, 206, 120, 32, hwnd, reinterpret_cast<HMENU>(104), nullptr, nullptr);
        g_status = CreateWindowW(L"STATIC", L"Ready. Add a machine using a legally obtained ISO.", WS_CHILD | WS_VISIBLE,
                                 20, 345, 455, 42, hwnd, nullptr, nullptr, nullptr);
        return 0;
    }
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 101: addMachine(hwnd); return 0;
        case 102: {
            std::wstring path = chooseFile(hwnd, L"QEMU executable\0qemu-system-*.exe\0Executable files\0*.exe\0All files\0*.*\0\0", L"Select QEMU executable");
            if (!path.empty()) { g_qemu = path; setStatus(L"QEMU selected: " + path); }
            return 0;
        }
        case 103: launchSelected(hwnd); return 0;
        case 104:
            MessageBoxW(hwnd, L"VDaniKex Universal is an experimental manager using an external QEMU installation. Machine entries currently last only until this window closes. It is not VirtualBox and does not guarantee guest OS compatibility.", L"About VDaniKex", MB_OK | MB_ICONINFORMATION);
            return 0;
        }
        break;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
}
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = instance;
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    if (!RegisterClassW(&wc)) return 1;
    HWND hwnd = CreateWindowW(kClassName, L"VDaniKex Universal — VM Manager",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 510, 440,
        nullptr, nullptr, instance, nullptr);
    if (!hwnd) return 2;
    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
