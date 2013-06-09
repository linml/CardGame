//
//  CGoldPlaceLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-8.
//
//

#include "AsgardLayer.h"

CAsgardLayer::CAsgardLayer()
{
    
}

CAsgardLayer::~CAsgardLayer()
{
    
}

bool CAsgardLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        initArsgard();
        bRet = true;
    } while (0);
    return bRet;
}

bool CAsgardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if(m_nTouchTag != -1)
    {
        return true;
    }
    
    return false;
}
void CAsgardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CAsgardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    handlerTouch();
}
void CAsgardLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CAsgardLayer::initArsgard()
{
    // init data:
    m_nTouchTag  = -1;
    
    // init map
}

void CAsgardLayer::handlerTouch()
{
    switch (m_nTouchTag) {
        case GLADSHEIM_TOUCH_TAG:

            break;
            
        case VALHALLA_TOUCH_TAG:
            
            break;
        case BIFROST_TOUCH_TAG:
            
            break;
        case ZIONCANYON_TOUCH_TAG:
            
            break;
        default:
            break;
    }
}
