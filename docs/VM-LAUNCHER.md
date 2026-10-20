# VDaniKex VM Launcher (experimental)

## What this milestone does

`vdk-vm` is a small launcher for an external QEMU emulator. It can start an x86 virtual machine from a user-supplied ISO, optionally attach an existing disk image, and set guest RAM. It does not emulate Windows by itself and does not include QEMU or any operating-system image.

## Requirements

- A compatible QEMU Windows build installed separately. Older Windows hosts may require an older QEMU release; the host OS and QEMU build must be checked independently.
- An ISO and optional disk image that the user is legally entitled to use.
- Enough free memory and disk space for the guest.

## Usage

```powershell
.\vdk-vm.exe --help
.\vdk-vm.exe --qemu "C:\QEMU\qemu-system-i386.exe" --iso "C:\ISO\installer.iso" --memory 256
.\vdk-vm.exe --qemu "C:\QEMU\qemu-system-i386.exe" --iso "C:\ISO\installer.iso" --disk "C:\VM\legacy.img" --memory 512
```

RAM must be between 32 and 4096 MB. The guest network is disabled using QEMU's `-net none` option. If a disk image is supplied, QEMU opens that existing image; the launcher does not create or format it. Back up important data and use a disposable VM disk.

## Validation and limitations

- `--self-test` validates that the launcher starts and its basic command entry point is reachable. It does not start QEMU.
- CI builds the launcher for x86 and x64 and runs the self-test on the CI host.
- A green build does not prove QEMU works on every Windows version, nor does it prove a guest OS boots. Runtime validation must be performed on each supported host using a compatible QEMU release and test media.
- This is a launcher prototype, not a complete emulator and not a Windows API compatibility layer.
- Windows 7–11 support is a target for validation, not a claim that every QEMU release runs on every one of those systems.
