//
//  CFightingCardLayerLogic.cpp
//  91.cube
//
//  Created by linminglu on 13-7-15.
//
//

#include "CFightingCardLayerLogic.h"
#include "CCard.h"
#include "gamePlayer.h"
#include "CFightSkillManager.h"
#include "SEveryATKData.h"
#include "CAnimationSpriteGameFight.h"
#include "CSkillData.h"
#include "PtActionUtility.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CFightingCardLayerLogic::CFightingCardLayerLogic()
{
    
}

CFightingCardLayerLogic::~CFightingCardLayerLogic()
{
    DELETE_POINT_VECTOR(m_vFightingCard,vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_vMonsterCard, vector<CFightCard *>);
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
        if(tempSinglePlayer->m_hashmapMonsterCard[i])
        {
            this->m_vMonsterCard.push_back(new CFightCard(*tempSinglePlayer->m_hashmapMonsterCard[i]));
            m_vMonsterCard[i]->tag=1000*(i+1);
        }
        else{
            this->m_vMonsterCard.push_back(NULL);
        }
    }
    return true;
}

bool  CFightingCardLayerLogic::loadFromServerTest(int  loadTeamIndex)
{
    CGamePlayer *tempSinglePlayer=SinglePlayer::instance();
    tempSinglePlayer->backUpFightTeam(loadTeamIndex);
    if(tempSinglePlayer->isLoadEndCardTeam &&tempSinglePlayer->m_vvBattleArray.size()>0&&loadTeamIndex<tempSinglePlayer->m_vvBattleArray.size()&&tempSinglePlayer->m_vvBattleArray[loadTeamIndex].size()>0)
    {
        for (int i=0; i<SinglePlayer::instance()->m_vvBattleArray[loadTeamIndex].size();i++)
        {
            if(tempSinglePlayer->m_vvBattleArray[loadTeamIndex][i])
            {
                m_vFightingCard.push_back(new CFightCard(*(tempSinglePlayer->m_vvBattleArray[loadTeamIndex][i])));
                m_vFightingCard[i]->tag=100*(i+1);
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
        return false;
    }
    else
    {
        m_enWinStatus=winStatus;
        SinglePlayer::instance()->m_enWinStatus=winStatus;
        CCLog("winStatus:%d",(int)winStatus);
        return  true;
    }
}

void CFightingCardLayerLogic::loadAnimatePlist()
{
    CFightSkillManager *skillManager=G_FightSkillManager::instance();
    for (int i= 0; i<skillManager->m_animationVector.size(); i++) {
        if(skillManager->m_animationVector[i])
        {
            CSkillData *pSkilldata=SinglePlayer::instance()->getSkillBySkillId(skillManager->m_animationVector[i]->m_iSKillId);
            if(pSkilldata)
            {
                bool  needInsert=true;
                for (int i=0; i<m_vSskillFile.size(); i++)
                {
                    if(m_vSskillFile[i]==pSkilldata->effect_plist.c_str())
                    {
                        needInsert=false;
                        break;
                    }
                }
                if(needInsert)
                {
                    string strPlist=pSkilldata->effect_plist+"_l.plist";
                    CCLog("load plist %s",pSkilldata->effect_plist.c_str());
                    m_vSskillFile.push_back(pSkilldata->effect_plist);
                    PtActionUtility::getAppendHBActionCachWithActionFile(strPlist, m_vNeedAnimate);
                }
            }
        }
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
    CCLog("BEGIN 当前攻击顺序是%d",int(m_enHuiheIndex));
    if(m_vFightingCard[m_iFightCardIndex])
    {
        CCLog("左边主将血液是:%d,%d",m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_iFightCardIndex);
    }
    if(m_vMonsterCard[m_iMonsterCardIndex])
    {
        CCLog("右边边主将血液是:%d,%d",m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp,m_iMonsterCardIndex);
    }
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD)
    {
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
    {
        //发动拥护技能
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[4], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD)
    {
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[4],m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
        
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
    CCLog("END:当前攻击顺序是%d",int(m_enHuiheIndex));
    if(m_vFightingCard[m_iFightCardIndex])
    {
        CCLog("左边主将血液是:%d,%d",m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_iFightCardIndex);
    }
    if(m_vMonsterCard[m_iMonsterCardIndex])
    {
        CCLog("右边边主将血液是:%d,%d",m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp,m_iMonsterCardIndex);
    }
    return true;
}

bool CFightingCardLayerLogic::checkIsDead()
{
    bool result=false;
    if (m_iFightCardIndex<m_vFightingCard.size()-1)
    {
        if(!m_vFightingCard[m_iFightCardIndex])
    {
        m_iFightCardIndex++;
        result=true;
    }
    else if(m_vFightingCard[m_iFightCardIndex]&&m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            int backIndex=m_iFightCardIndex;
            m_vFightingCard[m_iFightCardIndex]->m_iCurrHp=0;
            m_vFightingCard[m_iFightCardIndex]->isDead=true;
            //发动死亡技能
            CCLog("左边第%d个 发动死亡技能",m_iFightCardIndex);
            G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_DEAD,m_enHuiheIndex);
            appendHpAngryUpdate();
            do
            {
                m_iFightCardIndex++;
            } while (m_vFightingCard[m_iFightCardIndex]==NULL &&m_iFightCardIndex<m_vFightingCard.size()-1);
            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iFightCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_LEFT_MOVE);
            result=true;
        }
    }
    
    if (m_iMonsterCardIndex<m_vMonsterCard.size()-1)
    {

        if(!m_vMonsterCard[m_iMonsterCardIndex])
        {
            m_iMonsterCardIndex++;
            result=true;
        }
        else if (m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            //发动死亡技能
            int backIndex=m_iMonsterCardIndex;
            m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp=0;
            m_vMonsterCard[m_iMonsterCardIndex]->isDead=true;
            CCLog("右边第%d个 发动死亡技能",m_iMonsterCardIndex);
            G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard ,m_vFightingCard,m_iMonsterCardIndex ,m_iFightCardIndex,EN_SEND_SKILL_DEAD,m_enHuiheIndex);
            appendHpAngryUpdate();
            do {
                m_iMonsterCardIndex++;
            } while (m_vMonsterCard[m_iMonsterCardIndex]==NULL &&m_iMonsterCardIndex<m_vMonsterCard.size()-1);
            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iMonsterCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_RIGHT_MOVE);
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
        CCLog("左边第%d个发动增幅技能",m_iFightCardIndex);
        m_vFightingCard[m_iFightCardIndex]->isSendZengfu=true;
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_LEFT_LORD);
        result=true;
        appendHpAngryUpdate();
    }
    if(!m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu)//判断是否触发了 增幅技能
    {
        CCLog("右边第%d个发动增幅技能",m_iFightCardIndex);
        m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu=true;
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex],m_vMonsterCard,m_vMonsterCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_RIGHT_LORD);
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
        SinglePlayer::instance()->appendAtkData(pEveryAtk);
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
    //初始化
    m_iTotalHuihe=0;
    m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    m_iFightCardIndex=0;
    m_iMonsterCardIndex=0;
    for (int i=0; i<m_vFightingCard.size(); i++) {
        if(m_vFightingCard[i])
        {
            m_iFightCardIndex=i;
            break;
        }
    }
    for (int i=0; i<m_vMonsterCard.size(); i++) {
        if(m_vMonsterCard[i])
        {
            m_iMonsterCardIndex=i;
            break;
        }
    }
}