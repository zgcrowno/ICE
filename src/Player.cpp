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

        orxConfig_PopSection();
    }
    for (ScrollObject* child = GetOwnedChild(); child; child = child->GetOwnedChild())
    {
        if (!orxString_Compare(child->GetModelName(), "O-ProjectilePlayerSpawner"))
        {
            m_pProjectilePlayerSpawner = orxSPAWNER(_orxObject_GetStructure(GetOwnedChild()->GetOrxObject(), orxSTRUCTURE_ID_SPAWNER));
        }
    }
    orxSpawner_Enable(m_pProjectilePlayerSpawner, false);
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
                orxSpawner_Enable(m_pProjectilePlayerSpawner, true);
            }
            else
            {
                orxSpawner_Enable(m_pProjectilePlayerSpawner, false);
            }
        }
    }
    // Set aiming rotation of player.
    orxVECTOR curPos = orxVECTOR_0;
    GetPosition(curPos);
    orxVECTOR mousePos = orxVECTOR_0;
    orxMouse_GetPosition(&mousePos);
    orxRender_GetWorldPosition(&mousePos, orxViewport_Get("MainViewport"), &mousePos);
    // Get the angle between the player and the mouse.
    float angleBetween = atan2f(mousePos.fY - curPos.fY, mousePos.fX - curPos.fX);
    SetRotation(angleBetween);
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
