//
//  CPtRecharge.h
//  91.cube
//
//  Created by xianbei1987 on 13-10-14.
//
//

#ifndef _1_cube_CPtRecharge_h
#define _1_cube_CPtRecharge_h

#include "cocos2d.h"
#include "gameConfig.h"
USING_NS_CC;

class CPtRecharge : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CPtRecharge);
    
//    static CPtRecharge* create(void* parent);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool handleTouchSpritePool(CCPoint point);
    
    CCSprite* createIAPItem(CCDictionary* dicItem);
    
    void sendIapItem(int itemId);
    
    void onReceiveIAPBuyMsg(CCObject *pOject);

private:
    vector<TouchRect> vTouchRect;
    
};

#endif
