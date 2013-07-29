//
//  CSaveConfirmLayer.h
//  91.cube
//
//  Created by phileas on 13-7-29.
//
//

#ifndef ___1_cube__CSaveConfirmLayer__
#define ___1_cube__CSaveConfirmLayer__


#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "Utility.h"

using namespace cocos2d;
using namespace std;

class CSaveConfirmLayer : public CCLayerColor {
    
public:
    CREATE_FUNC(CSaveConfirmLayer);
    
public:
    CSaveConfirmLayer();
    virtual ~CSaveConfirmLayer();
    
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
    int m_nResult;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
};




#endif /* defined(___1_cube__CSaveConfirmLayer__) */
