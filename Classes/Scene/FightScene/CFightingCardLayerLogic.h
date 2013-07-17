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
    void logicFightGame(vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,CFightCard *pFight,CFightingCardLayerScene *scene);
    //当前是否可以发动怒气技能
    bool isCanSpendAngrySkill(CFightCard *pFight);
    //当前是否可以攻击
    bool isCanSpendAtkToMonster(CFightCard *pFight);
    //当前添加buffer的属性
    void addOrSubBuffer(CFightCard *pFight);
    //物理免疫的buffer;
    static bool isHavaPhysicHarmMagic(CFightCard *pMonstFight);
    //怒气免疫的buffer;
    static bool isHaveMagicHarm(CFightCard *pMonstFight);
    
    //普通伤害计算
    void basicAtk(CFightCard *pFightCard,CFightCard *pMonstFight);

};

#endif /* defined(___1_cube__CFightingCardLayerLogic__) */
