#include "Mover.h"
#include "Circuit.h"

using namespace ice;

void Mover::OnCreate()
{
    if (orxConfig_PushSection(GetModelName()))
    {
        m_fMoveSpeed = orxConfig_GetFloat("MoveSpeed");

        orxConfig_PopSection();
    }
}

void Mover::OnDelete()
{

}

orxBOOL Mover::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void Mover::Update(const orxCLOCK_INFO& _rstInfo)
{

}

void Mover::MoveLeftRight(const bool _left, const float _fDT)
{
    // Get the mover's current position.
    orxVECTOR curPos = orxVECTOR_0;
    GetPosition(curPos);
    // Set the mover's new position.
    float newXPos = _left ? curPos.fX - m_fMoveSpeed * _fDT : curPos.fX + m_fMoveSpeed * _fDT;
    float newYPos = orxLERP(
        m_pCircuitSegment->m_vRightVertex.fY,
        m_pCircuitSegment->m_vLeftVertex.fY,
        (m_pCircuitSegment->m_vRightVertex.fX - newXPos) / (m_pCircuitSegment->m_vRightVertex.fX - m_pCircuitSegment->m_vLeftVertex.fX));
    SetPosition({ newXPos, newYPos, curPos.fZ });
    // Transition the mover between circuit segment's if appropriate.
    if (_left && newXPos <= m_pCircuitSegment->m_vLeftVertex.fX
        || !_left && newXPos >= m_pCircuitSegment->m_vRightVertex.fX)
    {
        TransitionToCircuitSegment(_left, curPos);
    }
}

void Mover::MoveUpDown(const bool _up)
{
    const float distance = _up ? -10000 : 10000;
    orxVECTOR directionVec = { 0, distance };
    orxVECTOR raycastBegin = orxVECTOR_0;
    GetPosition(raycastBegin);
    orxVECTOR raycastEnd = orxVECTOR_0;
    orxVector_Add(&raycastEnd, &raycastBegin, &directionVec);
    if (RaycastAndHopToActiveCircuit(raycastBegin, raycastEnd, directionVec))
    {
        return;
    }
    else
    {
        GetPosition(raycastBegin);
        raycastEnd = raycastBegin;
        raycastBegin = { raycastBegin.fX, raycastBegin.fY - distance };
        RaycastAndHopToActiveCircuit(raycastBegin, raycastEnd, directionVec);
    }
}

void Mover::HopToCircuitSegment(orxOBJECT* _circuitSegment)
{
    m_pCircuitSegment = static_cast<CircuitSegment*>(orxObject_GetUserData(_circuitSegment));
    orxVECTOR curPos = orxVECTOR_0;
    GetPosition(curPos);
    orxVECTOR centerAtX = m_pCircuitSegment->GetCenterAtX(curPos.fX);
    SetPosition({ centerAtX.fX, centerAtX.fY, curPos.fZ });
}

const bool Mover::RaycastAndHopToActiveCircuit(
    orxVECTOR& _raycastBegin,
    orxVECTOR& _raycastEnd,
    const orxVECTOR& _directionVec)
{
    orxVECTOR hit = orxVECTOR_0;
    orxVECTOR normal = orxVECTOR_0;
    orxOBJECT* hitCircuitSegment = orxObject_Raycast(&_raycastBegin, &_raycastEnd, 0xFFFF, orxPhysics_GetCollisionFlagValue("circuitSegment"), false, &hit, &normal);
    while (hitCircuitSegment != nullptr)
    {
        Circuit* hitCircuit = static_cast<Circuit*>(orxObject_GetUserData(orxOBJECT(orxObject_GetOwner(hitCircuitSegment))));
        if (hitCircuit->IsActivated())
        {
            Circuit* curCircuit = static_cast<Circuit*>(orxObject_GetUserData(orxOBJECT(orxObject_GetOwner(m_pCircuitSegment->GetOrxObject()))));
            if (hitCircuit != curCircuit)
            {
                HopToCircuitSegment(hitCircuitSegment);
                return true;
            }
            else
            {
                IterateCircuitRaycast(hitCircuitSegment, _raycastBegin, _raycastEnd, _directionVec);
            }
        }
        else
        {
            IterateCircuitRaycast(hitCircuitSegment, _raycastBegin, _raycastEnd, _directionVec);
        }
    }

    return false;
}

void Mover::IterateCircuitRaycast(
    orxOBJECT* _hitCircuitSegment,
    orxVECTOR& _raycastBegin,
    orxVECTOR& _raycastEnd,
    const orxVECTOR& _directionVec)
{
    orxVECTOR curPos = orxVECTOR_0;
    GetPosition(curPos);
    CircuitSegment* seg = static_cast<CircuitSegment*>(orxObject_GetUserData(_hitCircuitSegment));
    orxVECTOR hitCircuitSegmentCenter = seg->GetCenterAtX(curPos.fX);
    _raycastBegin = { hitCircuitSegmentCenter.fX, hitCircuitSegmentCenter.fY, curPos.fZ };
    _raycastEnd = *orxVector_Add(&_raycastEnd, &_raycastBegin, &_directionVec);
    orxVECTOR hit = orxVECTOR_0;
    orxVECTOR normal = orxVECTOR_0;
    _hitCircuitSegment = orxObject_Raycast(&_raycastBegin, &_raycastEnd, 0xFFFF, orxPhysics_GetCollisionFlagValue("circuitSegment"), false, &hit, &normal);
}
