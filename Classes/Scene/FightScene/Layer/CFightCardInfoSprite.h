//
//  CFightCardInfoSprite.h
//  91.cube
//
//  Created by linminglu on 14-1-2.
//
//

#ifndef ___1_cube__CFightCardInfoSprite__
#define ___1_cube__CFightCardInfoSprite__

#include <iostream>
#include "cocos2d.h"
#include "CGamesCard.h"
using namespace cocos2d;
class CFightCard;
class CFightCardInfoSprite :public CGamesCard
{
public:
    CFightCardInfoSprite();
    ~CFightCardInfoSprite();
    static CFightCardInfoSprite * CreateSprite(CFightCard *pCard,int nIndex);
    bool initSprite(CFightCard *pCard,int nIndex);
    void setNowAnger(int value);
    void setDead();
    void setLive();
private:
    CC_SYNTHESIZE(CFightCard *, m_pFightCard, FightCardInfo);
    int m_nIndex;
};

#endif /* defined(___1_cube__CFightCardInfoSprite__) */
