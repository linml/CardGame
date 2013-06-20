//
//  Craftsmanship.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "Craftsmanship.h"

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


CCraftsmanship::CCraftsmanship()
{
    
}

CCraftsmanship::~CCraftsmanship()
{
    
}



int  CCraftsmanship::getSkillIndex()
{
    return m_indexSkill;
}

void CCraftsmanship::setSkillIndex(int index)
{
    this->m_indexSkill=index;
}

void CCraftsmanship::setSkillCategorey(EN_SKILL enskill)
{
    this->m_eskill=enskill;
}

string CCraftsmanship::getSkillFileName()
{
    return m_sSkillFileName;
}

EN_SKILL CCraftsmanship::getSkillCategory()
{
    return m_eskill;
}

string CCraftsmanship::getSkillName()
{
    return m_strSkillName;
}
