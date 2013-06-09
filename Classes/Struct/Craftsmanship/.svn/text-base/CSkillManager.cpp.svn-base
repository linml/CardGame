//
//  GongjiPutongJineng.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "CSkillManager.h"
#include "gameConfig.h"
/*
 class CCraftsmanship gstr_sskillList[]=
 {
 {EN_SKILL_BASICSKILL,1,"女神之枪"},
 {EN_SKILL_BLESSINGSKILL,2,"女神之拥"},
 {EN_SKILL_ADVOCACYSKILL,3,"英灵颂歌"},
 {EN_SKILL_BASICSKILL,4,"寒霜号角"},
 {EN_SKILL_BLESSINGSKILL,5,"水晶龙甲"},
 {EN_SKILL_ADVOCACYSKILL,6,"岩石技能"},
 {EN_SKILL_BASICSKILL,7,"藤编"},
 {EN_SKILL_BLESSINGSKILL,8,"冰雪森林"},
 {EN_SKILL_ADVOCACYSKILL,9,"回复术"},
 {EN_SKILL_BASICSKILL,10,"铁锤挥击"},
 {EN_SKILL_BLESSINGSKILL,11,"大地守护"},
 {EN_SKILL_ADVOCACYSKILL,12,"精锻"},
 {EN_SKILL_BASICSKILL,13,"霜爪击"},
 {EN_SKILL_BLESSINGSKILL,14,"狼魂"},
 {EN_SKILL_ADVOCACYSKILL,15,"撕咬"},
 };
 */

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CSkillManager::CSkillManager()
{
        m_skillList.push_back(new CCraftsmanship(EN_SKILL_BASICSKILL,1,"女神之枪",g_str_skillPathPlist+"0-sanmang.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BLESSINGSKILL,2,"女神之拥",g_str_skillPathPlist+"0-xingxing.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_ADVOCACYSKILL,3,"英灵颂歌",g_str_skillPathPlist+"7-zhongji.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BASICSKILL,4,"寒霜号角",g_str_skillPathPlist+"10-chouxue.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BLESSINGSKILL,5,"水晶龙甲",g_str_skillPathPlist+"14-buff.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_ADVOCACYSKILL,6,"岩石技能",g_str_skillPathPlist+"18-xingxing.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BASICSKILL,7,"藤编",g_str_skillPathPlist+"26-xingxing.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BLESSINGSKILL,8,"冰雪森林",g_str_skillPathPlist+"32-xuanyunbuff.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_ADVOCACYSKILL,9,"回复术",g_str_skillPathPlist+"35-xingxing.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BASICSKILL,10,"铁锤挥击",g_str_skillPathPlist+"32-xuanyunbuff.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BLESSINGSKILL,11,"大地守护",g_str_skillPathPlist+"penxue.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_ADVOCACYSKILL,12,"精锻",g_str_skillPathPlist+"home-jiesuo.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BASICSKILL,13,"霜爪击",g_str_skillPathPlist+"changjing-yan.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_BLESSINGSKILL,14,"狼魂",g_str_skillPathPlist+"32-xuanyunbuff.plist"));//,
         m_skillList.push_back(new CCraftsmanship(EN_SKILL_ADVOCACYSKILL,15,"撕咬",g_str_skillPathPlist+"14-buff.plist"));//,
}

CSkillManager::~CSkillManager()
{
    DELETE_POINT_VECTOR(m_skillList,vector<CCraftsmanship* >);
}


CCraftsmanship *CSkillManager::getSkillByIndex(int index)
{
    if (index<m_skillList.size())
    {
        return m_skillList[index];
    }
    return NULL;
}


