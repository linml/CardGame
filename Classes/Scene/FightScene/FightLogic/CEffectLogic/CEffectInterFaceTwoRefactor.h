//
//  CEffectInterFaceTwoRefactor.h
//  91.cube
//
//  Created by linminglu on 13-11-1.
//
//

#ifndef ___1_cube__CEffectInterFaceTwoRefactor__
#define ___1_cube__CEffectInterFaceTwoRefactor__

#include <iostream>
#include "CEffectInterface.h"

class CEffectInterFaceTwoRefactor:public CEffectInterface
{
public:
    CEffectInterFaceTwoRefactor();
    ~CEffectInterFaceTwoRefactor();
    void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact);
};
#endif /* defined(___1_cube__CEffectInterFaceTwoRefactor__) */
