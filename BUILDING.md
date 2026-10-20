# Building VDaniKex and its installer

This repository is an experimental diagnostic and VM-launcher prototype, not a completed Windows compatibility layer. The VM launcher invokes external QEMU; it does not emulate a guest by itself and does not include OS images.

## Safety

- Build in a disposable development environment.
- Use snapshots and disposable virtual disks for runtime testing.
- The installer is per-user and does not register global DLL hooks or change system-wide compatibility settings.
- QEMU and OS installation media are supplied separately.
- Review source and CI logs before running experimental binaries.

## Build locally

Use a compatible Visual Studio C++ toolchain and CMake. Run one architecture at a time.

### x86

    cmake -S . -B build-x86 -A Win32
    cmake --build build-x86 --config Release
    .\build-x86\Release\vdk-probe.exe
    .\build-x86\Release\vdk-vm.exe --self-test

### x64

    cmake -S . -B build-x64 -A x64
    cmake --build build-x64 --config Release
    .\build-x64\Release\vdk-probe.exe
    .\build-x64\Release\vdk-vm.exe --self-test

## GitHub Actions

1. Push a branch or open a pull request to main.
2. Open the repository's Actions tab.
3. Inspect both x86 and x64 jobs.
4. Download VDaniKex-Setup-x86 or VDaniKex-Setup-x64 only after the corresponding job succeeds.
5. Artifacts are retained for 14 days; this workflow does not publish a GitHub Release automatically.

## What CI validates

CI checks compilation on a Windows runner, runs the diagnostic and launcher entry-point smoke tests, and packages the per-user installer. The vdk-vm --self-test command does not start QEMU. CI does not prove compatibility on Windows 7, 8.1, 10, or 11, nor does it prove a guest OS boots. Runtime validation needs a compatible QEMU build on each host and a legally obtained ISO.

## Runtime testing

Use a disposable VM. Record the host Windows version/build, QEMU version, guest ISO source, architecture, exact command line, and observed result. If attaching --disk, use a newly created test disk or a backup; the launcher does not create or format disks.
