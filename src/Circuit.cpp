#include "Circuit.h"
#include "Util.h"

using namespace ice;

void Circuit::OnCreate()
{
    if (orxConfig_PushSection(GetModelName()))
    {
        // Set up deactivation cooldown.
        m_fMaxDeactivationCooldown = orxConfig_GetFloat("MaxDeactivationCooldown");
        // Set up enemy spawn cooldown.
        m_fMinEnemySpawnCooldown = orxConfig_GetFloat("MinEnemySpawnCooldown");
        m_fMaxEnemySpawnCooldown = orxConfig_GetFloat("MaxEnemySpawnCooldown");
        m_fEnemySpawnCooldown = orxMath_GetRandomFloat(m_fMinEnemySpawnCooldown, m_fMaxEnemySpawnCooldown);
        // Set up circuit segments.
        int numCircuitVertices = orxConfig_GetListCount("CircuitVertices");
        if (numCircuitVertices >= 2)
        {
            for (int i = 0; i < numCircuitVertices - 1; ++i)
            {
                // Get the left and right vertices of the segment.
                orxVECTOR segmentLeftVec = orxVECTOR_0;
                orxVECTOR segmentRightVec = orxVECTOR_0;
                orxConfig_GetListVector("CircuitVertices", i, &segmentLeftVec);
                orxConfig_GetListVector("CircuitVertices", i + 1, &segmentRightVec);
                // Convert the config's grid values into Orx's world space.
                segmentLeftVec = Util::LevelGridToOrxSpace(segmentLeftVec);
                segmentRightVec = Util::LevelGridToOrxSpace(segmentRightVec);
                // Get the angle and distance between the left and right vertices of the segment
                float angleBetween = atan2f(segmentRightVec.fY - segmentLeftVec.fY, segmentRightVec.fX - segmentLeftVec.fX);
                float distanceBetween = orxVector_GetDistance(&segmentLeftVec, &segmentRightVec);
                // Create the segment.
                CircuitSegment* seg = static_cast<CircuitSegment*>(ICE::GetInstance().CreateObject("O-CircuitSegment"));
                // Get the segment's default position.
                orxVECTOR segmentDefaultPos = orxVECTOR_0;
                seg->GetPosition(segmentDefaultPos);
                // Get the segment's default scale.
                orxVECTOR segmentDefaultScale = orxVECTOR_0;
                seg->GetScale(segmentDefaultScale);
                // Set the position, rotation and scale of the segment as appropriate.
                seg->SetPosition({ segmentLeftVec.fX, segmentLeftVec.fY, segmentDefaultPos.fZ });
                seg->SetRotation(angleBetween);
                seg->SetScale({ distanceBetween, segmentDefaultScale.fY, segmentDefaultScale.fZ });
                // Set the segment's left and right vertices.
                seg->m_vLeftVertex = segmentLeftVec;
                seg->m_vRightVertex = segmentRightVec;
                // Ensure that the segment is owned by this circuit.
                orxObject_SetOwner(seg->GetOrxObject(), GetOrxObject());
                // Add the segment to m_cCircuitSegments.
                m_cCircuitSegments.push_back(seg);
            }
            // Set up the circuit segments' previous and next segments so that they can function like elements of a circularly linked list.
            for (int i = 0; i < m_cCircuitSegments.size(); i++)
            {
                CircuitSegment* seg = m_cCircuitSegments.at(i);
                if (i == 0)
                {
                    seg->m_pPreviousSegment = m_cCircuitSegments.at(m_cCircuitSegments.size() - 1);
                }
                else
                {
                    seg->m_pPreviousSegment = m_cCircuitSegments.at(i - 1);
                }
                if (i == m_cCircuitSegments.size() - 1)
                {
                    seg->m_pNextSegment = m_cCircuitSegments.at(0);

                    // Set up enemy.
                    m_pEnemy = static_cast<Enemy*>(ICE::GetInstance().CreateObject("O-Enemy"));
                    orxVECTOR enemyCurPos = orxVECTOR_0;
                    m_pEnemy->GetPosition(enemyCurPos);
                    m_pEnemy->m_vSpawnPosition = { seg->m_vRightVertex.fX, seg->m_vRightVertex.fY, enemyCurPos.fZ };
                    m_pEnemy->SetPosition(m_pEnemy->m_vSpawnPosition);
                    m_pEnemy->m_pCircuitSegment = seg;
                    m_pEnemy->m_pSpawnSegment = seg;
                    m_pEnemy->Enable(false);
                    orxObject_SetOwner(m_pEnemy->GetOrxObject(), GetOrxObject());
                }
                else
                {
                    seg->m_pNextSegment = m_cCircuitSegments.at(i + 1);
                }
            }
        }
        else
        {
            orxLOG("The number of CircuitSegments associated with circuit [%s] is less than 2. Ensure that it's more than 2, and retry.", GetModelName());
        }

        orxConfig_PopSection();
    }
}

void Circuit::OnDelete()
{

}

orxBOOL Circuit::OnCollide(
    ScrollObject* _poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR& _rvPosition,
    const orxVECTOR& _rvNormal)
{
    return orxTRUE;
}

void Circuit::Update(const orxCLOCK_INFO& _rstInfo)
{
    if (m_bActive)
    {
        if (IsActivated())
        {
            // Decrement enemy spawn cooldown.
            if (!m_pEnemy->IsEnabled() && m_fEnemySpawnCooldown > 0)
            {
                m_fEnemySpawnCooldown -= _rstInfo.fDT;
            }
            // Spawn enemy.
            else if (!m_pEnemy->IsEnabled())
            {
                m_fEnemySpawnCooldown = orxMath_GetRandomFloat(m_fMinEnemySpawnCooldown, m_fMaxEnemySpawnCooldown);

                m_pEnemy->Enable(true);
            }
        }
        else
        {
            m_fDeactivationCooldown -= _rstInfo.fDT;
            if (m_fDeactivationCooldown <= 0)
            {
                Activate(true);
            }
        }
    }
}

void Circuit::Activate(const bool _activate)
{
    orxCOLOR col = {};
    col.fAlpha = 1;
    if (_activate)
    {
        col.vRGB = { 0, 1, 0 };
    }
    else
    {
        m_fDeactivationCooldown = m_fMaxDeactivationCooldown;
        col.vRGB = { 1, 0, 0 };
    }
    for (CircuitSegment* seg : m_cCircuitSegments)
    {
        seg->SetColor(col);
    }
}

const bool Circuit::IsActivated() const
{
    return m_fDeactivationCooldown <= 0;
}
