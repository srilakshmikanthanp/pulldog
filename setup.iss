; This Script used to build installer for pulldog application using inno setup

; constants used in the script
#define PulldogPublisher "srilakshmikanthanp"
#define PulldogName "pulldog"
#define PulldogVersion "1.0.0"
#define PulldogUUID "3A8D9FCA-9F95-4947-8AB0-3E364ED765E1"
#define PulldogURL "https://github.com/srilakshmikanthanp/pulldog"
#define PulldogExeName "pulldog.exe"

[Setup]
AppPublisher={#PulldogPublisher}
AppVerName={#PulldogName} {#PulldogVersion}
AppId={#PulldogUUID}
AppVersion={#PulldogVersion}
AppName={#PulldogName}
AppPublisherURL={#PulldogURL}
AppSupportURL={#PulldogURL}
AppUpdatesURL={#PulldogURL}
DefaultDirName={autopf}\{#PulldogName}
DisableProgramGroupPage=yes
LicenseFile=NOTICE
OutputBaseFilename=pulldog-x64
SetupIconFile=assets\images\logo.ico
OutputDir=.\dist
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\{#PulldogExeName}
UsePreviousLanguage=no

[Registry]
; To start app on windows startup
Root      : HKLM;                                             \
Subkey    : "Software\Microsoft\Windows\CurrentVersion\Run";  \
ValueType : string;                                           \
ValueName : "{#PulldogName}";                                \
ValueData : """{app}\{#PulldogExeName}""";                   \
Flags     : uninsdeletevalue

[Files]
; List of files to be included in the installer
Source  : ".\.setup\*";                                 \
DestDir : "{app}";                                      \
Flags   : ignoreversion recursesubdirs createallsubdirs

[Languages]
; Include English language
Name          : "english";              \
MessagesFile  : "compiler:Default.isl"

[Tasks]
; Create desktop icon Optionality
Name              : "desktopicon";             \
Description       : "{cm:CreateDesktopIcon}";  \
GroupDescription  : "{cm:AdditionalIcons}";    \
Flags             : unchecked

[Icons]
; Desktop Icon
Name      : "{autodesktop}\{#PulldogName}"; \
Filename  : "{app}\{#PulldogExeName}";      \
Tasks     : desktopicon

; Start Menu Icon
Name      : "{autoprograms}\{#PulldogName}"; \
Filename  : "{app}\{#PulldogExeName}"

[Run]
; Install VC++ Redistributables
Filename    : {app}\vc_redist.x64.exe;              \
Parameters  : "/q /norestart";                      \
StatusMsg   : "Installing VC++ Redistributables..."

; Start the application after installation
Filename    : "{app}\{#PulldogExeName}";                                    \
Description : "{cm:LaunchProgram,{#StringChange(PulldogName, '&', '&&')}}"; \
Flags       : nowait postinstall skipifsilent
