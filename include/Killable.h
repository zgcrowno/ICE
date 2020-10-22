#pragma once

namespace ice
{
    class Killable
    {
    public:
        int m_iHP;

        virtual void Die() = 0;
        virtual void TakeDamage(const int _damage) = 0;
    };
}
