#include "ProjectilePlayer.h"

using namespace ice;

void ProjectilePlayer::OnCreate()
{
    Projectile::OnCreate();
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
    
}
