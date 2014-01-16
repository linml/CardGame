//
//  CPVPAddTiaoZhanCountLayer.h
//  91.cube
//
//  Created by phileas on 14-1-15.
//
//

#ifndef ___1_cube__CPVPAddTiaoZhanCountLayer__
#define ___1_cube__CPVPAddTiaoZhanCountLayer__

#include "cocos2d.h"
using namespace cocos2d;

class CPVPAddTiaoZhanCountLayer : public CCLayer
{
public:
    CREATE_FUNC(CPVPAddTiaoZhanCountLayer);
    CPVPAddTiaoZhanCountLayer();
    virtual ~CPVPAddTiaoZhanCountLayer();
    virtual bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initCPVPAddTiaoZhanCountLayer();
    void onClickAddChangeNum();
    void updateNumState();
    void sendAddChangeNumRequest();
    void receiveAddChangeNumMsg(CCObject *pObject);
protected:
    CCNode *m_pBtn;
    CCLabelTTF *m_pNumLabel;
};

#endif /* defined(___1_cube__CPVPAddTiaoZhanCountLayer__) */
