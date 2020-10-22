#include "ProjectilePlayer.h"

using namespace ice;

void ProjectilePlayer::OnCreate()
{
    Mover::OnCreate();
}

void ProjectilePlayer::OnDelete()
{
    
}

orxBOOL ProjectilePlayer::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void ProjectilePlayer::Update(const orxCLOCK_INFO& _rstInfo)
{
    MoveLeftRight(false, _rstInfo.fDT);
}

void ProjectilePlayer::TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos)
{
    if (_left)
    {
        m_pCircuitSegment = m_pCircuitSegment->m_pPreviousSegment;
        SetPosition({ m_pCircuitSegment->m_vRightVertex.fX, m_pCircuitSegment->m_vRightVertex.fY, _curPos.fZ });
    }
    else
    {
        // If the player projectile has reached the end of the rightmost circuit segment on the circuit,
        // destroy it.
        if (m_pCircuitSegment->m_vRightVertex.fX > m_pCircuitSegment->m_pNextSegment->m_vRightVertex.fX)
        {
            SetLifeTime(0);
        }
        else
        {
            m_pCircuitSegment = m_pCircuitSegment->m_pNextSegment;
            SetPosition({ m_pCircuitSegment->m_vLeftVertex.fX, m_pCircuitSegment->m_vLeftVertex.fY, _curPos.fZ });
        }
    }
}
