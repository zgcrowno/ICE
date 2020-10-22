#pragma once

#include "ICE.h"
#include "CircuitSegment.h"

namespace ice
{
    class Mover : public ScrollObject
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
        float m_fMoveSpeed;
        CircuitSegment* m_pCircuitSegment;

        void MoveLeftRight(const bool _left, const float _fDT);
        void MoveUpDown(const bool _up);
        virtual void TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos) = 0;
        void HopToCircuitSegment(orxOBJECT* _circuitSegment);
        // Returns true if an appropriate circuit has been found.
        const bool RaycastAndHopToActiveCircuit(
            orxVECTOR& _raycastBegin,
            orxVECTOR& _raycastEnd,
            const orxVECTOR& _directionVec);
    };
}
