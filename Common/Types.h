#pragma once

#include <stdint.h>
#include <list>

// Making sure that std::list is always 12 bytes
template <typename T>
class ListWrap : public std::list<T>
{
#if NDEBUG
    int32_t dummy;
#endif
};
static_assert(sizeof(ListWrap<int>) == 12);

typedef unsigned char byte_t;
typedef unsigned long offset_t;
