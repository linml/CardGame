//
//  CGameUserPropDialogLayer.cpp
//  91.cube
//
//  Created by phileas on 13-10-30.
//
//

#include "CGameUserPropDialogLayer.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CPtRecharge.h"
#include "CReward.h"
#define XIESIDESHANGDIAN_BYJIQIMAO 800000

CGameUserPropDialogLayer * CGameUserPropDialogLayer::create(USERTYPE inFunctionId)
{
    CGameUserPropDialogLayer *layer = new CGameUserPropDialogLayer();
    if (layer)
    {
        layer->setFunctonId(inFunctionId);
        if (layer->init())
        {
            layer->autorelease();
        }else
        {
            CC_SAFE_DELETE(layer);
            layer = NULL;
        }
        
    }
    return layer;
}

CGameUserPropDialogLayer * CGameUserPropDialogLayer::create(int inPropId)
{
    CGameUserPropDialogLayer *layer = new CGameUserPropDialogLayer();
    if (layer)
    {
        layer->setPropId(inPropId);
        if (layer->init())
        {
            layer->autorelease();
        }else
        {
            CC_SAFE_DELETE(layer);
            layer = NULL;
        }
        
    }
    return layer;
    
}

CGameUserPropDialogLayer::CGameUserPropDialogLayer()
{
    m_bHaveProp = false;
    m_pSuccessCallBack = NULL;
    m_pSuccessHandler = NULL;
}
CGameUserPropDialogLayer::~CGameUserPropDialogLayer()
{
    
}

bool CGameUserPropDialogLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initCGameUserPropDialogLayer();
        bRet = true;
    } while (0);
    return bRet;
}

void CGameUserPropDialogLayer::updatePropId(int inType)
{
    if (inType ==1)
    {
        m_nPropId = CPtPropUserManager::getInstance()->getGetHavePropIdByFunctionId(m_eFunctionId);
        if(m_nPropId != -1)
        {
            m_bHaveProp = true;
            
        }else
        {
            m_nPropId = CPtPropUserManager::getInstance()->getFirstPropIdByFunctionId(m_eFunctionId);
            m_bHaveProp = false;
        }

    }
    else if(inType == 2)
    {
        m_bHaveProp =  SinglePlayer::instance()->getPropCountFromBag(m_nPropId) > 0 ;
    }
    
}

void CGameUserPropDialogLayer::initCGameUserPropDialogLayer()
{
   
    CCAssert(m_nPropId != -1, "function.plist 文件内没有配置该id");
    loadResource();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "dialog_bg.plist"), CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    // create bg:
    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    CCSpriteBatchNode * bathNode = CCSpriteBatchNode::createWithTexture(texture, 5);
    
    CCPoint point = ccp(280, 280);
    CCSprite * top = CCSprite::createWithSpriteFrameName("dialog_top.png");
    CCSprite * mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
    CCSprite * bottom = CCSprite::createWithSpriteFrameName("dialog_bottom.png");
    bottom->setPosition(point);
    bottom->setAnchorPoint(CCPointZero);
    top->setAnchorPoint(CCPointZero);
    mid->setAnchorPoint(CCPointZero);
    CCSize size1 = bottom->boundingBox().size;
    mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height-1));
    bathNode->addChild(mid);
    CCSize size2 = mid->boundingBox().size;
    int count = 2;
    for (int i = 1; i < 2; i++)
    {
        mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
        mid->setAnchorPoint(CCPointZero);
        mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height+i*size2.height-i-1));
        bathNode->addChild(mid);
    }
    
    top->setPosition(ccp(mid->getPositionX(), mid->getPositionY()+size2.height-count));
    bathNode->addChild(top);
    bathNode->addChild(bottom);
    this->addChild(bathNode);
    
    
    // left btn:
    CCSprite *node = NULL;
    m_pLeft = CCLabelTTF::create("取消", "Arial", 18);
    
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
    m_pFrame[0] = frame;
    CC_SAFE_RETAIN(m_pFrame[0]);
    node =CCSprite::createWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    CCSize size =  node->boundingBox().size;
    m_pLeft->setPosition(ccp(size.width/2, size.height/2));
    m_pLeft->setColor(ccc3(126, 60, 30));
    node->addChild(m_pLeft);
    m_pBtn[0] = node;
    Utility::addTouchRect(LEFT_TAG, node, m_cTouches);
    addChild(node, 100);
    node->setPosition(ccpAdd(point,ccp(60, 55)));
    
    // right btn
    const char * text = m_bHaveProp ? "使用":"购买";
   
    node=CCSprite::createWithSpriteFrame(frame);
    m_pRight = CCLabelTTF::create(text, "Arial", 18);
    node->addChild(m_pRight);
    node->setAnchorPoint(ccp(0,1));
    size =  node->boundingBox().size;
    m_pRight->setPosition(ccp(size.width/2, size.height/2));
    m_pRight->setColor(ccc3(126, 60, 30));
    frame  = cach->spriteFrameByName("Use_Pressed.png");
    m_pFrame[1] = frame;
    CC_SAFE_RETAIN(m_pFrame[1]);
    m_pBtn[1] = node;
    Utility::addTouchRect(RIGHT_TAG, node, m_cTouches);
    addChild(node, 100);
    node->setPosition(ccpAdd(point,ccp(300, 55)));
    
    CCNode *temp = createPropInfo();
    temp->setPosition(ccpAdd(point,ccp(80, 120)));
    addChild(temp);
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    
    setRightHandler(this, callfuncO_selector(CGameUserPropDialogLayer::onClickUserOrRecharge));
    setLeftHandler(this, callfuncO_selector(CGameUserPropDialogLayer::onClickCancel));
    setAutoClose(false);
}

CCNode * CGameUserPropDialogLayer::createPropInfo()
{
    CPtPropConfigData *propConfigData =SinglePropConfigData::instance();
    CPtProp *prop = propConfigData->getPropById(m_nPropId);
    CCAssert(prop, "没有该道具");
    CCNode *node = CCNode::create();

    CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, prop->getIconName().c_str()));
    node->addChild(icon);
    CCSize size = icon->getContentSize();

    CCLabelTTF *label = CCLabelTTF::create(prop->getPropName().c_str(), "Arial", 13);
    label->setColor(g_custom_color[2]);
    label->setAnchorPoint(ccp(0.5, 1));
    label->setPosition(ccp(0.5, -size.height/2));
    node->addChild(label);
    
    label = CCLabelTTF::create(prop->getTips().c_str(), "Arial", 18);
    label->setDimensions(CCSizeMake(300, 0));
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setPosition(ccp(size.width, size.height/2));
    label->setAnchorPoint(ccp(0, 1));
    label->setColor(g_custom_color[2]);
    node->addChild(label);
    return node;

}

bool CGameUserPropDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pBtn[0], pTouch))
    {
        m_pBtn[0]->setDisplayFrame(m_pFrame[1]);
        m_nTouchTag = LEFT_TAG;
    }
    else if(CPtTool::isInNode(m_pBtn[1], pTouch))
    {
        m_pBtn[1]->setDisplayFrame(m_pFrame[1]);
        m_nTouchTag = RIGHT_TAG;
    }
    else
    {
        m_nTouchTag = -1;
    }
    
    
    return true;

    
}

void CGameUserPropDialogLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        if (m_nTouchTag == RIGHT_TAG)
        {
            m_pBtn[1]->setDisplayFrame(m_pFrame[0]);
            
        }else if(m_nTouchTag == LEFT_TAG)
        {
            m_pBtn[0]->setDisplayFrame(m_pFrame[0]);
        }
        
        if((m_nTouchTag== RIGHT_TAG && CPtTool::isInNode(m_pBtn[1], pTouch))||(m_nTouchTag == LEFT_TAG && CPtTool::isInNode(m_pBtn[0], pTouch)))
        {
            handlerTouch();
            PtSoundTool::playSysSoundEffect("UI_click.wav");
        }else
        {
            
        }
    }
    
}

void CGameUserPropDialogLayer::setSucessHandler(cocos2d::CCObject *pObject, SEL_CallFunc inCallBack)
{
    m_pSuccessHandler = pObject;
    m_pSuccessCallBack = inCallBack;
}

void CGameUserPropDialogLayer::onClickUserOrRecharge(cocos2d::CCObject *pObject)
{
    char buffer[400] = {0};
    if (m_bHaveProp)
    {
        sprintf(buffer, "sig=%s&item_id=%d&num=%d",STR_USER_SIG , m_nPropId, 1);
        ADDHTTPREQUESTPOSTDATA(STR_URL_USE_GRID(196), "CALLBACK_CGameUserPropDialogLayer::onClickUserOrRecharge", "REQUEST_CGameUserPropDialogLayer::onClickUserOrRecharge", buffer, callfuncO_selector(CGameUserPropDialogLayer::onRecieveRequestMsg));
    }
    else
    {
        /*
         *模块 ：api.php?m=shop&a=buy&uid=194(用户ID)
         参数 ：
         &sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&shop_id=1&item_id=1(物品ID)&num=1(数量)&seller_shop_type=0(0普通商店 1道具商店 2卡牌商店)&is_use=0(1 直接使用 0不直接使用)
         */
        if (hasEnouchCoin())
        {
            int shopId =800000;
            int num = 1;
            sprintf(buffer,"sig=%s&shop_id=%d&item_id=%d&num=%d&seller_shop_type=0&is_use=1", STR_USER_SIG, shopId, m_nPropId, num);
            ADDHTTPREQUESTPOSTDATA(STR_URL_GETSHOPBUY(196), "CALLBACK_CGameUserPropDialogLayer::onClickUserOrRecharge", "REQUEST_CGameUserPropDialogLayer::onClickUserOrRecharge", buffer, callfuncO_selector(CGameUserPropDialogLayer::onRecieveRequestMsg));
            
        }else
        {
            CPtRecharge * layer = CPtRecharge::create();
            layer->setTouchPriority(getTouchPriority()-1);
            addChild(layer, 1000);
        }
      
      
    }
}

void CGameUserPropDialogLayer::onClickCancel(cocos2d::CCObject *pObject)
{
    onClose();
}

void CGameUserPropDialogLayer::onRecieveRequestMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGameUserPropDialogLayer::onClickUserOrRecharge");
    char *buffer = (char *)pObject;
    if(buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary * pResultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        int code = GameTools::intForKey("code", pResultDict);
        if (code == 0)
        {
            pResultDict = (CCDictionary*) pResultDict->objectForKey("result");
            if (m_bHaveProp)
            {
                handlerUserResult(pResultDict);
            }
            else
            {
                handlerBuyResult(pResultDict);
            }
            
            if (m_pSuccessCallBack && m_pSuccessHandler)
            {
                (m_pSuccessHandler->*m_pSuccessCallBack)();
            }

        }
        else
        {
            CCMessageBox(CCString::createWithFormat("error code :%d", code)->getCString(), "error tip");
        }
    }
    onClose();
    
}

bool CGameUserPropDialogLayer::hasEnouchCoin()
{
    bool bRet = false;
    CGamePlayer *player = SinglePlayer::instance();
    CStructShopSellItem *shopItem =player->getShopItemByPropId(m_nPropId);
    CCAssert(shopItem, "商店里没有买这种道具，找猫去");
    m_nCostType = player->getShopType(XIESIDESHANGDIAN_BYJIQIMAO);
    m_nCostValue = shopItem->getValue();
    if (m_nCostType == 1)
    {
        bRet = m_nCostValue <= player->getPlayerCash();
    }
    else
    {
        bRet = m_nCostValue <= player->getCoin();
    }
    return bRet;
}

void CGameUserPropDialogLayer::handlerUserResult(CCDictionary *inResultDict)
{
    if (inResultDict == NULL)
    {
        return;
    }
    CReward *addReward = NULL;
    CReward *decReward = NULL;
    CCDictionary *rewardDict = (CCDictionary*) inResultDict->objectForKey("reward");
    CCDictionary* tmpDict = NULL;
    if(rewardDict)
    {
        CCDictionary* addDict = (CCDictionary*)  rewardDict->objectForKey("add");
        if (addDict)
        {
            // get item effect:
            tmpDict = (CCDictionary*) addDict->objectForKey("item");
            addReward= CReward::create(tmpDict);
            if (addReward)
            {
                addReward->excuteReward(ADD);
            }
        }
        CCDictionary* decDict = (CCDictionary*)  rewardDict->objectForKey("dec");
        if (decDict)
        {
            // get item effect:
            tmpDict = (CCDictionary*) decDict->objectForKey("item");
            decReward= CReward::create(tmpDict);
            if (decReward)
            {
                decReward->excuteReward(DEC);
            }
            
        }

    }
    CCDictionary *effectDict = (CCDictionary*)inResultDict->objectForKey("item_effect");
    if (effectDict)
    {
        if (GameTools::intForKey("card_max", effectDict) == 10)
        {
            SinglePlayer::instance()->appendCardBag();
            CCMessageBox("扩展了10格卡位", "扩展卡包");
        }
    }


       
}
void CGameUserPropDialogLayer::handlerBuyResult(CCDictionary *inResultDict)
{
    if (inResultDict == NULL)
    {
        return;
    }
    CReward *addReward = NULL;
    CReward *decReward = NULL;
    CCDictionary *rewardDict = (CCDictionary*) inResultDict->objectForKey("reward");
    CCDictionary* tmpDict = NULL;
    if(rewardDict)
    {
        CCDictionary* addDict = (CCDictionary*)  rewardDict->objectForKey("add");
        if (addDict)
        {
            // get item effect:
            tmpDict = (CCDictionary*) addDict->objectForKey("shop");
            addReward= CReward::create(tmpDict);
            if (addReward)
            {
                addReward->excuteReward(ADD);
            }
            
            
        }
        CCDictionary* decDict = (CCDictionary*)  rewardDict->objectForKey("dec");
        if (decDict)
        {
            // get item effect:
            tmpDict = (CCDictionary*) decDict->objectForKey("shop");
            decReward= CReward::create(tmpDict);
            if (decReward)
            {
                decReward->excuteReward(DEC);
            }
            
        }
        
        
    }
    CCDictionary *effectDict = (CCDictionary*)inResultDict->objectForKey("item_effect");
    if (effectDict)
    {
        if (GameTools::intForKey("card_max", effectDict) == 10)
        {
            SinglePlayer::instance()->appendCardBag();
            CCMessageBox("扩展了10格卡位", "扩展卡包");
        }
    }
}
