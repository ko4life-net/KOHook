# The following script prepares development environment.
# 1. It builds the submodules and prepare them to be used in the project.
# 2. Create symbolic links of the compiled DLLs to the destination paths.
# 3. Update the *.vcxproj.user files to allow for debugging the dlls through VS.
# 4. Starts the project when done.

# Change the paths accordinly:
$AIServer_FILE_PATH = "D:\KO\server\1299_official\AIServer\AI Server.exe"
$Ebenezer_FILE_PATH = "D:\KO\server\1299_official\Ebenezer\Ebenezer.exe"
$KnightOnline_FILE_PATH = "D:\KO\clients\Official\KnightOnline_1299\KnightOnLine.exe"

if (!(Test-Path $AIServer_FILE_PATH -PathType leaf)) {
    Write-Host "Failed to find AIServer." -ForegroundColor red
    exit 1
}
if (!(Test-Path $Ebenezer_FILE_PATH -PathType leaf)) {
    Write-Host "Failed to find Ebenezer." -ForegroundColor red
    exit 1
}
if (!(Test-Path $KnightOnline_FILE_PATH -PathType leaf)) {
    Write-Host "Failed to find KnightOnline." -ForegroundColor red
    exit 1
}

Remove-Item -LiteralPath "build" -Force -Recurse -ErrorAction SilentlyContinue
git submodule update --init --recursive

pushd thirdparty\spdlog
git clean -xffdq .
git reset --hard
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A Win32 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.\dist
cmake --build . --config Release --target install
popd

pushd thirdparty\d3d8to9
git clean -xffdq .
git reset --hard
git apply ..\patches\d3d8to9.patch
popd

pushd thirdparty\imgui
git clean -xffdq .
git reset --hard
popd

msbuild KOHook.sln

# Create symbolic links to the DLLs for faster development:
$AIServer_DIR_PATH = (Split-Path -Path $AIServer_FILE_PATH)
$Ebenezer_DIR_PATH = (Split-Path -Path $Ebenezer_FILE_PATH)
$KnightOnline_DIR_PATH = (Split-Path -Path $KnightOnline_FILE_PATH)

$AIServer_LINK = "$AIServer_DIR_PATH\AIServer.dll"
$Ebenezer_LINK = "$Ebenezer_DIR_PATH\Ebenezer.dll"
$KnightOnline_LINK = "$KnightOnline_DIR_PATH\KnightOnline.dll"

Remove-Item $AIServer_LINK -Force -ErrorAction SilentlyContinue
Remove-Item $Ebenezer_LINK -Force -ErrorAction SilentlyContinue
Remove-Item $KnightOnline_LINK -Force -ErrorAction SilentlyContinue

$currentPath = (Get-Item .).FullName
New-Item -ItemType SymbolicLink -Path "$AIServer_DIR_PATH" -Name "AIServer.dll" -Target "$currentPath\build\bin\AIServer.dll"
New-Item -ItemType SymbolicLink -Path "$Ebenezer_DIR_PATH" -Name "Ebenezer.dll" -Target "$currentPath\build\bin\Ebenezer.dll"
New-Item -ItemType SymbolicLink -Path "$KnightOnline_DIR_PATH" -Name "KnightOnline.dll" -Target "$currentPath\build\bin\KnightOnline.dll"

# Replace the working directory for debugging the dlls in VS:
$AIServer_vcxprojUser = "$currentPath\AIServer\AIServer.vcxproj.user"
$Ebenezer_vcxprojUser = "$currentPath\Ebenezer\Ebenezer.vcxproj.user"
$KnightOnline_vcxprojUser = "$currentPath\KnightOnline\KnightOnline.vcxproj.user"
(Get-Content -Path $AIServer_vcxprojUser -Raw) -replace [regex]::escape('~~~REPLACE_PATH~~~'), $AIServer_DIR_PATH | Out-File -Encoding ascii $AIServer_vcxprojUser
(Get-Content -Path $Ebenezer_vcxprojUser -Raw) -replace [regex]::escape('~~~REPLACE_PATH~~~'), $Ebenezer_DIR_PATH | Out-File -Encoding ascii $Ebenezer_vcxprojUser
(Get-Content -Path $KnightOnline_vcxprojUser -Raw) -replace [regex]::escape('~~~REPLACE_PATH~~~'), $KnightOnline_DIR_PATH | Out-File -Encoding ascii $KnightOnline_vcxprojUser

Start-Process KOHook.sln
Write-Host "Done."
