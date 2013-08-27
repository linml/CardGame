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
class CGlobalSchedule: public CCObject {
public:
    // 开始全局定时器 fInterval: 时间间隔 ; fDelay: 延迟运行
    static void start(float fInterval = 0.0f, float fDelay = 0.0f);
    // 停止全局定时器
    static void stop();
    // 全局定时器暂停
    static void pause();
    // 全局定时器暂停恢复
    static void resume();
    
    // 全局定时器主逻辑实现
    void globalUpdate();
    
private:
    // 构造函数私有化，只能通过 start 来启用全局定时器
    CGlobalSchedule(float fInterval, float fDelay);
    
    ~CGlobalSchedule();
    
    // 静态变量保持单例
    static CGlobalSchedule* m_pSchedule;
};

#endif /* defined(___1_cube__GlobalSchedule__) */
