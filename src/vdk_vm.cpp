#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

namespace {
std::wstring quote(const std::wstring& value) {
    std::wstring out = L"\"";
    unsigned int slashes = 0;
    for (wchar_t ch : value) {
        if (ch == L'\\') {
            ++slashes;
        } else if (ch == L'"') {
            out.append(slashes * 2 + 1, L'\\');
            out.push_back(L'"');
            slashes = 0;
        } else {
            out.append(slashes, L'\\');
            slashes = 0;
            out.push_back(ch);
        }
    }
    out.append(slashes * 2, L'\\');
    out.push_back(L'"');
    return out;
}

bool fileExists(const std::wstring& path) {
    const DWORD attrs = GetFileAttributesW(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES &&
           (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

void usage() {
    std::wcout
        << L"VDaniKex VM Launcher (experimental prototype)\n\n"
        << L"Usage:\n"
        << L"  vdk-vm.exe --qemu <qemu-system-i386.exe> --iso <installer.iso> [--disk <disk-image>] [--memory <MB>]\n\n"
        << L"Example:\n"
        << L"  vdk-vm.exe --qemu C:\\QEMU\\qemu-system-i386.exe --iso C:\\ISO\\old-windows.iso --memory 256\n\n"
        << L"QEMU must be installed separately. This tool does not download OS images.\n"
        << L"Network access for the guest is disabled by default. Use legally obtained media.\n";
}

int fail(const wchar_t* message, int code) {
    std::wcerr << L"VDaniKex VM Launcher: " << message << L"\n";
    return code;
}
}

int wmain(int argc, wchar_t** argv) {
    if (argc == 2 && (std::wstring(argv[1]) == L"--help" ||
                      std::wstring(argv[1]) == L"/?")) {
        usage();
        return 0;
    }
    if (argc == 2 && std::wstring(argv[1]) == L"--self-test") {
        std::wcout << L"vdk-vm self-test passed (argument entry point available).\n";
        return 0;
    }

    std::wstring qemu, iso, disk;
    int memory = 256;
    for (int i = 1; i < argc; ++i) {
        const std::wstring arg(argv[i]);
        if ((arg == L"--qemu" || arg == L"--iso" || arg == L"--disk" || arg == L"--memory") && i + 1 < argc) {
            const std::wstring value(argv[++i]);
            if (arg == L"--qemu") qemu = value;
            else if (arg == L"--iso") iso = value;
            else if (arg == L"--disk") disk = value;
            else {
                wchar_t* end = nullptr;
                const long parsed = wcstol(value.c_str(), &end, 10);
                if (end == value.c_str() || *end != L'\0' || parsed < 32 || parsed > 4096) {
                    return fail(L"--memory must be an integer from 32 to 4096 MB.", 2);
                }
                memory = static_cast<int>(parsed);
            }
        } else {
            usage();
            return fail(L"Unknown option or missing option value.", 2);
        }
    }

    if (qemu.empty() || iso.empty()) {
        usage();
        return fail(L"Both --qemu and --iso are required.", 2);
    }
    if (!fileExists(qemu)) return fail(L"QEMU executable was not found. Check --qemu path.", 3);
    if (!fileExists(iso)) return fail(L"ISO image was not found. Check --iso path.", 4);
    if (!disk.empty() && !fileExists(disk)) return fail(L"Disk image was not found. Check --disk path.", 5);

    std::wstring command = quote(qemu) + L" -machine pc -m " + std::to_wstring(memory) +
                           L" -vga std -net none -boot order=d -cdrom " + quote(iso);
    if (!disk.empty()) command += L" -hda " + quote(disk);

    STARTUPINFOW startup = {};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process = {};
    std::vector<wchar_t> mutableCommand(command.begin(), command.end());
    mutableCommand.push_back(L'\0');

    if (!CreateProcessW(qemu.c_str(), mutableCommand.data(), nullptr, nullptr, FALSE,
                        CREATE_NEW_PROCESS_GROUP, nullptr, nullptr, &startup, &process)) {
        const DWORD error = GetLastError();
        std::wcerr << L"CreateProcess failed with Windows error " << error << L".\n";
        return 6;
    }

    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    std::wcout << L"QEMU started. The guest is isolated from networking by default.\n";
    return 0;
}
