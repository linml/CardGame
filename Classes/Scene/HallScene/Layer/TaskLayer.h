//
//  TaskLayer.h
//  91.cube
//
//  Created by phileas on 13-6-18.
//
//

#ifndef ___1_cube__TaskLayer__
#define ___1_cube__TaskLayer__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace cocos2d;
using namespace std;

class CTaskLayer : public CCLayerColor {
    
    
public:
    CREATE_FUNC(CTaskLayer);
    
public:
    CTaskLayer();
    virtual ~CTaskLayer();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initTask();
    void handlerTouch();
protected:
    
    int m_nTouchTag;
    bool m_bGoEnable;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
    
};


#endif /* defined(___1_cube__TaskLayer__) */
