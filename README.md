# KOHook

Simple hooking system.

## How to use?

When the project is built, the DLL is generated in the project's root directory -> `build/bin`.

Use [CFF Explorer][4] on `KnightOnLine.exe` to add the exported functions of the custom DLLs into the executable `Import Table` by following these steps:
1. Open CFF Explorer.
2. Drag KnightOnLine.exe into the GUI.
3. Click on `Import Addr` on the left pane and then click `Add` for adding a new module/dll.
4. Select the corresponding DLL for the loaded executable. e.g. `AIServer.exe` needs `AIServer.dll`.
5. In the `Exported Functions` pane select all of them and `Import by Oridinal`.
6. Final step is to click `Rebuild Import Table` and then `Save`.

If everything is done correctly, the modified executable should automatically load the custom DLL, just like as it would be injected.

HINT: for development instead of copying the DLL, I simply create symbolic link to where the DLLs are compiled. e.g. for KnightOnline.dll in the location where the executable is:
```sh
mklink KnightOnline.dll "D:\KOHook\build\bin\KnightOnline.dll"
```
Then every time recompilation takes place, it will use the latest version.

[4]:https://ntcore.com/?page_id=388
