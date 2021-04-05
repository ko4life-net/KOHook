#pragma once

#include "JvCryption.h"
#include "Common/Constants.h"

#include <winsock.h>

#pragma pack(push, 1)

class CIOCPSocket2
{
public:
    CIOCPSocket2()          = default;
    virtual ~CIOCPSocket2() = default;

    inline bool IsOnline() { return m_byState == SocketState::CONNECTED; };
    inline bool IsOffline() { return m_byState == SocketState::DISCONNECTED; };
    inline bool IsInGame() { return m_byState == SocketState::GAMESTART; };

public:
    int32_t     m_iUnknown_0x4;
    int16_t     m_sSocketErr;
    int16_t     m_sPending;
    int16_t     m_sWouldblock;
    int16_t     m_sUnknown_0xE;
    uintptr_t * m_pRegionBuffer;
    int32_t     m_iUnknown_0x14;
    uintptr_t * m_pCompressMng;
    uintptr_t * m_pIOCPort;
    uintptr_t * m_pBuffer;
    SOCKET      m_Socket;
    uint8_t     m_pRecvBuff[MAX_PACKET_SIZE];
    uint8_t     m_pSendBuff[MAX_PACKET_SIZE];
    HANDLE      m_hSockEvent;
    OVERLAPPED  m_RecvOverlapped;
    OVERLAPPED  m_SendOverlapped;
    uint8_t     m_byType;
    uint8_t     m_byState;
    int8_t      m_byState_PADDING[2];
    int32_t     m_iSid;
    char *      m_szConnectAddress;
    CJvCryption m_JvCryption;
    int32_t     m_CryptionFlag;
    int32_t     m_iUnknown_0x807C;
    T_KEY       m_PublicKey;
    uint32_t    m_Sen_val;
    uint32_t    m_Rec_val;
    int8_t      m_byUnknown_0x8090;
    int8_t      m_byUnknown_0x8090_PADDING[3];
    int32_t     m_iUnknown_0x8094;
};
#pragma pack(pop)
static_assert(sizeof(CIOCPSocket2) == 32920);