//
//  FightResultConfirm.h
//  91.cube
//
//  Created by phileas on 13-6-17.
//
//

#ifndef ___1_cube__FightResultConfirm__
#define ___1_cube__FightResultConfirm__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace std;
using namespace cocos2d;




class FightResultConfirm : public CCLayerColor {
    
public:
    CREATE_FUNC(FightResultConfirm);
    
public:
    FightResultConfirm();
    virtual ~FightResultConfirm();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initFightResultConfirm();
    void handlerTouch();
protected:
    int m_nResult;
    int m_nTouchTag;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
};


#endif /* defined(___1_cube__FightResultConfirm__) */
