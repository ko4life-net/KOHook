#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Common/Logger.h"
#include "Common/Utility.h"

int __cdecl main()
{
    std::string dllPath = "AIServer.dll";
    int         pid     = KOHook::GetProcessPID(L"AI Server.exe");

    //std::string dllPath = "Ebenezer.dll";
    //int         pid     = KOHook::GetProcessPID(L"Ebenezer.exe");

    //std::string dllPath = "KnightOnline.dll";
    //int         pid     = KOHook::GetProcessPID(L"KnightOnLine.exe");
    if (pid == 0)
    {
        LOG_ERROR("Failed to find PID.");
        return 1;
    }

    auto procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!procHandle)
    {
        LOG_ERROR("Failed to get process handle.");
        return 1;
    }

    auto virtualMem = VirtualAllocEx(procHandle, 0, dllPath.size(), MEM_COMMIT, PAGE_READWRITE);
    if (!virtualMem)
    {
        LOG_ERROR("Failed to get allocate memory in process.");
        return 1;
    }

    WriteProcessMemory(procHandle, virtualMem, (LPVOID)dllPath.c_str(), dllPath.size(), 0);
    auto kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32)
    {
        LOG_ERROR("Failed to get a handle to kernel32 dll.");
        return 1;
    }

    auto LoadLibraryA = GetProcAddress(kernel32, "LoadLibraryA");
    if (!LoadLibraryA)
    {
        LOG_ERROR("Failed to get a handle to LoadLibraryA within kernel32.");
        return 1;
    }

    HANDLE remoteThread = CreateRemoteThread(procHandle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, virtualMem, 0, 0);
    if (!remoteThread)
    {
        LOG_ERROR("Failed to create remote thread.");
        return 1;
    }

    // Make sure to wait for the loader thread to finish execution on LoadLibraryA
    WaitForSingleObject(remoteThread, INFINITE);

    LOG_INFO("Allocated memory space: {:X}", virtualMem);
    VirtualFreeEx(procHandle, virtualMem, dllPath.size(), MEM_RELEASE);

    return 0;
}
