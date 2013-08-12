//
//  Biforest.h
//  91.cube
//
//  Created by phileas on 13-6-13.
//
//

#ifndef ___1_cube__Biforest__
#define ___1_cube__Biforest__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace cocos2d;
using namespace std;

class CBiforestLayer : public CCLayerColor {
    
    
public:
    CREATE_FUNC(CBiforestLayer);
    
public:
    CBiforestLayer();
    virtual ~CBiforestLayer();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initBiforest();
    void handlerTouch();
protected:
    
    int m_nTouchTag;
    bool m_bSectionTouchEnable;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
    CCSprite * m_pTouchSprite;
    
};

#endif /* defined(___1_cube__Biforest__) */
