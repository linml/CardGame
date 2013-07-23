//
//  CEffectInterface.h
//  91.cube
//
//  Created by linminglu on 13-7-23.
//
//

#ifndef ___1_cube__CEffectInterface__
#define ___1_cube__CEffectInterface__

#include <iostream>
//定义一个计算effect的 抽象类
class CFightCard;
class CImapact;
class CEffectInterface
{
public:
    CEffectInterface();
    virtual ~CEffectInterface();
    virtual void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)=0;
    
};

#endif /* defined(___1_cube__CEffectInterface__) */
