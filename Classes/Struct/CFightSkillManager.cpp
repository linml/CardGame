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
static    map <string,pFunc>m_vSkillManagerLogic;
static    map <string,pbFunc>m_vCostFuncManager;
static    map<string, pFunc>m_vEffictManager;

void CFightSkillManager::logicSkill_Putong(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    //普通攻击
    int value=pSkill->parameter_1;
    CImapact *pImpact=SinglePlayer::instance()->findByImpactId(value);
    if (pImpact)
    {
        char data[20];
        sprintf(data, "%d",pImpact->m_iStimpact_id);
        IteratorMapPfunc pt=m_vEffictManager.find(data);
        if(pt!=m_vEffictManager.end())
        {
            pFunc ptfunction=pt->second;
            (*ptfunction)(pCard,pGamePlayer,pMonterCard,pSkill,pImpact);
 
        }
    }
}

void CFightSkillManager::logicSkill_1(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    //通过找到logc
    int i=pSkill->cost_1_func;
    char data[20];
    sprintf(data, "%d",i);
    IteratorMapPbFunc it=m_vCostFuncManager.find(data);
    if( it!=m_vCostFuncManager.end())
    {
        pbFunc func=it->second;
        if(func(pCard,pGamePlayer,pMonterCard,pSkill,pCimapact))
        {
            //满足条件
        }
        else
        {
            //发动普通攻击
        }
    }
}

void CFightSkillManager::logicSkill_2(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}

void CFightSkillManager::logicSkill_3(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}


void CFightSkillManager::logicSkill_4(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}


bool CFightSkillManager::costFunc_0(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
    return false;
}

bool CFightSkillManager::costFunc_1(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    if(pCard->m_iCurrEngry>=100)
    {
         pCard->m_iCurrEngry-=100;
        //发动怒气技能 并影响起伤害值得
        return true;
    }
    return false;
}

bool CFightSkillManager::costFunc_2(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
        return   false;
}
bool CFightSkillManager::costFunc_3(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    return   false;
}
bool CFightSkillManager::costFunc_4(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    return   false;
}
bool CFightSkillManager::costFunc_5(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
        return false;
}
bool CFightSkillManager::costFunc_6(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
        return false;
}

bool CFightSkillManager::costFunc_7(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
        return false;
}

bool CFightSkillManager::costFunc_8(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    return false;
    
}

bool CFightSkillManager::costFunc_9(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    return false;
}




//每一种的效果id 对应的参数值得又不一样
void CFightSkillManager::effect_0(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    if (pCimapact->m_iParameter_1) {
        pMonterCard->m_iCurrHp-=50;
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

void CFightSkillManager::effect_1(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_2(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact){
    
}
void CFightSkillManager::effect_3(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_4(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_5(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_6(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_7(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_8(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_9(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}
void CFightSkillManager::effect_10(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact)
{
    
}





void CFightSkillManager::initSkill()
{
    m_vSkillManagerLogic["0"]=&CFightSkillManager::logicSkill_Putong;
    m_vSkillManagerLogic["1"]=&CFightSkillManager::logicSkill_2;
    m_vSkillManagerLogic["2"]=&CFightSkillManager::logicSkill_3;
    m_vSkillManagerLogic["3"]=&CFightSkillManager::logicSkill_4;
    
    
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
    
    m_vEffictManager["0"]=&CFightSkillManager::effect_0;
    m_vEffictManager["1"]=&CFightSkillManager::effect_1;
    m_vEffictManager["2"]=&CFightSkillManager::effect_2;
    m_vEffictManager["3"]=&CFightSkillManager::effect_3;
    m_vEffictManager["4"]=&CFightSkillManager::effect_4;
    m_vEffictManager["5"]=&CFightSkillManager::effect_5;
    m_vEffictManager["6"]=&CFightSkillManager::effect_6;
    m_vEffictManager["7"]=&CFightSkillManager::effect_7;
    m_vEffictManager["8"]=&CFightSkillManager::effect_8;
    m_vEffictManager["9"]=&CFightSkillManager::effect_9;
    m_vEffictManager["10"]=&CFightSkillManager::effect_10;
}