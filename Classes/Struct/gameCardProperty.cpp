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
    m_iJiChuJineng=-1;
    m_iYongHuJineng=-1;
    m_iZhangHunJiachi=-1;
    m_iZhengFuJineng=-1;
}

void CGameCardProperty::setAddCurrentHp(int iHp)
{
    m_unCurrentHp +=iHp;
    if(m_unCurrentHp>m_unHp)
    {
        m_unCurrentHp=m_unHp;
    }
}

void CGameCardProperty::setAddHp(int  iHp)
{
    m_unHp+=iHp;
    m_unCurrentHp +=iHp;
    if(m_unCurrentHp>m_unHp)
    {
        m_unCurrentHp=m_unHp;
    }
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
    
}