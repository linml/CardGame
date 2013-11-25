//
//  CBufferLogo.cpp
//  91.cube
//
//  Created by phileas on 13-11-4.
//
//

#include "CBufferLogo.h"

CBufferLogo::CBufferLogo(const Buffer &inBuffer):m_rBuffer(inBuffer)
{
    m_pLogo = NULL;
    m_pTime = NULL;
    
}
CBufferLogo::~CBufferLogo()
{
    
}

bool CBufferLogo::initCBufferLog(const std::string &inIcon)
{
    bool bRet = false;
    do {
        
        m_pLogo = CCSprite::create(inIcon.c_str());
        addChild(m_pLogo);
        m_pTime = CCLabelTTF::create("", "Arial", 13);
        m_pTime->setPosition(ccp(0, -30));
        addChild(m_pTime);
        CCSize size(m_pLogo->getContentSize());
        size.width *= 2;
        size.height *= 2;
        setContentSize(size);
        bRet = true;
    } while (0);
    return bRet;
}

void CBufferLogo::updateTime(int inTime)
{
    char buffer[30]={0};
    sprintf(buffer,"the time: %d", inTime);
    if (m_pTime)
    {
        m_pTime->setString(buffer);
    }
    
}

void CBufferLogo::updateTime()
{
    char buffer[30]={0};
    if (m_rBuffer.getAltarBufferType() == KEEPTIME)
    {
        sprintf(buffer,"the time: %d", m_rBuffer.getKeep());
    }else
    {
        sprintf(buffer,"the times: %d", m_rBuffer.getKeep());
    }
    
    if (m_pTime)
    {
        m_pTime->setString(buffer);
    }
    
}






