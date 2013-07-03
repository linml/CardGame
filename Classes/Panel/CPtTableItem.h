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

class CPtTableItem : public CCSprite, public CCTouchDelegate
{
    
public:
    static CPtTableItem* create(const char *pszFileName);
    static CPtTableItem* create(const char *pszFileName, const CCRect& rect);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual CCNode *getDisplayView(){return this;};
    void  setLayerParent(CCNode *inLayer);
    void  setTouchNode(CCNode *node);
protected:
    bool isSelect(CCNode * node, CCTouch *pTouch);
    bool isSelect(CCNode * node, CCPoint location);
    
    
protected:
    CCNode * m_cDisplayView;
    CCNode * parent;
    CCNode * touchNode;
    
};

#endif /* defined(___1_cube__CPtTableItem__) */
