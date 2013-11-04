//
//  CEffectInterFaceFiveRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-11-4.
//
//

#include "CEffectInterFaceFiveRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"

CEffectInterFaceFiveRefactor::CEffectInterFaceFiveRefactor()
{
    
}
CEffectInterFaceFiveRefactor::~CEffectInterFaceFiveRefactor()
{
    
}
void CEffectInterFaceFiveRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    int nZishenHaoxueliang=pCard->m_iHp-pCard->m_iCurrHp;
    int nMubiaoHaoxueliang=pMonster->m_iHp-pCard->m_iCurrHp;
    //（自身当前攻击力+参数1+自身当前攻击力*（参数2+自身耗血量/自身总血量*参数7）+自身耗血量*参数6/100）
    int value1=(pCard->m_attack + pImapact->m_iParameter_1 +pCard->m_attack *(pImapact->m_iParameter_2+nZishenHaoxueliang/pCard->m_iHp *pImapact->m_iParameter_7 )+nZishenHaoxueliang*pImapact->m_iParameter_6*0.01)- (pMonster->m_defend - pImapact->m_iParameter_4-pMonster->m_defend *(pImapact->m_iParameter_5+nMubiaoHaoxueliang/pMonster->m_iHp *pImapact->m_iParameter_9)+nMubiaoHaoxueliang*pImapact->m_iParameter_8*0.01);
    //（自身当前攻击力+参数1+自身当前攻击力*（参数2+自身耗血量/自身总血量*参数7）+自身耗血量*参数6/100）*0.2
    int value2=(pCard->m_attack +pImapact->m_iParameter_1 +pCard->m_attack*(pImapact->m_iParameter_2+nZishenHaoxueliang/pCard->m_iHp*pImapact->m_iParameter_7) +nZishenHaoxueliang*pImapact->m_iParameter_6*0.01)*0.2;
    iChangeShanghaiHp= - (MAX(value1,value2)*(1+pImapact->m_iParameter_3));
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}