//
//  CAnimationSpriteGameFight.cpp
//  91.cube
//
//  Created by linminglu on 13-7-17.
//
//

#include "CAnimationSpriteGameFight.h"
CAnimationSpriteGameFight::CAnimationSpriteGameFight()
{
    
}

CAnimationSpriteGameFight::CAnimationSpriteGameFight(
                          EN_ANIMATIONTYPE enanimationtype,
                          EN_ATKFIGHT_INDEX enAtkFight,
                          int iAtkindex,
                          int iDefIndex,
                          int iAddHp,
                          int iSubHp,
                          int iAddEngry,
                          int iSubEngry,
                          int skillid,
                          int ispritetag)
{
    this->m_iAddHp=iAddHp;
    this->m_iATKindex=iAtkindex;
    this->m_iDefIndex=iDefIndex;
    this->m_iSKillId=skillid;
    this->m_iSubHp=iSubHp;
    this->m_iAddEngry=iAddEngry;
    this->m_iSubEngry=iSubEngry;
    this->m_enAnimationType=enanimationtype;
    this->m_enAtkFightIndex=enAtkFight;
    this->spritetag=ispritetag;
}