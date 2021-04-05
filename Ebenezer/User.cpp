#include "pch.h"
#include "User.h"

#include <assert.h>

CUser::CUser()  = default;
CUser::~CUser() = default;

bool CUser::IsInParty() const
{
    return m_iPartyStatus != 0 || m_iPartyIndex != -1;
};

bool CUser::IsRogue() const
{
    assert(m_pUserData != nullptr);
    // Each nation:
    // regular -> level 10 quest -> master quest
    return m_pUserData->m_sClass == 102 || m_pUserData->m_sClass == 107 ||
           m_pUserData->m_sClass == 108 || m_pUserData->m_sClass == 202 ||
           m_pUserData->m_sClass == 207 || m_pUserData->m_sClass == 208;
}

uint32_t CUser::GetAssassinSkillPoints() const
{
    assert(m_pUserData != nullptr);
    return m_pUserData->m_bySkillCategory2;
}
