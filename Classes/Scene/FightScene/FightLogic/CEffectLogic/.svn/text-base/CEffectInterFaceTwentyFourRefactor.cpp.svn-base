//
//  CEffectInterFaceTwentyFourRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#include "CEffectInterFaceTwentyFourRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"
CEffectInterFaceTwentyFourRefactor::CEffectInterFaceTwentyFourRefactor()
{
    
}

CEffectInterFaceTwentyFourRefactor::~CEffectInterFaceTwentyFourRefactor()
{
    
}

void CEffectInterFaceTwentyFourRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    dealLogicSuanfa(pCard->m_attack, pImapact->m_iParameter_1, pImapact->m_iParameter_4,'-');
    dealLogicSuanfa(pCard->m_defend, pImapact->m_iParameter_2, pImapact->m_iParameter_5,'-');
    dealLogicSuanfa(pCard->m_iCurrHp, pImapact->m_iParameter_3, pImapact->m_iParameter_6,'-');
    dealLogicSuanfa(pCard->m_iHp, pImapact->m_iParameter_3, pImapact->m_iParameter_6,'-');
}