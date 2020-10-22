#pragma once

#include "ICE.h"
#include "Activable.h"
#include "Killable.h"
#include <vector>
#include <map>

namespace ice
{
    class Boss : public ScrollObject, public Killable, public Activable
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
    private:
        void TakeDamage(const int _damage);
        void Die();
    public:
        float m_fAbilityCooldown;
        std::vector<const orxCHAR*> m_cAbilityNames;
        std::map<const orxCHAR*, std::pair<const float, const float>> m_mAbilityMap;
    };
}
