//
//  CEffectInterFaceThreeRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-11-4.
//
//

#include "CEffectInterFaceThreeRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"
CEffectInterFaceThreeRefactor::CEffectInterFaceThreeRefactor()
{
    
}
CEffectInterFaceThreeRefactor::~CEffectInterFaceThreeRefactor()
{
    
}
void CEffectInterFaceThreeRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    int nMubiaoHaoxueLiang=pMonster->m_iHp- pMonster->m_iCurrHp;
        //目标初始攻击力 *（参数2）+ 目标初始攻击力*（目标耗血量/总血量*参数4）
    int nMubiaoChushiGongjiLi=pMonster->getAddValue(pMonster->m_iCurrLevel, 1) ;
    int nMubiaoChushiFangyuLi=pMonster->getAddValue(pMonster->m_iCurrLevel, 2) ;
//    攻击力增加=（参数1+目标耗血量*参数3/100+目标初始攻击力*（参数2/100）+自身总血量*参数4/100
    iChangeatk = (pImapact->m_iParameter_1+ nMubiaoHaoxueLiang*pImapact->m_iParameter_3*0.01 + nMubiaoChushiGongjiLi* pImapact->m_iParameter_2*0.01+pMonster->m_iHp * pImapact->m_iParameter_4*0.01);
    // 防御力增加=（参数5+目标耗血量*参数7/100+目标初始防御力*（参数6/100）+自身总血量*参数8/100
     iChangedef =  pImapact->m_iParameter_5 +nMubiaoHaoxueLiang *pImapact->m_iParameter_7*0.01 + nMubiaoChushiFangyuLi*pImapact->m_iParameter_6*0.01+pMonster->m_iHp *pImapact->m_iParameter_8*0.01;
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}