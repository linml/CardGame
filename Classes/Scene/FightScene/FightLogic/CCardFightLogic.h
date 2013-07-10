//
//  CCardFightLogic.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CCardFightLogic__
#define ___1_cube__CCardFightLogic__

#include <iostream>
#include "cocos2d.h"
using namespace std;

class CFightCard;
//class CCard
class CCardFightLogic :cocos2d::CCObject
{
public:
    bool  logicGameFighting(vector<CFightCard *>FightCardList,vector<CFightCard *>MonsterCardList,CFightCard *fightPoint);
};

#endif /* defined(___1_cube__CCardFightLogic__) */
