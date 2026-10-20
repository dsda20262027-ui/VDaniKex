# VDaniKex architecture and milestones

## Design principles

1. **User-mode first.** Do not introduce a kernel driver in the initial project.
2. **Opt-in.** A test app should load a shim explicitly. Avoid automatic global injection and registry changes during early development.
3. **Small, testable API surface.** Implement a single missing API at a time, with documented inputs, outputs, error behavior, and tests.
4. **No fake success.** Unsupported APIs should fail clearly rather than report success without performing the documented operation.
5. **Separate OS capability from OS version.** A Windows 7 machine with updates can differ from an unpatched installation.

## Planned components

- **VDaniKexCore.dll** — exported version/diagnostic interface; future API shim modules can be separated by API family.
- **vdk-probe.exe** — reads the host NT version and prints diagnostics; it does not install or activate anything.
- **Compatibility manifest** — future data describing an application, required API set, known limitations, and test status.
- **Test harness** — tiny programs that exercise each supported API and compare expected behavior on supported host versions.

## Milestones

### M0 — scaffold
- [x] CMake project and exported DLL diagnostics.
- [x] OS probe for Windows 7 / 8.1 family.
- [x] Safety and architecture documentation.

### M1 — build and test baseline
- [ ] Build with a legacy-compatible Windows SDK/toolset.
- [ ] Test in Windows 7 SP1 x86 and x64 VMs.
- [ ] Test in Windows 8.1 x86 and x64 VMs.
- [ ] Record runtime prerequisites and test results.

### M2 — first compatibility API
- [ ] Choose one concrete API required by a specific test application.
- [ ] Verify its documented behavior and minimum OS version.
- [ ] Implement a narrow wrapper or fallback only where semantics can be preserved.
- [ ] Add positive, negative, and error-path tests.

### M3 — explicit test loader
- [ ] Design an opt-in loader for a dedicated test application only.
- [ ] Validate DLL search paths and architecture matching.
- [ ] Provide a clean disable/removal path before any broader integration.

### M4 — application profiles
- [ ] Add compatibility profiles only after reproducible tests.
- [ ] Document known unsupported features and crash-reporting procedure.

## Important limitations

A compatibility layer cannot simply pretend the OS is newer. Many APIs depend on kernel behavior, security features, graphics stacks, cryptography, or runtime versions that cannot be recreated by a small wrapper. Windows 7 and Windows 8.1 are separate targets and require independent validation.
