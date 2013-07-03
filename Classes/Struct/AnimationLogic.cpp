//
//  AnimationLogic.cpp
//  91.cube
//
//  Created by linminglu on 13-7-3.
//
//

#include "AnimationLogic.h"

SAnimationFps::SAnimationFps()
{
    
}

SAnimationFps::~SAnimationFps()
{
    
}
SAnimationFps::SAnimationFps(int spendAnimationAuthor,int skillIndex,int spendCurrHp,int spendHp,int monsterCurrHp,int monsterHp,int iHuihe,int iJiaxue,int iShanghai)
{
    m_skillIndex=skillIndex;
    m_monsterCurrHp=monsterCurrHp;
    m_monsterHp=monsterHp;
    m_spendAnimationAuthor=spendAnimationAuthor;
    m_spendCurrHp=spendCurrHp;
    m_spendHp=spendHp;
    m_iHuihe=iHuihe;
    m_iJiaxue=iJiaxue;
    m_iShanghai=iShanghai;
}
