#include "LevelBound.h"

using namespace ice;

void LevelBound::OnCreate()
{

}

void LevelBound::OnDelete()
{

}

orxBOOL LevelBound::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    // Only projectiles may collide with a level bound, so we can feel perfectly comfortable
    // deleting any object which prompts a collision event.
    _poCollider->SetLifeTime(0);
    
    return orxTRUE;
}

void LevelBound::Update(const orxCLOCK_INFO& _rstInfo)
{

}
