#pragma once

#include "ICE.h"
#include "Projectile.h"

namespace ice
{
    class ProjectilePlayer : public Mover
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
    public:
        void TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos);
    };
}
