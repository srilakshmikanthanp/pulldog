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

#-------------------------- pulldog package --------------------------#

# Buil the pulldog with release configuration
Write-Host "Building pulldog with Release configuration" -ForegroundColor Green
cmake -G "Visual Studio 17 2022" -B ./build && cmake --build ./build --config Release

# pulldog package data directory
$Pulldog = "./.setup"

# clean the package directory items except .gitignore
Write-Host "Cleaning the package directory $Pulldog" -ForegroundColor Green
Remove-Item -Recurse -Force $Pulldog/* -Exclude .gitignore

# create conf directory in the package directory
Write-Host "Creating conf directory in $Pulldog" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $Pulldog/conf

# copy the conf folder to the package directory
Write-Host "Copying ./conf/* to $Pulldog" -ForegroundColor Green
Copy-Item ./conf/* $Pulldog/conf

# create assets directory in the package directory
Write-Host "Creating assets directory in $Pulldog" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $Pulldog/assets/images

# copy the Image files to the package directory
Write-Host "Copying ./assets/images/* to $Pulldog/assets/images" -ForegroundColor Green
Copy-Item ./assets/images/* $Pulldog/assets/images

# Copy All Qt dlls to the package directory
Write-Host "Creating the package as Release version" -ForegroundColor Green
windeployqt ./build/Release/pulldog.exe --dir $Pulldog --release

# copy All vc dlls to the package directory
Write-Host "Copying all vc dlls to $Pulldog" -ForegroundColor Green
Copy-Item $env:VC_DLLS_DIR/* $Pulldog -Recurse

# copy the pulldog.exe to the package directory
Write-Host "Copying ./build/Release/pulldog.exe to $Pulldog" -ForegroundColor Green
Copy-Item ./build/Release/pulldog.exe $Pulldog
