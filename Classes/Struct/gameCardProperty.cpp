//
//  gameCardProperty.cpp
//  91.cube
//
//  Created by linminglu on 13-5-27.
//
//

#include "gameCardProperty.h"
#include "Craftsmanship.h"

#define DELETE_SAFT(POINT) \
{ \
if(POINT) \
{ \
delete POINT;  POINT=NULL ; \
}\
}

CGameCardProperty::CGameCardProperty()
{
    //m_vSkillList.clear();// NULL;
    m_iJiChuJineng=-1;
    m_iYongHuJineng=-1;
    m_iZhangHunJiachi=-1;
    m_iZhengFuJineng=-1;
}

void CGameCardProperty::initJineng(int jichujineng,int zhengfuskill,int zhanghunskill,int yonghuskill)
{
    m_iJiChuJineng=jichujineng;
    m_iZhengFuJineng=zhengfuskill;
    m_iZhangHunJiachi=zhanghunskill;
    m_iYongHuJineng=yonghuskill;
}


CGameCardProperty::~CGameCardProperty()
{
    //    DELETE_SAFT(m_pAdvocacy);
    //    DELETE_SAFT(m_pBasicSkill);
    //    DELETE_SAFT(m_pDeadSkill);
    //    DELETE_SAFT(m_pIncreaseSkill);
}