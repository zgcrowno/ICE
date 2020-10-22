/**
 * @file ICE.cpp
 * @date 13-Oct-2020
 */

#define __SCROLL_IMPL__
#include "ICE.h"
#undef __SCROLL_IMPL__

// BEGIN Use non-dedicated graphics card, if appropriate.
#ifdef __orxWINDOWS__
#include <windows.h>

extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
// END Use non-dedicated graphics card, if appropriate.

#include "Activable.h"
#include "Object.h"
#include "Boss.h"
#include "Circuit.h"
#include "CircuitSegment.h"
#include "Enemy.h"
#include "Level.h"
#include "LevelBound.h"
#include "Player.h"
#include "Projectile.h"
#include "ProjectileEnemy.h"
#include "ProjectilePlayer.h"
#include "Scene.h"

using namespace ice;

/** Update function, it has been registered to be called every tick of the core clock
 */
void ICE::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ICE::Init()
{
    // Add the event handler.
    orxEvent_AddHandler(orxEVENT_TYPE_SPAWNER, EventHandler);
    orxEvent_AddHandler(orxEVENT_TYPE_OBJECT, EventHandler);
    // Create the scene
    CreateObject("O-LevelTest");

    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it should not contain any game logic
 */
orxSTATUS ICE::Run()
{
    // Return orxSTATUS_FAILURE to instruct orx to quit
    return orxSTATUS_SUCCESS;
}

/** Exit function, it is called before exiting from orx
 */
void ICE::Exit()
{
    // Let Orx clean all our mess automatically. :)
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ICE::BindObjects()
{
    // Bind the Object class to the Object config section
    ScrollBindObject<Object>("Object");

    ScrollBindObject<Boss>("O-Boss");
    ScrollBindObject<Circuit>("O-Circuit");
    ScrollBindObject<CircuitSegment>("O-CircuitSegment");
    ScrollBindObject<Enemy>("O-Enemy");
    ScrollBindObject<Level>("O-Level");
    ScrollBindObject<LevelBound>("O-LevelBound");
    ScrollBindObject<Player>("O-Player");
    ScrollBindObject<Projectile>("O-Projectile");
    ScrollBindObject<ProjectileEnemy>("O-ProjectileEnemy");
    ScrollBindObject<ProjectilePlayer>("O-ProjectilePlayer");
    ScrollBindObject<Scene>("O-Scene");
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ICE::Bootstrap() const
{
    // Add a config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

std::vector<ScrollObject*> ICE::GetPlayerProjectiles() const
{
    std::vector<ScrollObject*> retVal;

    for (ProjectilePlayer* playerProjectile = GetNextObject<ProjectilePlayer>(); playerProjectile; playerProjectile = GetNextObject<ProjectilePlayer>(playerProjectile))
    {
        retVal.push_back(playerProjectile);
    }

    return retVal;
}

orxSTATUS orxFASTCALL ICE::EventHandler(const orxEVENT* _pstEvent)
{
    switch (_pstEvent->eType)
    {
    case orxEVENT_TYPE_SPAWNER:
        switch (_pstEvent->eID)
        {
        // Ensure that all objects created by an orxSPAWNER are owned by the object which owns said orxSPAWNER.
        // This way, we can restart levels using the command module, and even formerly un-owned objects will be
        // appropriately deleted.
        case orxSPAWNER_EVENT_SPAWN:
            orxObject_SetOwner(orxOBJECT(_pstEvent->hRecipient), orxOBJECT(orxStructure_GetOwner(orxSPAWNER(_pstEvent->hSender))));
            break;
        }
        break;
    case orxEVENT_TYPE_OBJECT:
        switch (_pstEvent->eID)
        {
        case orxOBJECT_EVENT_DELETE:
            // The level should only perform any real actions after the level start text has been deleted,
            // so we activate all activables upon the level start text's deletion.
            if (!orxString_Compare(orxObject_GetName(orxOBJECT(_pstEvent->hSender)), "O-LevelStart"))
            {
                for (orxOBJECT* obj = orxOBJECT(orxStructure_GetFirst(orxSTRUCTURE_ID_OBJECT));
                    obj != nullptr;
                    obj = orxOBJECT(orxStructure_GetNext(obj)))
                {
                    ScrollObject* scrollObj = (ScrollObject*)(orxObject_GetUserData(obj));
                    Activable* activable = dynamic_cast<Activable*>(scrollObj);
                    if (activable != nullptr)
                    {
                        activable->m_bActive = true;
                    }
                }
            }
            break;
        }
        break;
    }

    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Execute our game
    ICE::GetInstance().Execute(argc, argv);

    // Done!
    return EXIT_SUCCESS;
}
