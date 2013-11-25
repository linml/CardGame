//
//  PtShake.h
//  91.cube
//
//  Created by xianbei1987 on 13-6-17.
//
//

#ifndef __SHAKE_H__
#define __SHAKE_H__

#include "CCActionInterval.h"


class CPtShake : public cocos2d::CCActionInterval
{
public:
    CPtShake();
    
    // Create the action with a time and a strength (same in x and y)
    static CPtShake* create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static CPtShake* createWithStrength(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
protected:
    
    void startWithTarget(cocos2d::CCNode *pTarget);
    void update(float time);
    void stop(void);
    
    
    // Initial position of the shaked node
    float m_initial_x, m_initial_y;
    // Strength of the action
    float m_strength_x, m_strength_y;
};

#endif //__SHAKE_H__
