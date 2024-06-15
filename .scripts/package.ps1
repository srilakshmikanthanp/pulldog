# Copyright (c) 2023 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT


# if is not Developer Command Prompt for VS 2022
if ($env:VSCMD_ARG_app_plat -ne  "Desktop") {
  Write-Host "Please run this script in Developer Command Prompt for VS 2022" -ForegroundColor Red
  exit 1
}

# Check if current directory has CMakeLists.txt
if (-not (Test-Path ./CMakeLists.txt)) {
  Write-Host "CMakeLists.txt not found in current working directory" -ForegroundColor Red
  exit 1
}

#-------------------------- clipbird package --------------------------#

# Buil the clipbird with release configuration
Write-Host "Building clipbird with Release configuration" -ForegroundColor Green
cmake -G "Visual Studio 17 2022" -B ./build && cmake --build ./build --config Release

# clipbird package data directory
$ClipbirDir = "./.setup"

# clean the package directory items except .gitignore
Write-Host "Cleaning the package directory $ClipbirDir" -ForegroundColor Green
Remove-Item -Recurse -Force $ClipbirDir/* -Exclude .gitignore

# create conf directory in the package directory
Write-Host "Creating conf directory in $ClipbirDir" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $ClipbirDir/conf

# copy the conf folder to the package directory
Write-Host "Copying ./conf/* to $ClipbirDir" -ForegroundColor Green
Copy-Item ./conf/* $ClipbirDir/conf

# create assets directory in the package directory
Write-Host "Creating assets directory in $ClipbirDir" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $ClipbirDir/assets/images

# copy the Image files to the package directory
Write-Host "Copying ./assets/images/* to $ClipbirDir/assets/images" -ForegroundColor Green
Copy-Item ./assets/images/* $ClipbirDir/assets/images

# Copy All Qt dlls to the package directory
Write-Host "Creating the package as Release version" -ForegroundColor Green
windeployqt ./build/Release/clipbird.exe --dir $ClipbirDir --release

# copy the clipbird.exe to the package directory
Write-Host "Copying ./build/Release/clipbird.exe to $ClipbirDir" -ForegroundColor Green
Copy-Item ./build/Release/clipbird.exe $ClipbirDir
