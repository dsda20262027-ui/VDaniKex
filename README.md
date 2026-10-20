# VDaniKex

**VDaniKex** is an experimental, user-mode Windows API compatibility project targeting Windows 7 SP1 and Windows 8.1.

> Status: early scaffold only. This repository does not yet implement a general-purpose compatibility layer and will not make arbitrary Windows 10/11 applications run on Windows 7/8.1.

## Goals

- Provide an auditable, modular user-mode API shim.
- Detect the host OS without changing system files.
- Add individual API implementations only when their semantics can be tested.
- Keep compatibility opt-in per test application; do not install global hooks or alter Image File Execution Options automatically.
- Build and test on clean virtual machines for Windows 7 SP1 and Windows 8.1.

## Initial modules

- `VDaniKexCore`: small DLL with version and diagnostic exports.
- `vdk-probe`: command-line OS diagnostic utility.
- `docs/ARCHITECTURE.md`: planned design and milestones.

## Build

Use a compatible Visual Studio/Windows SDK toolchain or equivalent. Windows 7/8.1 are legacy systems; validate toolchain and runtime requirements before deployment.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The initial scaffold is deliberately small and does not install itself or modify registry settings.

## Safety and scope

- Do not test on a machine containing important data; use snapshots in a virtual machine.
- Do not load an experimental shim into critical system processes.
- Compatibility functions must be implemented and tested individually. Returning a fake success value for an unsupported API can corrupt application state.
- Windows 7 and Windows 8.1 have different API baselines; feature support must be capability-based, not assumed from a shared version number.

## License

MIT for the original VDaniKex scaffold. If code from other projects is ever incorporated, its original license and attribution must be preserved. This scaffold does not copy VxKex source code.
