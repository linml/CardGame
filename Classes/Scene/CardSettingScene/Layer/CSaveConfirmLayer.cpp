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
    pLabelTip = NULL;
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
    
       
    
    
    pLabelTip = CCLabelTTF::create("loading...","Scissor Cuts", 20);
    pLabelTip->setPosition(ccp(240,140));
    
    CCNode * node = m_cMaps->getElementByTags("2,0,0");
    if(node)
    {
        node->addChild(pLabelTip);
    }
    
}
/*
 * @param inCode: 
 *        1. 0: success
 *        2. 1: 没有主将
 *        3. 2: 没有援护
 *        4. 3: 领导力不够
 *        5. 4: 没有材料卡或被强化的卡
 *        6. 5: 金币不够
 */

void CSaveConfirmLayer::setResultCode(const int &inCode, bool delay)
{
    m_nResult = inCode;

    if (delay)
    {
        runAction(CCSequence::create(CCDelayTime::create(0.8f), CCCallFunc::create(this, callfunc_selector(CSaveConfirmLayer::updateText)), NULL));
    }else
    {
        updateText();
    }
    CCLog("2233");
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

void CSaveConfirmLayer::updateText()
{
    int inCode = m_nResult;
    if (pLabelTip == NULL)
    {
        return;
    }
    char buff[50] = {0};
    switch (inCode)
    {
        case 0:
            sprintf(buff, "%s","设置成功");
            break;
            
        case 1:
            sprintf(buff, "%s","没有主将");
            break;
        case 2:
            sprintf(buff, "%s","没有援护卡牌");
            break;
        case 3:
            sprintf(buff, "%s","援护位卡牌没有援护技能");
            break;
        case 4:
            sprintf(buff, "%s","领导力不够");
            break;
        case 5:
            sprintf(buff, "%s","没有材料卡或被强化的卡");
            break;
        case 6:
            sprintf(buff, "%s","金币不够");
            break;
        default:
            sprintf(buff, "%s", "设置失败");
            break;
    }
    CCLog("%s", buff);
    pLabelTip->setString(buff);
}