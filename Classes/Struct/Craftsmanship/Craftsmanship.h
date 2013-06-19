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


class  SAnimationFps {
public:
    SAnimationFps();
    ~SAnimationFps();
    SAnimationFps(int spendAnimationAuthor,int skillIndex,int spendCurrHp,int spendHp,int monsterCurrHp,int monsterHp,int iHuihe,int iJiaxue=0,int iShanghai=0);
public:
    int  m_spendAnimationAuthor;
    int  m_skillIndex;
    int  m_spendCurrHp;
    int  m_spendHp;
    int  m_monsterCurrHp;
    int  m_monsterHp;
    int  m_iHuihe;
    int  m_iShanghai;
    int  m_iJiaxue;
};

class CCraftsmanship
{
public:
    CCraftsmanship();
    ~CCraftsmanship();
    CCraftsmanship(EN_SKILL skill,int index,string strSkillName,string strSkillFileName,int huihe,int gailv)
    {
        this->m_eskill=skill;
        this->m_indexSkill=index;
        this->m_strSkillName=strSkillName;
        this->m_sSkillFileName=strSkillFileName;
        this->m_ihuihe=huihe;
        this->m_igailv=gailv;
    }
public:
    int     getSkillIndex();
    void    setSkillIndex(int index);
    void    setSkillCategorey(EN_SKILL );
    string  getSkillName();
    string  getSkillFileName();
    EN_SKILL getSkillCategory();
   
public:
    EN_SKILL  m_eskill;         //技能的类型
    int m_indexSkill;            //jineng
    string m_strSkillName;       //skillName
    string m_sSkillFileName;
    int  m_ihuihe;
    int  m_igailv;
};

class  BasicCCraftsmanship :public CCraftsmanship
{
public:
    BasicCCraftsmanship(int index,string strSkillName,string strSkillFileName,int huihe,int gailv):
    CCraftsmanship(EN_SKILL_BASICSKILL,index,strSkillName,strSkillFileName,huihe,gailv)
    {
     
    }
};

class  DeadCCraftsmanship :public CCraftsmanship
{
public:
    DeadCCraftsmanship(int index,string strSkillName,string strSkillFileName,int huihe,int gailv):
    CCraftsmanship(EN_SKILL_BLESSINGSKILL,index,strSkillName,strSkillFileName,huihe,gailv)
    {
        
    }

};
class  IncreaseCCraftsmanship :public CCraftsmanship
{
public:
    IncreaseCCraftsmanship(int index,string strSkillName,string strSkillFileName,int huihe,int gailv):
    CCraftsmanship(EN_SKILL_INCREASESKILL,index,strSkillName,strSkillFileName,huihe,gailv)
    {
        
    }
};


class  AdvocacyCCraftsmanship :public CCraftsmanship
{
public:
    AdvocacyCCraftsmanship(int index,string strSkillName,string strSkillFileName,int huihe,int gailv):
    CCraftsmanship(EN_SKILL_BLESSINGSKILL,index,strSkillName,strSkillFileName,huihe,gailv)
    {
        
    }
};


//不同技能的概率
#endif /* defined(__en_cube__Craftsmanship__) */
