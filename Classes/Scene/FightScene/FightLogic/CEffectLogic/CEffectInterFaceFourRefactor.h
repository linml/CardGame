//
//  CEffectInterFaceFourRefactor.h
//  91.cube
//
//  Created by linminglu on 13-11-4.
//
//

#ifndef ___1_cube__CEffectInterFaceFourRefactor__
#define ___1_cube__CEffectInterFaceFourRefactor__

#include <iostream>
#include "CEffectInterface.h"
class CEffectInterFaceFourRefactor :public CEffectInterface
{
public:
    CEffectInterFaceFourRefactor();
    ~CEffectInterFaceFourRefactor();
    void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact);
};
#endif /* defined(___1_cube__CEffectInterFaceFourRefactor__) */
