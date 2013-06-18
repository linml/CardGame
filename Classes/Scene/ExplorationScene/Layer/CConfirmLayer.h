//
//  CConfirmLayer.h
//  91.cube
//
//  Created by phileas on 13-6-14.
//
//

#ifndef ___1_cube__CConfirmLayer__
#define ___1_cube__CConfirmLayer__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "Utility.h"

using namespace cocos2d;
using namespace std;

class CConfirmLayer : public CCLayerColor {
    
public:
    CREATE_FUNC(CConfirmLayer);
    
public:
    CConfirmLayer();
    virtual ~CConfirmLayer();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initConfirm();
    void handlerTouch();

protected:
    bool m_bFight;
    int m_nTouchTag;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
};



#endif /* defined(___1_cube__CConfirmLayer__) */
