#include "CircuitSegment.h"

using namespace ice;

void CircuitSegment::OnCreate()
{

}

void CircuitSegment::OnDelete()
{

}

orxBOOL CircuitSegment::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void CircuitSegment::Update(const orxCLOCK_INFO& _rstInfo)
{

}

orxVECTOR CircuitSegment::GetCenterAtX(const float _x)
{
    float newYPos = orxLERP(
        m_vRightVertex.fY,
        m_vLeftVertex.fY,
        (m_vRightVertex.fX - _x) / (m_vRightVertex.fX - m_vLeftVertex.fX));
    return { _x, newYPos };
}
