#pragma once

#include "HookUtility.h"

namespace KOHook
{
    // TODO: Implement base common implementation for a patch. e.g.
    // class PatchDrainLimit : public Patch {};
    class Patch
    {
    public:
        Patch(offset_t target, offset_t retn) {}
        virtual ~Patch() {}

        virtual bool Apply()  = 0;
        virtual bool Remove() = 0;
    };

} // namespace KOHook