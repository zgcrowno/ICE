#include "Player.h"
#include "Enemy.h"
#include "ProjectileEnemy.h"
#include "ProjectilePlayer.h"

using namespace ice;

void Player::OnCreate()
{
    Mover::OnCreate();

    if (orxConfig_PushSection(GetModelName()))
    {
        m_iHP = orxConfig_GetU32("HP");
        m_iMaxActiveProjectiles = orxConfig_GetU32("MaxActiveProjectiles");
        m_fMaxWaveDelay = orxConfig_GetFloat("MaxWaveDelay");

        orxConfig_PopSection();
    }
}

void Player::OnDelete()
{

}

orxBOOL Player::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    // TODO: Probably replace this dynamic casts with collider part name checks.
    ProjectileEnemy* enemyProjectile = dynamic_cast<ProjectileEnemy*>(_poCollider);
    if (enemyProjectile != nullptr)
    {
        TakeDamage(1);
    }
    else
    {
        Enemy* enemy = dynamic_cast<Enemy*>(_poCollider);
        if (enemy != nullptr)
        {
            TakeDamage(1);
            enemy->Die();
        }
    }

    return orxTRUE;
}

void Player::Update(const orxCLOCK_INFO& _rstInfo)
{
    if (m_bActive)
    {
        if (m_iHP > 0)
        {
            // INPUTS
            // Move left or right.
            if (orxInput_IsActive("Left"))
            {
                MoveLeftRight(true, _rstInfo.fDT);
            }
            if (orxInput_IsActive("Right"))
            {
                MoveLeftRight(false, _rstInfo.fDT);
            }
            // Hop up or down between circuits.
            if (orxInput_HasBeenActivated("Up"))
            {
                MoveUpDown(true);
            }
            else if (orxInput_HasBeenActivated("Down"))
            {
                MoveUpDown(false);
            }
            // Shoot projectiles.
            if (orxInput_IsActive("Shoot"))
            {
                Shoot();
            }
            // DECREMENTS
            if (m_fWaveDelay > 0)
            {
                m_fWaveDelay -= _rstInfo.fDT;
            }
        }
    }
}

void Player::TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos)
{
    if (_left)
    {
        m_pCircuitSegment = m_pCircuitSegment->m_pPreviousSegment;
        SetPosition({ m_pCircuitSegment->m_vRightVertex.fX, m_pCircuitSegment->m_vRightVertex.fY, _curPos.fZ });
    }
    else
    {
        m_pCircuitSegment = m_pCircuitSegment->m_pNextSegment;
        SetPosition({ m_pCircuitSegment->m_vLeftVertex.fX, m_pCircuitSegment->m_vLeftVertex.fY, _curPos.fZ });
    }
}

void Player::Die()
{
    orxOBJECT* level = orxOBJECT(orxObject_GetOwner(GetOrxObject()));
    orxObject_AddTimeLineTrack(level, "TT-LevelFailed");
}

void Player::TakeDamage(const int _damage)
{
    m_iHP -= _damage;
    if (m_iHP <= 0)
    {
        Die();
    }
}

void Player::Shoot()
{
    if (m_fWaveDelay <= 0 && (m_iMaxActiveProjectiles < 0 || ICE::GetInstance().GetPlayerProjectiles().size() < m_iMaxActiveProjectiles))
    {
        m_fWaveDelay = m_fMaxWaveDelay;
        orxVECTOR curPos = orxVECTOR_0;
        GetPosition(curPos);
        ProjectilePlayer* shot = static_cast<ProjectilePlayer*>(ICE::GetInstance().CreateObject("O-ProjectilePlayer"));
        shot->SetPosition(curPos);
        shot->m_pCircuitSegment = m_pCircuitSegment;
        orxObject_SetOwner(shot->GetOrxObject(), GetOrxObject());
    }
}
