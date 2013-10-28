//
//  CGameBufferTip.cpp
//  91.cube
//
//  Created by phileas on 13-10-24.
//
//

#include "CGameBufferTip.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "CPtTool.h"
CGameBufferTipLayer * CGameBufferTipLayer::create(int inBufferKey)
{
    CGameBufferTipLayer *layer = new CGameBufferTipLayer();
    if (layer && layer->initCGameBufferTipLayer(inBufferKey))
    {
        layer->autorelease();
    }else
    {
        CC_SAFE_RELEASE_NULL(layer);
    }
    return layer;
}


CGameBufferTipLayer::CGameBufferTipLayer()
{
    m_nBufferKey = 0;
    m_nBufferType = 0;
    m_nTouchTag = -1;
    m_pCloseBtn = NULL;
    m_pCloseHandler = NULL;
    m_pCloseSelector = NULL;
}

CGameBufferTipLayer::~CGameBufferTipLayer()
{
    
}

bool CGameBufferTipLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
 
    m_nTouchTag = -1;
   
   if(CPtTool::isInNode(m_pCloseBtn, pTouch))
    {
        m_nTouchTag = 2002;
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeSelected.png"));
    }
    return true;

}
void CGameBufferTipLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameBufferTipLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        handlerTouch(pTouch);
    }

    
}
void CGameBufferTipLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

bool CGameBufferTipLayer::initCGameBufferTipLayer(int inBufferKey)
{
    bool bRet = false;
    loadResource();
    m_nBufferKey = inBufferKey;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initCGameBufferTipUI();
        bRet = true;
    } while (0);
    return bRet;
}


void CGameBufferTipLayer::setCloseHanlder(CCObject *inTarget, SEL_CallFuncO inSelector)
{
    m_pCloseHandler = inTarget;
    m_pCloseSelector = inSelector;
}
void CGameBufferTipLayer::initCGameBufferTipUI()
{
    
    const char * buffer = "王老板的钥匙一串，下次战斗卡牌血量提升10%"; // get from skill_effect_config.plist  file , key=parameter_10 int -> char * --> gettips from dictionary plist file:
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite*bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "altar_bg.png"));
    CCSize bgSize = bg->getContentSize();
    bg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    CCLabelTTF * tipLabel = CCLabelTTF::create(buffer, "Arial", 16);
    tipLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    tipLabel->setColor(ccc3(0, 0, 0));
    tipLabel->setPosition(ccp(bgSize.width*0.6, bgSize.height*0.55));
    tipLabel->setDimensions(CCSizeMake(bgSize.width*0.8, 0));
    bg->addChild(tipLabel);
    
    CCSprite * cancelBtn = CCSprite::createWithSpriteFrameName("closeNormal.png");
    cancelBtn->setAnchorPoint(ccp(1,1));
    cancelBtn->setPosition(ccp(bgSize.width-40, bgSize.height-40));
    bg->addChild(cancelBtn);
    m_pCloseBtn = cancelBtn;
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(CGAMEBUFFERTIPDIALOG_TOUCH_PRORITY);
    
    
}

void CGameBufferTipLayer::createBufferTipUI()
{
    
}
void CGameBufferTipLayer::createDebufferTipUI()
{
    
    
}

void CGameBufferTipLayer::loadResource()
{
      CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "seller.plist"), CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));
}

void CGameBufferTipLayer::handlerTouch(CCTouch* pTouch)
{
   if(m_nTouchTag == 2002)
    {
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeNormal.png"));
    }
    
    if(CPtTool::isInNode(m_pCloseBtn, pTouch) && m_nTouchTag == 2002)
    {
        onClickClose();
    }
    

}
void CGameBufferTipLayer:: onClickClose()
{
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)(NULL);
    }
    removeFromParentAndCleanup(true);
    
}