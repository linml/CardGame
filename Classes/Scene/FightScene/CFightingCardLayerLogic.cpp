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
#include "CFightCardBufferData.h"

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
        else
        {
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
            if(!checkIsDead() && checklogicBuffAndDead())
            {
                if(checklogicCheckIsCanSendAtk())
                {
                    checkFighting();
                }
            }
            else
            {
                whenDeadSendDeadSkillAndMove();
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

bool CFightingCardLayerLogic::checklogicBuffAndDead()
{
    m_enHuiheIndex++;
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD&&m_vFightingCard[m_iFightCardIndex])
    {
        CFightSkillManager::dealWithBuffer(m_vFightingCard[m_iFightCardIndex],m_iFightCardIndex,m_iMonsterCardIndex, EN_ATKFIGHT_INDEX_LEFT_LORD);
        if(m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            return false;
        }
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT && m_vFightingCard[4])
    {
        CFightSkillManager::dealWithBuffer(m_vFightingCard[4],m_iFightCardIndex,m_iMonsterCardIndex, EN_ATKFIGHT_INDEX_LEFT_SUPPORT);
        if(m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            return false;
        }
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD && m_vMonsterCard[m_iMonsterCardIndex])
    {
        CFightSkillManager::dealWithBuffer(m_vMonsterCard[m_iMonsterCardIndex],m_iMonsterCardIndex,m_iFightCardIndex, EN_ATKFIGHT_INDEX_RIGHT_LORD);
        if(m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            return false;
        }
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT && m_vMonsterCard[4])
    {
        CFightSkillManager::dealWithBuffer(m_vMonsterCard[4],m_iMonsterCardIndex,m_iFightCardIndex, EN_ATKFIGHT_INDEX_RIGHT_SUPPORT);
        if(m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            return false;
        }
    }
    return true;
}
bool CFightingCardLayerLogic::checklogicCheckIsCanSendAtk()
{
    return true;
}
bool CFightingCardLayerLogic::checkFighting()
{
    
    if(m_vFightingCard[m_iFightCardIndex]==NULL || m_vMonsterCard[m_iMonsterCardIndex]==NULL)
    {
        return false;
    }
    CCLog("BEGIN 当前攻击顺序是%d",int(m_enHuiheIndex));
    if(m_vFightingCard[m_iFightCardIndex])
    {
        CCLog("左边主将血液是:%d,%d",m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_iFightCardIndex);
    }
    if(m_vMonsterCard[m_iMonsterCardIndex])
    {
        CCLog("右边边主将血液是:%d,%d",m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp,m_iMonsterCardIndex);
    }
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD&&m_vFightingCard[m_iFightCardIndex])
    {
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
        appendUpdateAction();
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT && m_vFightingCard[4])
    {
        //发动拥护技能
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[4], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
               appendUpdateAction();
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD && m_vMonsterCard[m_iMonsterCardIndex])
    {
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex);
               appendUpdateAction();
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT && m_vMonsterCard[4])
    {
        
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[4],m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex);
        appendUpdateAction();
    }
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
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
int CFightingCardLayerLogic::getNextFightCard(int index,bool isLeftFightTeam)
{
    if(isLeftFightTeam)
    {
        do
        {
            index++;
        } while (m_vFightingCard[index]==NULL && index<m_vFightingCard.size()-1);
        return index;
    }
    else
    {
        do
        {
            index++;
        } while (m_vMonsterCard[index]==NULL && index<m_vMonsterCard.size()-1);
        return index;
    }
}

bool CFightingCardLayerLogic::whenDeadSendDeadSkillAndMove()
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
            CCLog("左边第%d个 发动死亡技能",backIndex);
            int indexNext= getNextFightCard(m_iFightCardIndex,true);
            if(indexNext<m_vFightingCard.size()-1)
            {
                G_FightSkillManager::instance()->CardFighting(m_vFightingCard[backIndex], m_vFightingCard,m_vMonsterCard,backIndex, m_iMonsterCardIndex,EN_SEND_SKILL_DEAD,EN_ATKFIGHT_INDEX_LEFT_LORD);
                appendUpdateAction();
                m_iFightCardIndex=indexNext;
                
            }
            CCLog("用户移动位置");
            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iFightCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_LEFT_MOVE);
            appendUpdateAction();
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
            
            int indexNext= getNextFightCard(m_iMonsterCardIndex,false);
            if(indexNext<m_vFightingCard.size()-1)
            {
                G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[backIndex], m_vMonsterCard ,m_vFightingCard,backIndex ,m_iFightCardIndex,EN_SEND_SKILL_DEAD,EN_ATKFIGHT_INDEX_RIGHT_LORD);
                appendUpdateAction();
                m_iMonsterCardIndex=indexNext;
                
            }
            
            CCLog("对方移动位置");
            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iMonsterCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_RIGHT_MOVE);
            appendUpdateAction();
            result=true;
        }
    }
    return result;
}

bool CFightingCardLayerLogic::checkIsDead()
{
    if(m_vFightingCard[m_iFightCardIndex]&&m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
    {
        return true;
    }
    if(m_vMonsterCard[m_iMonsterCardIndex]&&m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
    {
        return true;
    }
    return false;
}

bool CFightingCardLayerLogic::checkSendZengfu()
{
    bool result=false;
    if(m_vFightingCard[m_iFightCardIndex] && !m_vFightingCard[m_iFightCardIndex]->isSendZengfu) //判断是否触发了 增幅技能
    {
        CCLog("左边第%d个发动增幅技能",m_iFightCardIndex);
        m_vFightingCard[m_iFightCardIndex]->isSendZengfu=true;
        G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_LEFT_LORD);
        //获得当前的buffer列表
        result=true;
        appendHpAngryUpdate();
        appendUpdateBuffer();
    }
    if(!m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu)//判断是否触发了 增幅技能
    {
        CCLog("右边第%d个发动增幅技能",m_iFightCardIndex);
        m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu=true;
        G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex],m_vMonsterCard,m_vMonsterCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_RIGHT_LORD);
        appendUpdateAction();
        result=true;
    }
    return result;
}

void CFightingCardLayerLogic::appendUpdateBuffer()
{
    if(m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.size()==0&& m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.size()==0)
    {
        SinglePlayer::instance()->appendCFightCardFightingBuffer(NULL);
        return;
    }

    CFightCardFightingBuffer *fightBuffer=new CFightCardFightingBuffer;
    fightBuffer->m_index=m_iTotalHuihe;
    for (list<CCardBufferStatusRefactor *>::iterator itAfter=m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.begin(); itAfter!=m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.end(); itAfter++)
    {
        fightBuffer->append((*itAfter)->m_iEffectid, (*itAfter)->m_iKeepTime, true);
    }
    for (list<CCardBufferStatusRefactor *>::iterator itAfter=m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.begin(); itAfter!=m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.end(); itAfter++)
    {
        fightBuffer->append((*itAfter)->m_iEffectid, (*itAfter)->m_iKeepTime, false);
    }
    SinglePlayer::instance()->appendCFightCardFightingBuffer(fightBuffer);

}

void CFightingCardLayerLogic::appendUpdateAction()
{
    appendHpAngryUpdate();
    appendUpdateBuffer();
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
        CCLog(" pEveryAtk->data[0]->m_iCurrTotalHp:%d",pEveryAtk->data[0]->m_iCurrHp);
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
        CCLog(" pEveryAtk->data[1]->m_iCurrTotalHp:%d",pEveryAtk->data[1]->m_iCurrHp);
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
    CCLog("m_iFightCardIndex=%d",m_iFightCardIndex);
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