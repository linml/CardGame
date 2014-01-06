//
//  CPVPRegulationLayer.h
//  91.cube
//
//  Created by linminglu on 14-1-6.
//
//

#ifndef ___1_cube__CPVPRegulationLayer__
#define ___1_cube__CPVPRegulationLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class CPVPRegulationLayer:public cocos2d::CCLayer
{
public:
    CPVPRegulationLayer();
    ~CPVPRegulationLayer();
    CREATE_FUNC(CPVPRegulationLayer);
    bool init();
    CCLayer *createContianLayer();
private:
    void createBackGround();
    void createQuitButton();
protected:
    void registerWithTouchDispatcher();
    void onExit();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
private:
    CCSize size;
    
};

#endif /* defined(___1_cube__CPVPRegulationLayer__) */
