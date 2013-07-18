//
//  CFightingCardLayerLogic.h
//  91.cube
//
//  Created by linminglu on 13-7-15.
//
//
/**
 *该类为计算打斗时候的界面逻辑类
 *
 */

#ifndef ___1_cube__CFightingCardLayerLogic__
#define ___1_cube__CFightingCardLayerLogic__

#include <iostream>
#include <vector>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class CFightCard;
class CFightingCardLayerScene;
class CFightingCardLayerLogic :public CCObject
{
public:
    CFightingCardLayerLogic();
    ~CFightingCardLayerLogic();
public:
    void logicFightGame(vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int fightCardIndex,int MonsterCardIndex,CFightCard *pFight,CFightingCardLayerScene *scene);
};

#endif /* defined(___1_cube__CFightingCardLayerLogic__) */
