//
//  CEveryDayLoginRewardInfoLayer.h
//  91.cube
//
//  Created by phileas on 13-12-9.
//
//

#ifndef ___1_cube__CEveryDayLoginRewardInfoLayer__
#define ___1_cube__CEveryDayLoginRewardInfoLayer__

#include "cocos2d.h"
#include "CEveryDayLoginData.h"
using namespace cocos2d;

class CEveryDayLoginRewardInfoLayer : public CCLayer
{
public:
    static CEveryDayLoginRewardInfoLayer *create(CEveryDayLoginDataStruct * data);
public:
    CEveryDayLoginRewardInfoLayer();
    virtual ~CEveryDayLoginRewardInfoLayer();
    virtual bool init();
    void updateView(CEveryDayLoginDataStruct * data);
protected:
    void initCEveryDayLoginRewardInfoLayer();
protected:
    CCArray *m_pItemLabels;
    CCSprite *m_pDropIcon;
};

#endif /* defined(___1_cube__CEveryDayLoginRewardInfoLayer__) */
