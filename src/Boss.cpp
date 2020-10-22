#include "Boss.h"
#include "ProjectilePlayer.h"

using namespace ice;

void Boss::OnCreate()
{
    if (orxConfig_PushSection(GetModelName()))
    {
        m_iHP = orxConfig_GetU32("HP");

        if (orxConfig_GetListCount("Abilities") % 3 == 0)
        {
            for (int i = 0; i < orxConfig_GetListCount("Abilities"); i += 3)
            {
                m_cAbilityNames.push_back(orxConfig_GetListString("Abilities", i));
                m_mAbilityMap.insert({ orxConfig_GetListString("Abilities", i), { orxConfig_GetListFloat("Abilities", i + 1), orxConfig_GetListFloat("Abilities", i + 2) } });
            }
        }
        else
        {
            orxLOG("%s's [Abilities] list is not a multiple of 3. Ensure it's a multiple of 3 and try again.", GetModelName());
        }

        orxConfig_PopSection();
    }
    // Disable child spawners by default
    for (ScrollObject* child = GetOwnedChild(); child; child = child->GetOwnedSibling())
    {
        child->Enable(false);
    }
}

void Boss::OnDelete()
{

}

orxBOOL Boss::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    ProjectilePlayer* playerProjectile = ScrollCast<ProjectilePlayer*>(_poCollider);
    if (playerProjectile != nullptr)
    {
        TakeDamage(1);
        playerProjectile->SetLifeTime(0);
    }

    return orxTRUE;
}

void Boss::Update(const orxCLOCK_INFO& _rstInfo)
{
    if (m_bActive)
    {
        if (m_iHP > 0)
        {
            if (m_fAbilityCooldown <= 0)
            {
                const orxCHAR* randomAbilityName = m_cAbilityNames.at(orxMath_GetRandomU32(0, m_cAbilityNames.size() - 1));
                orxObject_AddTimeLineTrack(GetOrxObject(), randomAbilityName);
                auto abilityCooldownRange = m_mAbilityMap.at(randomAbilityName);
                m_fAbilityCooldown = orxMath_GetRandomFloat(abilityCooldownRange.first, abilityCooldownRange.second);
            }
            else
            {
                m_fAbilityCooldown -= _rstInfo.fDT;
            }
        }
    }
}

void Boss::TakeDamage(const int _damage)
{
    m_iHP -= _damage;
    if (m_iHP <= 0)
    {
        Die();
    }
}

void Boss::Die()
{
    orxOBJECT* level = orxOBJECT(orxObject_GetOwner(GetOrxObject()));
    orxObject_AddTimeLineTrack(level, "TT-LevelCompleted");
}
