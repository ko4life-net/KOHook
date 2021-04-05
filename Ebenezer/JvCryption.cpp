#include "pch.h"
#include "JvCryption.h"

CJvCryption::CJvCryption()
    : m_PublicKey(0xAFFA12875C643D9B)
    , m_PrivateKey(0x1234567890123456)
    , m_TKey(0)
{
}

CJvCryption::~CJvCryption() = default;

void CJvCryption::SetPublicKey(T_KEY pk)
{
    m_PublicKey = pk;
    Init();
}

void CJvCryption::SetPrivateKey(T_KEY pk)
{
    m_PrivateKey = pk;
    Init();
}

void CJvCryption::Init()
{
    m_TKey = m_PublicKey ^ m_PrivateKey;
}
