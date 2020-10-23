#pragma once

#include "ICE.h"
#include "Activable.h"
#include "CircuitSegment.h"
#include "Enemy.h"
#include <vector>

namespace ice
{
    class Circuit : public ScrollObject, public Activable
    {
    protected:
        virtual void OnCreate();
        virtual void OnDelete();
        virtual orxBOOL OnCollide(
            ScrollObject* _poCollider,
            const orxSTRING _zPartName,
            const orxSTRING _zColliderPartName,
            const orxVECTOR& _rvPosition,
            const orxVECTOR& _rvNormal);
        virtual void Update(const orxCLOCK_INFO& _rstInfo);
    public:
        bool m_bAllowsTerminalTeleportation;
        float m_fMaxDeactivationCooldown;
        float m_fDeactivationCooldown;
        float m_fMinEnemySpawnCooldown;
        float m_fMaxEnemySpawnCooldown;
        float m_fEnemySpawnCooldown;
        Enemy* m_pEnemy;
        std::vector<CircuitSegment*> m_cCircuitSegments;

        void Activate(const bool _activate);
        const bool IsActivated() const;
    };
}
