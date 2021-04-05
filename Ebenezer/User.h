#pragma once

#include "IOCPSocket2.h"
#include "Common/Structs.h"

#pragma pack(push, 1)

struct Type4Duration
{
    int16_t m_sDuration;
    int16_t _padding;
    float   m_fStartTime;
};

class CUser : public CIOCPSocket2
{
public:
    CUser();
    ~CUser() override;

    bool     IsInParty() const;
    bool     IsRogue() const;
    uint32_t GetAssassinSkillPoints() const;

public:
    _USER_DATA *  m_pUserData;
    char          m_strAccountID[MAX_ID_SIZE + 1];
    char          m_strKeyExchange[33]; // Client sends this on CUser::SelectCharacter
    uint16_t      m_sRegionX;
    uint16_t      m_sRegionZ;
    uint8_t       m_sRegionZ_PADDING[2];
    int32_t       m_iMaxExp;
    uint16_t      m_sMaxWeight; // (weight / 10) and (weight % 10) for getting the fraction.
    uint8_t       m_bySpeed;
    int8_t        m_bySpeed_PADDING[1];
    int16_t       m_sBodyAc;
    int16_t       m_sTotalHit; // Attack Power
    int16_t       m_sTotalAc;
    int16_t       m_sTotalAcUnkown; // Some AC related bonus
    float         m_fTotalHitrate;
    float         m_fTotalEvasionrate;
    int16_t       m_sItemMaxHp;
    int16_t       m_sItemMaxMp;
    uint16_t      m_sItemWeight;
    int16_t       m_sItemHit;
    int16_t       m_sItemAc;
    int16_t       m_sItemStr;
    int16_t       m_sItemSta;
    int16_t       m_sItemDex;
    int16_t       m_sItemIntel;
    int16_t       m_sItemCham;
    int16_t       m_sItemHitrate;
    int16_t       m_sItemEvasionrate;
    uint16_t      m_sFireR;
    uint16_t      m_sColdR;
    uint16_t      m_sLightningR;
    uint16_t      m_sMagicR;
    uint16_t      m_sDiseaseR;
    uint16_t      m_sPoisonR;
    uint8_t       m_byMagicTypeLeftHand;
    uint8_t       m_byMagicTypeRightHand;
    int16_t       m_sMagicAmountLeftHand;
    int16_t       m_sMagicAmountRightHand;
    int16_t       m_sDaggerR;
    int16_t       m_sSwordR;
    int16_t       m_sAxeR;
    int16_t       m_sMaceR;
    int16_t       m_sSpearR;
    int16_t       m_sBowR;
    uint16_t      m_sMaxHp;
    uint16_t      m_sMaxMp;
    uint16_t      m_sZoneIndex;
    float         m_fWill_x, m_fWill_z, m_fWill_y;
    uint8_t       m_byAlive;
    uint8_t       m_byResHpType;
    uint8_t       m_byWarp;
    uint8_t       m_byNeedParty;
    int32_t       m_iPartyIndex;
    uint16_t      m_sExchangeUser;
    uint8_t       m_byExchangeOK;
    int8_t        m_byExchangeOK_PADDING[1];
    ItemList      m_ExchangeItemList;
    _ITEM_DATA    m_MirrorItem[HAVE_MAX];
    int16_t       m_sPrivateChatUser;
    int8_t        m_sPrivateChatUser_PADDING[2];
    float         m_fHPLastTimeNormal; // For Automatic HP recovery.
    float         m_fHPStartTimeNormal;
    int16_t       m_sHPAmountNormal;
    uint8_t       m_byHPDurationNormal;
    uint8_t       m_byHPIntervalNormal;
    float         m_fHPLastTime[MAX_TYPE3_REPEAT]; // For durational HP recovery.
    float         m_fHPStartTime[MAX_TYPE3_REPEAT];
    int16_t       m_sHPAmount[MAX_TYPE3_REPEAT];
    uint8_t       m_byHPDuration[MAX_TYPE3_REPEAT];
    uint8_t       m_byHPInterval[MAX_TYPE3_REPEAT];
    int16_t       m_sSourceID[MAX_TYPE3_REPEAT];
    int32_t       m_iType3Flag;
    float         m_fAreaLastTime; // For Area Damage spells Type 3.
    float         m_fAreaStartTime;
    uint8_t       m_byAreaInterval;
    int8_t        m_byAreaInterval_PADDING[3];
    int32_t       m_iAreaMagicID;
    uint8_t       m_byAttackSpeedAmount;
    uint8_t       m_bySpeedAmount;
    uint16_t      m_sACAmount;
    uint8_t       m_byAttackAmount;
    int8_t        m_byAttackAmount_PADDING[1];
    uint16_t      m_sMaxHPAmount;
    int16_t       m_sUnknown2;
    int16_t       m_sUnknown3;
    uint8_t       m_byHitRateAmount;
    int8_t        m_byHitRateAmount_PADDING[1];
    int16_t       m_sAvoidRateAmount;
    int16_t       m_byStrAmount;
    int16_t       m_byStaAmount;
    int16_t       m_byDexAmount;
    int16_t       m_byIntelAmount;
    int16_t       m_byChaAmount;
    uint8_t       m_byFireRAmount;
    uint8_t       m_byColdRAmount;
    uint8_t       m_byLightningRAmount;
    uint8_t       m_byMagicRAmount;
    uint8_t       m_byDiseaseRAmount;
    uint8_t       m_byPoisonRAmount;
    Type4Duration m_Type4Duration[14];
    int32_t       field_86D4;
    int32_t       field_86D8;
    int32_t       field_86DC;
    int16_t       field_86E0;
    int8_t        field_86E0_PADDING[2];
    int32_t       m_iType4Flag;
    uint8_t       m_byType4Buff[MAX_TYPE4_BUFF];
    int16_t       m_sTransformDuration;
    int16_t       m_sTransformDurationStart;
    float         m_fTransformDurationNow;
    int32_t       m_iTransformIsActive;
    int16_t       m_sTransformMaxHp;
    int16_t       m_sTransformMaxMp;
    uint8_t       m_byTransformSpeed;
    int8_t        m_byTransformSpeed_PADDING[1];
    int16_t       m_sTransformAttackSpeed;
    int16_t       m_sTransformTotalHit;
    int16_t       m_sTransformTotalAc;
    int16_t       m_sTransformTotalHitRate;
    int16_t       m_sTransformTotalEvasionRate;
    int16_t       m_sTransformTotalFireR;
    int16_t       m_sTransformTotalColdR;
    int16_t       m_sTransformTotalLightningR;
    int16_t       m_sTransformTotalMagicR;
    int16_t       m_sTransformTotalDiseaseR;
    int16_t       m_sTransformTotalPoisonR;
    uintptr_t *   m_pMain;            // CEbenezerDlg
    int8_t        m_MagicProcess[14]; // CMagicProcess stack allocated
    int8_t        m_FILL_UNKNOWN1[246];
    float         m_fSpeedHackClientTime;
    float         m_fSpeedHackServerTime;
    bool          m_bSpeedHackCheck;
    int8_t        m_bSpeedHackCheck_PADDING[3];
    float         m_fBlinkStartTime;
    int16_t       m_sAliveCount;
    int8_t        m_sAliveCount_PADDING[2];
    int32_t       m_iAbnormalType;
    int16_t       m_sTransformSize;
    int8_t        m_sTransformSize_PADDING[2];
    int32_t       field_886C;
    int16_t       m_sWhoKilledMe;
    int8_t        m_sWhoKilledMe_PADDING[2];
    int32_t       m_iLostExp;
    float         m_fLastTrapAreaTime;
    int32_t       m_iZoneChangeFlag;
    int8_t        m_bRegeneType;
    int8_t        m_bRegeneType_PADDING[3];
    float         m_fLastRegeneTime;
    bool          m_bZoneChangeSameZone;
    int8_t        m_bZoneChangeSameZone_PADDING[3];
    int32_t       m_iSelMsgEvent[MAX_MESSAGE_EVENT];
    int16_t       m_sEventNid;
    int8_t        m_sEventNid_PADDING[2];
    ListWrap<int> m_UserEventList;
    char          m_strCouponId[MAX_COUPON_ID_LENGTH];
    char          m_strCouponIdUnknown[MAX_COUPON_ID_LENGTH];
    int32_t       m_iEditBoxEvent;
    int32_t       m_iEditBoxEventUnknown;
    int8_t        m_byEvent[MAX_CURRENT_EVENT];
    bool          m_bUnknown;
    int8_t        m_bUnknown_PADDING[3];
    int8_t        m_FILL_UNKNOWN2[189];
    uint8_t       m_byMerchantState;
    uint16_t      m_sMerchantTarget;
    int8_t        m_FILL_UNKNOWN3[52];
    int32_t       m_iPartyStatus;
    int8_t        m_FILL_UNKNOWN4[116];
    uint8_t       m_byMonthRank;
    uint8_t       m_byGeneralRank;
};

#pragma pack(pop)
static_assert(sizeof(Type4Duration) == 8);
static_assert(sizeof(CUser) == 35470);
