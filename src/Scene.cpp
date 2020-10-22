#include "Scene.h"

using namespace ice;

void Scene::OnCreate()
{
    
}

void Scene::OnDelete()
{

}

orxBOOL Scene::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void Scene::Update(const orxCLOCK_INFO& _rstInfo)
{

}
