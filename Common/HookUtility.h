#pragma once
#pragma warning(disable : 26812) // enum class

#include "Types.h"
#include <vector>

class Hook
{
public:
    enum Opcode : byte_t
    {
        LEA      = 0x8D,
        NOP      = 0x90,
        CALL     = 0xE8,
        JMP      = 0xE9,
        JMPShort = 0xEB,
        RET      = 0xC3,
    };

    static void PatchBytes(offset_t address, const std::vector<byte_t> & bytes);
    static void PatchFill(offset_t address, byte_t value, uint32_t size);
    static void PatchDetour(offset_t address, void * callback, Opcode type = Opcode::JMP);
};