
//
//  CFightSkillManager.cpp
//  91.cube
//
//  Created by linminglu on 13-7-16.
//
//

#include "CFightSkillManager.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gamePlayer.h"
#include "CEffectInterface.h"
#include "CEffectInterFaceOneRefactor.h"
#include "CEffectInterFaceTwoRefactor.h"
#include "CEffectInterFaceThreeRefactor.h"
#include "CEffectInterFaceFourRefactor.h"
#include "CEffectInterFaceFiveRefactor.h"
#include "CEffectInterFaceSixRefactor.h"
#include "CEffectInterFaceSevenRefactor.h"
#include "CEffectInterFaceEightRefactor.h"
#include "CEffectInterFaceElevenRefactor.h"
#include "CEffectInterFaceTwentyOneRefactor.h"
#include "CEffectInterFaceTwentyFourRefactor.h"
#include "CFightCardBufferData.h"
#include "gameStruct.h"
#include <vector>
using namespace std;
//static  int totoalanimation=0;

#include "CAnimationSpriteGameFight.h"
#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,_ITEMTYPE_) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< _ITEMTYPE_ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}



static    map <string,pFunc>m_vSkillManagerLogic;
static    map<string, pbEffFunc>m_vEffictManager;
vector<CAnimationSpriteGameFight *>CFightSkillManager::m_animationVector;
vector<CFightCardBufferDataEveryFight *>CFightSkillManager::m_animationBufferVector;
vector<CAnimationSctrip *>CFightSkillManager::m_vAnimationStrip;
string CFightSkillManager::logicSkillFight(CFightCard *pCard,vector< CFightCard *> pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,int paramid,EN_ATKFIGHT_INDEX enAtkIndex,bool isDirenTuandui,bool isQunti)
{
    //走向具体的攻击过程
    //普通攻击
    
    int value=paramid;
    string resualtFile="";
    if (paramid==0)
    {
        return resualtFile;
    }
    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(value);
    if (pImpact)
    {
        IteratorEffMapPfunc pt=m_vEffictManager.find(ConvertToString(pImpact->m_ieffect_logic));
        if(pt!=m_vEffictManager.end())
        {
            pbEffFunc ptfunction=pt->second;
            if (!isQunti)
            {
                if(pMonterCard[MonsterIndex]&&pMonterCard[MonsterIndex]->m_iCurrHp>0) //攻击第一个 血量大于0的人 也就是主将
                {
                    resualtFile=(*ptfunction)(pCard,pMonterCard[MonsterIndex],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
                    CCLog("resualtFile:%s",resualtFile.c_str());
                }
                if(pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            else{
                if(isDirenTuandui)
                {
                    for (int i=MonsterIndex; i<pMonterCard.size(); i++)
                    {
                        if(pMonterCard[i]&&pMonterCard[i]->m_iCurrHp>=0)
                        {
                            (*ptfunction)(pCard,pMonterCard[i],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
                        }
                    }
                }
                else
                {
                    for (int i=FightIndex; i<pFightCard.size(); i++)
                    {
                        if(pFightCard[i]&&pFightCard[i]->m_iCurrHp>=0)
                        {
                            (*ptfunction)(pCard,pFightCard[i],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
                        }
                    }
                }
                if(pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
                
            }
        }
    }
    if (isDirenTuandui && !resualtFile.empty())
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_BUFFER_MONSTER,enAtkIndex,m_nEffectId[0],FightIndex,MonsterIndex,0,0,resualtFile);
    } else if (!isDirenTuandui && !resualtFile.empty())
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_BUFFER_OWN,enAtkIndex,m_nEffectId[0],FightIndex,MonsterIndex,0,0,resualtFile);
    }
    return resualtFile;
}

void CFightSkillManager::logicSkill_Putong(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{
    
    logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
    logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
    logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
    logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
    logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,true,true);
    logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,false,true);
    
}
int CFightSkillManager::m_nEffectId[6];
void CFightSkillManager::InitEffectId(CSkillData *pSkill)
{
    m_nEffectId[0]=pSkill->parameter_1;
    m_nEffectId[1]=pSkill->parameter_2;
    m_nEffectId[2]=pSkill->parameter_3;
    m_nEffectId[3]=pSkill->parameter_4;
    m_nEffectId[4]=pSkill->parameter_5;
    m_nEffectId[5]=pSkill->parameter_6;
}

void CFightSkillManager::logicSkill_0(CFightCard *pCard, vector<CFightCard *> FightCard, vector<CFightCard *> MonsterCard, int FightIndex, int MonsterIndex, CSkillData *pSkill, EN_ATKFIGHT_INDEX enAtkIndex)
{
    //发动
    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    CSkillData *pPutongSkill=SinglePlayer::instance()->getPutongGongji(pCard);
    if (pPutongSkill)
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pPutongSkill->skill_id,FightIndex,MonsterIndex,0,0,pPutongSkill->effect_plist);
        logicSkill_Putong(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pPutongSkill,enAtkIndex);
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        FightCard[FightIndex]->appendEngry(20);
        for (int i=FightIndex+1; i<FightCard.size(); i++)
        {
            if(FightCard[i])
            {
                FightCard[i]->appendEngry(10);
            }
        }
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        MonsterCard[MonsterIndex]->appendEngry(30);
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
        CCLog("用户发动了用户的技能2");
        appendVector(EN_ANIMATIONTYPEREFACTOR_SHANGHAI, enAtkIndex, 0, FightIndex, MonsterIndex, -currHp, -monstercurrHp, "");
       // appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pPutongSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
    }

}


void CFightSkillManager::logicSkill_1(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{
    
    //发动
    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    // 满足条件
    if(costFunc(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill)>=1)   //判断单体的条件是否满足。
    {
        //发动skill
        /*    CAnimationSctrip(EN_ANIMATIONTYPEREFACTOR entype,EN_ATKFIGHT_INDEX enAtkIndex,
         int skillId,
         int nATKindex,
         int nDefIndex,
         int nAddHp,int nSubHp,string m_sRunActionFile);
         */
        //添加
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pSkill->skill_id,FightIndex,MonsterIndex,0,0,pSkill->effect_plist);
        CCLog("条件满足 爆发了技能id:%d",pSkill->skill_id);
        // 给敌方主卡的效果
       string effecitFile[6];
       effecitFile[0] =logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
       // 给自己主卡的效果
       effecitFile[1] = logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
        // 给敌方主卡的效果
       effecitFile[2] = logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
        // 给我方主卡的效果
       effecitFile[3] =logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
        // 给敌方上场卡的效果
       effecitFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,true,true);
        // 给我方上场卡的效果
       effecitFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,false,true);
        switch (pSkill->skill_type)
        {
            case 1:
                //如果是普通攻击对手加怒气30
                MonsterCard[MonsterIndex]->appendEngry(30);
                break;
            case 3:
                
                for (int i=FightIndex+1; i<FightCard.size(); i++)
                {
                    if(FightCard[i])
                    {
                        FightCard[i]->appendEngry(10);
                    }
                }
                break;
            default:
                break;
        }
        
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();
        CCLog("用户发动了用户的技能1");
       // appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
        for (int i=0; i<6; i++)
        {
            if (!effecitFile[i].empty())
            {
                //整个敌方有错误的。。。
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enAtkIndex,FightIndex,MonsterIndex,0,0,0,0,0,m_nEffectId[i],effecitFile[i]));
            }
        }
        appendVector(EN_ANIMATIONTYPEREFACTOR_SHANGHAI, enAtkIndex, 0, FightIndex, MonsterIndex, -currHp, -monstercurrHp, "");
        
    }
    else
    {
        logicSkill_0(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,enAtkIndex);
    }
}

void CFightSkillManager::logicSkill_2(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{

    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    // 满足条件
    int costfunctValue=costFunc(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill);
    if (costfunctValue>=1)
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pSkill->skill_id,FightIndex,MonsterIndex,0,0,pSkill->effect_plist);
        CCLog("costfunctValue %d",costfunctValue);
        string resultEffectFile[6];
        if(costfunctValue==1)
        {
           resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
           resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
           resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,false,true);
           resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,true,true);
        }
        else if(costfunctValue==2)   //判断单体的条件是否满足。
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[2]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
            resultEffectFile[3]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
            resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,true,true);
            resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,false,true);
        }
        switch (pSkill->skill_type)
        {
            case 1:
                MonsterCard[MonsterIndex]->appendEngry(30);
                break;
            case 3:
                for (int i=FightIndex+1; i<FightCard.size(); i++) {
                    if(FightCard[i])
                    {
                        FightCard[i]->appendEngry(10);
                    }
                }
                break;
            default:
                break;
        }
        
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        //用户发动了拥护的技能
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
        CCLog("用户发动了用户的技能3");
        appendVector(EN_ANIMATIONTYPEREFACTOR_SHANGHAI, enAtkIndex, 0, FightIndex, MonsterIndex, -currHp, -monstercurrHp, "");
       // appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
        for (int i=0; i<6; i++)
        {
            CCLog("resultEffectFile[i] :%d :%s",i,resultEffectFile[i].c_str());
            if (!resultEffectFile[i].empty())
            {
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enAtkIndex,FightIndex,MonsterIndex,0,0,0,0,0,CFightSkillManager::m_nEffectId[i],resultEffectFile[i]));
            }
        }
    }
    else
    {
        logicSkill_0(pCard, FightCard, MonsterCard, FightIndex, MonsterIndex, pSkill, enAtkIndex);
    }
}

void CFightSkillManager::logicSkill_3(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{
    
    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    // 满足条件
    int costfunctValue=costFunc(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill);
    if (costfunctValue>=1)
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pSkill->skill_id,FightIndex,MonsterIndex,0,0,pSkill->effect_plist);
        
        CCLog("costfunctValue %d",costfunctValue);
        string resultEffectFile[6];
        if(costfunctValue==1)
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,true,true);
            resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,false,true);
        }
        else if(costfunctValue==2)   //判断单体的条件是否满足。
        {
            resultEffectFile[2]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
            resultEffectFile[3]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
            resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,0,pSkill,pSkill->parameter_5,enAtkIndex,true,true);
            resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,0,pSkill,pSkill->parameter_6,enAtkIndex,false,true);
        }
        switch (pSkill->skill_type)
        {
            case 1:
                MonsterCard[MonsterIndex]->appendEngry(30);
                break;
            case 3:
                for (int i=FightIndex+1; i<FightCard.size(); i++) {
                    if(FightCard[i])
                    {
                        FightCard[i]->appendEngry(10);
                    }
                }
                break;
            default:
                break;
        }
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        //用户发动了拥护的技能
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
        CCLog("用户发动了用户的技能3");
        appendVector(EN_ANIMATIONTYPEREFACTOR_SHANGHAI, enAtkIndex, 0, FightIndex, MonsterIndex, -currHp, -monstercurrHp, "");
        //appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
        for (int i=0; i<6; i++)
        {
            CCLog("resultEffectFile[i] :%d :%s",i,resultEffectFile[i].c_str());
            if (!resultEffectFile[i].empty())
            {
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enAtkIndex,FightIndex,MonsterIndex,0,0,0,0,0,m_nEffectId[i],resultEffectFile[i]));
            }
        }
    }
    else
    {
        logicSkill_0(pCard, FightCard, MonsterCard, FightIndex, MonsterIndex, pSkill, enAtkIndex);
    }
    
}


void CFightSkillManager::logicSkill_4(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{
    
    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    // 满足条件
    int costfunctValue=costFunc(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill);
    if (costfunctValue>=1)
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pSkill->skill_id,FightIndex,MonsterIndex,0,0,pSkill->effect_plist);
        
        CCLog("costfunctValue %d",costfunctValue);
        string resultEffectFile[6];
        if(costfunctValue==1)
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[2]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
            resultEffectFile[3]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
        }
        else if(costfunctValue==2)   //判断单体的条件是否满足。
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[2]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
            resultEffectFile[3]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
            resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_5,enAtkIndex,true);
            resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_6,enAtkIndex,false);
        }
        switch (pSkill->skill_type)
        {
            case 1:
                MonsterCard[MonsterIndex]->appendEngry(30);
                break;
            case 3:
                for (int i=FightIndex+1; i<FightCard.size(); i++) {
                    if(FightCard[i])
                    {
                        FightCard[i]->appendEngry(10);
                    }
                }
                break;
            default:
                break;
        }
        
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        //用户发动了拥护的技能
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
        CCLog("用户发动了用户的技能3");
       // appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
        for (int i=0; i<6; i++)
        {
            CCLog("resultEffectFile[i] :%d :%s",i,resultEffectFile[i].c_str());
            if (!resultEffectFile[i].empty())
            {
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enAtkIndex,FightIndex,MonsterIndex,0,0,0,0,0,m_nEffectId[i],resultEffectFile[i]));
            }
        }
    }
    else
    {
        logicSkill_0(pCard, FightCard, MonsterCard, FightIndex, MonsterIndex, pSkill, enAtkIndex);
    }
}


void CFightSkillManager::logicSkill_5(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enAtkIndex)
{
    
    //通过找到logc
    int currHp=FightCard[FightIndex]->m_iCurrHp;
    int engry=FightCard[FightIndex]->m_iCurrEngry;
    int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
    int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
    // 满足条件
    int costfunctValue=costFunc(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill);
    if (costfunctValue>=1)
    {
        appendVector(EN_ANIMATIONTYPEREFACTOR_SKILL,enAtkIndex,pSkill->skill_id,FightIndex,MonsterIndex,0,0,pSkill->effect_plist);
        CCLog("costfunctValue %d",costfunctValue);
        string resultEffectFile[6];
        if(costfunctValue==1)
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[2]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3,enAtkIndex,true);
            resultEffectFile[3]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4,enAtkIndex,false);
        }
        else if(costfunctValue==2)   //判断单体的条件是否满足。
        {
            resultEffectFile[0]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1,enAtkIndex,true);
            resultEffectFile[1]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2,enAtkIndex,false);
            resultEffectFile[4]=logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_5,enAtkIndex,true);
            resultEffectFile[5]=logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_6,enAtkIndex,false);
        }
        switch (pSkill->skill_type)
        {
            case 1:
                MonsterCard[MonsterIndex]->appendEngry(30);
                break;
            case 3:
                for (int i=FightIndex+1; i<FightCard.size(); i++) {
                    if(FightCard[i])
                    {
                        FightCard[i]->appendEngry(10);
                    }
                }
                break;
            default:
                break;
        }
        
        currHp-=FightCard[FightIndex]->m_iCurrHp;
        engry -=FightCard[FightIndex]->m_iCurrEngry;
        monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
        monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
        //用户发动了拥护的技能
        FightCard[FightIndex]->setNegativeToZero();
        MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
        CCLog("用户发动了用户的技能3");
        //appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enAtkIndex);
        for (int i=0; i<6; i++)
        {
            CCLog("resultEffectFile[i] :%d :%s",i,resultEffectFile[i].c_str());
            if (!resultEffectFile[i].empty())
            {
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enAtkIndex,FightIndex,MonsterIndex,0,0,0,0,0,m_nEffectId[i],resultEffectFile[i]));
            }
        }
    }
    else
    {
        logicSkill_0(pCard, FightCard, MonsterCard, FightIndex, MonsterIndex, pSkill, enAtkIndex);
    }
}


int CFightSkillManager::costFunc(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pData)
{
    if(checkIsManzuCostTiaojian(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pData->cost_func_1,pData->cost_parameter_1,pData->cost_parameter_p_1))
    {
        //发动怒气技能 并影响起伤害值得
        CCLog("=============aaa=======,%d",pData->cost_func_2);
        if(checkIsManzuCostTiaojian(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pData->cost_func_2,pData->cost_parameter_2,pData->cost_parameter_p_2))
        {
            
            return 2;
        }
        return 1;
    }
    CCLog("=>CurrEngry:%d",pCard->m_iCurrEngry);
    return 0;
}

bool CFightSkillManager::checkIsManzuCostTiaojian(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,int costType,int costParam,int costParamP_n)
{
    switch (costType) {
        case 0:
            return  true;
            break;
        case 1:
        {
            if(pCard->m_iCurrEngry>=costParam)
            {
                CCLog("2CurrEngry:%d",pCard->m_iCurrEngry);
                pCard->m_iCurrEngry -=costParam;
                CCLog("2CurrEngry:%d",pCard->m_iCurrEngry);
                return true;
            }
            return false;
        }
        case 2:
        {
            CFightCard *pMonster=MonsterCard[MonsterIndex];
            //发动怒气技能 并影响起伤害值得
            if(pMonster->isHaveBufferRefactor(costParam))
            {
                return true;
            }
            return false;
        }
        case 3:
        {
            CFightCard *pFightCard=FightCard[FightIndex];
            if(pFightCard->isHaveBufferRefactor(costParam))
            {
                return true;
            }
            return false;
        }
        case 4:
        {
            for (int i=0;i<MonsterCard.size(); i++)
            {
                if(MonsterCard[i])
                {
                    if (MonsterCard[i]->m_pCard->m_icard_stirps==costParam)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        case 5:
        {
            CFightCard *pFightCard=FightCard[FightIndex];
            int value=pFightCard->m_iCurrHp/pFightCard->m_iHp;
            if(value>=costParam &&value<=costParamP_n)
            {
                return true;
            }
            return false;
        }
        case 6:
        {
            CFightCard *pMonster=MonsterCard[MonsterIndex];
            //发动怒气技能 并影响起伤害值得
            if(pMonster->m_pCard->m_nCard_star==costParam)
            {
                return true;
            }
            return false;
        }
        case 7:
        {
            return SinglePlayer::instance()->isCheckNeedSatisfied(costParam,pCard->getTEAMPOSTION());
        }
        case 8:
        {
            CFightCard *pMonster=MonsterCard[MonsterIndex];
            return  pMonster->isHaveParamid10(costParam);
        }
        default:
            break;
    }
    
    return true;
}



//每一种的效果id 对应的参数值得又不一样
string CFightSkillManager::effect_0(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    cout<<"0:pMonterCard->m_iCurrHp"<<pMonterCard->m_iCurrHp<<endl;
    //    CEffectInterface *effect=new CEffectInterfaceOne
    return "";
}

string CFightSkillManager::effect_1(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    //伤害值=（parameter_1+自身卡牌当前攻击力*（parameter_2/100)+目标总血量*parameter_3/100-目标卡牌当前的防御力
    CEffectInterface *effect=new CEffectInterfaceOneRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_2(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceTwoRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_3(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceThreeRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_4(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceFourRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_5(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceFiveRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_6(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
//    CEffectInterface *effect=new CEffectInterFaceSixRefactor();
//    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
//    string result=effect->getNeedAppendBufferFile();
//    CC_SAFE_DELETE(effect);
//    return result;
    return "";
}

string CFightSkillManager::effect_7(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    return "";
}

string CFightSkillManager::effect_8(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
       CEffectInterface *effect=new CEffectInterFaceEightRefactor();
        effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
        string result=effect->getNeedAppendBufferFile();
        result+=","+effect->getNeadLongLiveBufferFile();
        CC_SAFE_DELETE(effect);
        return result;
}

string CFightSkillManager::effect_9(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    return "";
}

string CFightSkillManager::effect_10(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    return "";
}

string CFightSkillManager::effect_11(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CCLog("pCimapact:%d",pCimapact->m_ieffect_id);
    CEffectInterface *effect=new CEffectInterFaceElevenRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CCLog("result :%s",result.c_str());
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_21(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceTwentyOneRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

string CFightSkillManager::effect_24(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceTwentyFourRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    string result=effect->getNeedAppendBufferFile();
    result+=","+effect->getNeadLongLiveBufferFile();
    CC_SAFE_DELETE(effect);
    return result;
}

bool CFightSkillManager::CardFighting(CFightCard *pCard,vector<CFightCard *>fightCard,vector<CFightCard *>monsterCard,int FightIndex,int MonstIndex,EN_SEND_SKILL enskill,EN_ATKFIGHT_INDEX enAtkFightIndex)
{
    if(pCard)
    {
        CSkillData *pSkilldata=NULL;
        switch (enskill)
        {
            case EN_SEND_SKILL_ANGRY:
                pSkilldata=SinglePlayer::instance()->getSkillBySkillId(pCard->m_pCard->m_iskillLine);
                break;
            case EN_SEND_SKILL_DEAD:
                pSkilldata=SinglePlayer::instance()->getSkillBySkillId(pCard->m_pCard->m_iskillDead);
                break;
            case EN_SEND_SKILL_HELP:
                pSkilldata=SinglePlayer::instance()->getSkillBySkillId(pCard->m_pCard->m_iskillHelp);
                break;
            case EN_SEND_SKILL_BUFF:
                pSkilldata=SinglePlayer::instance()->getSkillBySkillId(pCard->m_pCard->m_iskillBuff);
                CCLog("用户发动了用户的技能5");
                break;
            default:
                break;
        }
        
        if(pSkilldata)
        {
            char data[4];
            sprintf(data,"%d",pSkilldata->skill_logic);
            CCLog("pSkilldata->skill_logic:data=%s,%d",data,pSkilldata->skill_id);
            IteratorMapPfunc it=m_vSkillManagerLogic.find(data);
            if(it!=m_vSkillManagerLogic.end())
            {
                // 攻击可能是群体攻击 所以这个地方需要修改
                pFunc pfuncCallBack=it->second;
                // 调用发动技能
                InitEffectId(pSkilldata);
                if(fightCard[FightIndex]&& monsterCard[MonstIndex])
                {
                    (*pfuncCallBack)(pCard,fightCard,monsterCard,FightIndex,MonstIndex,pSkilldata,enAtkFightIndex);
                    return true;
                }
                
            }
            
        }
    }
    return false;
}

//void CFightSkillManager::appendAnimation(int AtkIndex,int DefIndex,int AddHp,int SubHp,int skillid,int AddEngry,int subAngry,EN_ANIMATIONTYPE enAnimationType,EN_ATKFIGHT_INDEX enatkindex,int effectPlistId)
//{
//    
//    CCLog("aaaaaappend %d",totoalanimation++);
//    m_animationVector.push_back(new CAnimationSpriteGameFight(enAnimationType,enatkindex,AtkIndex,DefIndex,AddHp,SubHp,AddEngry,subAngry,skillid,effectPlistId));
//}

void CFightSkillManager::appendBuffIcon(CFightCard *fightCard,CFightCard *monster,EN_ATKFIGHT_INDEX enAtkFightIndex)
{
    switch (enAtkFightIndex) {
        case EN_ATKFIGHT_INDEX_LEFT_LORD:
        {
            CFightCardBufferDataEveryFight *fightVectorItem=new CFightCardBufferDataEveryFight;
            for (list<CCardBufferStatusRefactor *>::iterator it=fightCard->m_vlistBuffer.begin(); it!=fightCard->m_vlistBuffer.end(); it++) {
                fightVectorItem->m_arrayFight[0]->appendBufferPngList((*it)->m_iEffectid,(*it)->m_iKeepTime);
            }
            for (list<CCardBufferStatusRefactor *>::iterator it=monster->m_vlistBuffer.begin(); it!=monster->m_vlistBuffer.end(); it++) {
                fightVectorItem->m_arrayFight[1]->appendBufferPngList((*it)->m_iEffectid,(*it)->m_iKeepTime);
            }
            
            m_animationBufferVector.push_back(fightVectorItem);
        }
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_LORD:
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
            break;
        default:
            break;
    }
}

void CFightSkillManager::clearAnimationList()
{
    DELETE_POINT_VECTOR(m_animationVector, vector<CAnimationSpriteGameFight *>,CAnimationSpriteGameFight);
    m_animationVector.clear();
    DELETE_POINT_VECTOR(m_vAnimationStrip, vector<CAnimationSctrip *>,CAnimationSctrip);
    m_vAnimationStrip.clear();
    
}

void CFightSkillManager::appendVector(EN_ANIMATIONTYPEREFACTOR entype,EN_ATKFIGHT_INDEX enAtkIndex,
                                                  int skillId,
                                                  int nATKindex,
                                                  int nDefIndex,
                                                  int nAddHp,int nSubHp,string sRunActionFile)
{
    CCLog("sRunActionFile:%s",sRunActionFile.c_str());
    if ((entype==EN_ANIMATIONTYPEREFACTOR_BUFFER_OWN||entype==EN_ANIMATIONTYPEREFACTOR_BUFFER_MONSTER) && sRunActionFile.empty())
    {
        return ;
    }else if(entype==EN_ANIMATIONTYPEREFACTOR_SHANGHAI && (nAddHp==0 && nSubHp==0))
    {
        return ;
    }
    m_vAnimationStrip.push_back(new CAnimationSctrip(entype,enAtkIndex,skillId,nATKindex,nDefIndex,nAddHp,nSubHp,sRunActionFile));
}

void CFightSkillManager::initBeginFightStatus(CFightCard *pInitCard, int effectSkillId)
{
    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(effectSkillId);
    if (pImpact)
    {
        string loginIdString=ConvertToString(pImpact->m_ieffect_logic);
        IteratorEffMapPfunc pt=m_vEffictManager.find(loginIdString.c_str());
        if(pt!=m_vEffictManager.end())
        {
            pbEffFunc ptfunction=pt->second;
            if(pInitCard&& pInitCard->m_iCurrHp>0) //攻击第一个 血量大于0的人 也就是主将
            {
                (*ptfunction)(pInitCard,NULL,NULL,pImpact,EN_ATKOBJECT_INITFIGHT);//攻击对象对方当个
            }
        }
    }
    
}

void CFightSkillManager::initSkill()
{
    m_vSkillManagerLogic["1"]=&CFightSkillManager::logicSkill_1;
    m_vSkillManagerLogic["2"]=&CFightSkillManager::logicSkill_2;
    m_vSkillManagerLogic["3"]=&CFightSkillManager::logicSkill_3;
    m_vSkillManagerLogic["4"]=&CFightSkillManager::logicSkill_4;
    m_vSkillManagerLogic["5"]=&CFightSkillManager::logicSkill_5;

    m_vEffictManager["0"] =&CFightSkillManager::effect_0;
    m_vEffictManager["1"] =&CFightSkillManager::effect_1;
    m_vEffictManager["2"] =&CFightSkillManager::effect_2;
    m_vEffictManager["3"] =&CFightSkillManager::effect_3;
    m_vEffictManager["4"] =&CFightSkillManager::effect_4;
    m_vEffictManager["5"] =&CFightSkillManager::effect_5;
    m_vEffictManager["6"] =&CFightSkillManager::effect_6;
    m_vEffictManager["7"] =&CFightSkillManager::effect_7;
    m_vEffictManager["8"] =&CFightSkillManager::effect_8;
    m_vEffictManager["9"] =&CFightSkillManager::effect_9;
    m_vEffictManager["10"]=&CFightSkillManager::effect_10;
    m_vEffictManager["11"]=&CFightSkillManager::effect_11;
    m_vEffictManager["21"]=&CFightSkillManager::effect_21;
    m_vEffictManager["24"]=&CFightSkillManager::effect_24;
}

//当前是否可以攻击
bool CFightSkillManager::isCanSpendAtkToMonster(CFightCard *pFight)
{
    return true;
}


void CFightSkillManager::dealWithBuffer(CFightCard *pFightCard,int AtkIndex, int DefIndex,EN_ATKFIGHT_INDEX enAtkIndex)//处理自身的buffer
{
    if (pFightCard==NULL ||pFightCard->m_iCurrHp<=0 || pFightCard->m_vlistBuffer.size()==0)
    {
        return;
    }
    for (list<CCardBufferStatusRefactor *>::iterator it=pFightCard->m_vlistBuffer.begin(); it!=pFightCard->m_vlistBuffer.end();)
    {
        
        CCardBufferStatusRefactor *pCardBufferRefactor=(*it);
        bool isNeedAddIterator=true;
        if (pCardBufferRefactor)
        {
            if(pCardBufferRefactor->m_iEffect_time<=0 &&pCardBufferRefactor ->m_iKeepTime<=0)
            {
                if(pCardBufferRefactor->m_iNeedAddBack)
                {
                    pFightCard->appendHp(-pCardBufferRefactor->m_iHp);
                    pFightCard->subAtk(-pCardBufferRefactor->m_iAtk);
                    pFightCard->subDef(-pCardBufferRefactor->m_iDef);
                    pFightCard->appendEngry(-pCardBufferRefactor->m_iEngry);
                }
                //添加一个删除
                //appendAnimation(AtkIndex, DefIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_REMOVEPLIST, enAtkIndex,pCardBufferRefactor->m_iEffectid);
                appendVector(EN_ANIMATIONTYPEREFACTOR_REMOVEPLIST, enAtkIndex, pCardBufferRefactor->m_iEffectid, AtkIndex, DefIndex, 0, 0, "");
                
                CC_SAFE_DELETE(pCardBufferRefactor);
                it=pFightCard->m_vlistBuffer.erase(it);
                isNeedAddIterator=false;
            }
            else
            {
                if(pCardBufferRefactor->m_iEffect_time>0)
                {
                    pCardBufferRefactor->m_iEffect_time--;
                    pCardBufferRefactor->m_iKeepTime--;
                    pFightCard->appendHp(pCardBufferRefactor->m_iHp);
                    pFightCard->subAtk(pCardBufferRefactor->m_iAtk);
                    pFightCard->subDef(pCardBufferRefactor->m_iDef);
                    pFightCard->appendEngry(pCardBufferRefactor->m_iEngry);
                }
                else if(pCardBufferRefactor->m_iKeepTime>0)
                {
                    pCardBufferRefactor->m_iKeepTime--;
                    //appendAnimation(AtkIndex, DefIndex, pCardBufferRefactor->m_iHp, 0, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enAtkIndex);
                }
            }
            
        }
        if(isNeedAddIterator)
        {
            it++;
        }
    }
}

