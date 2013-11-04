//
//  CEffectInterFaceFourRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-11-4.
//
//

#include "CEffectInterFaceFourRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"

CEffectInterFaceFourRefactor::CEffectInterFaceFourRefactor()
{
    
}
CEffectInterFaceFourRefactor::~CEffectInterFaceFourRefactor()
{
    
}
void CEffectInterFaceFourRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    int nMubiaoHaoxueLiang=pMonster->m_iHp- pMonster->m_iCurrHp;
    //目标初始攻击力 *（参数2）+ 目标初始攻击力*（目标耗血量/总血量*参数4）
    int nMubiaoChushiGongjiLi=pMonster->getAddValue(pMonster->m_iCurrLevel, 1) ;
    int nMubiaoChushiFangyuLi=pMonster->getAddValue(pMonster->m_iCurrLevel, 2) ;
    iChangeatk = pImapact->m_iParameter_1+ nMubiaoHaoxueLiang*pImapact->m_iParameter_3*0.01 + nMubiaoChushiGongjiLi* pImapact->m_iParameter_2+nMubiaoChushiGongjiLi*(nMubiaoHaoxueLiang/pMonster->m_iHp * pImapact->m_iParameter_4);
    iChangeatk = -iChangedef;
    //		防御力增加=（参数5+目标耗血量*参数7/100+目标初始防御力*（参数6）+目标初始防御力*（目标耗血量/总血量*参数8）
    iChangedef =  pImapact->m_iParameter_5 +nMubiaoHaoxueLiang *pImapact->m_iParameter_7*0.01 + nMubiaoChushiFangyuLi*pImapact->m_iParameter_6+nMubiaoChushiFangyuLi *(nMubiaoHaoxueLiang/pMonster->m_iHp *pImapact->m_iParameter_8);
    iChangedef = -iChangedef;
    
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}