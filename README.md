# VDaniKex

**VDaniKex** is an experimental Windows diagnostics and virtual-machine launcher project.

> Status: early prototype. The VM launcher starts an externally installed QEMU emulator from a user-supplied ISO. It is not an emulator itself, does not bundle Windows, and does not implement a general-purpose Windows API compatibility layer.

## Current milestone

- VDaniKexCore: small diagnostic DLL.
- vdk-probe: command-line host OS diagnostic.
- vdk-vm: experimental launcher for external QEMU, with optional existing disk image and guest RAM setting.
- Per-user installer; no automatic system-wide hooks or registry changes.
- CI compiles x86 and x64 and runs basic executable smoke tests.

See docs/VM-LAUNCHER.md for requirements, command examples, and limitations. QEMU and legal installation media must be supplied separately. Guest networking is disabled by default.

## Build

On a Windows machine with CMake and a compatible Visual Studio C++ toolchain:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
.\build\Release\vdk-probe.exe
.\build\Release\vdk-vm.exe --self-test
```

## Validation scope

A green CI build proves that the sources compile on the CI runner, the simple smoke tests pass, and the installer is packaged. It does not prove that QEMU runs on every Windows version or that a guest operating system boots. Those require runtime tests on each target host with a compatible QEMU release and test media.

## Safety

- Use disposable virtual machines and snapshots for testing.
- Do not use a disk image containing important data.
- Do not load experimental DLLs into critical processes.
- Do not download operating-system images from untrusted sources.
- No unsupported API is faked as successful.
- This scaffold does not copy VxKex source code.

## License

MIT for the original VDaniKex scaffold. Preserve license and attribution if code from other projects is ever incorporated.
