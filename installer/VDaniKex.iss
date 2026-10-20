#ifndef AppArch
  #define AppArch "x86"
#endif

#ifndef SourceDir
  #define SourceDir "build/Release"
#endif

#define ProductName "VDaniKex"
#define ProductVersion "0.2.0-dev"
#if AppArch == "x64"
  #define ProductAppId "{{B5D91D3A-8E19-4D22-9B62-A19E7FCD3022}"
#else
  #define ProductAppId "{{A4C80E0A-8B16-4D89-8CB8-DC5B6A7E0211}"
#endif

[Setup]
AppId={#ProductAppId}
AppName={#ProductName} ({#AppArch})
AppVersion={#ProductVersion}
AppPublisher=VDaniKex contributors
DefaultDirName={localappdata}\Programs\VDaniKex-{#AppArch}
DefaultGroupName=VDaniKex ({#AppArch})
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
OutputDir=..\dist
OutputBaseFilename=VDaniKex-Setup-{#AppArch}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayName={#ProductName} ({#AppArch})
Uninstallable=yes
CloseApplications=no
RestartApplications=no
#if AppArch == "x64"
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#else
ArchitecturesAllowed=x86 x64
#endif

[Files]
Source: "{#SourceDir}\VDaniKexCore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\vdk-probe.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\vdk-vm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\docs\VM-LAUNCHER.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\VDaniKex Host Diagnostic"; Filename: "{app}\vdk-probe.exe"
Name: "{group}\VDaniKex VM Launcher Help"; Filename: "{app}\vdk-vm.exe"; Parameters: "--help"
Name: "{group}\VDaniKex Project README"; Filename: "{app}\README.md"

[Run]
Filename: "{app}\vdk-probe.exe"; Description: "Run VDaniKex host diagnostic"; Flags: postinstall nowait skipifsilent
Filename: "{app}\vdk-vm.exe"; Parameters: "--help"; Description: "Show VDaniKex VM launcher help"; Flags: postinstall nowait skipifsilent

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
  #if AppArch == "x64"
  if not IsWin64 then
  begin
    MsgBox('This x64 package requires 64-bit Windows.', mbError, MB_OK);
    Result := False;
  end;
  #endif
end;
