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
#include "CPtPropConfigData.h"
#include "CGameButtonControl.h"
#include "gamePlayer.h"
#include "CSkillData.h"
#include "CPtPropUseManager.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CReward.h"
#include "CPlayerBufferManager.h"

//implement class of CAltarBufferLogo
CAltarBufferLogo * CAltarBufferLogo::create(AltarBuffer &inAltarBuffer)
{
    CAltarBufferLogo *logo = new CAltarBufferLogo(inAltarBuffer);
    if (logo && logo->initCAltarBufferLog(inAltarBuffer.getSkillEffectId()))
    {
  
        logo->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(logo);
    }
    return logo;
}

CAltarBufferLogo::CAltarBufferLogo(AltarBuffer &inAltarBuffer):m_rAltarBuffer(inAltarBuffer)
{
    m_pLogo = NULL;
    m_pTime = NULL;
}

CAltarBufferLogo::~CAltarBufferLogo()
{
    
}

bool CAltarBufferLogo::initCAltarBufferLog(int inSkillEffectId)
{
    bool bRet = false;
    do {
        std::string icon = SinglePlayer::instance()->getBufferPngByEffectId(inSkillEffectId);
        m_pLogo = CCSprite::create(icon.c_str());
        addChild(m_pLogo);
        m_pTime = CCLabelTTF::create("", "Arial", 13);
        updateTime();
        m_pTime->setPosition(ccp(0, -30));
        addChild(m_pTime);
        CCSize size(m_pLogo->getContentSize());
        size.width *= 2;
        size.height *= 2;
        setContentSize(size);
        bRet = true;
    } while (0);
    return bRet;
}

void CAltarBufferLogo::updateTime(int inTime)
{
    char buffer[30]={0};
    sprintf(buffer,"the time: %d", inTime);
    if (m_pTime)
    {
        m_pTime->setString(buffer);
    }
    
}

void CAltarBufferLogo::updateTime()
{
    char buffer[30]={0};
    if (m_rAltarBuffer.getAltarBufferType() == KEEPTIME)
    {
         sprintf(buffer,"the time: %d", m_rAltarBuffer.getKeep());
    }else
    {
         sprintf(buffer,"the times: %d", m_rAltarBuffer.getKeep());
    }
   
    if (m_pTime)
    {
        m_pTime->setString(buffer);
    }

}

// implement class of CGameBufferTipLayer
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
    m_pUseBtn = NULL;
    m_pImapact = NULL;
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
    }else if(CPtTool::isInNode(m_pUseBtn, pTouch))
    {
        m_nTouchTag = 2001;
        m_pUseBtn->selected();
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
    
    m_pImapact = SinglePlayer::instance()->getEffectTableByEffectId(m_nBufferKey);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite*bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "altar_bg.png"));
    CCSize bgSize = bg->getContentSize();
    bg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    CCSprite * cancelBtn = CCSprite::createWithSpriteFrameName("closeNormal.png");
    cancelBtn->setAnchorPoint(ccp(1,1));
    cancelBtn->setPosition(ccp(bgSize.width-40, bgSize.height-40));
    bg->addChild(cancelBtn);
    m_pCloseBtn = cancelBtn;
    
    if (m_pImapact)
    {
        m_nTipId = m_pImapact->m_iParameter_10;
        if (m_pImapact->m_ieffect_logic == 21)
        {
            createBufferTipUI(bg);
        }else if(m_pImapact->m_ieffect_logic == 24)
        {
             createDebufferTipUI(bg);
        }
        
       
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(CGAMEBUFFERTIPDIALOG_TOUCH_PRORITY);
    
    
}

void CGameBufferTipLayer::createBufferTipUI(CCSprite* inBg)
{
    const char * buffer = "王老板的钥匙一串，下次战斗卡牌血量提升10%"; // get from skill_effect_config.plist  file , key=parameter_10 int -> char * --> gettips from dictionary plist file:
    buffer = Utility::getWordWithFile("dictionary.plist", CCString::createWithFormat("%d",m_nTipId)->getCString()).c_str();
    CCSize bgSize = inBg->getContentSize();
    CCLabelTTF * tipLabel = CCLabelTTF::create(buffer, "Arial", 16);
    tipLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    tipLabel->setColor(ccc3(0, 0, 0));
    tipLabel->setPosition(ccp(bgSize.width*0.6, bgSize.height*0.55));
    tipLabel->setDimensions(CCSizeMake(bgSize.width*0.8, 0));
    inBg->addChild(tipLabel);
}
void CGameBufferTipLayer::createDebufferTipUI(CCSprite* inBg)
{
    const char * buffer = "王老板的钥匙一串，下次战斗卡牌血量提升10%"; // get from skill_effect_config.plist  file , key=parameter_10 int -> char * --> gettips from dictionary plist file:
    int tipId =   SinglePlayer::instance()->getEffectTableByEffectId(m_nBufferKey)->m_iParameter_10;
    buffer = Utility::getWordWithFile("dictionary.plist", CCString::createWithFormat("%d",tipId)->getCString()).c_str();
    CCSize bgSize = inBg->getContentSize();
    CCLabelTTF * tipLabel = CCLabelTTF::create(buffer, "Arial", 16);
    tipLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    tipLabel->setColor(ccc3(0, 0, 0));
    tipLabel->setPosition(ccp(bgSize.width*0.6, bgSize.height*0.8));
    tipLabel->setDimensions(CCSizeMake(bgSize.width*0.8, 0));
    inBg->addChild(tipLabel);
    m_nPropId = CPtPropUserManager::getInstance()->getUserPropsBy(XIAOBUFFER)->at(0);
    CCNode *node = createShopItem(m_nPropId);
    node->setPosition(ccp(bgSize.width/2, bgSize.height*0.4));
    inBg->addChild(node);
}

void CGameBufferTipLayer::loadResource()
{
      CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "seller.plist"), CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "backpack.plist"), CSTR_FILEPTAH(g_mapImagesPath,"backpack.png"));
}

void CGameBufferTipLayer::handlerTouch(CCTouch* pTouch)
{
   if(m_nTouchTag == 2002)
    {
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeNormal.png"));
    }else if(m_nTouchTag == 2001)
    {
        m_pUseBtn->unselected();
    }
    
    if(CPtTool::isInNode(m_pCloseBtn, pTouch) && m_nTouchTag == 2002)
    {
        onClickClose();
    }else if(CPtTool::isInNode(m_pUseBtn, pTouch) && m_nTouchTag == 2001)
    {
        onClickUse();
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

void CGameBufferTipLayer::onClickUse()
{
    // user prop:
    if(SinglePlayer::instance()->haveEnoughPropById(m_nPropId, 1) == 1)
    {
        onSendCancelBufferRequest();
    }else
    {
        CCMessageBox("没有该道具", "没有该道具");
    }
}

/*
 * sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&item_id=1(道具ID)&num=1(数量)&( 2探路,3消buff)
 * special_type=3&special_params={"effect_id"(buff的效果id):1}
 * special_type=2
 */
void CGameBufferTipLayer::onSendCancelBufferRequest()
{
  
    char buffer[300] = {0};
    sprintf(buffer, "sig=%s&item_id=%d&num=1&special_type=3&special_params={\"effect_id\":%d}",STR_USER_SIG, m_nPropId, m_nBufferKey);
    CCLog("the buffer: %s", buffer);
    ADDHTTPREQUESTPOSTDATA(STR_URL_USESPEACIALPROP(196), "CALLBACK_CGameBufferTipLayer::onSendCancelBufferRequest", "REQUEST_CGameBufferTipLayer::onSendCancelBufferRequest", buffer, callfuncO_selector(CGameBufferTipLayer::onReceiveMsg));
}

void CGameBufferTipLayer::onReceiveMsg(cocos2d::CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGameBufferTipLayer::onSendCancelBufferRequest");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCDictionary *dict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (dict)
        {
            int code = GameTools::intForKey("code", dict);
            if (code == 0)
            {
            
                CPlayerBufferManager::getInstance()->clearAltarBufferById(m_nBufferKey, false);
                CCDictionary *tmpDict = (CCDictionary*) dict->objectForKey("result");
                if(tmpDict)
                {
                    tmpDict = (CCDictionary*) (tmpDict->objectForKey("reward"));
                    if (tmpDict)
                    {
                         tmpDict = (CCDictionary*)  tmpDict->objectForKey("dec");
                         tmpDict = (CCDictionary*) tmpDict->objectForKey("item");
                        CReward * reward = CReward::create(tmpDict);
                        if (reward)
                        {
                            reward->excuteReward(DEC);
                        }
                    }
                    
                }
                removeFromParentAndCleanup(true);
                CCMessageBox("取消成功", "success");
            }
            else
            {
                CCMessageBox(CCString::createWithFormat("the error code : %d", code)->getCString(), "cancelbuffer error");
            }
        }
    }
    
}

CCNode * CGameBufferTipLayer::createShopItem(int inPropId)
{
    CPtPropConfigData* propData = SinglePropConfigData::instance();
    CCNode *node = CCNode::create();
    CPtProp * item = propData->getPropById(inPropId);
    if (item)
    {
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "shangchengzixiang.png"));
        node->addChild(sprite,0,1);
        node->setContentSize(sprite->getContentSize());
    }
    
    CCLabelTTF *labelTTF=CCLabelTTF::create(item->getPropName().c_str(), "Arial", 15);
    node->addChild(labelTTF, 1, 10);
    labelTTF->setPosition(ccp(0,85));
    //    const CCSize& dimensions, CCTextAlignment hAlignment
    CCLabelTTF *labelTTFTip=CCLabelTTF::create(item->getTips().c_str(), "Arial", 15,CCSizeMake(130, 100),kCCTextAlignmentLeft);
    node->addChild(labelTTFTip, 1, 11);
    labelTTFTip->setAnchorPoint(ccp(0,0.5));
    labelTTFTip->setPosition(ccp(0,0));
    
    CCSprite *itemPng=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, item->getIconName().c_str()));
    node->addChild(itemPng,1,12);
    itemPng->setPosition(ccp(-70,8));

    
        
    CGameButtonControl *pGamebutton=CGameButtonControl::createButton(TEXTMID, "使用", "normal.png", "pressed.png","disabled.png");
    node->addChild(pGamebutton,1,20);
    pGamebutton->setFontColor(g_custom_color[17]);
    pGamebutton->setPosition(ccp(10,-80));
    m_pUseBtn = pGamebutton;
    return node;
    
}
