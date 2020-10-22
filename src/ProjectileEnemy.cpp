#include "ProjectileEnemy.h"

using namespace ice;

void ProjectileEnemy::OnCreate()
{

}

void ProjectileEnemy::OnDelete()
{

}

orxBOOL ProjectileEnemy::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void ProjectileEnemy::Update(const orxCLOCK_INFO& _rstInfo)
{

}
