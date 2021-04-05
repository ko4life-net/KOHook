#include "pch.h"
#include "App.h"
#include "GameUtils.h"
#include "d3d8to9.hpp"

AppData App::s_Data;

App::App()
    : m_Patches()
    , m_DeviceRef(nullptr)
#if defined(ENABLE_IMGUI)
    , m_GameUI(std::make_unique<GameUI>())
#endif
{
}

constexpr offset_t DAMAGE_OVERFLOW_HOOK = 0x005D52A0;
constexpr offset_t DAMAGE_OVERFLOW_RETN = 0x005D52C1;
void __declspec(naked) Hook_DamageOverflowFix()
{
    // Parsing the packet received from Ebenezer as integer instead of short
    __asm {
        add edx, 0x4
        mov dword ptr ds:[ecx], edx
        mov esi, dword ptr ds:[eax+0x4]
        cmp edx, esi
        jle parse_succeed
        xor ebp, ebp
        jmp parse_failed

    parse_succeed :
        mov esi, dword ptr ds:[eax+0x8]
        mov ebp, dword ptr ds:[esi+edx*1-0x4]

    parse_failed :
        mov eax, dword ptr ss:[esp+0x14]
        test eax, eax
        jmp DAMAGE_OVERFLOW_RETN
    }
}

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
                //LOG_INFO("s_fSecPerFrm={}, s_fFrmPerSec={}", *s_fSecPerFrm, *s_fFrmPerSec);
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
    LOG_INFO("Installing hooks...");

#if defined(ENABLE_IMGUI)
    CreateDeviceCallback = [&](IDirect3DDevice9 * device)
    {
        m_DeviceRef = device;
        m_GameUI->Initialize(device);
        EndSceneCallback = [&](){ m_GameUI->Render(); };
    };
#endif

    PatchMemory();
}

void App::PatchMemory()
{
    // CN3Eng::ctor - activates conversion from DirectX 8 to 9
    Hook::PatchDetour(0x0043A260, Direct3DCreate8, Hook::Opcode::CALL);

    // TODO: Figure out why the CGameProcLogo fails to display the video when using DirectX-9.
    // Something to do with the mss32.dll being incompatible with AIL library on tick and render.
    // WinMain - Skip the CGameProcLogo by changing procedure to CGameProcLogin pointer
    Hook::PatchBytes(0x0064C397 + 2, { 0xA8, 0x89, 0x81, 0x00 }); // 0x008189A8

    // WinMain - Take over the game loop
    Hook::PatchFill(GAME_LOOP_HOOK, Hook::Opcode::NOP, GAME_LOOP_RETN - GAME_LOOP_HOOK);
    Hook::PatchDetour(GAME_LOOP_HOOK, Hook_GameLoop);

    // CGameProcMain::MsgRecv_TargetHP
    Hook::PatchFill(DAMAGE_OVERFLOW_HOOK, Hook::Opcode::NOP, DAMAGE_OVERFLOW_RETN - DAMAGE_OVERFLOW_HOOK);
    Hook::PatchDetour(DAMAGE_OVERFLOW_HOOK, Hook_DamageOverflowFix);
}

void App::Stop()
{
    LOG_INFO("Uninstalling hooks...");
    // TODO
}