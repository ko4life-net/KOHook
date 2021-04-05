#pragma once

#include "Types.h"
#include "Constants.h"

typedef ListWrap<struct _EXCHANGE_ITEM *> ItemList;

#pragma pack(push, 1)

struct _ITEM_DATA
{
    int32_t  iNum;
    uint16_t sDuration;
    uint16_t sCount;
    uint8_t  byItemType;
    uint8_t  byUnknown1;
    uint16_t sRemainingTime;
    int32_t  iCreateTime;
    uint64_t lSerialNum;
};

struct _EXCHANGE_ITEM
{
    int32_t  iItemId;
    int32_t  iCount;
    int16_t  sDuration;
    uint8_t  byPosition;
    uint64_t lSerialNum;
};

struct _ITEM_DATA_WAREHOUSE
{
    int32_t  iNum;
    uint16_t sDuration;
    uint16_t sCount;
    uint64_t lSerialNum;
};

struct _QUEST_LIST
{
    uint16_t sNum;
    uint8_t  byStatus;
    uint8_t  byFree;
};

struct _SAVED_MAGIC
{
    int32_t  iNum;
    uint16_t sDuration;
    uint16_t sUnknown1;
};

struct _USER_DATA
{
    char                 m_strId[MAX_ID_SIZE + 1];
    char                 m_strAccountId[MAX_ID_SIZE + 1];
    char                 m_strUnknown[18];
    uint8_t              m_byZone;
    uint8_t              m_byUnknown_0x3D;
    uint8_t              m_byUnknown_0x3E;
    uint8_t              m_byUnknown_0x3F;
    float                m_fCurX;
    float                m_fCurZ;
    float                m_fCurY;
    uint8_t              m_byNation;
    uint8_t              m_byRace;
    uint16_t             m_sClass;
    uint8_t              m_byHairColor;
    uint8_t              m_byRank;
    uint8_t              m_byTitle;
    uint8_t              m_byLevel;
    int32_t              m_iExp;
    int32_t              m_iLoyalty;
    int32_t              m_iLoyaltyMonthly;
    uint8_t              m_byFace;
    uint8_t              m_byCity;
    uint16_t             m_bKnights;
    uint8_t              m_byFame;
    uint8_t              m_byUnknown_0x65;
    uint16_t             m_sHp;
    uint16_t             m_sMp;
    uint16_t             m_sSp;
    uint8_t              m_byStr;
    uint8_t              m_bySta;
    uint8_t              m_byDex;
    uint8_t              m_byIntel;
    uint8_t              m_byCha;
    uint8_t              m_byAuthority;
    uint8_t              m_byPoints;
    uint8_t              m_byUnknown_0x73;
    int32_t              m_iGold;
    uint16_t             m_sBind;
    uint16_t             m_sUnknown_0x7A;
    int32_t              m_iBank;
    uint8_t              m_bySkillPoints;
    uint8_t              m_byLeaderShip;
    uint8_t              m_byLinguistics;
    uint8_t              m_byPolitics;
    uint8_t              m_bySiegeWeapon;
    uint8_t              m_bySkillCategory1; // Archery
    uint8_t              m_bySkillCategory2; // Assassin
    uint8_t              m_bySkillCategory3; // Explore
    uint8_t              m_bySkillMaster;
    uint8_t              m_byUnknown_0x89;
    uint8_t              m_byLogout;
    uint8_t              m_byWarehouse;
    uint32_t             m_iTime;
    _ITEM_DATA           m_ItemArray[HAVE_MAX + SLOT_MAX];
    _ITEM_DATA_WAREHOUSE m_WarehouseArray[WAREHOUSE_MAX];
    uint8_t              m_byUnknown_0x1080[26];
    uint16_t             m_byQuestCount;
    _QUEST_LIST          m_QuestArray[MAX_QUEST];
    _SAVED_MAGIC         m_SavedMagicArray[MAX_SAVED_MAGIC];
    uint8_t              m_byPremiumType;
    uint8_t              m_byUnknown_0x127D;
    uint16_t             m_sPremiumDays;
    int32_t              m_iMannerPoint;
};

struct _PARTY_GROUP
{
    uint32_t iIndex;
    int16_t  sUid[8];
    int16_t  sMaxHp[8];
    int16_t  sHp[8];
    uint8_t  byLevel[8];
    int16_t  sClass[8];
    uint8_t  byItemRouting;
    int16_t  sLeaderUid;
    _PARTY_GROUP()
        : iIndex(0)
        , sLeaderUid(-1)
        , byItemRouting(0)
    {
        for (uint32_t i = 0; i < MAX_PARTY_MEMBERS; i++)
        {
            sUid[i]    = -1;
            sMaxHp[i]  = 0;
            sHp[i]     = 0;
            byLevel[i] = 0;
            sClass[i]  = 0;
        }
    };
};

#pragma pack(pop)
static_assert(sizeof(_ITEM_DATA) == 24);
static_assert(sizeof(_EXCHANGE_ITEM) == 19);
static_assert(sizeof(_ITEM_DATA_WAREHOUSE) == 16);
static_assert(sizeof(_QUEST_LIST) == 4);
static_assert(sizeof(_SAVED_MAGIC) == 8);
static_assert(sizeof(_USER_DATA) == 4740);
static_assert(sizeof(_PARTY_GROUP) == 79);