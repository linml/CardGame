//
//  CPtTableItem.h
//  91.cube
//
//  Created by phileas on 13-6-26.
//
//

#ifndef ___1_cube__CPtTableItem__
#define ___1_cube__CPtTableItem__

#include "cocos2d.h"

using namespace cocos2d;

class CPtTableItem : public CCNode, public CCTouchDelegate
{
    
public:
    
    CPtTableItem();
    ~CPtTableItem();
    CREATE_FUNC(CPtTableItem);
    
    virtual bool init(){ m_bIsLoad = true; return true;};
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual CCNode *getDisplayView(){return  m_pDisplayView == NULL? this : m_pDisplayView;};
    virtual void setDisplayView(CCNode * inDisplayview);
    void  setDelegate(CCTouchDelegate * inTouchDelegate){ m_pTouchDelegate = inTouchDelegate;};
    void  setLayerParent(CCNode *inLayer);
    void  setTouchNode(CCNode *node);
    bool  isLoad(){return m_bIsLoad ; };
    void  load(){};
protected:
    bool isSelect(CCNode * node, CCTouch *pTouch);
    bool isSelect(CCNode * node, CCPoint location);
    
protected:
    bool m_bIsLoad;
    int m_nIndex;
    CCNode * m_pDisplayView;
    CCNode * parent;
    CCNode * touchNode;
    CCTouchDelegate * m_pTouchDelegate;
};

#endif /* defined(___1_cube__CPtTableItem__) */
