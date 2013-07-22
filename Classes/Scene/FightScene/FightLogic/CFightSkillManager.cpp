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
#include "CAnimationSpriteGameFight.h"
#include <vector>
using namespace std;

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
            // 满足条件
            // 发动effect的攻击效果
        }
        else
        {
            CSkillData *pPutongSkill=SinglePlayer::instance()->getPutongGongji();
            if (pPutongSkill)
            {
                logicSkill_Putong(pCard,FightCard,MonsterCard,FightIndex,MonsterIndex,pPutongSkill);
                appendAnimation(FightIndex, MonsterIndex, 0, 0, pPutongSkill->skill_id, 0, 0, EN_ANIMATIONTYPE_HERO, enatk);
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
    
    return false;
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
    cout<<"pMonterCard->m_iCurrHp"<<pMonterCard->m_iCurrHp<<endl;
    if (pCimapact->m_iParameter_1) {
        pMonterCard->m_iCurrHp-=pCimapact->m_iParameter_1;
    }
    
    if (pCimapact->m_iParameter_2)
    {
        //计算普通攻击的伤害值得
    }
    
    if (pCimapact->m_iParameter_3)
    {
        
    }
    if (pCimapact->m_iParameter_4)
    {
        
    }
    if (pCimapact->m_iParameter_5)
    {
        
    }
    if (pCimapact->m_iParameter_6)
    {
        
    }
    if (pCimapact->m_iParameter_7)
    {
        
    }
    if (pCimapact->m_iParameter_8)
    {
        
    }
    if (pCimapact->m_iParameter_9)
    {
        
    }
    if (pCimapact->m_iParameter_10)
    {
        
    }
    
}

void CFightSkillManager::effect_1(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject)
{
    cout<<"pMonterCard->m_iCurrHp"<<pMonterCard->m_iCurrHp<<endl;
    //伤害值=（parameter_1+自身卡牌当前攻击力*（parameter_2/100)+目标总血量*parameter_3/100-目标卡牌当前的防御力
    
    
    int mShanghai = pCimapact->m_iParameter_1 +pCard->m_attack*pCimapact->m_iParameter_2/100+pMonterCard->m_iHp*pCimapact->m_iParameter_3/100-pMonterCard->m_defend;
    
    
    int atk = pMonterCard->m_attack-pCimapact->m_iParameter_8 -pMonterCard->m_attack*pCimapact->m_iParameter_9/100;
    
    
    int def = (pMonterCard->m_defend -pCimapact->m_iParameter_4)-pMonterCard->m_defend*pCimapact->m_iParameter_5/100;
    
    int angry= (pMonterCard->m_iEngryMax)-pCimapact->m_iParameter_6 -
    pMonterCard->m_iEngryMax *pCimapact->m_iParameter_7/100;
    
    if (atk) {
        pMonterCard->m_attack=atk;
    }
    if (def) {
        pMonterCard->m_defend=def;
    }
    if (angry) {
        pMonterCard
    }
    
    
    
    
    
    
    
    
    
    
    目标怒气=目标当前怒气-parameter_6-目标怒气满值*parameter_7/100
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
    
    m_vEffictManager["1"] =&CFightSkillManager::effect_0;
    m_vEffictManager["2"] =&CFightSkillManager::effect_1;
    m_vEffictManager["3"] =&CFightSkillManager::effect_2;
    m_vEffictManager["4"] =&CFightSkillManager::effect_3;
    m_vEffictManager["5"] =&CFightSkillManager::effect_4;
    m_vEffictManager["6"] =&CFightSkillManager::effect_5;
    m_vEffictManager["7"] =&CFightSkillManager::effect_6;
    m_vEffictManager["8"] =&CFightSkillManager::effect_7;
    m_vEffictManager["9"] =&CFightSkillManager::effect_8;
    m_vEffictManager["10"] =&CFightSkillManager::effect_9;
    m_vEffictManager["11"]=&CFightSkillManager::effect_10;
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
