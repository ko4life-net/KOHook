#include "pch.h"
#include "App.h"
#include "GameUtils.h"

AppData App::s_Data;

App::App() {}

constexpr offset_t GAME_LOOP_HOOK = 0x0064C411;
constexpr offset_t GAME_LOOP_RETN = 0x0064C447;
constexpr offset_t CGameProcedure__TickActive = 0x006468E0;
constexpr offset_t CGameProcedure__RenderActive = 0x00646920;
void App::StartGameLoop()
{
    // [Frame] SecPerFrame=11235
    //if (s_fSecPerFrmPrecise >= 0.011111f)
    //{
    //    if (s_fSecPerFrmPrecise > 0.033333f)
    //        s_fSecPerFrmPrecise = 0.033333f;
    //    else
    //        s_fSecPerFrmPrecise = 0.011111f;
    //}

    MSG msg{};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        else if (!App::s_Data.bFPSControlEnabled)
        {
            __asm {
                call CGameProcedure__TickActive
                call CGameProcedure__RenderActive
            }
        }
        else
        {
            static double s_dFrmElapsed = 0;
            if (s_dFrmElapsed >= App::s_Data.fSecPerFrmPrecise)
            {
                if (s_dFrmElapsed >= 1.0)
                    s_dFrmElapsed = 0.033333f;
                *App::s_Data.fSecPerFrm = static_cast<float>(s_dFrmElapsed);
                *App::s_Data.fFrmPerSec = 1.0f / static_cast<float>(s_dFrmElapsed);
                s_dFrmElapsed = 0.0;
                __asm {
                    call CGameProcedure__TickActive
                    call CGameProcedure__RenderActive
                }
            }
            else
            {
                s_dFrmElapsed += Timer::Process(Timer::Command::GetElapsedTime);
            }
        }
    }
}

void __declspec(naked) Hook_GameLoop()
{
    App::Instance().StartGameLoop();
    __asm jmp GAME_LOOP_RETN;
}

void App::Start()
{
    // WinMain - Take over the game loop
    Hook::PatchFill(GAME_LOOP_HOOK, Hook::Opcode::NOP, GAME_LOOP_RETN - GAME_LOOP_HOOK);
    Hook::PatchDetour(GAME_LOOP_HOOK, Hook_GameLoop);

    // CN3Sky::Render - Fixes broken sky fog render states
    Hook::PatchFill(0x005AE0BE, Hook::Opcode::NOP, 0x005AE0E6 - 0x005AE0BE);
}

void App::Stop() {}
