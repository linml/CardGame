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
using namespace cocos2d;
class CFightCard;
class CFightCardInfoSprite :public CCNode
{
public:
    CFightCardInfoSprite();
    ~CFightCardInfoSprite();
    static CFightCardInfoSprite * CreateSprite(CFightCard *pCard,int nIndex);
    bool initSprite(CFightCard *pCard,int nIndex);
    void setNowAnger(int value);
private:
    CC_SYNTHESIZE(CFightCard *, m_pFightCard, FightCardInfo);
};

#endif /* defined(___1_cube__CFightCardInfoSprite__) */
