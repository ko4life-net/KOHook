#pragma once

typedef unsigned char T_OCTET;
typedef __int64       T_KEY;

class CJvCryption
{
public:
    CJvCryption();
    ~CJvCryption();

    void SetPublicKey(T_KEY pk);
    void SetPrivateKey(T_KEY pk);

    void Init();

    // NOTE: Fill the implementation for encryption only if needed.

private:
    T_KEY m_PublicKey;
    T_KEY m_PrivateKey;
    T_KEY m_TKey;
};
static_assert(sizeof(CJvCryption) == 24);