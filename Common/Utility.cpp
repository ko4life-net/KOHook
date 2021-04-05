#include "pch.h"
#include "Utility.h"
#include <iostream>
#include <tlhelp32.h>

namespace KOHook
{

    bool LoggerInit(const char * title)
    {
        static bool bConsoleInit = false;
#ifdef _DEBUG
        if (!bConsoleInit)
        {
            bConsoleInit = AllocConsole();
            SetConsoleTitleA(title);
            FILE * p;
            freopen_s(&p, "CONIN$", "r", stdin);
            freopen_s(&p, "CONOUT$", "w", stdout);
            freopen_s(&p, "CONOUT$", "w", stderr);
        }
#endif

        return bConsoleInit;
    }

    void ExitProgram(char exitCode)
    {
        std::cin.get();
        ExitProcess(exitCode);
    }

    int GetProcessPID(const std::wstring & processName)
    {
        int            pid      = 0;
        HANDLE         snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &entry))
        {
            while (Process32Next(snapshot, &entry))
            {
                if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
                {
                    pid = entry.th32ProcessID;
                    break;
                }
            }
        }

        CloseHandle(snapshot);
        return pid;
    }

} // namespace KOHook