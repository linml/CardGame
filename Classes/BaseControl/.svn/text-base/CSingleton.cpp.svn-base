//
//  CSingleton.cpp
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#include "CSingleton.h"
CResGuard::CResGuard()
{
    m_lGrdCnt = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_init(&m_cs, NULL);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    InitializeCriticalSection(&m_cs);
#endif
}

CResGuard::~CResGuard()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    if(pthread_mutex_trylock(&m_cs)==0)
    {
        pthread_mutex_unlock(&m_cs);
    }
    pthread_mutex_destroy(&m_cs);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    DeleteCriticalSection(&m_cs);
#endif
}

// IsGuarded is used for debugging
bool CResGuard::IsGuarded() const
{
    return(m_lGrdCnt > 0);
}

void CResGuard::Guard()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
        pthread_mutex_lock(&m_cs);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    EnterCriticalSection(&m_cs);
#endif
    m_lGrdCnt++;
}

void CResGuard::Unguard()
{
    m_lGrdCnt--;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_unlock(&m_cs);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    EnterCriticalSection(&m_cs);
#endif
}
