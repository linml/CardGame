//
//  CPanelLoadingLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-22.
//
//

#ifndef ___1_cube__CPanelLoadingLayer__
#define ___1_cube__CPanelLoadingLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CPanelLoadingLayer :public CCLayer
{
    typedef CCLayer  SUPER;
public:
    CPanelLoadingLayer();
    ~CPanelLoadingLayer();
    CREATE_FUNC(CPanelLoadingLayer)

public:
    bool init();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter(void);
    void onExit();
};

#endif /* defined(___1_cube__CPanelLoadingLayer__) */
