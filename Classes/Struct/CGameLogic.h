//
//  CGameLogic.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CGameLogic__
#define ___1_cube__CGameLogic__
#include "cocos2d.h"
#include <iostream>
using namespace cocos2d;
using namespace std;
class CFightCard;
class CGameLogic :public cocos2d::CCObject
{
public:
    //在背包中
    bool isCanCardLevelUp(vector<CFightCard *>user_card,vector<CFightCard *>user_cardBag);

    
    
    
};

#endif /* defined(___1_cube__CGameLogic__) */
