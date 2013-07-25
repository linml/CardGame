//
//  CEffectInterfaceEleven.h
//  91.cube
//
//  Created by linminglu on 13-7-25.
//
//

#ifndef ___1_cube__CEffectInterfaceEleven__
#define ___1_cube__CEffectInterfaceEleven__

#include <iostream>
#include "CEffectInterface.h"
class CEffectInterfaceEleven :public CEffectInterface
{
public:
    CEffectInterfaceEleven();
    ~CEffectInterfaceEleven();
    void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact);
};
#endif /* defined(___1_cube__CEffectInterfaceEleven__) */
