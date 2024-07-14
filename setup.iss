; This Script used to build installer for pulldog application using inno setup

; constants used in the script
#define PulldogPublisher "srilakshmikanthanp"
#define PulldogName "pulldog"
#define PulldogVersion "0.0.1"
#define PulldogUUID "f4b84e82-54df-47fc-8a90-05fd21f42dd1"
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
LicenseFile=LICENSE
OutputBaseFilename=pulldog-x64
SetupIconFile=assets\images\logo.ico
OutputDir=.\dist
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\{#PulldogExeName}
UsePreviousLanguage=no
PrivilegesRequired=lowest

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

Name: "{userstartup}\{#PulldogName}"; \
Filename: "{app}\PulldogExeName.exe";  \
WorkingDir: "{app}"

; Start Menu Icon
Name      : "{autoprograms}\{#PulldogName}"; \
Filename  : "{app}\{#PulldogExeName}"

[Run]
; Start the application after installation
Filename    : "{app}\{#PulldogExeName}";                                    \
Description : "{cm:LaunchProgram,{#StringChange(PulldogName, '&', '&&')}}"; \
Flags       : nowait postinstall skipifsilent
