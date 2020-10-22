#pragma once

#include "ICE.h"
#include "Activable.h"
#include "Killable.h"
#include "Mover.h"

namespace ice
{
    class Player : public Mover, public Killable, public Activable
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
        int m_iMaxActiveProjectiles;
        float m_fMaxWaveDelay;
        float m_fWaveDelay;

        void TransitionToCircuitSegment(const bool _left, const orxVECTOR& _curPos);
        void Die();
        void TakeDamage(const int _damage);
        void Shoot();
    };
}
