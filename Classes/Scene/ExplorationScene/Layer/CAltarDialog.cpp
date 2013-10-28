//
//  CAltarDialog.cpp
//  91.cube
//
//  Created by phileas on 13-10-23.
//
//

#include "CAltarDialog.h"
#include "gameConfig.h"
#include "CEventConfigData.h"
#include "CPtTool.h"
#include <string>

CAltarDialogLayer* CAltarDialogLayer::create(int inEventId)
{
    CAltarDialogLayer * layer = new CAltarDialogLayer();
    if (layer && layer->initCAltarDialogLayer(inEventId))
    {
        layer->autorelease();
    }else
    {
        CC_SAFE_RELEASE_NULL(layer);
    }
    return  layer;
}

CAltarDialogLayer::CAltarDialogLayer()
{
    m_pCloseBtn = NULL;
    m_pCloseHandler = NULL;
    m_pCloseSelector = NULL;
}

CAltarDialogLayer::~CAltarDialogLayer()
{
    
}

bool CAltarDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    m_nTouchTag = -1;
    if(CPtTool::isInNode(m_pCloseBtn, pTouch))
    {
        m_nTouchTag = 2002;
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeSelected.png"));
    }
    return true;

}

void CAltarDialogLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CAltarDialogLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        handlerTouch(pTouch);
    }
}

void CAltarDialogLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

bool CAltarDialogLayer::initCAltarDialogLayer(int inEventId)
{
    bool bRet = false;
    do
    {
        loadResource();
        setEventId(inEventId);
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        iniCAltarDialogUI();
        bRet = true;
    } while (0);
    return bRet;
   
}

void CAltarDialogLayer::setCloseHandler(CCObject *inTarget, SEL_CallFuncO inSelector)
{
    m_pCloseHandler = inTarget;
    m_pCloseSelector = inSelector;
}

void CAltarDialogLayer::iniCAltarDialogUI()
{
    
    CEventData * eventData = SingleEventDataConfig::instance()->getEventById(m_nEventId);
    // CCAssert(eventData, "the event is null");
    
    const char *person = "shangren_01.png";
    std::string stip ="王老板，带着他的小姨子跑了，抛弃了他的好基友机器猫,机器猫发动了技能，你的防御力降低， 战斗中小心了！" ;//Utility::getWordWithFile("dictionary", CCString::createWithFormat("%d", eventData->getEventTipId())->getCString());
    
    
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "shangren_bg.png"));
    bg->setPosition(ccp(512, 388));
    CCSize bgSize = bg->getContentSize();
    addChild(bg);
    
    //add seller picture
    CCSprite *sellerPic = CCSprite::create(CSTR_FILEPTAH(g_sellerPath, person));
    sellerPic->setPosition(ccp(bgSize.width*(0.125), 0));
    sellerPic->setAnchorPoint(ccp(0.5, 0));
    bg->addChild(sellerPic);
    
    CCSprite * cancelBtn = CCSprite::createWithSpriteFrameName("closeNormal.png");
    cancelBtn->setAnchorPoint(ccp(1,1));
    cancelBtn->setPosition(ccp(bgSize.width-10, bgSize.height-10));
    bg->addChild(cancelBtn);
    
    m_pCloseBtn = cancelBtn;
    CCLabelTTF *tips = CCLabelTTF::create(stip.c_str(), "Arial", 13);
    tips->setHorizontalAlignment(kCCTextAlignmentLeft);
    tips->setAnchorPoint(CCPointZero);
    tips->setPosition(ccp(bgSize.width/2, bgSize.height/2));
    tips->setDimensions(CCSizeMake(200, 0));
    bg->addChild(tips);

    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(CALTARRDIALOG_TOUCH_PRORITY);
}


void CAltarDialogLayer::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "seller.plist"), CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));

}

void CAltarDialogLayer::handlerTouch(cocos2d::CCTouch *pTouch)
{
    if(m_nTouchTag == 2002)
    {
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeNormal.png"));
    }
    if(CPtTool::isInNode(m_pCloseBtn, pTouch) && m_nTouchTag == 2002)
    {
        onCloseBtn();
    }
    

}

void CAltarDialogLayer::onCloseBtn()
{
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)(NULL);
    }
    removeFromParentAndCleanup(true);
}