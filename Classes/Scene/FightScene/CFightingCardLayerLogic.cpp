//
//  CFightingCardLayerLogic.cpp
//  91.cube
//
//  Created by linminglu on 13-7-15.
//
//

#include "CFightingCardLayerLogic.h"
#include "CFightingCardLayerScene.h"
#include "CCard.h"

void CFightingCardLayerLogic::logicFightGame(vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,CFightCard *pFight,CFightingCardLayerScene *scene)

{
    if(!pFight)
    {
        return ;
    }
    CFightingCardLayerScene *m_friendScene=scene;
    if(m_friendScene->m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT
       ||m_friendScene->m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT )
    {
        //判断 当前传输进来的卡牌是否有拥护技能
        
        
    }
    
}


//当前是否可以发动怒气技能
bool CFightingCardLayerLogic::isCanSpendAngrySkill(CFightCard *pFight)
{
    if(pFight->m_pCard->m_iskillLine==-1)
    {
        return false;
    }
    if (pFight->m_pCard->m_iskillLine==0)
    {
        return true;
    }
    return false;
}

//当前是否可以攻击
bool CFightingCardLayerLogic::isCanSpendAtkToMonster(CFightCard *pFight)
{
    if(pFight->m_vBuffer.size()>0)
    {
        return false;
    }
    return true;
}

//当前添加buffer的属性
void CFightingCardLayerLogic::addOrSubBuffer(CFightCard *pFight)
{
    
    for (int i=0; i<pFight->m_vBuffer.size(); i++) {
        
    }
    return ;    
}

bool CFightingCardLayerLogic::isHavaPhysicHarmMagic(CFightCard *pMonstFight)
{
    
    return false;
}
bool CFightingCardLayerLogic::isHaveMagicHarm(CFightCard *pMonstFight)
{
    return false;
}

//普通伤害计算
void CFightingCardLayerLogic::basicAtk(CFightCard *pFightCard,CFightCard *pMonstFight)
{
    //取得技能上的值得 来计算；
    //pFightCard->m_pCard->m_iskillLine
}