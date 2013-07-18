//
//  CAnimationSpriteGameFight.h
//  91.cube
//
//  Created by linminglu on 13-7-17.
//
//

#ifndef ___1_cube__CAnimationSpriteGameFight__
#define ___1_cube__CAnimationSpriteGameFight__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"


class CAnimationSpriteGameFight
{
    //暂且先定义这些
public:
    CAnimationSpriteGameFight();
    CAnimationSpriteGameFight(
                              EN_ANIMATIONTYPE enanimationtype,
                              EN_ATKFIGHT_INDEX enAtkFight,
                              int iAtkindex,
                              int iDefIndex,
                              int iAddHp,
                              int iSubHp,
                              int iAddEngry,
                              int iSubEngry,
                              int skillid,
                              int spritetag);
    
    EN_ANIMATIONTYPE m_enAnimationType;
    EN_ATKFIGHT_INDEX m_enAtkFightIndex;
    int  m_iATKindex;
    int  m_iDefIndex;
    int  m_iAddHp;
    int  m_iSubHp;
    int  m_iAddEngry;
    int  m_iSubEngry;
    int  m_iSKillId;
    int  spritetag;
    
};

#endif /* defined(___1_cube__CAnimationSpriteGameFight__) */
