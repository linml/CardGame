//
//  CConfirmLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-14.
//
//

#include "CConfirmLayer.h"
#include "gameConfig.h"
#include "AsgardLayer.h"
#include "ExplorationScene.h"


CConfirmLayer::CConfirmLayer()
{
    
}

CConfirmLayer::~CConfirmLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CConfirmLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125,125,125,125)));
        
        initConfirm();
        bRet  = true;
    } while (0);
    return  bRet;
}

bool CConfirmLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CConfirmLayer::ccTouchBegan");

    return true;
}
void CConfirmLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CConfirmLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    handlerTouch();
}
void CConfirmLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CConfirmLayer::initConfirm()
{
    // init data:
    m_nTouchTag  = -1;
 
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    
    m_cMaps->getElementByTags("2,1")->setVisible(false);
    m_cMaps->getElementByTags("2,2")->setVisible(false);
    //
    switch (g_array[g_index])
    {
        case 0:
            // red
            break;
        case 1:
            // yellow
            break;
        case 2:
            //blue
            m_cMaps->getElementByTags("2,2")->setVisible(true);
            break;
        case 3:
            //white
            m_cMaps->getElementByTags("2,1")->setVisible(true);
            break;
        default:
            break;
    }
}


void CConfirmLayer::handlerTouch()
{
    switch (m_nTouchTag)
    {
        case 2001:
            break;
            
        default:
            break;
    }
    
}