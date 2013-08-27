//
//  GlobalSchedule.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "CGlobalSchedule.h"
#define SCHEDULE CCDirector::sharedDirector()->getScheduler()
CGlobalSchedule* CGlobalSchedule::m_pSchedule = NULL;

CGlobalSchedule::CGlobalSchedule(float fInterval, float fDelay) {
    CCLog("GlobalSchedule()");
    
    CCAssert(!m_pSchedule, "以定义，不能重复定义");
    
    SCHEDULE->scheduleSelector(
                               schedule_selector(CGlobalSchedule::globalUpdate), this, fInterval,
                               false,
                               kCCRepeatForever, fDelay);
    
    m_pSchedule = this;
}

CGlobalSchedule::~CGlobalSchedule()
{
    CCLog("GlobalSchedule().~()");
    
    SCHEDULE->unscheduleSelector(
                                 schedule_selector(CGlobalSchedule::globalUpdate), this);
}

void CGlobalSchedule::globalUpdate() {
    // 这里写全局定时器的逻辑处理代码
    CCLog("global update");
}

void CGlobalSchedule::start(float fInterval, float fDelay) {
    new CGlobalSchedule(fInterval, fDelay);
}

void CGlobalSchedule::stop() {
    CCLog("GlobalSchedule().clean()");
    
    CCAssert(m_pSchedule, "未定义");
    CC_SAFE_DELETE(m_pSchedule);
}

void CGlobalSchedule::pause() {
    CCLog("GlobalSchedule().pause()");
    
    CCAssert(m_pSchedule, "为定义");
    SCHEDULE->pauseTarget(m_pSchedule);
}

void CGlobalSchedule::resume() {
    CCLog("GlobalSchedule().resume()");
    
    CCAssert(m_pSchedule, " 未定义");
    SCHEDULE->resumeTarget(m_pSchedule);
}