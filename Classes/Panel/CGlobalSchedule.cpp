//
//  GlobalSchedule.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "CGlobalSchedule.h"
#define SCHEDULE CCDirector::sharedDirector()->getScheduler()

CGlobalSchedule::CGlobalSchedule(int TagValue) {
    setTag(TagValue);
    m_bTimerStatus=EN_GLOBALTIMER_NONE;
}

CGlobalSchedule::~CGlobalSchedule()
{
    CCLog("GlobalSchedule().~()");
    if (m_bTimerStatus==EN_GLOBALTIMER_RUNNING || m_bTimerStatus==EN_GLOBALTIMER_PAUSE )
    {
        stop();
    }
    
}

void CGlobalSchedule::globalUpdate(float t) {
    // 这里写全局定时器的逻辑处理代码
    //CCLog("global update %.2f,%x,%x",t,this,m_pSchedule);
}

void CGlobalSchedule::start(float fInterval, float fDelay) {
    
    if (m_bTimerStatus==EN_GLOBALTIMER_RUNNING || m_bTimerStatus==EN_GLOBALTIMER_PAUSE) {
        stop();
    }
    m_nInterval=fInterval;
    SCHEDULE->scheduleSelector(
                               schedule_selector(CGlobalSchedule::globalUpdate), this, fInterval,
                               false,
                               kCCRepeatForever, fDelay);
    this->m_bTimerStatus=EN_GLOBALTIMER_RUNNING;
}
void CGlobalSchedule::startOldTimer()
{
    if(m_nInterval==0)
    {
        m_nInterval=5;
    }
    start(m_nInterval, 0.0);
}

void CGlobalSchedule::stop() {
    this->m_bTimerStatus=EN_GLOBALTIMER_STOP;
    unsigned long long timestamp = time(NULL);
    struct tm *ptm = localtime((time_t*)&timestamp);
    char tmp[100] = {0};
    memset(tmp, 0x0, 100);
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", ptm);
    CCLog("stop %x CGlobalSchedule %s",this,tmp);
    SCHEDULE->unscheduleSelector(schedule_selector(CGlobalSchedule::globalUpdate), this);
}

void CGlobalSchedule::pause() {
    this->m_bTimerStatus=EN_GLOBALTIMER_PAUSE;
    SCHEDULE->pauseTarget(this);
}

void CGlobalSchedule::resume() {
        this->m_bTimerStatus=EN_GLOBALTIMER_RUNNING;
        SCHEDULE->resumeTarget(this);
}