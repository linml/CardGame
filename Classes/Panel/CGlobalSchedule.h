//
//  GlobalSchedule.h
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#ifndef ___1_cube__CGlobalSchedule__
#define ___1_cube__CGlobalSchedule__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
enum EN_GLOBALTIMER {
    EN_GLOBALTIMER_NONE,
    EN_GLOBALTIMER_RUNNING,
    EN_GLOBALTIMER_STOP,
    EN_GLOBALTIMER_PAUSE
};



class CGlobalSchedule: public CCObject {
public:
    // 开始全局定时器 fInterval: 时间间隔 ; fDelay: 延迟运行
     void start(float fInterval, float fDelay);
    // 停止全局定时器
     void stop();
    // 全局定时器暂停
     void pause();
    // 全局定时器暂停恢复
     void resume();
     void startOldTimer();
    
    // 全局定时器主逻辑实现
    virtual void globalUpdate(float var);
    CGlobalSchedule(int iValueTag);
    virtual ~CGlobalSchedule();
private:
    CC_SYNTHESIZE(int,m_iTag, Tag);
    CC_SYNTHESIZE(int, m_nInterval,Interval);
    CC_SYNTHESIZE(EN_GLOBALTIMER, m_bTimerStatus, TimerStatus);
    // 构造函数私有化，只能通过 start 来启用全局定时器
};

#endif /* defined(___1_cube__GlobalSchedule__) */
