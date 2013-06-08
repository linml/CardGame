//
//  Craftsmanship.h
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#ifndef __en_cube__Craftsmanship__
#define __en_cube__Craftsmanship__

#include <iostream>
using namespace std;
#include "CardSprite.h"
//建立一个技能特效表
/**
 *Advocacy 拥护
 *Increase 增幅
 *Blessing 加持
 */

enum EN_SKILL
{
    EN_SKILL_BASICSKILL  = 0, //
    EN_SKILL_BLESSINGSKILL = 1, //死亡技能
    EN_SKILL_INCREASESKILL =2, //
    EN_SKILL_ADVOCACYSKILL =3
};


class CCraftsmanship
{
public:
    CCraftsmanship();
    ~CCraftsmanship();
    CCraftsmanship(EN_SKILL skill,int index,string strSkillName,string strSkillFileName)
    {
        this->m_eskill=skill;
        this->m_indexSkill=index;
        this->m_strSkillName=strSkillName;
        this->m_sSkillFileName=strSkillFileName;
    }
public:
    int     getSkillIndex();
    void    setSkillIndex(int index);
    void    setSkillCategorey(EN_SKILL );
    string  getSkillName();
    string  getSkillFileName();
    EN_SKILL getSkillCategory();
   
public:
    EN_SKILL  m_eskill; //技能的类型
    int m_indexSkill;   //jineng
    string m_strSkillName;       //skillName
    string m_sSkillFileName;
};

#endif /* defined(__en_cube__Craftsmanship__) */
