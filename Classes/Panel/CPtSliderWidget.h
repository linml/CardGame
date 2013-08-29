//
//  CPtSliderWidget.h
//  91.cube
//
//  Created by phileas on 13-8-29.
//
//

#ifndef ___1_cube__CPtSliderWidget__
#define ___1_cube__CPtSliderWidget__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class CPtSliderWidget : public CCControlSlider
{
    
public:
    static CPtSliderWidget* create(CCSprite * backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite);
public:
    CPtSliderWidget(){};
    virtual ~CPtSliderWidget(){};
    

    bool touchBegan(CCTouch* touch, CCEvent* pEvent);
    bool touchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void touchEnded(CCTouch *pTouch, CCEvent *pEvent);


};



#endif /* defined(___1_cube__CPtSliderWidget__) */
