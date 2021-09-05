#include "pch.h"
#include "App.h"

void __declspec(dllexport) InstallHooks()
{
    App::Instance().Start();
}

void __declspec(dllexport) UninstallHooks()
{
    App::Instance().Stop();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        InstallHooks();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        UninstallHooks();
    }

    return TRUE;
}
