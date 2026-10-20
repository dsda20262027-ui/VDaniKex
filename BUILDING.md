# Building VDaniKex and its installer

This repository currently provides a diagnostic scaffold and installer source, not a completed compatibility layer. Building an installer does not mean Windows 10/11 applications will run on Windows 7 or 8.1.

## Safety

- Build in a disposable development environment.
- Test on virtual machines with snapshots, not on a machine containing important data.
- The installer is per-user (PrivilegesRequired=lowest), does not request administrator rights, and does not register global DLL hooks or change system-wide compatibility settings.
- The installer adds the diagnostic DLL and probe utility only. It does not inject the DLL into other processes.
- Do not run unsigned experimental binaries from unknown sources. Review source and CI logs first.

## Recommended build environment

Use Visual Studio 2019 Build Tools with the C++ workload, CMake, and a Windows SDK/toolset compatible with the intended legacy OS. The GitHub Actions workflow uses a Windows Server 2019 runner and builds separate Win32 and x64 packages; this checks compilation on that runner, not runtime behavior on Windows 7 or 8.1.

Legacy operating systems may need specific servicing updates and runtime prerequisites. Confirm the chosen compiler's minimum supported OS and test on clean VMs before distributing binaries.

## Local build

Open a Developer PowerShell for Visual Studio and run one architecture at a time.

### x86

    cmake -S . -B build-x86 -A Win32
    cmake --build build-x86 --config Release
    & "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe" /DAppArch=x86 /DSourceDir=build-x86\Release /Odist installer\VDaniKex.iss

### x64

    cmake -S . -B build-x64 -A x64
    cmake --build build-x64 --config Release
    & "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe" /DAppArch=x64 /DSourceDir=build-x64\Release /Odist installer\VDaniKex.iss

The resulting installers should be written to dist/VDaniKex-Setup-x86.exe and dist/VDaniKex-Setup-x64.exe.

## GitHub Actions

1. Push the branch or open a pull request to main.
2. Open the repository's Actions tab.
3. Select Build VDaniKex installers and inspect both architecture jobs.
4. Download the VDaniKex-Setup-x86 and VDaniKex-Setup-x64 artifacts only after their jobs succeed.
5. The workflow retains artifacts for 14 days. It does not publish a GitHub Release automatically.

## Runtime validation checklist

On separate Windows 7 SP1 and Windows 8.1 virtual machines, for both applicable architectures:

- [ ] Confirm installer starts and installs under the current user without elevation.
- [ ] Confirm the Start Menu diagnostic shortcut opens.
- [ ] Record OS edition, architecture, service pack, installed updates, and exit code.
- [ ] Confirm uninstall removes the per-user installation.
- [ ] Verify no global hooks or system-wide registry changes were made.
- [ ] Keep the VM snapshot and logs so a failed test is reproducible.

A successful CI build proves that the sources compiled and the installer was packaged on the runner. It does not prove the installer or executable works on Windows 7/8.1; only runtime tests on those systems can establish that.
