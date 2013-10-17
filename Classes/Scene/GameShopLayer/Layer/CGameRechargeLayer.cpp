//
//  CGameRechargeLayer.cpp
//  91.cube
//
//  Created by phileas on 13-10-15.
//
//

#include "CGameRechargeLayer.h"
#include "gameConfig.h"
#include "CStructShopSellItem.h"
#include "CPtPropConfigData.h"
#include "gamePlayer.h"

CGameRechargeLayer* CGameRechargeLayer::create(CStructShopSellItem *inShopItem)
{
    CGameRechargeLayer *pRet = new CGameRechargeLayer();
    if (pRet && pRet->init(inShopItem))
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

CGameRechargeLayer::CGameRechargeLayer()
{
    m_nTouchTag = -1;
    m_nMaxCount = 0 ;
    m_nCurrentCount = 1;
    m_nCashPerItem = 0;
    
    m_pNumberLabel = NULL;
    m_cMaps = NULL;
    for (int i = 0; i <2 ; i++)
    {
        m_pBtn[i] = NULL;
    }
    m_pConfirmSelector = NULL;
    m_pCancelSelector = NULL;
    m_pTarget = NULL;
    m_pCash = NULL;

}

CGameRechargeLayer::~CGameRechargeLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
}

void CGameRechargeLayer::setHanlder(CCObject *inTarget, SEL_CallFuncO inConfirmSelector, SEL_CallFuncO inCancelSelector)
{
    m_pTarget = inTarget;
    m_pConfirmSelector = inConfirmSelector;
    m_pCancelSelector = inConfirmSelector;

}


bool CGameRechargeLayer::init(CStructShopSellItem *item)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCGameRechargeLayer(item);
        bRet = true;
    } while (0);
    return bRet;
}

bool CGameRechargeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bLongPress = false;
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
            startLongPress(RECHARGE_SUB_TAG);
            
        }else if(m_nTouchTag == 2002)
        {
            if (m_pBtn[1])
            {
                m_pBtn[1]->setTextureRect(m_cTouchSpriteFrameRect[1]);
            }
            startLongPress(RECHARGE_ADD_TAG);
            
        }else if(m_nTouchTag == 2)
        {
            
        }else if(m_nTouchTag == 3)
        {
            
        }
    }
    return true;

}
void CGameRechargeLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
  
}
void CGameRechargeLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
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
        m_nCurrentCount = m_nCurrentCount >0? m_nCurrentCount : 1;
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
        updateTexture();
        
        
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
void CGameRechargeLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
  
}

void CGameRechargeLayer::initCGameRechargeLayer(CStructShopSellItem *item)
{
    CCAssert(item, "shopsellItem null");
    
    m_nMaxCount = getMaxCount(item);
    m_nCashPerItem = item->getValue();
    CPtProp* propItem = item->getShopSellItemPropData();
    std::string str =  propItem->getIconName();
  
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
    CCLabelTTF * label = CCLabelTTF::create("请确定你要购买的道具数目", "Arial", 18);
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
    array[2] = 4;
    char buff[30] ={0};
    sprintf(buff, "%d/%d", m_nCurrentCount, m_nMaxCount);
    parent = (CCSprite *)m_cMaps->getElementByTags(array, 3);
    m_pNumberLabel = CCLabelTTF::create(buff, "Arial", 18);
    size =  parent->boundingBox().size;
    m_pNumberLabel->setPosition(ccp(size.width/2-25, size.height/2));
    parent->addChild(m_pNumberLabel);
    m_pNumberLabel->setColor(ccc3(0, 240, 255));
    
    // cash tip:
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "你所消耗的现金币: %d", m_nCurrentCount*m_nCashPerItem);
    m_pCash = CCLabelTTF::create(buff, "Arial", 15);
    m_pCash->setPosition(ccp(-30, -30));
    m_pCash ->setColor(ccc3(126, 60, 30));
    parent->addChild(m_pCash);
    
    // touch event:
    
    CCNode *node = NULL;
    array[1]= 2, array[2] = 2;
    node = m_cMaps->getElementByTags(array, 3);
    Utility::addTouchRect(node->getTag(), node, m_cTouches);
    label = CCLabelTTF::create("确定", "Arial", 18);
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
    

}
void CGameRechargeLayer::handlerTouch()
{
    if (m_nTouchTag == 2)
    {
        // confirm
        int* integer = new int(m_nCurrentCount);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("CGameRechargeLayercallback_layer", (CCObject*)integer);
        removeFromParentAndCleanup(true);

    }else if(m_nTouchTag == 3)
    {
        // cancle
        removeFromParentAndCleanup(true);
    }
    
}
void CGameRechargeLayer::updateTexture()
{
    char buff[30] ={0};
    sprintf(buff, "%d/%d", m_nCurrentCount, m_nMaxCount);
    m_pNumberLabel->setString(buff);
    
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "你所消耗的现金币: %d", m_nCurrentCount*m_nCashPerItem);
    m_pCash->setString(buff);
    
}

void CGameRechargeLayer::startLongPress(int inType)
{
    m_nType = inType;
    m_bLongPress = false;
    this->schedule(schedule_selector(CGameRechargeLayer::longTouchCallBack), 0.5f);
    
}
void CGameRechargeLayer::stopLongPress()
{
    this->unschedule(schedule_selector(CGameRechargeLayer::longTouchCallBack));
}
void CGameRechargeLayer::longTouchCallBack(float dt)
{
   
    m_bLongPress = true;
    if (m_nType ==  RECHARGE_SUB_TAG)
    {
        if (m_nCurrentCount >0)
        {
            m_nCurrentCount -= 10;
            m_nCurrentCount = m_nCurrentCount <= 0 ? 1 : m_nCurrentCount;

        }
     
    }else if(RECHARGE_ADD_TAG)
    {
       
        if (m_nCurrentCount < m_nMaxCount)
        {
            m_nCurrentCount += 10;
            m_nCurrentCount = m_nCurrentCount >= m_nMaxCount ? m_nMaxCount : m_nCurrentCount;
        }
    
    }else
    {
        stopLongPress();
    }
    
    updateTexture();
}

int CGameRechargeLayer::getMaxCount(CStructShopSellItem* inItem)
{
    int count = 0;
    CGamePlayer * player = SinglePlayer::instance();
    if (inItem)
    {
        unsigned int maxGroup = inItem->getItemSellMaxNum();
        unsigned int countInGroup = inItem->getGroupNum();
        CCAssert(countInGroup != 0, "grounNum is zero");
        int cashPerGroup = inItem ->getValue();
        int maxGroupsCanInBackPack = player->getPropMaxCountAddToBag(inItem->getShopSellItemPropData()->getPropId());
        maxGroupsCanInBackPack /= countInGroup;
        count = maxGroup >= maxGroupsCanInBackPack ? maxGroupsCanInBackPack : maxGroup;
        CCLog("the count %d", maxGroupsCanInBackPack);
        int cash = player->getPlayerCash();
        maxGroupsCanInBackPack = cash/cashPerGroup;
        count = count >= maxGroupsCanInBackPack ? maxGroupsCanInBackPack : count;
    }
    return count;
    
}



