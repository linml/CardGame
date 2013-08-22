
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
#include "CFightingCardLayerScene.h"
#include "CEffectInterface.h"
#include "CEffectInterFaceOneRefactor.h"
#include "CEffectInterFaceElevenRefactor.h"
#include "CFightCardBufferData.h"
#include <vector>
using namespace std;
static  int totoalanimation=0;

#include "CAnimationSpriteGameFight.h"
#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}



static    map <string,pFunc>m_vSkillManagerLogic;
static    map <string,pbFunc>m_vCostFuncManager;
static    map<string, pbEffFunc>m_vEffictManager;
vector<CAnimationSpriteGameFight *>CFightSkillManager::m_animationVector;
vector<CFightCardBufferDataEveryFight *>CFightSkillManager::m_animationBufferVector;

void CFightSkillManager::logicSkillFight(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,int paramid)
{
    //普通攻击
    int value=paramid;
    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(value);
    if (pImpact)
    {
        char data[20];
        sprintf(data, "%d",pImpact->m_ieffect_logic);
        IteratorEffMapPfunc pt=m_vEffictManager.find(data);
        if(pt!=m_vEffictManager.end())
        {
            pbEffFunc ptfunction=pt->second;
            if(pMonterCard[MonsterIndex]&&pMonterCard[MonsterIndex]->m_iCurrHp>0) //攻击第一个 血量大于0的人 也就是主将
            {
                (*ptfunction)(pCard,pMonterCard[MonsterIndex],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
            }
        }
    }
}

void CFightSkillManager::logicSkill_Putong(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill)
{
    //普通攻击
    if(pSkill->parameter_1!=0)
    {
        logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1);
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_1);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
    if(pSkill->parameter_2!=0)
    {
        logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2);
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_2);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
    if(pSkill->parameter_3!=0)
    {
        logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3);
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_3);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
    
    if(pSkill->parameter_4!=0)
    {
        logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4);
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_4);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
    if(pSkill->parameter_5!=0)
    {
        //发动增幅技能的特效plist
        for (int i=MonsterIndex; i<pMonterCard.size(); i++)
        {
            if(pMonterCard[i]&&pMonterCard[i]->m_iCurrHp>=0)
            {
                logicSkillFight(pCard,pFightCard,pMonterCard,FightIndex,i,pSkill,pSkill->parameter_5);
            }
        }
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_5);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
    if(pSkill->parameter_6!=0)
    {
        
        //觉得这里需要添加个特效的文件
        for (int i=FightIndex; i<pFightCard.size(); i++)
        {
            if(pFightCard[i]&&pFightCard[i]->m_iCurrHp>=0)
            {
                logicSkillFight(pCard,pFightCard,pFightCard,FightIndex,i,pSkill,pSkill->parameter_6);
            }
        }
        CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_6);
        if (pImpact&&pImpact->m_irandom)
        {
            SinglePlayer::instance()->randRomIndexAdd();
        }
    }
}

void CFightSkillManager::logicSkill_1(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    //通过找到logc
    int i=pSkill->skill_logic;
    char data[20];
    sprintf(data, "%d",i);
    IteratorMapPbFunc it=m_vCostFuncManager.find(data);
    if( it!=m_vCostFuncManager.end())
    {
       
        pbFunc func=it->second;
        int currHp=FightCard[FightIndex]->m_iCurrHp;
        int engry=FightCard[FightIndex]->m_iCurrEngry;
        int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
        int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
        // 满足条件
        if((*func)(pCard,MonsterCard[MonsterIndex],pSkill)==1)   //判断单体的条件是否满足。
        {
             CCLog("条件满足 爆发了技能id:%d",pSkill->skill_id);
            // 发动effect的攻击效果
            if(pSkill->parameter_1!=0)
            {
                logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1);
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_1);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            if(pSkill->parameter_2!=0)
            {
                logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2);
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_2);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            if(pSkill->parameter_3!=0)
            {
                logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3);
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_3);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            
            if(pSkill->parameter_4!=0)
            {
                logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4);
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_4);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            if(pSkill->parameter_5!=0)
            {
                //发动增幅技能的特效plist
                for (int i=MonsterIndex; i<MonsterCard.size(); i++)
                {
                    if(MonsterCard[i]&&MonsterCard[i]->m_iCurrHp>=0)
                    {
                        logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,i,pSkill,pSkill->parameter_5);
                    }
                }
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_5);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            if(pSkill->parameter_6!=0)
            {
                //觉得这里需要添加个特效的文件
                for (int i=FightIndex; i<FightCard.size(); i++)
                {
                    if(FightCard[i]&&FightCard[i]->m_iCurrHp>=0)
                    {
                        logicSkillFight(pCard,FightCard,FightCard,FightIndex,i,pSkill,pSkill->parameter_6);
                    }
                }
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_6);
                if (pImpact&&pImpact->m_irandom)
                {
                    SinglePlayer::instance()->randRomIndexAdd();
                }
            }
            switch (pSkill->skill_type) {
                case 1:
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
            appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
            
        }
        else
        {
          
            CSkillData *pPutongSkill=SinglePlayer::instance()->getPutongGongji();
             CCLog("条件不满足 爆发了技能id:%d",pPutongSkill->skill_id);
            if (pPutongSkill)
            {
                logicSkill_Putong(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pPutongSkill);
                currHp-=FightCard[FightIndex]->m_iCurrHp;
                FightCard[FightIndex]->m_iCurrEngry+=20;
                for (int i=FightIndex+1; i<FightCard.size(); i++)
                {
                    if(FightCard[i])
                    {
                        FightCard[i]->m_iCurrEngry+=10;
                    }
                }
                engry -=FightCard[FightIndex]->m_iCurrEngry;
                monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
                MonsterCard[MonsterIndex]->m_iCurrEngry+=30;
                monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
                FightCard[FightIndex]->setNegativeToZero();
                MonsterCard[MonsterIndex]->setNegativeToZero();            //用户发动了用户的技能
                CCLog("用户发动了用户的技能2");
                appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pPutongSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
            }
        }
    }
}

void CFightSkillManager::logicSkill_2(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    //通过找到logc
    int i=pSkill->skill_logic;
    char data[20];
    sprintf(data, "%d",i);
    IteratorMapPbFunc it=m_vCostFuncManager.find(data);
    if( it!=m_vCostFuncManager.end())
    {
        pbFunc func=it->second;
        int currHp=FightCard[FightIndex]->m_iCurrHp;
        int engry=FightCard[FightIndex]->m_iCurrEngry;
        int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
        int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
        // 满足条件
        int costfunctValue=(*func)(pCard,MonsterCard[MonsterIndex],pSkill);
        if (costfunctValue>=1)
        {
            if(costfunctValue==1)
            {
                if(pSkill->parameter_1!=0)
                {
                    logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_1);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_2!=0)
                {
                    logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_2);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_5!=0)
                {
                    //发动增幅技能的特效plist
                    for (int i=MonsterIndex; i<MonsterCard.size(); i++)
                    {
                        if(MonsterCard[i]&&MonsterCard[i]->m_iCurrHp>=0)
                        {
                            logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,i,pSkill,pSkill->parameter_5);
                        }
                    }
                    //觉得这里需要添加个特效的文件
                    //appendAnimation(FightIndex, MonsterIndex, 0, 0, pSkill->skill_id,0, 0, EN_ANIMATIONTYPE_SKILL, enatk);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_5);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_6!=0)
                {
                    //觉得这里需要添加个特效的文件
                    for (int i=FightIndex; i<FightCard.size(); i++)
                    {
                        if(FightCard[i]&&FightCard[i]->m_iCurrHp>=0)
                        {
                            logicSkillFight(pCard,FightCard,FightCard,FightIndex,i,pSkill,pSkill->parameter_6);
                        }
                    }
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_6);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                
            }
            else if(costfunctValue==2)   //判断单体的条件是否满足。
            {
                // 发动effect的攻击效果
                if(pSkill->parameter_1!=0)
                {
                    logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_1);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_2!=0)
                {
                    logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_2);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_3!=0)
                {
                    logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_3);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_4!=0)
                {
                    logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_4);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_5!=0)
                {
                    //发动增幅技能的特效plist
                    for (int i=MonsterIndex; i<MonsterCard.size(); i++)
                    {
                        if(MonsterCard[i]&&MonsterCard[i]->m_iCurrHp>=0)
                        {
                            logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,i,pSkill,pSkill->parameter_5);
                            
                        }
                    }
                    //觉得这里需要添加个特效的文件
                    //appendAnimation(FightIndex, MonsterIndex, 0, 0, pSkill->skill_id,0, 0, EN_ANIMATIONTYPE_SKILL, enatk);
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_5);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
                if(pSkill->parameter_6!=0)
                {
                    //觉得这里需要添加个特效的文件
                    for (int i=FightIndex; i<FightCard.size(); i++)
                    {
                        if(FightCard[i]&&FightCard[i]->m_iCurrHp>=0)
                        {
                            logicSkillFight(pCard,FightCard,FightCard,FightIndex,i,pSkill,pSkill->parameter_6);
                        }
                    }
                    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_6);
                    if (pImpact&&pImpact->m_irandom)
                    {
                        SinglePlayer::instance()->randRomIndexAdd();
                    }
                }
            }
            switch (pSkill->skill_type) {
                case 1:
                    MonsterCard[MonsterIndex]->m_iCurrEngry+=30;
                    break;
                case 3:
                    for (int i=FightIndex+1; i<FightCard.size(); i++) {
                        if(FightCard[i])
                        {
                            FightCard[i]->m_iCurrEngry+=10;
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
            appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
            if(costfunctValue==2&&pSkill->parameter_3!=0)
            {
                CCLog("pSkill->parameter_3================>%d",pSkill->parameter_3);
                CImapact *pImpact=SinglePlayer::instance()->findByImpactId(pSkill->parameter_3);
                CCLog("apppppend%d",totoalanimation++);
                m_animationVector.push_back(new CAnimationSpriteGameFight(EN_ANIMATIONTYPE_BUFFPLISTOTHER,enatk,FightIndex,MonsterIndex,0,0,0,0,0,0,SinglePlayer::instance()->getBufferPlistByEffectId(pImpact->m_ieffect_id)));
                CCLog("pSkill->parameter_3================>%s",pImpact->m_sEffectFile.c_str());
            }
        }
        else
        {
            CSkillData *pPutongSkill=SinglePlayer::instance()->getPutongGongji();
            if (pPutongSkill)
            {
                logicSkill_Putong(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pPutongSkill);
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
                CCLog("用户发动了用户的技能4");
                appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pPutongSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
            }
        }
    }
    
}

void CFightSkillManager::logicSkill_3(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    
}


void CFightSkillManager::logicSkill_4(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    
}


int CFightSkillManager::costFunc_0(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    
    return true;
}

int CFightSkillManager::costFunc_1(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    if(pCard->m_iCurrEngry>=pData->cost_parameter_1)
    {
        CCLog("CurrEngry:%d",pCard->m_iCurrEngry);
        pCard->m_iCurrEngry -=pData->cost_parameter_1;
        CCLog("CurrEngry:%d",pCard->m_iCurrEngry);
        //发动怒气技能 并影响起伤害值得
        return 1;
    }
    CCLog("=>CurrEngry:%d",pCard->m_iCurrEngry);
    return 0;
}

int CFightSkillManager::costFunc_2(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    if(pData->cost_func_1!=0&&pCard->m_iCurrEngry>=pData->cost_parameter_1)
    {
        CCLog("2CurrEngry:%d",pCard->m_iCurrEngry);
        pCard->m_iCurrEngry -=pData->cost_parameter_1;
        CCLog("2CurrEngry:%d",pCard->m_iCurrEngry);
        //发动怒气技能 并影响起伤害值得
        if(pData->cost_func_2!=0&&pMonster->isHaveBufferRefactor(pData->cost_parameter_2))
        {
            return 2;
        }
        return 1;
    }
    CCLog("=>CurrEngry:%d",pCard->m_iCurrEngry);
    return 0;
}
int CFightSkillManager::costFunc_3(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return   false;
}
int CFightSkillManager::costFunc_4(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return   false;
}
int CFightSkillManager::costFunc_5(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return false;
}
int CFightSkillManager::costFunc_6(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return false;
}
int CFightSkillManager::costFunc_7(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return false;
}

int CFightSkillManager::costFunc_8(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return false;
    
}

int CFightSkillManager::costFunc_9(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData)
{
    return false;
}




//每一种的效果id 对应的参数值得又不一样
void CFightSkillManager::effect_0(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    cout<<"0:pMonterCard->m_iCurrHp"<<pMonterCard->m_iCurrHp<<endl;
    //    CEffectInterface *effect=new CEffectInterfaceOne
    
}

void CFightSkillManager::effect_1(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    //伤害值=（parameter_1+自身卡牌当前攻击力*（parameter_2/100)+目标总血量*parameter_3/100-目标卡牌当前的防御力
    CEffectInterface *effect=new CEffectInterfaceOneRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    delete effect;
    effect=NULL;
}

void CFightSkillManager::effect_2(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}

void CFightSkillManager::effect_3(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_4(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_5(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_6(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_7(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_8(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_9(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_10(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    
}
void CFightSkillManager::effect_11(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    CEffectInterface *effect=new CEffectInterFaceElevenRefactor();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    delete effect;
    effect=NULL;
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
                if(fightCard[FightIndex]&& monsterCard[MonstIndex])
                {
                    (*pfuncCallBack)(pCard,fightCard,monsterCard,FightIndex,MonstIndex,pSkilldata,enAtkFightIndex);
                                    return true;
                }
                return false;

            }
            return false;
        }
        return false;
    }
    return false;
}

void CFightSkillManager::appendAnimation(int AtkIndex,int DefIndex,int AddHp,int SubHp,int skillid,int AddEngry,int subAngry,EN_ANIMATIONTYPE enAnimationType,EN_ATKFIGHT_INDEX enatkindex)
{

    CCLog("aaaaaappend %d",totoalanimation++);
    m_animationVector.push_back(new CAnimationSpriteGameFight(enAnimationType,enatkindex,AtkIndex,DefIndex,AddHp,SubHp,AddEngry,subAngry,skillid,0                                                      ));
}

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
    DELETE_POINT_VECTOR(m_animationVector, vector<CAnimationSpriteGameFight *>);
    m_animationVector.clear();
    
}

void CFightSkillManager::initSkill()
{
    
    m_vSkillManagerLogic["1"]=&CFightSkillManager::logicSkill_1;
    m_vSkillManagerLogic["2"]=&CFightSkillManager::logicSkill_2;
    m_vSkillManagerLogic["3"]=&CFightSkillManager::logicSkill_3;
    m_vSkillManagerLogic["4"]=&CFightSkillManager::logicSkill_4;
    
    //添加costfunc
    m_vCostFuncManager["0"]=&CFightSkillManager::costFunc_0;
    m_vCostFuncManager["1"]=&CFightSkillManager::costFunc_1;
    m_vCostFuncManager["2"]=&CFightSkillManager::costFunc_2;
    m_vCostFuncManager["3"]=&CFightSkillManager::costFunc_3;
    m_vCostFuncManager["4"]=&CFightSkillManager::costFunc_4;
    m_vCostFuncManager["5"]=&CFightSkillManager::costFunc_5;
    m_vCostFuncManager["6"]=&CFightSkillManager::costFunc_6;
    m_vCostFuncManager["7"]=&CFightSkillManager::costFunc_7;
    m_vCostFuncManager["8"]=&CFightSkillManager::costFunc_8;
    m_vCostFuncManager["9"]=&CFightSkillManager::costFunc_9;
    
    //添加effect id
    
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
}



//当前是否可以发动怒气技能
bool CFightSkillManager::isCanSpendAngrySkill(CFightCard *pFight)
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
bool CFightSkillManager::isCanSpendAtkToMonster(CFightCard *pFight)
{
    return true;
}

//当前添加buffer的属性
void CFightSkillManager::addOrSubBuffer(CFightCard *pFight)
{
    
        return ;
}

bool CFightSkillManager::isHavaPhysicHarmMagic(CFightCard *pMonstFight)
{
    
    return false;
}
bool CFightSkillManager::isHaveMagicHarm(CFightCard *pMonstFight)
{
    return false;
}

//普通伤害计算
void CFightSkillManager::basicAtk(CFightCard *pFightCard,CFightCard *pMonstFight)
{
    //取得技能上的值得 来计算；
    //pFightCard->m_pCard->m_iskillLine
}

void CFightSkillManager::dealWithBuffer(CFightCard *pFightCard,int AtkIndex, int DefIndex,EN_ATKFIGHT_INDEX enatkindex)//处理自身的buffer
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
                delete pCardBufferRefactor;
                pCardBufferRefactor=NULL;
                it=pFightCard->m_vlistBuffer.erase(it);
                isNeedAddIterator=false;
            }
            else{
                
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
                    appendAnimation(AtkIndex, DefIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enatkindex);

                }
            }
      
        }
        if(isNeedAddIterator)
        {
            it++;
        }
    }
}

