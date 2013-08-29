//
//  CButtonControl.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CButtonControl.h"

CButtonControl::CButtonControl():m_bIsEnabled(false),m_bIsSelected(false)
{
    m_pNormalFrame=NULL;
    m_pSelectFrame=NULL;
}

CButtonControl::~CButtonControl()
{
    
}
CCRect CButtonControl::rect()
{
    return  boundingBox();
}

void CButtonControl::selected()
{
    if(m_pSelectFrame)
    {
        setDisplayFrame(m_pSelectFrame);
    }
}

void CButtonControl::unselected()
{
    if(m_pNormalFrame)
    {
        setDisplayFrame(m_pNormalFrame);
    }
}



CCSpriteFrame * CButtonControl::getNormalFrame()
{
    return m_pNormalFrame;
}

void CButtonControl::setNormalFrame(CCSpriteFrame * temp)
{
    if(temp!=NULL)
    {
        m_pNormalFrame=temp;
    }
}
CCSpriteFrame *  CButtonControl::getSelectFrame()
{
    return m_pSelectFrame;
}

void CButtonControl::setSelectFrame(cocos2d::CCSpriteFrame *var)
{
    if(var!=NULL)
    {
        m_pSelectFrame=var;
    }
}
