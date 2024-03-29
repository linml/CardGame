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
#include "gameMiddle.h"

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
    m_tempGamePlayer=SinglePlayer::instance();
}

CFightingCardLayerLogic::~CFightingCardLayerLogic()
{
    DELETE_POINT_VECTOR(m_vFightingCard,vector<CFightCard *>);
    //DELETE_POINT_VECTOR(m_vMonsterCard, vector<CFightCard *>);
}

bool CFightingCardLayerLogic::loadFromCardTeamTest()
{
    CGamePlayer *tempSinglePlayer=m_tempGamePlayer;
    if(tempSinglePlayer->m_hashmapMonsterCard.size()==0)
    {
        return false;
    }
    for (int i=0; i<tempSinglePlayer->m_hashmapMonsterCard.size(); i++)
    {
        if(tempSinglePlayer->m_hashmapMonsterCard[i])
        {
            this->m_vMonsterCard.push_back(tempSinglePlayer->m_hashmapMonsterCard[i]);
            m_vMonsterCard[i]->tag=1000*(i+1);
            m_vMonsterCard[i]->setTEAMPOSTION(EN_RIGHTTEAM);
            //从后面的低5个开始代表敌方的阵容.
            SinglePlayer::instance()->zhongzuCount[m_vMonsterCard[i]->m_pCard->m_icard_stirps+3]++;
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
    CGamePlayer *tempSinglePlayer=m_tempGamePlayer;
    tempSinglePlayer->backUpFightTeam(loadTeamIndex);
    if(tempSinglePlayer->getLoadEndCardTeam() &&
       tempSinglePlayer->getCardBattleArray().size()>0&&
       loadTeamIndex<tempSinglePlayer->getCardBattleArray().size()&&
       tempSinglePlayer->getCardBattleArray()[loadTeamIndex].size()>0)
    {
        for (int i=0; i<m_tempGamePlayer->getCardBattleArray()[loadTeamIndex].size();i++)
        {
            if(tempSinglePlayer->getCardBattleArray()[loadTeamIndex][i])
            {
                m_vFightingCard.push_back(new CFightCard(*(tempSinglePlayer->getCardBattleArray()[loadTeamIndex][i])));
                m_vFightingCard[i]->tag=100*(i+1);
                m_vFightingCard[i]->setTEAMPOSTION(EN_LEFTTEAM);
                SinglePlayer::instance()->zhongzuCount[m_vFightingCard[i]->m_pCard->m_icard_stirps-1]++;
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
        Middle::showAlertView("读取阵容信息有错误,最大可能是初始化没读取到阵容信息");
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
        m_tempGamePlayer->setWinOrLoseStatus(winStatus);
        CCLog("winStatus:%d",(int)winStatus);
        return  true;
    }
}

void CFightingCardLayerLogic::loadAnimatePlist()
{
    CFightSkillManager *skillManager=G_FightSkillManager::instance();
    for (int i= 0; i<skillManager->m_vAnimationStrip.size(); i++)
    {
        vector<string>temp_plist=GameTools::splitArgString(skillManager->m_vAnimationStrip[i]->m_sRunActionFile.c_str(), ",");
        CCLog("aaaaaadasdfsadfasdfas %s",skillManager->m_vAnimationStrip[i]->m_sRunActionFile.c_str());
        for (int i=0; i<temp_plist.size(); i++) {
            string  strPlist=temp_plist[i];
            if(strPlist.empty() || strPlist=="NULL")
            {
                continue;
            }
            if(strPlist.compare(strPlist.length()-6, 6, ".plist"))
            {
                strPlist+="_l.plist";
            }
            bool  needInsert=true;
            for (int j=0; j<m_vSskillFile.size(); j++)
            {
                string data=m_vSskillFile[j];
                if(data==strPlist) {
                    needInsert=false;
                    break;
                }
            }
            if(needInsert)
            {
                CCLog("load plist %s",strPlist.c_str());
                m_vSskillFile.push_back(strPlist);
                PtActionUtility::getAppendHBActionCachWithActionFile(strPlist, m_vNeedAnimate);
            }
        }
        
    }
}

bool CFightingCardLayerLogic::checklogicBuffAndDead()
{
    //坑爹的XCODE5.1 必须这么做
    int value=(int)m_enHuiheIndex;
    m_enHuiheIndex=(EN_ATKFIGHT_INDEX)(++value);
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
    CFightCard *card=NULL;
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD&&m_vFightingCard[m_iFightCardIndex])
    {
        card=m_vFightingCard[m_iFightCardIndex];
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT && m_vFightingCard[4])
    {
        card=m_vFightingCard[4];
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD && m_vMonsterCard[m_iMonsterCardIndex])
    {
        card=m_vMonsterCard[m_iMonsterCardIndex];
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT && m_vMonsterCard[4])
    {
        card=m_vMonsterCard[4];
    }
    if (card) {
        return card->isCanFight();
    }
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
        if(G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex))
        {
            appendUpdateAction();
        }
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT && m_vFightingCard[4])
    {
        //发动拥护技能
        if(G_FightSkillManager::instance()->CardFighting(m_vFightingCard[4], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex))
        {
            appendUpdateAction();
        }
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD && m_vMonsterCard[m_iMonsterCardIndex])
    {
        if(G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex], m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_ANGRY,m_enHuiheIndex))
        {
            appendUpdateAction();
        }
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT && m_vMonsterCard[4])
    {
        
        if(G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[4],m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_HELP,m_enHuiheIndex))
        {
            appendUpdateAction();
        }
    }
    
    CCLog("END:当前攻击顺序是%d",int(m_enHuiheIndex));
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    }
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
                if(G_FightSkillManager::instance()->CardFighting(m_vFightingCard[backIndex], m_vFightingCard,m_vMonsterCard,backIndex, m_iMonsterCardIndex,EN_SEND_SKILL_DEAD,EN_ATKFIGHT_INDEX_LEFT_LORD))
                {
                    appendUpdateAction();
                }
                m_iFightCardIndex=indexNext;
                
            }
            CCLog("用户移动位置");
            //            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iFightCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_LEFT_MOVE);
            G_FightSkillManager::instance()->appendVector(EN_ANIMATIONTYPEREFACTOR_DEADMOVE, EN_ATKFIGHT_INDEX_LEFT_MOVE, 0, backIndex, 0, 0, 0, "");
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
            if(indexNext<m_vMonsterCard.size()-1)
            {
                if(G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[backIndex], m_vMonsterCard ,m_vFightingCard,backIndex ,m_iFightCardIndex,EN_SEND_SKILL_DEAD,EN_ATKFIGHT_INDEX_RIGHT_LORD))
                {
                    appendUpdateAction();
                }
                m_iMonsterCardIndex=indexNext;
                
            }
            
            CCLog("对方移动位置");
            //            G_FightSkillManager::instance()->appendAnimation(backIndex, m_iMonsterCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_DEADMOVE, EN_ATKFIGHT_INDEX_RIGHT_MOVE);
            G_FightSkillManager::instance()->appendVector(EN_ANIMATIONTYPEREFACTOR_DEADMOVE, EN_ATKFIGHT_INDEX_RIGHT_MOVE, 0, backIndex, 0, 0, 0, "");
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
        if(G_FightSkillManager::instance()->CardFighting(m_vFightingCard[m_iFightCardIndex], m_vFightingCard,m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_LEFT_LORD))
        {
            appendUpdateAction();
        }
        //获得当前的buffer列表
        result=true;
    }
    if(!m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu)//判断是否触发了 增幅技能
    {
        CCLog("右边第%d个发动增幅技能",m_iFightCardIndex);
        m_vMonsterCard[m_iMonsterCardIndex]->isSendZengfu=true;
        if(G_FightSkillManager::instance()->CardFighting(m_vMonsterCard[m_iMonsterCardIndex],m_vMonsterCard,m_vMonsterCard,m_iMonsterCardIndex,m_iFightCardIndex,EN_SEND_SKILL_BUFF,EN_ATKFIGHT_INDEX_RIGHT_LORD))
        {
            appendUpdateAction();
        }
        result=true;
    }
    return result;
}

void CFightingCardLayerLogic::appendUpdateBuffer()
{
    
    if((m_vFightingCard[m_iFightCardIndex] && m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.size()==0 )&& m_vMonsterCard[m_iMonsterCardIndex]&& m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.size()==0)
    {
        m_tempGamePlayer->appendCFightCardFightingBuffer(NULL);
        return;
    }
    
    CFightCardFightingBuffer *fightBuffer=new CFightCardFightingBuffer;
    if (m_vFightingCard[m_iFightCardIndex])
    {
        for (list<CCardBufferStatusRefactor *>::iterator itAfter=m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.begin(); itAfter!=m_vFightingCard[m_iFightCardIndex]->m_vlistBuffer.end(); itAfter++)
        {
            fightBuffer->append((*itAfter)->m_iEffectid, (*itAfter)->m_iKeepTime, true);
        }
    }
    if (m_vMonsterCard[m_iMonsterCardIndex]) {
        for (list<CCardBufferStatusRefactor *>::iterator itAfter=m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.begin(); itAfter!=m_vMonsterCard[m_iMonsterCardIndex]->m_vlistBuffer.end(); itAfter++)
        {
            fightBuffer->append((*itAfter)->m_iEffectid, (*itAfter)->m_iKeepTime, false);
        }
    }
   
    m_tempGamePlayer->appendCFightCardFightingBuffer(fightBuffer);
    
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
        pEveryAtk->data[0]->actionIndex=m_iFightCardIndex;
        pEveryAtk->data[0]->m_iCurrTotalHp=m_vFightingCard[m_iFightCardIndex]->m_iHp;
        pEveryAtk->data[0]->m_iEngry=m_vFightingCard[m_iFightCardIndex]->m_iCurrEngry;
        int value=0;
        for (int i=0; i<m_vFightingCard.size()-1; i++,value++) {
            if (m_vFightingCard[i]) {
                pEveryAtk->data[0]->m_HouxuKaDeEngry[value]=m_vFightingCard[i]->m_iCurrEngry;
                CCLog("pEveryAtk->data[0]->m_iEngry finghte %d, %d",i,m_vFightingCard[i]->m_iCurrEngry);
            }
        }
        CCLog("pEveryAtk->data[0]->m_iEngry:%d",pEveryAtk->data[0]->m_iEngry);
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
        int value=0;
        for (int i=0; i<m_vMonsterCard.size()-1; i++,value++) {
            if (m_vMonsterCard[i]) {
                pEveryAtk->data[1]->m_HouxuKaDeEngry[value]=m_vMonsterCard[i]->m_iCurrEngry;
                 CCLog("pEveryAtk->data[0]->m_iEngry monster:%d, %d",i,m_vMonsterCard[i]->m_iCurrEngry);
            }
        }
        pEveryAtk->data[0]->actionIndex=m_iMonsterCardIndex;
    }
    if(pEveryAtk)
    {
        m_tempGamePlayer->appendAtkData(pEveryAtk);
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
        if(m_vFightingCard[i] &&! m_vFightingCard[i]->isDead) //没有死亡 拥户不算
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

void CFightingCardLayerLogic::dealWithShenTanBuffer(CFightCard *pFightCard)
{
    CFightSkillManager *tempFightSkillManager=G_FightSkillManager::instance();
    //计算玩家身上的buffer
    for (list<int >::iterator it=m_tempGamePlayer->m_vPlayerBufferList.begin(); it!=m_tempGamePlayer->m_vPlayerBufferList.end(); it++)
    {
        tempFightSkillManager->initBeginFightStatus(pFightCard, *it);
    }
}


void CFightingCardLayerLogic::initFightLogic(int  loadTeamIndex)
{
    SinglePlayer::instance()->initZhongZu();
    loadFromServerTest(loadTeamIndex);//读取当前卡牌阵容，
    loadFromCardTeamTest();
    //初始化
    m_iTotalHuihe=0;
    m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    m_iFightCardIndex=0;
    m_iMonsterCardIndex=0;
    bool isSelect=false;
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        if(m_vFightingCard[i])
        {
            //处理神坛技能buffer
            if(!m_tempGamePlayer->getIsFightWithTeam())
            {
              dealWithShenTanBuffer(m_vFightingCard[i]);
            }
            if(!isSelect)
            {
                isSelect=true;
                m_iFightCardIndex=i;
            }
        }
    }
    
    for (int i=0; i<m_vMonsterCard.size(); i++) {
        if(m_vMonsterCard[i])
        {
            m_iMonsterCardIndex=i;
            break;
        }
    }
    if(m_vMonsterCard.size()==0)
    {
        logicFighting();
    }
    else{
    appendUpdateAction();
    }
    
}