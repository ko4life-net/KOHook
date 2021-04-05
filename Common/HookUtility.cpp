#include "pch.h"
#include "HookUtility.h"

void Hook::PatchBytes(offset_t address, const std::vector<byte_t> & bytes)
{
    offset_t protectOld, protectNew;
    auto     hook = reinterpret_cast<void *>(address);
    VirtualProtect(hook, bytes.size(), PAGE_EXECUTE_READWRITE, &protectOld);
    memmove(hook, bytes.data(), bytes.size());
    VirtualProtect(hook, bytes.size(), protectOld, &protectNew);
}

void Hook::PatchFill(offset_t address, byte_t value, uint32_t size)
{
    std::vector<byte_t> bytes(size, value);
    PatchBytes(address, bytes);
}

void Hook::PatchDetour(offset_t address, void * callback, Opcode type /*= Opcode::JMP*/)
{
    if (type != Opcode::JMP && type != Opcode::CALL) return;

    std::vector<byte_t> bytes(5, 0);
    bytes[0] = type;
    *reinterpret_cast<offset_t *>(&bytes[1]) = reinterpret_cast<offset_t>(callback) - address - bytes.size();
    PatchBytes(address, bytes);
}