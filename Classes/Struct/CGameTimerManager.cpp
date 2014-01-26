//
//  CGameTimerManager.cpp
//  91.cube
//
//  Created by phileas on 13-10-31.
//
//

#include "CGameTimerManager.h"

CGameTimerManager* CGameTimerManager::s_pInstance = NULL;

CGameTimerManager* CGameTimerManager::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CGameTimerManager();
    }
    return s_pInstance;
}

void CGameTimerManager::releaseManager()
{
    if (s_pInstance)
    {
        s_pInstance->stopTimer();
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

void CGameTimerManager::startTimer()
{
    if (!m_bStartTimer)
    {
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CGameTimerManager::timeCallBack),this, (float)m_fInterval, false, kCCRepeatForever,0);
        m_bStartTimer = true;
        
    }

}


void CGameTimerManager::stopTimer()
{
    if(m_bStartTimer)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CGameTimerManager::timeCallBack),this);
        m_bStartTimer = false;
    }
  
}

void CGameTimerManager::startAltarBuffer()
{
    m_bStartAltarBuffer = true;
    if (!m_bStartTimer)
    {
        startTimer();
    }
    
}
void CGameTimerManager::stopAltarBuffer()
{
     m_bStartAltarBuffer = false;
    if (!m_pPropBufferManager->hasPropBuffer())
    {
        stopTimer();
    }
}
CGameTimerManager::CGameTimerManager()
{
    m_bStartTimer = false;
    m_fInterval = 1.0;
    m_pPropBufferManager = CPlayerBufferManager::getInstance();
    m_bStartAltarBuffer = false;
}

CGameTimerManager::~CGameTimerManager()
{
    stopTimer();
}

void CGameTimerManager::timeCallBack(float dt)
{
   // CCLog("timer:: --- %d",CCDirector::sharedDirector()->)
    if (m_pPropBufferManager->hasAnyBuffer() == false)
    {
        m_bStartAltarBuffer = false;
        stopTimer();
    }
    if (m_pPropBufferManager->hasPropBuffer())
    {
        CCLog("1111111");
        map<int,PropBuffer> & allPropBuffer = m_pPropBufferManager->getAllPropBuffer();
        map<int, PropBuffer>::iterator it = allPropBuffer.begin();
        for (; it != allPropBuffer.end(); it++)
        {
            if(it->second.getAltarBufferType()== KEEPTIME)
            {
                int time = it->second.getKeep();
                time -= 1;
                if (time <= 0)
                {
                    time = 0;
                }
                it->second.setKeep(time);
            
            }
        }
    }
    if (m_bStartAltarBuffer && m_pPropBufferManager->hasAltarPropBuffer())
    {
        vector<AltarBuffer>& allAltarBuffer = m_pPropBufferManager->getAllAltarBuffer();
        vector<AltarBuffer>::iterator it = allAltarBuffer.begin();
        for (; it != allAltarBuffer.end(); it++)
        {
            if(it->getAltarBufferType()==KEEPTIME)
            {
                int time = it->getKeep();
                time -= m_fInterval;
                if (time <= 0)
                {
                    time = 0;
                }
                it->setKeep(time);
            }
        }
        
    }

}

