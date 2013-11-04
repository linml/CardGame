//
//  CEffectInterFaceEightRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-11-4.
//
//

#include "CEffectInterFaceEightRefactor.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
#include "gamePlayer.h"

CEffectInterFaceEightRefactor::CEffectInterFaceEightRefactor()
{
    
}
CEffectInterFaceEightRefactor::~CEffectInterFaceEightRefactor()
{
    
}
void CEffectInterFaceEightRefactor::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    dealWithBufferAndImmediately(pCard, pMonster, pImapact);
}