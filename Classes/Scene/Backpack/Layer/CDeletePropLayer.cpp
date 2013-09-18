//
//  CDeletePropLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-29.
//
//

#include "CDeletePropLayer.h"
#include "gameConfig.h"
#include "CPtPropConfigData.h"



CDeletePropLayer::CDeletePropLayer()
{
    m_nTouchTag = -1;
    m_nMaxCount = 0 ;
    m_nCurrentCount = 1;
    
    m_pNumberLabel = NULL;
    m_pSlider = NULL;
    m_cMaps = NULL;
    for (int i = 0; i <2 ; i++)
    {
        m_pBtn[i] = NULL;
    }
    m_pThum = NULL;
    m_pConfirmSelector = NULL;
    m_pCancelSelector = NULL;
    m_pTarget = NULL;
}

CDeletePropLayer::~CDeletePropLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
    
}

void CDeletePropLayer::setHanlder(CCObject *inTarget, SEL_CallFuncO inConfirmSelector, SEL_CallFuncO inCancelSelector)
{
    m_pTarget = inTarget;
    m_pConfirmSelector = inConfirmSelector;
    m_pCancelSelector = inConfirmSelector;
}

CDeletePropLayer* CDeletePropLayer::create(PropItem *inPropItem)
{
    CDeletePropLayer *pRet = new CDeletePropLayer();
    if (pRet && pRet->init(inPropItem))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }

}
bool CDeletePropLayer::init(PropItem *inPropItem)
{
    bool bRet = false;
    do
    {
        
        CC_BREAK_IF(!CCLayer::init());
        initDeletePropLayer(inPropItem);
        bRet = true;
    } while (0);
    return  bRet;
    
}

bool CDeletePropLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
   m_bMove = false;
   m_bLongPress = false;
   m_bCanDrag =  m_pSlider->touchBegan(pTouch, pEvent);
   if (m_bCanDrag)
   {
       m_pThum->setTextureRect(m_cTouchSpriteFrameRect[5]);
       return true;
   }
    
    m_nTouchTag = TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches);
    CCLog("m_nTouchTag: %d", m_nTouchTag);
    if (m_nTouchTag != -1)
    {
        if (m_nTouchTag == 2001)
        {
            if (m_pBtn[0])
            {
                m_pBtn[0]->setTextureRect(m_cTouchSpriteFrameRect[3]);
            }
            startLongPress(SUB_TAG);

        }else if(m_nTouchTag == 2002)
        {
            if (m_pBtn[1])
            {
                m_pBtn[1]->setTextureRect(m_cTouchSpriteFrameRect[1]);
            }
            startLongPress(ADD_TAG);
            
        }else if(m_nTouchTag == 2)
        {
            
        }else if(m_nTouchTag == 3)
        {
            
        }
    }
    return true;
}
void CDeletePropLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getDelta().x) > 3)
    {
        m_bMove = true;
    }
    if (m_bCanDrag)
    {
      m_bCanDrag =  m_pSlider->touchMoved(pTouch, pEvent);
      updateTexture(true);

    }
   
}
void CDeletePropLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bCanDrag)
    {
        updateTexture(true, true);
        m_pThum->setTextureRect(m_cTouchSpriteFrameRect[4]);
        m_pSlider->touchEnded(pTouch, pEvent);
        CCLog("the current value: %d", m_nCurrentCount);
        return;
    }
    
    stopLongPress();
    
    
    if (m_nTouchTag == 2001)
    {
        //sub:
        if (m_pBtn[0])
        {
            m_pBtn[0]->setTextureRect(m_cTouchSpriteFrameRect[2]);
        }
       
        if (m_bLongPress)
        {
            return;
        }
        m_nCurrentCount--;
        m_nCurrentCount = m_nCurrentCount >=0? m_nCurrentCount : 0;
        updateTexture();

        
    }else if(m_nTouchTag == 2002)
    {
        if (m_pBtn[1])
        {
            m_pBtn[1]->setTextureRect(m_cTouchSpriteFrameRect[0]);
        }
        if (m_bLongPress)
        {
            return;
        }
        m_nCurrentCount++;
        m_nCurrentCount = m_nCurrentCount <= m_nMaxCount ? m_nCurrentCount : m_nMaxCount;
        updateTexture(false, true);
        
        
    }else if(m_nTouchTag == 2)
    {
        
    }else if(m_nTouchTag == 3)
    {
        
    }

    if (m_nTouchTag == TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches))
    {
                
        handlerTouch();

    }
}
void CDeletePropLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CDeletePropLayer::initDeletePropLayer(PropItem *inPropItem)
{
    m_nMaxCount = inPropItem->propCount;
    m_nPropId = inPropItem->propId;
    std::string str;
    if(SinglePropConfigData::instance()->getPropDataById(inPropItem->propId))
    {
      str = SinglePropConfigData::instance()->getIconName();
    }else
    {
        return;
    }
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "deleteprop.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    m_cMaps->getTouchRects(m_cTouches);
    
    // set title
    int array[4]={1,6,0,0};
    CCSprite *parent = (CCSprite*) m_cMaps->getElementByTags(array, 3);
    CCLabelTTF * label = CCLabelTTF::create("你设定要删除道具的个数", "Arial", 18);
    label->setColor(ccc3(0, 240, 255));

    CCSize size =  parent->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    parent->addChild(label);
    
     // set icon
    array[1] = 4;
    array[2] = 0;
    parent = (CCSprite*) m_cMaps->getElementByTags(array, 3);
    size = parent->boundingBox().size;
    CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_propImagesPath, str.c_str()));
    CCSprite *icon = CCSprite::createWithTexture(texture);
    icon->setPosition(ccp(size.width/2, size.height/2));
    parent->addChild(icon);
    
    // set num:
    array[1] = 5;
    array[2] = 3;
    char buff[10] ={0};
    sprintf(buff, "%d", m_nCurrentCount);
    parent = (CCSprite *)m_cMaps->getElementByTags(array, 3);
    m_pNumberLabel = CCLabelTTF::create(buff, "Arial", 18);
    size =  parent->boundingBox().size;
    m_pNumberLabel->setPosition(ccp(size.width/2, size.height/2));
    parent->addChild(m_pNumberLabel);
    m_pNumberLabel->setColor(ccc3(0, 240, 255));
    
    // touch event:
   
    CCNode *node = NULL;
    array[1]= 2, array[2] = 2;
    node = m_cMaps->getElementByTags(array, 3);
    Utility::addTouchRect(node->getTag(), node, m_cTouches);
    label = CCLabelTTF::create("删除", "Arial", 18);
    size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
    
    array[2] = 3;
    node = m_cMaps->getElementByTags(array, 3);
    Utility::addTouchRect(node->getTag(), node, m_cTouches);
    label = CCLabelTTF::create("取消", "Arial", 18);
    size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);

    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCNode *parentNode = NULL;
    cache->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "delete_prop.plist"), CSTR_FILEPTAH(g_mapImagesPath, "delete_prop.png"));
    CCSprite * btn = CCSprite::createWithSpriteFrameName("addBtn_normal.png");
    array[1]= 5, array[2] = 8;
    node = m_cMaps->getElementByTags(array, 3);
    btn->setAnchorPoint(node->getAnchorPoint());
    btn->setPosition(node->getPosition());
    m_pBtn[1] = btn;

    parentNode = node->getParent();
    node->removeFromParentAndCleanup(true);
    parentNode->addChild(btn, 20, 2002);
    Utility::addTouchRect(2002, btn, m_cTouches);
    
    btn = CCSprite::createWithSpriteFrameName("subBtn_normal.png");
    array[2] = 7;
    node = m_cMaps->getElementByTags(array, 3);
    btn->setAnchorPoint(node->getAnchorPoint());
    btn->setPosition(node->getPosition());
    parentNode = node->getParent();
    node->removeFromParentAndCleanup(true);
    parentNode->addChild(btn, 20, 2001);
    Utility::addTouchRect(2001, btn, m_cTouches);
    m_pBtn[0] = btn;

    m_cTouchSpriteFrameRect[0] = cache->spriteFrameByName("addBtn_normal.png")->getRect();
    m_cTouchSpriteFrameRect[1] = cache->spriteFrameByName("addBtn_pressed.png")->getRect();
    m_cTouchSpriteFrameRect[2] = cache->spriteFrameByName("subBtn_normal.png")->getRect();
    m_cTouchSpriteFrameRect[3] = cache->spriteFrameByName("subBtn_pressed.png")->getRect();
    m_cTouchSpriteFrameRect[4] = cache->spriteFrameByName("thum_normal.png")->getRect();
    m_cTouchSpriteFrameRect[5] = cache->spriteFrameByName("thum_pressed.png")->getRect();
    // add slider:
    CCPoint point = btn->getPosition();
    CCSprite * slider_bg = CCSprite::createWithSpriteFrameName("bar_bg.png");
    CCSprite * slider_bg2 = CCSprite::createWithSpriteFrameName("bar_bg.png");
    m_pThum = CCSprite::createWithSpriteFrameName("thum_normal.png");
    m_pSlider= CPtSliderWidget::create(slider_bg, slider_bg2, m_pThum);
    m_pSlider->setMinimumValue(0.0f); // Sets the min value of range
    m_pSlider->setMaximumValue(m_nMaxCount); // Sets the max value of range
    m_pSlider->setPosition(ccp(point.x+45, point.y-30));
    m_pSlider->setAnchorPoint(CCPointZero);
    m_pSlider->setValue(1.0f);
    addChild(m_pSlider, 1000);
    
}

void CDeletePropLayer::handlerTouch()
{
    // m_nTouchTag : 2, 3, 20001, 20002
    if (m_nTouchTag == 2)
    {
        PropItem item;
        item.propId = m_nPropId;
        item.propCount = m_nCurrentCount;
        if (m_nCurrentCount == 0)
        {
            removeFromParentAndCleanup(true);
            return;
        }
        // confirm:
        if (m_pTarget && m_pConfirmSelector)
        {
            (m_pTarget->*m_pConfirmSelector)(&item);
        }
    }
    else if(m_nTouchTag == 3)
    {
        if (m_pTarget && m_pCancelSelector)
        {
            (m_pTarget->*m_pCancelSelector)(NULL);
        }
        removeFromParentAndCleanup(true);
    }
}

void CDeletePropLayer::updateTexture(bool sliderEnable /* = false*/,bool inEnd /*= false*/)
{
    if (sliderEnable == false)
    {
        m_pSlider->setValue(m_nCurrentCount);
        char buff[10] ={0};
        sprintf(buff, "%d", m_nCurrentCount);
        m_pNumberLabel->setString(buff);
        
    }else
    {
        if (inEnd)
        {
            
            float value =  m_pSlider->getValue();
            m_nCurrentCount = m_pSlider->getValue();
            m_nCurrentCount = (int)(value+0.5)-m_nCurrentCount == 0 ? m_nCurrentCount : m_nCurrentCount+1;
            m_pSlider->setValue(m_nCurrentCount);
            char buff[10] ={0};
            sprintf(buff, "%d", m_nCurrentCount);
            m_pNumberLabel->setString(buff);
            
        }else
        {
            if (m_nCurrentCount != (int) m_pSlider->getValue())
            {
                m_nCurrentCount = m_pSlider->getValue();
                char buff[10] ={0};
                sprintf(buff, "%d", m_nCurrentCount);
                m_pNumberLabel->setString(buff);
                
            }
        }
     
    }
    
}

void CDeletePropLayer::startLongPress(int inType)
{
    m_nType = inType;
    m_bLongPress = false;
    this->schedule(schedule_selector(CDeletePropLayer::longTouchCallBack), 0.5f);
}
void CDeletePropLayer::stopLongPress()
{
    this->unschedule(schedule_selector(CDeletePropLayer::longTouchCallBack));
}

void CDeletePropLayer::longTouchCallBack(float dt)
{
   
    if (m_bMove)
    {
        stopLongPress();
    }
    m_bLongPress = true;
    if (m_nType ==  SUB_TAG)
    {
        m_nCurrentCount = 0.0f;
        m_pSlider->setValue(m_nCurrentCount);
    }else if(ADD_TAG)
    {
        m_nCurrentCount = m_nMaxCount;
        m_pSlider->setValue(m_nCurrentCount);

        
    }else
    {
        stopLongPress();
    }
    
    char buff[10] ={0};
    sprintf(buff, "%d", m_nCurrentCount);
    m_pNumberLabel->setString(buff);
}

