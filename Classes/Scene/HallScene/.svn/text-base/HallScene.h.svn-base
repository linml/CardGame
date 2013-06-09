//
//  CHallScene.h
//  91.cube
//
//  Created by phileas on 13-6-7.
//
//

#ifndef ___1_cube__HallScene__
#define ___1_cube__HallScene__

#include "cocos2d.h"
#include "LayoutLayer.h"

#define HALL_TOUCH_PRORITY (-1)

USING_NS_CC;

class CHallScene : public CCLayer {
    
public:
    static CCScene* scene();
    CREATE_FUNC(CHallScene);

public:
    CHallScene();
    ~CHallScene();

public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    bool initHall();
    
protected:
    LayoutLayer* m_cplist;
    

};

#endif /* defined(___1_cube__CHallScene__) */
