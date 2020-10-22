#include "Projectile.h"

using namespace ice;

void Projectile::OnCreate()
{

}

void Projectile::OnDelete()
{

}

orxBOOL Projectile::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void Projectile::Update(const orxCLOCK_INFO& _rstInfo)
{

}
