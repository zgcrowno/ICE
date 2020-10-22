#pragma once

#include "ICE.h"
#include "Mover.h"

namespace ice
{
    class Projectile : public ScrollObject
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
