#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <iostream>

/* RtlGetVersion reports the actual NT version and avoids legacy manifest
   virtualization associated with GetVersionEx on some Windows releases. */
typedef LONG (WINAPI *RtlGetVersionFn)(PRTL_OSVERSIONINFOW);

int wmain()
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (ntdll == nullptr) {
        std::wcerr << L"VDaniKex probe: ntdll.dll was not found.\n";
        return 2;
    }

    RtlGetVersionFn rtlGetVersion = reinterpret_cast<RtlGetVersionFn>(
        GetProcAddress(ntdll, "RtlGetVersion"));
    if (rtlGetVersion == nullptr) {
        std::wcerr << L"VDaniKex probe: RtlGetVersion is unavailable.\n";
        return 3;
    }

    RTL_OSVERSIONINFOW version = {};
    version.dwOSVersionInfoSize = sizeof(version);
    const LONG status = rtlGetVersion(&version);
    if (status != 0) {
        std::wcerr << L"VDaniKex probe: version query failed, NTSTATUS="
                   << status << L"\n";
        return 4;
    }

    std::wcout << L"VDaniKex host diagnostic\n"
               << L"NT version: " << version.dwMajorVersion << L'.'
               << version.dwMinorVersion << L'.' << version.dwBuildNumber << L"\n"
               << L"Service pack: " << version.szCSDVersion << L"\n";

    if (version.dwMajorVersion == 6 && version.dwMinorVersion == 1) {
        std::wcout << L"Detected Windows 7 family.\n";
    } else if (version.dwMajorVersion == 6 && version.dwMinorVersion == 3) {
        std::wcout << L"Detected Windows 8.1 family.\n";
    } else {
        std::wcout << L"This OS is outside the initial target set.\n";
    }

    std::wcout << L"This diagnostic does not enable compatibility or change system settings.\n";
    return 0;
}
