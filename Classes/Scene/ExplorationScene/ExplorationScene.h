//
//  ExplorationScene.h
//  91.cube
//
//  Created by phileas on 13-6-9.
//
//

#ifndef ___1_cube__ExplorationScene__
#define ___1_cube__ExplorationScene__

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace cocos2d;

#define LEFT_TOUCH_TAG (2001)
#define CENTER_TOUCH_TAG (LEFT_TOUCH_TAG+1)
#define RIGHT_TOUCH_TAG (CENTER_TOUCH_TAG+1)

class CExploration : public CCLayer
{
public:
    static CCScene *scene();
    CREATE_FUNC(CExploration);
    
public:
    CExploration();
    virtual ~CExploration();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    bool initExploration();
    void handlerTouch();

protected:
    int m_nTouchTag;
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
};


#endif /* defined(___1_cube__ExplorationScene__) */