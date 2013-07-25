//
//  CEffectInterfaceOne.h
//  91.cube
//
//  Created by linminglu on 13-7-23.
//
//

#ifndef ___1_cube__CEffectInterfaceOne__
#define ___1_cube__CEffectInterfaceOne__

#include <iostream>
#include "CEffectInterface.h"
class CEffectInterfaceOne :public CEffectInterface
{
public:
    CEffectInterfaceOne();
    ~CEffectInterfaceOne();
    void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact);
};
#endif /* defined(___1_cube__CEffectInterfaceOne__) */
