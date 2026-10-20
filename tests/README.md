# Test plan

The initial scaffold has no automated Windows VM runner yet.

For each supported VM, record:

- OS edition, architecture, service pack and installed updates
- compiler/toolset and Windows SDK version
- whether `vdk-probe.exe` reports the expected NT version
- whether `VDaniKexCore.dll` exports `VDK_GetApiVersion` and `VDK_GetBuildLabel`
- exact command, output and exit code

Do not test experimental loader behavior against system processes. Start with a disposable test executable and a VM snapshot.
