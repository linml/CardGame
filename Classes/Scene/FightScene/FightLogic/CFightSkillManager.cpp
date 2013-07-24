
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
#include "CEffectInterfaceOne.h"
#include <vector>
using namespace std;

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
            for (int i=0; i<pMonterCard.size()-1; i++)
            {
                if(pMonterCard[i]&&pMonterCard[i]->m_iCurrHp>0) //攻击第一个 血量大于0的人 也就是主将
                {
                    (*ptfunction)(pCard,pMonterCard[i],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
                    break;
                }
            }
            
        }
    }
}

void CFightSkillManager::logicSkill_Putong(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill)
{
    //普通攻击
    int value=pSkill->parameter_1;
    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(value);
    if (pImpact)
    {
        char data[20];
        sprintf(data, "%d",pImpact->m_ieffect_logic);
        IteratorEffMapPfunc pt=m_vEffictManager.find(data);
        if(pt!=m_vEffictManager.end())
        {
            pbEffFunc ptfunction=pt->second;
            for (int i=0; i<pMonterCard.size()-1; i++)
            {
                if(pMonterCard[i]&&pMonterCard[i]->m_iCurrHp>0)
                {
                     (*ptfunction)(pCard,pMonterCard[i],pSkill,pImpact,EN_ATKOBJECT_MONSTER);//攻击对象对方当个
                    break;
                }
            }
           
        }
    }
    
}

void CFightSkillManager::logicSkill_1(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    //通过找到logc
    int i=pSkill->cost_func_1;
    char data[20];
    sprintf(data, "%d",i);
    IteratorMapPbFunc it=m_vCostFuncManager.find(data);
    if( it!=m_vCostFuncManager.end())
    {
        pbFunc func=it->second;
        if((*func)(pCard))   //判断单体的条件是否满足。 
        {
            int currHp=FightCard[FightIndex]->m_iCurrHp;
            int engry=FightCard[FightIndex]->m_iCurrEngry;
            int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
            int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
            // 满足条件
            // 发动effect的攻击效果
            if(pSkill->parameter_1!=0)
            {
                logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_1);
            }
            if(pSkill->parameter_2!=0)
            {
                logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_2);
            }
            if(pSkill->parameter_3!=0)
            {
                logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pSkill,pSkill->parameter_3);
            }
            if(pSkill->parameter_4!=0)
            {
                logicSkillFight(pCard,FightCard,FightCard,FightIndex,FightIndex,pSkill,pSkill->parameter_4);
            }
            if(pSkill->parameter_5!=0)
            {
                for (int i=MonsterIndex; i<MonsterCard.size(); i++)
                {
                    if(MonsterCard[i]&&MonsterCard[i]->m_iCurrHp>=0)
                    {
                        logicSkillFight(pCard,FightCard,MonsterCard,FightIndex,i,pSkill,pSkill->parameter_5);
                    }
                }
            }
            if(pSkill->parameter_6!=0)
            {
                for (int i=FightIndex; i<FightCard.size(); i++)
                {
                    if(FightCard[i]&&FightCard[i]->m_iCurrHp>=0)
                    {
                        logicSkillFight(pCard,FightCard,FightCard,FightIndex,i,pSkill,pSkill->parameter_6);

                    }
                }
            }
            currHp-=FightCard[FightIndex]->m_iCurrHp;
            engry -=FightCard[FightIndex]->m_iCurrEngry;
            monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
            monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
            appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
        }
        else
        {
            CSkillData *pPutongSkill=SinglePlayer::instance()->getPutongGongji();
            if (pPutongSkill)
            {
                int currHp=FightCard[FightIndex]->m_iCurrHp;
                int engry=FightCard[FightIndex]->m_iCurrEngry;
                int monstercurrHp=MonsterCard[MonsterIndex]->m_iCurrHp;
                int monstercurrEngry=MonsterCard[MonsterIndex]->m_iCurrEngry;
                logicSkill_Putong(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pPutongSkill);
                currHp-=FightCard[FightIndex]->m_iCurrHp;
                engry -=FightCard[FightIndex]->m_iCurrEngry;
                monstercurrHp-=MonsterCard[MonsterIndex]->m_iCurrHp;
                monstercurrEngry-=MonsterCard[MonsterIndex]->m_iCurrEngry;
                appendAnimation(FightIndex, MonsterIndex, -currHp, -monstercurrHp, pPutongSkill->skill_id,-engry, -monstercurrEngry, EN_ANIMATIONTYPE_HERO, enatk);
            }
        }
    }
}

void CFightSkillManager::logicSkill_2(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    
}

void CFightSkillManager::logicSkill_3(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    
}


void CFightSkillManager::logicSkill_4(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk)
{
    
}


bool CFightSkillManager::costFunc_0(CFightCard *pCard)
{
    
    return true;
}

bool CFightSkillManager::costFunc_1(CFightCard *pCard)
{
    if(pCard->m_iCurrEngry>=100)
    {
         pCard->m_iCurrEngry-=100;
        //发动怒气技能 并影响起伤害值得
        return true;
    }
    return false;
}

bool CFightSkillManager::costFunc_2(CFightCard *pCard)
{
        return   false;
}
bool CFightSkillManager::costFunc_3(CFightCard *pCard)
{
    return   false;
}
bool CFightSkillManager::costFunc_4(CFightCard *pCard)
{
    return   false;
}
bool CFightSkillManager::costFunc_5(CFightCard *pCard)
{
        return false;
}
bool CFightSkillManager::costFunc_6(CFightCard *pCard)
{
        return false;
}
bool CFightSkillManager::costFunc_7(CFightCard *pCard)
{
        return false;
}

bool CFightSkillManager::costFunc_8(CFightCard *pCard)
{
    return false;
    
}

bool CFightSkillManager::costFunc_9(CFightCard *pCard)
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
    cout<<"pMonterCard->m_iCurrHp"<<pMonterCard->m_iCurrHp<<endl;
    //伤害值=（parameter_1+自身卡牌当前攻击力*（parameter_2/100)+目标总血量*parameter_3/100-目标卡牌当前的防御力
    CEffectInterface *effect=new CEffectInterfaceOne();
    effect->logicFightingCardByFightAndMonster(pCard,pMonterCard,pCimapact);
    delete effect;
    effect=NULL;
}
void CFightSkillManager::effect_2(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject){
    
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

void CFightSkillManager::CardFighting(CFightCard *pCard,vector<CFightCard *>fightCard,vector<CFightCard *>monsterCard,int FightIndex,int MonstIndex,EN_SEND_SKILL enskill,EN_ATKFIGHT_INDEX enAtkFightIndex)
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
                break;
            default:
                break;
        }
        
        if(pSkilldata)
        {
            char data[4];
            sprintf(data,"%d",pSkilldata->skill_logic);
            IteratorMapPfunc it=m_vSkillManagerLogic.find(data);
            if(it!=m_vSkillManagerLogic.end())
            {
                //攻击可能是群体攻击 所以这个地方需要修改
                pFunc pfuncCallBack=it->second;
                //调用发动技能
                (*pfuncCallBack)(pCard,fightCard,monsterCard,FightIndex,MonstIndex,pSkilldata,enAtkFightIndex);
            }
        }
    }   
    
}

void CFightSkillManager::appendAnimation(int AtkIndex,int DefIndex,int AddHp,int SubHp,int skillid,int AddEngry,int subAngry,EN_ANIMATIONTYPE enAnimationType,EN_ATKFIGHT_INDEX enatkindex)
{
    m_animationVector.push_back(new CAnimationSpriteGameFight(enAnimationType,enatkindex,AtkIndex,DefIndex,AddHp,SubHp,AddEngry,subAngry,skillid,0                                                      ));
    
    
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
    if(pFight->m_vBuffer.size()>0)
    {
        return false;
    }
    return true;
}

//当前添加buffer的属性
void CFightSkillManager::addOrSubBuffer(CFightCard *pFight)
{
    
    for (int i=0; i<pFight->m_vBuffer.size(); i++) {
        
    }
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
    
    if (pFightCard==NULL || pFightCard->m_vBuffer.size()==0) {
        return;
    }
    for (list<CCardBufferStatus *>::iterator it=pFightCard->m_vBuffer.begin(); it!=pFightCard->m_vBuffer.end();)
    {
        CCardBufferStatus *pCardBuffer=(*it);
        bool isNeedAddIterator=true;
        switch ((pCardBuffer)->m_enBuffer_Field)
        {
            case EN_BUFF_FIELD_TYPE_ATTACK:   //影响攻击力 攻击力我只回复一次。不带这样多次减去的
                if (pCardBuffer->m_iBuff_showTimes==0 && pCardBuffer->m_iBuff_effectTimes==0)
                {
                    pFightCard->m_attack+=-pCardBuffer->m_iValue*pCardBuffer->m_iBuff_effectTimes;   //恢复攻击力
                    pFightCard->m_vBuffer.erase(it);
                    delete pCardBuffer;
                    pCardBuffer=NULL;
                    isNeedAddIterator=false;
                }
                else
                {
                    if (pCardBuffer->m_iBuff_effectTimes>0)
                    {
                        pCardBuffer->m_iBuff_effectTimes--; //扣除减去的次数
                        pFightCard->m_attack+=pCardBuffer->m_iValue;

                    }
                    if (pCardBuffer->m_iBuff_showTimes>0)
                    {
                        pCardBuffer->m_iBuff_showTimes--;
                       appendAnimation(AtkIndex, DefIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enatkindex);
                    }
               }
                break;
            case EN_BUFF_FIELD_TYPE_DEFEND:   //影响防御力
            {
                if (pCardBuffer->m_iBuff_showTimes==0 && pCardBuffer->m_iBuff_effectTimes==0)
                {
                    pFightCard->m_defend+=-pCardBuffer->m_iValue*pCardBuffer->m_iBuff_effectTimes;   //恢复攻击力
                    pFightCard->m_vBuffer.erase(it);
                    delete pCardBuffer;
                    pCardBuffer=NULL;
                    isNeedAddIterator=false;
                }
                else
                {
                    if (pCardBuffer->m_iBuff_effectTimes>0)
                    {
                        pCardBuffer->m_iBuff_effectTimes--; //扣除减去的次数
                        pFightCard->m_defend+=pCardBuffer->m_iValue;
                        
                    }
                    if (pCardBuffer->m_iBuff_showTimes>0)
                    {
                        pCardBuffer->m_iBuff_showTimes--;
                        appendAnimation(AtkIndex, DefIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enatkindex);
                    }
                }

            }
                break;
            case EN_BUFF_FIELD_TYPE_ANGRY:    //怒气
            {
                if (pCardBuffer->m_iBuff_showTimes==0 && pCardBuffer->m_iBuff_effectTimes==0)
                {
                    pFightCard->m_vBuffer.erase(it);
                    delete pCardBuffer;
                    pCardBuffer=NULL;
                    isNeedAddIterator=false;
                }
                else
                {
                    if (pCardBuffer->m_iBuff_effectTimes>0)
                    {
                        pCardBuffer->m_iBuff_effectTimes--; //扣除减去的次数
                        pFightCard->m_iCurrHp+=pCardBuffer->m_iValue;
                        appendAnimation(AtkIndex, DefIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enatkindex);

                        
                    }
                    if (pCardBuffer->m_iBuff_showTimes>0)
                    {
                        pCardBuffer->m_iBuff_showTimes--;
                    }
                }
                
            }
                break;
            case EN_BUFF_FIELD_TYPE_HP:       //影响HP，
            {
                if (pCardBuffer->m_iBuff_showTimes==0 && pCardBuffer->m_iBuff_effectTimes==0)
                {
                    pFightCard->m_vBuffer.erase(it);
                    delete pCardBuffer;
                    pCardBuffer=NULL;
                    isNeedAddIterator=false;
                }
                else
                {
                    if (pCardBuffer->m_iBuff_effectTimes>0)
                    {
                        pCardBuffer->m_iBuff_effectTimes--; //扣除减去的次数
                        pFightCard->m_iCurrHp+=pCardBuffer->m_iValue;
                        appendAnimation(AtkIndex, DefIndex, 0, pCardBuffer->m_iValue, 0, 0, 0, EN_ANIMATIONTYPE_BUFFER, enatkindex);
                    }
                    if (pCardBuffer->m_iBuff_showTimes>0)
                    {
                        pCardBuffer->m_iBuff_showTimes--;
                    }
                }

            }
                break;
            case EN_BUFF_FIELD_TYPE_BINGFENG: //被冰封
                break;
            case EN_BUFF_FIELD_TYPE_XUANYUN:  //眩晕
                break;
            case EN_BUFF_FIELD_TYPE_DUYAO:    //毒药:
                break;
                
            default:
                break;
        }
        if(isNeedAddIterator)
        {
            it++;
        }
    }
}

