#pragma once

#include "IOCPSocket2.h"

#pragma pack(push, 1)
class CIOCPort
{
public:
    CIOCPort()          = default;
    virtual ~CIOCPort() = default;

public:
    SOCKET          m_ListenSocket;
    HANDLE          m_hListenEvent;
    HANDLE          m_hServerIOCPort;
    HANDLE          m_hClientIOCPort;
    HANDLE          m_hSendIOCPort;
    HANDLE          m_hAcceptThread;
    OVERLAPPED      m_PostOverlapped;
    int             m_SocketArraySize;
    int             m_ClientSockSize;
    ListWrap<int>   m_SidList;
    CIOCPSocket2 ** m_SockArray;
    CIOCPSocket2 ** m_SockArrayInActive;
    CIOCPSocket2 ** m_ClientSockArray;
    DWORD           m_dwNumberOfWorkers;
    DWORD           m_dwConcurrency;
};
#pragma pack(pop)
static_assert(sizeof(CIOCPort) == 88);