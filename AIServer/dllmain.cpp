#include "pch.h"

// clang-format off
/// <summary>
/// Drain max damage limit by the amount of rogues that can drain within the same party.
/// </summary>
const int DrainDamageLimit[MAX_PARTY_MEMBERS] = {
    90000, // 1 drainer
    50000, // 2 drainer
    36500, // etc...
    28000,
    23000,
    20000,
    17500,
    15500
};
// clang-format on

/// <summary>
/// Calculates the drain damage based on the drainers count in party before sending the packet
/// to the client. Drainers count considered to be the rogues that can use the drain skill that
/// are in the same party of the caster.
/// </summary>
/// <param name="percentage">The percentage to drain of the target's hp.</param>
/// <param name="currentHp">The current target hp.</param>
/// <param name="drainersCount">How many players can drain within the same party of the caster.</param>
/// <returns>End results of the drain damage.</returns>
int32_t GetDrainDamage(int16_t percentage, int32_t currentHp, int32_t drainersCount)
{
    int32_t damage = 0;
    if (percentage < 100)
        damage = (currentHp * percentage) / -100;
    else
        damage = (currentHp * (percentage - 100)) / 100;

    // Better seeing 1 damage reflected than nothing.
    if (damage == 0)
        damage = -1;

    int32_t drainLimit = -DrainDamageLimit[drainersCount - 1];
    if (damage < drainLimit)
    {
        LOG_INFO("Drain damage={}, Limit applied={}, Drainers Count={}", damage, drainLimit, drainersCount);
        damage = drainLimit;
    }

    LOG_INFO("Drainers: {}, CurrentHP: {}, Percentage: {}, Damage: {}",
             drainersCount, currentHp, percentage, damage);

    return damage;
}


int16_t sFirstDamage;
int32_t monsterHp, data2, damage;
constexpr offset_t DRAIN_LIMIT_HOOK = 0x0040D9D4;
constexpr offset_t DRAIN_LIMIT_RETN = 0x0040D9F9;
void __declspec(naked) Hook_DrainLimit()
{
    __asm {
        pushad
        pushfd
        mov sFirstDamage, ax
        mov eax, dword ptr ds:[ebx+0x4F8]
        mov monsterHp, eax
        mov eax, dword ptr ss:[ebp+0x14]
        mov data2, eax
        //mov eax, dword ptr ds:[esi+0x8]
        //mov userPtr, eax
        //mov eax, dword ptr ds:[eax+0x40]
        //mov userId, eax
    }
    damage = GetDrainDamage(sFirstDamage, monsterHp, data2);
    __asm {
        popfd
        popad
        mov dword ptr ss:[ebp+0x14], 0
        mov eax, damage
        jmp DRAIN_LIMIT_RETN
    }
}

void __declspec(dllexport) InstallHooks()
{
    LOG_INFO("Installing hooks...");

    // CUser::ExecuteType3
    Hook::PatchFill(DRAIN_LIMIT_HOOK, Hook::Opcode::NOP, DRAIN_LIMIT_RETN - DRAIN_LIMIT_HOOK);
    Hook::PatchDetour(DRAIN_LIMIT_HOOK, Hook_DrainLimit);
}

void __declspec(dllexport) UninstallHooks()
{
    LOG_INFO("Uninstalling hooks...");
    // TODO
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        KOHook::Logger::Init("AIServer Hooked");
        InstallHooks();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        UninstallHooks();
    }

    return TRUE;
}
