#include "Util.h"

using namespace ice;

orxVECTOR Util::LevelGridToOrxSpace(const orxVECTOR& _gridVec)
{
    orxVECTOR retVal = orxVECTOR_0;
    if (orxConfig_PushSection("O-Level"))
    {
        orxVECTOR levelSize = orxVECTOR_0;
        orxConfig_GetVector("Size", &levelSize);
        orxVECTOR gridCellSize = orxVECTOR_0;
        orxConfig_GetVector("GridCellSize", &gridCellSize);
        retVal = {
            (gridCellSize.fX / 2) + _gridVec.fX * gridCellSize.fX - (levelSize.fX / 2),
            (gridCellSize.fY / 2) + _gridVec.fY * gridCellSize.fY - (levelSize.fY / 2),
            _gridVec.fZ };

        orxConfig_PopSection();
    }
    return retVal;
}
