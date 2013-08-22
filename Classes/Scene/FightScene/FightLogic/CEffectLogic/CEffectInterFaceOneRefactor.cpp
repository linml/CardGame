//
//  CEffectInterFaceOneRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-8-21.
//
//

#include "CEffectInterFaceOneRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"
CEffectInterfaceOneRefactor::CEffectInterfaceOneRefactor()
{
    
}
CEffectInterfaceOneRefactor::~CEffectInterfaceOneRefactor()
{
    
}
void CEffectInterfaceOneRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    if (pImapact->m_iParameter_1!=0 ||pImapact->m_iParameter_2!=0 || pImapact->m_iParameter_3!=0)
    {
        int tempdata=pImapact->m_iParameter_1 +
        pCard->m_attack*pImapact->m_iParameter_2/100+pMonster->m_iHp*pImapact->m_iParameter_3/100;
        iChangeShanghaiHp=((tempdata-pMonster->m_defend)>=tempdata*0.2?tempdata-pMonster->m_defend :tempdata*0.2) ;
        iChangeShanghaiHp =-iChangeShanghaiHp;
    }
    if(pImapact->m_iParameter_8!=0||pImapact->m_iParameter_9!=0)
    {
        iChangeatk= +pImapact->m_iParameter_8 + pMonster->getAddValue(pMonster->m_iCurrLevel, 1)*pImapact->m_iParameter_9/100;
    }
    if(pImapact->m_iParameter_4 ||pImapact->m_iParameter_5)
    {
        iChangedef =  pImapact->m_iParameter_4  + pMonster->getAddValue(pMonster->m_iCurrLevel, 2)*pImapact->m_iParameter_5/100;
    }
    if(pImapact->m_iParameter_6!=0 ||pImapact->m_iParameter_7!=0)
    {
        iChangeengry= +pImapact->m_iParameter_6 +
        pMonster->m_iCurrEngry  * pImapact->m_iParameter_7/100;
        iChangeengry =-iChangeengry;
    }
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}