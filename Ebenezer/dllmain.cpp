#include "pch.h"
#include "User.h"
#include "IOCPort.h"

CIOCPort * m_Iocport         = &(*reinterpret_cast<CIOCPort *>(0x006827E0));
uintptr_t  g_CEbenezerDlgPtr = 0;


CUser * GetUserPtr(int userId)
{
    if (userId < 0 || userId >= MAX_USER || !m_Iocport->m_SockArray)
        return nullptr;

    return static_cast<CUser *>(m_Iocport->m_SockArray[userId]);
}

constexpr offset_t CSTLMap__PARTY_GROUP__GetData = 0x004010E6;
_PARTY_GROUP* PartyGroupGetData(int32_t partyId)
{
    uintptr_t partyPtr = 0;
    __asm {
        mov ecx, g_CEbenezerDlgPtr
        add ecx, 0x1470
        push partyId
        call CSTLMap__PARTY_GROUP__GetData
        test eax, eax
        je failed
        mov partyPtr, eax
    }

    return reinterpret_cast<_PARTY_GROUP*>(partyPtr);

failed:
    return nullptr;
}

uint8_t GetDrainersInPartyCount(int32_t userId)
{
    uint8_t drainersCount = 1;

    CUser * user = GetUserPtr(userId);
    if (!user)
    {
        LOG_CRITICAL("Failed to retrieve user pointer by id: {}", userId);
        KOHook::ExitProgram(1);
    }
    if (!user->IsInParty())
        return drainersCount;

    LOG_TRACE("PartyID: {}", user->m_iPartyIndex);
    _PARTY_GROUP * party = PartyGroupGetData(user->m_iPartyIndex);
    if (!party)
    {
        LOG_CRITICAL("Failed to retrieve party group of user: %s",
            user->m_pUserData->m_strId);
        KOHook::ExitProgram(1);
    }
    assert(party != nullptr);

    for (uint32_t i = 0; i < MAX_PARTY_MEMBERS; ++i)
    {
        LOG_TRACE("[PartyInfo_{}] iIndex={}, sUid[i]={},  sMaxHp[i]={},  sHp[i]={},  "
            "byLevel[i]={},  sClass[i]={}, sLeaderUid={}, byItemRouting={}",
            i, party->iIndex, party->sUid[i], party->sMaxHp[i], party->sHp[i],
            party->byLevel[i], party->sClass[i], party->sLeaderUid, party->byItemRouting);

        if (party->sUid[i] == userId)
            continue;

        CUser * member = GetUserPtr(party->sUid[i]);
        if (!member)
            continue;
        assert(member->m_pUserData != nullptr && "m_pUserData is null");

        if (!member->IsInGame() || !member->IsRogue())
            continue;

        // Have enough skill points for drains skills?
        // 10 Blood drain skill points requirement
        // 50 Vampiric touch skill points requirement
        if (member->GetAssassinSkillPoints() < 10)
            continue;

        // Passed all the checks, increment the drainers count.
        drainersCount++;

        // Sanity check
        if (drainersCount > MAX_PARTY_MEMBERS)
        {
            LOG_CRITICAL("Unexpected behavior. drainersCount was {}", drainersCount);
            KOHook::ExitProgram(1);
        }
    }

    LOG_INFO("drainersCount: {}, userId: {}", drainersCount, userId);
    return drainersCount;
}

int32_t userId, drainersCount;
constexpr offset_t DRAIN_LIMIT_HOOK = 0x00463C67;
constexpr offset_t DRAIN_LIMIT_RETN = 0x00463C72;
void __declspec(naked) Hook_DrainLimit()
{
    __asm {
        pushad
        pushfd
        mov eax, dword ptr ss:[ebp-0x4] // sid
        mov userId, eax
    }
    drainersCount = GetDrainersInPartyCount(userId);
    __asm {
        popfd
        popad
        mov eax, drainersCount
        mov dword ptr ss:[ebp-0x1C], eax
        lea eax, dword ptr ss:[ebp-0x1C]
        push edi
        push eax // data2
        jmp DRAIN_LIMIT_RETN
    }
}

constexpr offset_t ON_INIT_DIALOG_HOOK = 0x0041C15F;
constexpr offset_t ON_INIT_DIALOG_RETN = 0x0041C164;
void __declspec(naked) Hook_OnInitDialog()
{
    __asm {
        mov eax, 0x6293E8
        pushad
        pushfd
        mov g_CEbenezerDlgPtr, ecx
        popfd
        popad
        jmp ON_INIT_DIALOG_RETN
    }
}

//https://reverseengineering.stackexchange.com/questions/8120/making-application-load-dll-at-start
void __declspec(dllexport) InstallHooks()
{
    LOG_INFO("Installing hooks...");

    // CEbenezerDlg::OnInitDialog
    Hook::PatchDetour(ON_INIT_DIALOG_HOOK, Hook_OnInitDialog);

    // CMagicProcess::MagicPacket
    Hook::PatchFill(DRAIN_LIMIT_HOOK, Hook::Opcode::NOP, DRAIN_LIMIT_RETN - DRAIN_LIMIT_HOOK);
    Hook::PatchDetour(DRAIN_LIMIT_HOOK, Hook_DrainLimit);

    // Fix Damage Overflow
    Hook::PatchFill(0x004A763B, Hook::Opcode::NOP, 52);
    Hook::PatchBytes(0x004A763B, { Hook::Opcode::JMPShort , 0x2F }); // jmp 0x004A766C
    Hook::PatchBytes(0x004A766C, { Hook::Opcode::LEA , 0x45, 0x10 }); // lea eax, dword ptr ss:[ebp+0x10] damage
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
        KOHook::Logger::Init("Ebenezer Hooked");
        InstallHooks();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        UninstallHooks();
    }

    return TRUE;
}
