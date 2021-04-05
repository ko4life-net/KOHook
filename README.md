# KOHook

Simple hooking system.

## Build

Steps to build:
1. [Install Git][1]
2. [Install CMake][2]
3. [Install Visual Studio 2019][3]
4. Open `build.ps1` and change where your Ebenezer, AIServer and KnightOnline paths accordingly.
5. Open `Developer Command Prompt for VS 19` and execute the following commands:
    ```sh
    powershell
    .\build.ps1
    ```

After the first execution of the script, you can use the `KOHook.sln` to build from there or debug.

## How to use?

When the project is built, the DLLs are generated in the project's root directory -> `build/bin`.

Use [CFF Explorer][4] for each executable (`AIServer.exe`, `Ebenezer.exe` and `KnightOnLine.exe`) to add the exported functions of the custom DLLs into the executables' `Import Table` by following these steps:
1. Open CFF Explorer.
2. Drag the target executable into the GUI.
3. Click on `Import Addr` on the left pane and then click `Add` for adding a new module/dll.
4. Select the corresponding DLL for the loaded executable. e.g. `AIServer.exe` needs `AIServer.dll`.
5. In the `Exported Functions` pane select all of them and `Import by Oridinal`.
6. Final step is to click `Rebuild Import Table` and then `Save`.

If everything is done correctly, the modified executable should automatically load the custom DLL.

HINT: for development instead of copying the DLL, I simply create symbolic link to where the DLLs are compiled. e.g. for Ebenezer in the location where the executable is:
```sh
mklink Ebenezer.dll "D:\KOHook\build\bin\Ebenezer.dll"
```
Then every time recompilation takes place, it will use the latest version. The `build.ps1` script already does that for you.

[1]:https://git-scm.com/downloads
[2]:https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0-windows-x86_64.msi
[3]:https://visualstudio.microsoft.com/downloads/
[4]:https://ntcore.com/?page_id=388