#pragma once

#include "ICE.h"

namespace ice
{
    class LevelBound : public ScrollObject
    {
    protected:
        virtual void OnCreate();
        virtual void OnDelete();
        virtual orxBOOL OnCollide(
            ScrollObject* _poCollider,
            const orxSTRING _zPartName,
            const orxSTRING _zColliderPartName,
            const orxVECTOR& _rvPosition,
            const orxVECTOR& _rvNormal);
        virtual void Update(const orxCLOCK_INFO& _rstInfo);
    };
}
