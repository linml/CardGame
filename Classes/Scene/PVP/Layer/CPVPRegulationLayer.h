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
#include "cocos-ext.h"
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

//规则界面
class CPVPRegulationLayer:public cocos2d::CCLayer, public CCScrollViewDelegate
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
    void createScrollView();
protected:
    void registerWithTouchDispatcher();
    void onExit();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
private:
    CCSize size;
    CCScrollView *scroll ;
    float xOffSet;
    float yOffSet;
    
};

#endif /* defined(___1_cube__CPVPRegulationLayer__) */
