//
//  CCardInfoLayer.h
//  91.cube
//
//  Created by phileas on 13-8-14.
//
//

#ifndef ___1_cube__CCardInfoLayer__
#define ___1_cube__CCardInfoLayer__

#include "cocos2d.h"
#include <vector>
#include "LayoutLayer.h"
#include "CPtBatterArray.h"
#include "CCard.h"

using namespace cocos2d;

class CCardInfoLayer : public CCLayer
{
public:

    static CCardInfoLayer* create(CFightCard *card);

public:
    CCardInfoLayer();
    virtual ~CCardInfoLayer();
    void setCardData();
public:
    virtual bool init(CFightCard * card);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initCCardInfo(CFightCard * card);
    void handlerTouch();
    
protected:
    int m_nTouchTag;
    LayoutLayer *m_cMaps;
    std::vector<TouchRect> m_cTouches;
    CPtDisPlayCard *m_pCard;
 
};


#endif /* defined(___1_cube__CCardInfoLayer__) */
