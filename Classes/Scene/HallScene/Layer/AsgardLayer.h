//
//  CGoldPlaceLayer.h
//  91.cube
//
//  Created by phileas on 13-6-8.
//
//

#ifndef ___1_cube__AsgardLayer__
#define ___1_cube__AsgardLayer__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace std;
using namespace cocos2d;

#define GOLDPLACE_TOUCH_PRORITY (-2)

#define GLADSHEIM_TOUCH_TAG (2001)
#define VALHALLA_TOUCH_TAG  (GLADSHEIM_TOUCH_TAG+1)
#define BIFROST_TOUCH_TAG   (VALHALLA_TOUCH_TAG+1)
#define ZIONCANYON_TOUCH_TAG (BIFROST_TOUCH_TAG+1)

class CAsgardLayer : CCLayer {
    
public:
    CREATE_FUNC(CAsgardLayer);
    
public:
    CAsgardLayer();
    virtual ~CAsgardLayer();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initArsgard();
    void handlerTouch();
protected:
    
    int m_nTouchTag;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
};



#endif /* defined(___1_cube__CGoldPlaceLayer__) */