//
//  CEffectInterFaceElevenRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-8-21.
//
//

#include "CEffectInterFaceElevenRefactor.h"
#include "CSkillData.h"
#include "CCard.h"
#include "gamePlayer.h"
CEffectInterFaceElevenRefactor::CEffectInterFaceElevenRefactor()
{
    
}

CEffectInterFaceElevenRefactor::~CEffectInterFaceElevenRefactor()
{
    
}


void CEffectInterFaceElevenRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    if (pImapact->m_iParameter_1!=0 ||pImapact->m_iParameter_2!=0 || pImapact->m_iParameter_3!=0)
    {
        
        //  血量值的计算公式 parameter_1+自身卡牌攻击力*（parameter_2/100)+目标总血量*parameter_3/100
        iChangeShanghaiHp=pImapact->m_iParameter_1 +
        pCard->m_attack*pImapact->m_iParameter_2/100+pMonster->m_iHp*pImapact->m_iParameter_3/100;
        CCLog("伤害值");
    }
    if(pImapact->m_iParameter_8!=0||pImapact->m_iParameter_9!=0)
    {
        iChangeatk= pImapact->m_iParameter_8 + (pMonster->m_attack*(pImapact->m_iParameter_9/100))-pMonster->m_attack;
    }
    if(pImapact->m_iParameter_4 || pImapact->m_iParameter_5)
    {
        iChangedef =  pImapact->m_iParameter_4  + pMonster->m_defend*pImapact->m_iParameter_5/100;
        CCLog("防御值");
    }
    if(pImapact->m_iParameter_6!=0 ||pImapact->m_iParameter_7!=0)
    {
        iChangeengry= +pImapact->m_iParameter_6 +
        pMonster->m_iEngryMax  * pImapact->m_iParameter_7/100;
    }
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}