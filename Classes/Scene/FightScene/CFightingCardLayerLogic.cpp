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
#include "SEveryATKData.h"

CFightingCardLayerLogic::CFightingCardLayerLogic()
{
    
}

CFightingCardLayerLogic::~CFightingCardLayerLogic()
{
    
}

bool CFightingCardLayerLogic::loadFromCardTeamTest()
{
    CGamePlayer *tempSinglePlayer=SinglePlayer::instance();
    if(tempSinglePlayer->m_hashmapMonsterCard.size()==0)
    {
        return false;
    }
    for (int i=0; i<tempSinglePlayer->m_hashmapMonsterCard.size(); i++)
    {
        this->m_vMonsterCard.push_back(tempSinglePlayer->m_hashmapMonsterCard[i]);
    }
//    tempSinglePlayer->m_hashmapMonsterCard.erase(tempSinglePlayer->m_hashmapMonsterCard.begin(),tempSinglePlayer->m_hashmapMonsterCard.end());
//    tempSinglePlayer->m_hashmapMonsterCard.clear();
    return true;
}

bool  CFightingCardLayerLogic::loadFromServerTest(int  loadTeamIndex)
{
    CGamePlayer *tempSinglePlayer=SinglePlayer::instance();
    if(tempSinglePlayer->isLoadEndCardTeam &&tempSinglePlayer->m_vvBattleArray.size()>0&&loadTeamIndex<tempSinglePlayer->m_vvBattleArray.size()&&tempSinglePlayer->m_vvBattleArray[loadTeamIndex].size()>0)
    {
        for (int i=0; i<SinglePlayer::instance()->m_vvBattleArray[loadTeamIndex].size();i++)
        {
            if(tempSinglePlayer->m_vvBattleArray[loadTeamIndex][i])
            {
                m_vFightingCard.push_back(new CFightCard(*(tempSinglePlayer->m_vvBattleArray[loadTeamIndex][i])));
            }
            else
            {
                m_vFightingCard.push_back(NULL);
            }
        }
        return true;
    }
    else
    {
        return false;
    }    
}

bool CFightingCardLayerLogic::logicFighting()
{
    EN_GAMEFIGHTSTATUS winStatus=getWinStatus();
    //先判断是否全部死亡；
    if(winStatus==EN_GAMEFIGHTSTATUS_NONE)
    {
        if(!checkSendZengfu())
        {
            if(!checkIsDead())
            {
                checkFighting();
            }
        }
        return true;
    }
    else
    {
        CCLog("%d",(int)winStatus);
        return  false;
    }
}


bool CFightingCardLayerLogic::checkFighting()
{
    
    if(m_vFightingCard[m_iFightCardIndex]==NULL || m_vMonsterCard[m_iMonsterCardIndex]==NULL)
    {
        return false;
    }
    m_enHuiheIndex++;
    appendHpAngryUpdate();
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD)
    {
        g_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
    {
        //发动拥护技能
        g_FightSkillManager::instance()->CardFighting(m_vFightingCard[4], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD)
    {
        g_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        
        g_FightSkillManager::instance()->CardFighting(m_vMonsterCard[4],m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
        
        CFightSkillManager::dealWithBuffer(m_vFightingCard[m_iFightCardIndex],m_iFightCardIndex,m_iMonsterCardIndex, EN_ATKFIGHT_INDEX_LEFT_LORD);
        CFightSkillManager::dealWithBuffer(m_vFightingCard[4],m_iFightCardIndex,m_iMonsterCardIndex, EN_ATKFIGHT_INDEX_LEFT_SUPPORT);
        CFightSkillManager::dealWithBuffer(m_vMonsterCard[m_iMonsterCardIndex],m_iMonsterCardIndex,m_iFightCardIndex, EN_ATKFIGHT_INDEX_RIGHT_LORD);
        CFightSkillManager::dealWithBuffer(m_vMonsterCard[4],m_iMonsterCardIndex,m_iFightCardIndex, EN_ATKFIGHT_INDEX_RIGHT_SUPPORT);
        
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
        m_iTotalHuihe++;
        CCLog("========>%d",m_iTotalHuihe);
    }
    if(m_enHuiheIndex>EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    }
    return true;
}

bool CFightingCardLayerLogic::checkIsDead()
{
    bool result=false;
    if (m_iFightCardIndex<m_vFightingCard.size()-1)
    {
        if(m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            int backIndex=m_iFightCardIndex;
            m_vFightingCard[m_iFightCardIndex]->m_iCurrHp=0;
            m_vFightingCard[m_iFightCardIndex]->isDead=true;
            //发动死亡技能
            g_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_DEAD,m_enHuiheIndex);
            appendHpAngryUpdate();
            do
            {
                m_iFightCardIndex++;
            } while (m_vFightingCard[m_iFightCardIndex]==NULL &&m_iFightCardIndex<m_vFightingCard.size()-1);
            g_FightSkillManager::instance()->appendAnimation(backIndex, m_iFightCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_LEFT_MOVE);
            result=true;
        }
    }
    
    if (m_iMonsterCardIndex<m_vMonsterCard.size()-1)
    {
        
        if(m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            //发动死亡技能
            int backIndex=m_iMonsterCardIndex;
            m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp=0;
            m_vMonsterCard[m_iMonsterCardIndex]->isDead=true;
            g_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard ,m_vFightingCard,m_iMonsterCardIndex ,m_iFightCardIndex,EN_SEND_SKILL_DEAD,m_enHuiheIndex);
            appendHpAngryUpdate();
            do {
                m_iMonsterCardIndex++;
            } while (m_vMonsterCard[m_iMonsterCardIndex]==NULL &&m_iMonsterCardIndex<m_vMonsterCard.size()-1);
            g_FightSkillManager::instance()->appendAnimation(backIndex, m_iMonsterCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_RIGHT_MOVE);
            result=true;
        }
    }
    return result;
}

bool CFightingCardLayerLogic::checkSendZengfu()
{
    bool result=false;
    if(m_vFightingCard[m_iFightCardIndex] && !m_vFightingCard[m_iFightCardIndex]->isSendZengfu) //判断是否触发了 增幅技能
    {
        m_vFightingCard[m_iFightCardIndex]->isSendZengfu=true;
        g_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_LEFT_LORD);
        result=true;
        appendHpAngryUpdate();
    }
    if(!m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu)//判断是否触发了 增幅技能
    {
        m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu=true;
        g_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex],m_vMonsterCard,m_vMonsterCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_RIGHT_LORD);
        //append增幅技能
        appendHpAngryUpdate();
        result=true;
    }
    return result; 
}

void CFightingCardLayerLogic::appendHpAngryUpdate()
{
    SEveryATKData *pEveryAtk=NULL;;
    if (m_vFightingCard[m_iFightCardIndex]) {
        pEveryAtk=new SEveryATKData;
        pEveryAtk->data[0]=new SSpriteStatus;
        pEveryAtk->data[0]->m_iCurrHp=m_vFightingCard[m_iFightCardIndex]->m_iCurrHp;
        pEveryAtk->data[0]->m_iCurrTotalHp=m_vFightingCard[m_iFightCardIndex]->m_iHp;
        pEveryAtk->data[0]->m_iEngry=m_vFightingCard[m_iFightCardIndex]->m_iCurrEngry;
    }
    if (m_vMonsterCard[m_iMonsterCardIndex])
    {
        if (!pEveryAtk)
        {
            pEveryAtk=new SEveryATKData;
        }
        pEveryAtk->data[1]=new SSpriteStatus;
        pEveryAtk->data[1]->m_iCurrHp=m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp;
        pEveryAtk->data[1]->m_iCurrTotalHp=m_vMonsterCard[m_iMonsterCardIndex]->m_iHp;
        pEveryAtk->data[1]->m_iEngry=m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrEngry;
    }
    if(pEveryAtk)
    {
        m_vHpAngry.push_back(pEveryAtk);
    }
    
}

EN_GAMEFIGHTSTATUS  CFightingCardLayerLogic::getWinStatus()
{
    if (m_iTotalHuihe>=31)
    {
        return EN_GAMEFIGHTSTATUS_LOSE;
    }
    bool isCardAllDead=true;
    for (int i=m_iFightCardIndex; i<m_vFightingCard.size()-1; i++)
    {
        if(m_vFightingCard[i]&&!m_vFightingCard[i]->isDead) //没有死亡 拥户不算
        {
            isCardAllDead=false;
            break;
        }
    }
    //都死了
    if(isCardAllDead)
    {
        return EN_GAMEFIGHTSTATUS_LOSE;
    }
    if(m_vMonsterCard.size()>0)
    {
        //先算自己是否全部
        for (int i=m_iMonsterCardIndex; i<m_vMonsterCard.size()-1; i++)
        {
            if (m_vMonsterCard[i]&&!m_vMonsterCard[i]->isDead)
            {
                return EN_GAMEFIGHTSTATUS_NONE;
            }
        }
    }
    return EN_GAMEFIGHTSTATUS_WIN;
    
}

EN_ATKFIGHT_INDEX   CFightingCardLayerLogic::getHuiHeIndex()
{
    return m_enHuiheIndex;
}

void CFightingCardLayerLogic::initFightLogic(int  loadTeamIndex)
{
    loadFromServerTest(loadTeamIndex);//读取当前卡牌阵容，
    loadFromCardTeamTest();
}