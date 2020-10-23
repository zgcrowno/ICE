#include "Level.h"
#include "Circuit.h"
#include "Player.h"
#include "Util.h"

using namespace ice;

void Level::OnCreate()
{
    Scene::OnCreate();

    if (orxConfig_PushSection(GetModelName()))
    {
        // Create level bounds.
        ScrollObject* levelBounds = ICE::GetInstance().CreateObject("O-LevelBounds");
        orxObject_SetOwner(levelBounds->GetOrxObject(), GetOrxObject());
        // Create player and place on designated starting circuit's leftmost terminal.
        const orxCHAR* playerStartingCircuit = orxConfig_GetString("PlayerStartingCircuit");
        for (ScrollObject* child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
        {
            if (!orxString_Compare(child->GetModelName(), playerStartingCircuit))
            {
                Player* player = static_cast<Player*>(ICE::GetInstance().CreateObject("O-Player"));
                orxObject_SetOwner(player->GetOrxObject(), GetOrxObject());
                if (orxConfig_PushSection(child->GetModelName()))
                {
                    orxVECTOR playerStartingPosition = orxVECTOR_0;
                    orxConfig_GetListVector("CircuitVertices", 0, &playerStartingPosition);
                    playerStartingPosition = Util::LevelGridToOrxSpace(playerStartingPosition);
                    orxVECTOR playerDefaultPosition = orxVECTOR_0;
                    player->GetPosition(playerDefaultPosition);
                    player->SetPosition({ playerStartingPosition.fX, playerStartingPosition.fY, playerDefaultPosition.fZ });
                    player->m_pCircuitSegment = static_cast<Circuit*>(child)->m_cCircuitSegments.at(0);

                    orxConfig_PopSection();
                }
            }
        }

        orxConfig_PopSection();
    }
}

void Level::OnDelete()
{

}

orxBOOL Level::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void Level::Update(const orxCLOCK_INFO& _rstInfo)
{

}
