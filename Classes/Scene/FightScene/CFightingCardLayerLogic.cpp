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
#include "gamePlayer.h"
#include "CFightSkillManager.h"

CFightingCardLayerLogic::CFightingCardLayerLogic()
{
    
}

CFightingCardLayerLogic::~CFightingCardLayerLogic()
{
    
}

void CFightingCardLayerLogic::logicFightGame(vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int fightCardIndex,int MonsterCardIndex,CFightCard *pFight,CFightingCardLayerScene *scene)

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
        if (pFight)
        {
            if(SinglePlayer::instance()->getSkillBySkillId(pFight->m_pCard->m_iskillHelp))
            {
                //发动拥护技能
                g_FightSkillManager::instance()->CardFighting(pFight, FightCard,MonsterCard,fightCardIndex,MonsterCardIndex,EN_SEND_SKILL_HELP,m_friendScene->m_enHuiheIndex);
            }
        }
    }
    else  //发动普通战斗的逻辑
    {
        g_FightSkillManager::instance()->CardFighting(pFight, FightCard,MonsterCard,fightCardIndex,MonsterCardIndex,EN_SEND_SKILL_ANGRY,m_friendScene->m_enHuiheIndex);
    }
}
