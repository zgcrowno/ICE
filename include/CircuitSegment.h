#pragma once

#include "ICE.h"

namespace ice
{
    class CircuitSegment : public ScrollObject
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
        orxVECTOR m_vLeftVertex;
        orxVECTOR m_vRightVertex;
        CircuitSegment* m_pPreviousSegment;
        CircuitSegment* m_pNextSegment;

        orxVECTOR GetCenterAtX(const float _x);
    };
}
