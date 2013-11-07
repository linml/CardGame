//
//  CGameUserPropDialogLayer.h
//  91.cube
//
//  Created by phileas on 13-10-30.
//
//

#ifndef ___1_cube__CGameUserPropDialogLayer__
#define ___1_cube__CGameUserPropDialogLayer__

#include "cocos2d.h"
using namespace cocos2d;

class CGameUserPropDialogLayer : public CCLayerColor
{
public:
    static CGameUserPropDialogLayer * create(int inPropId);
public:
    CGameUserPropDialogLayer();
    virtual ~CGameUserPropDialogLayer();
    
    bool initCGameUserPropDialogLayer();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void setPropId(int inPropId){m_nPropId = inPropId;};
protected:

    int m_nPropId;
    
  
};

#endif /* defined(___1_cube__CGameUserPropDialogLayer__) */
