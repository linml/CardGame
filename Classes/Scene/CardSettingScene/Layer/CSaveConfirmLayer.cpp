//
//  CSaveConfirmLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-29.
//
//

#include "CSaveConfirmLayer.h"
#include "gameConfig.h"

CSaveConfirmLayer::CSaveConfirmLayer()
{
    
}

CSaveConfirmLayer::~CSaveConfirmLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CSaveConfirmLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125,125,125,125)));
        
        initConfirm();
        bRet  = true;
    } while (0);
    return  bRet;
    
}
bool CSaveConfirmLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CSaveConfirmLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
  
}
void CSaveConfirmLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    handlerTouch();
}
void CSaveConfirmLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void CSaveConfirmLayer::initConfirm()
{
    // init data:
    m_nTouchTag = -1;
    m_nResult = (int)getUserData();
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-INT_MAX);
    
    m_cMaps->getElementByTags("2")->setVisible(true);
    m_cMaps->getElementByTags("2,1")->setVisible(false);
    m_cMaps->getElementByTags("2,2")->setVisible(false);
    m_cMaps->getElementByTags("3")->setVisible(false);
    
    string word ;
    if (m_nResult == 0)
    {
       word = "设置成功";
    }
    else if(m_nResult == 30001)
    {
        word = "设置成功";
    }
    else if(m_nResult == 4001)
    {
        word = "设置成功";
    }
    else
    {
        word = "设置失败";
    }
    
    
    
    CCLabelTTF* pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
    pLabel->setPosition(ccp(240,140));
    
    CCNode * node = m_cMaps->getElementByTags("2,0,0");
    if(node)
    {
        CCLog("exist,%s", word.c_str());
        node->addChild(pLabel);
    }
    
}

void CSaveConfirmLayer::handlerTouch()
{
    CCLog("m_nTag: %d", m_nTouchTag);
    switch (m_nTouchTag)
    {
        case  2001:
            removeFromParentAndCleanup(true);
            break;
            
        default:
            break;
    }
   
}
