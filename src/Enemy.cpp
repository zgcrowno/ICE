#include "Enemy.h"
#include "Circuit.h"
#include "ProjectilePlayer.h"

using namespace ice;

void Enemy::OnCreate()
{
    Mover::OnCreate();

    if (orxConfig_PushSection(GetModelName()))
    {
        m_iHP = orxConfig_GetU32("HP");

        orxConfig_PopSection();
    }
}

void Enemy::OnDelete()
{

}

orxBOOL Enemy::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    ProjectilePlayer* playerProjectile = dynamic_cast<ProjectilePlayer*>(_poCollider);
    if (playerProjectile != nullptr)
    {
        TakeDamage(1);
        playerProjectile->SetLifeTime(0);
    }

    return orxTRUE;
}

void Enemy::Update(const orxCLOCK_INFO& _rstInfo)
{
    if (IsEnabled())
    {
        MoveLeftRight(true, _rstInfo.fDT);
    }
}

void Enemy::TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos)
{
    if (_left)
    {
        // If the enemy has reached the origin of the leftmost circuit segment on the circuit,
        // deactivate the circuit, and call the enemy's Die function.
        if (m_pCircuitSegment->m_vLeftVertex.fX < m_pCircuitSegment->m_pPreviousSegment->m_vLeftVertex.fX)
        {
            static_cast<Circuit*>(orxObject_GetUserData(orxOBJECT(orxObject_GetOwner(m_pCircuitSegment->GetOrxObject()))))->Activate(false);
            Die();
        }
        else
        {
            m_pCircuitSegment = m_pCircuitSegment->m_pPreviousSegment;
            SetPosition({ m_pCircuitSegment->m_vRightVertex.fX, m_pCircuitSegment->m_vRightVertex.fY, _curPos.fZ });
        }
    }
    else
    {
        m_pCircuitSegment = m_pCircuitSegment->m_pNextSegment;
        SetPosition({ m_pCircuitSegment->m_vLeftVertex.fX, m_pCircuitSegment->m_vLeftVertex.fY, _curPos.fZ });
    }
}

void Enemy::Die()
{
    m_pCircuitSegment = m_pSpawnSegment;
    SetPosition(m_vSpawnPosition);
    Enable(false);
}

void Enemy::TakeDamage(const int _damage)
{
    m_iHP -= _damage;
    if (m_iHP <= 0)
    {
        Die();
    }
}
