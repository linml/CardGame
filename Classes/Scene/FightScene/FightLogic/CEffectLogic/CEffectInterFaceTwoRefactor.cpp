//
//  CEffectInterFaceTwoRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-11-1.
//
//

#include "CEffectInterFaceTwoRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"


CEffectInterFaceTwoRefactor::CEffectInterFaceTwoRefactor()
{
    
}
CEffectInterFaceTwoRefactor::~CEffectInterFaceTwoRefactor()
{
    
}

void CEffectInterFaceTwoRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
        //扣血=参数1+目标总血量*参数2/100+目标耗血量*参数3/100+(参数4 * max(自身当前攻击力+自身当前攻击力*目标耗血量/总血量*参数5+目标耗血量*参数6/100-(目标当前防御力-目标当前防御力*自身耗血量/总血量*参数7-自身当前耗血量*参数8/100),(自身当前攻击力+自身当前攻击力*目标耗血量/总血量*参数5+目标耗血量*参数6/100)*0.2));
    int nMubiaoHaoxueliang=pMonster->m_iHp-pMonster->m_iCurrHp;
    int nZishenHaoxueliang=pCard->m_iHp-pCard->m_iCurrHp;
    int tempdata=pCard->m_attack+pCard->m_attack *nMubiaoHaoxueliang/pMonster->m_iHp
    *pImapact->m_iParameter_5+nMubiaoHaoxueliang*pImapact->m_iParameter_6/100-(pMonster->m_defend-pMonster->m_defend*nZishenHaoxueliang/pCard->m_iHp*pImapact->m_iParameter_7-nZishenHaoxueliang*pImapact->m_iParameter_8/100);
    pCard->m_attack+自身当前攻击力*目标耗血量/总血量*参数5+目标耗血量*参数6/100)
    iChangeShanghaiHp=((tempdata-pMonster->m_defend)>=tempdata*0.2 ? tempdata-pMonster->m_defend :tempdata*0.2) ;
        iChangeShanghaiHp =-iChangeShanghaiHp;
   
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
    
}