//
//  CGameTimerManager.h
//  91.cube
//
//  Created by phileas on 13-10-31.
//
//

#ifndef ___1_cube__CGameTimerManager__
#define ___1_cube__CGameTimerManager__

#include "cocos2d.h"
using namespace cocos2d;
#include "CPlayerBufferManager.h"
class CPlayerBufferManager;
class CGameTimerManager :public CCObject
{
public:
    static CGameTimerManager* getInstance();
    static void releaseManager();
protected:
    static CGameTimerManager* s_pInstance;
public:
    void startTimer();
    void stopTimer();
    
    void startAltarBuffer();
    void stopAltarBuffer();
    
protected:
    CGameTimerManager();
    ~CGameTimerManager();
    
    void timeCallBack(float dt);
protected:
    bool m_bStartTimer;
    float m_fInterval;
    CCNode* m_pTarget;
    CPlayerBufferManager *m_pPropBufferManager;
    
    CC_SYNTHESIZE(bool, m_bStartAltarBuffer, StartAlarBuffer);
    
};

#endif /* defined(___1_cube__CGameTimerManager__) */
