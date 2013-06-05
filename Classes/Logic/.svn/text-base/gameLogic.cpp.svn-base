//
//  gameLogic.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-5-23.
//
//

#include "gameLogic.h" 

namespace PtGameLogic {
    
    STC_HOUSEPOS getGoPos(STC_HOUSEPOS& vPos, int& nFaceTo, int nTurnTo)
    {
        vPos.x += aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_X];
        vPos.y += aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_Y];
        vPos.z += aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_Z];
        nFaceTo = calFaceTo(nFaceTo, nTurnTo);
        return vPos;
    }
    
    int calFaceTo(int& nFaceTo, int nTurnTo)
    {
        int nValTurnTo =aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_X] +
                        aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_Y]*2 +
                        aaanCoordinate[nFaceTo][nTurnTo][EN_COORDINATE_Z]*4 +
                        EN_VAL_TURNTO_BASE;
        switch (nValTurnTo) {
            case EN_VAL_TURNTO_PX:
                nFaceTo = EN_FACETO_PX;
                break;
            case EN_VAL_TURNTO_NX:
                nFaceTo = EN_FACETO_NX;
                break;
            case EN_VAL_TURNTO_PY:
                nFaceTo = EN_FACETO_PY;
                break;
            case EN_VAL_TURNTO_NY:
                nFaceTo = EN_FACETO_NY;
                break;
            case EN_VAL_TURNTO_PZ:
                nFaceTo = EN_FACETO_PZ;
                break;
            case EN_VAL_TURNTO_NZ:
                nFaceTo = EN_FACETO_NZ;
                break;
        }
        return nFaceTo;
    }
}
