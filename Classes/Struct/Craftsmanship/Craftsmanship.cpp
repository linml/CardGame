//
//  Craftsmanship.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "Craftsmanship.h"


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
