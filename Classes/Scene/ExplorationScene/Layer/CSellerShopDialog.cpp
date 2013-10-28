//
//  CSellerShopDialog.cpp
//  91.cube
//
//  Created by phileas on 13-10-18.
//
//

#include "CSellerShopDialog.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "CPtTool.h"
#include "CStructShopSellItem.h"
#include "CGameRechargeLayer.h"
#include "CGameRechargeTip.h"
#include "CGameArrageBackpackTip.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CReward.h"


CSellerShopDialog* CSellerShopDialog::create(SELLER_DATA inSellData, int inSellerType, int inShopId, string inSellerName)
{
    CSellerShopDialog *shops = new CSellerShopDialog();
    if (shops && shops->initCSellerShopDialog(inSellData, inSellerType, inShopId, inSellerName))
    {
        shops->autorelease();
    }else
    {
        CC_SAFE_DELETE(shops);
        shops = NULL;
    }
    return shops;
}

CSellerShopDialog::CSellerShopDialog()
{
    loadResouce();
    m_pPropData = SinglePropConfigData::instance();
    for (int i = 0; i < 3; i++)
    {
        m_pBuyBtn[i] = NULL;
        m_pLimitLabel[i] = NULL;
    }
    m_pCloseBtn = NULL;
    m_pCurrency = NULL;
    m_pPlayer = SinglePlayer::instance();
    m_pTotalMoney = NULL;
    m_nShopId = 0;
    m_nLimitCount = 0;
    m_pCloseHandler = NULL;
    m_pCloseSelector = NULL;
    m_nSellerType = 1;
    m_nSelectPropId = 0;
}

CSellerShopDialog::~CSellerShopDialog()
{
    
}


bool CSellerShopDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    if (CPtTool::isInNode(m_pCloseBtn, pTouch))
    {
        m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeSelected.png"));
        m_nTouchTag = 3;
    }
    for (int i = 0; i < 3; i++)
    {
        if (m_pBuyBtn[i]->getEnable()&&CPtTool::isInNode(m_pBuyBtn[i], pTouch))
        {
            m_pBuyBtn[i]->selected();
            m_nTouchTag = i;
            break;
        }
    }
    return true;
}
void CSellerShopDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CSellerShopDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        handlerTouch(pTouch);
    }
    
}
void CSellerShopDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSellerShopDialog::handlerTouch(CCTouch *pTouch)
{
   
    if (m_nTouchTag>=0 && m_nTouchTag <3)
    {
        m_pBuyBtn[m_nTouchTag]->unselected();
    }
    if (m_nTouchTag == 3)
    {
      m_pCloseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeNormal.png"));
      if (CPtTool::isInNode(m_pCloseBtn, pTouch))
      {
          removeFromParentAndCleanup(true);
          return;
      }
        
    }
    
    for (int i = 0; i < 3; i++)
    {
        if (m_pBuyBtn[i]->getEnable()&&CPtTool::isInNode(m_pBuyBtn[i], pTouch) && m_nTouchTag == i)
        {
            if (m_nSellerType ==  1)
            {
                onClickBuyBtn(i);
            }else if(m_nSellerType == 2)
            {
                onClickExtract(i);
            }
        
            break;
        }
    }

    
   
}

bool CSellerShopDialog::initCSellerShopDialog(SELLER_DATA inSellData, int inSellerType, int inShopId, string inSellerName)
{
    m_sSellData = inSellData;
    m_nShopId = inShopId;
    m_nSellerType = inSellerType;
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        char buff[100]={0};
        sprintf(buff, "%s的商店",inSellerName.c_str());
        initCSellerShopUI(buff);
        bRet = true;
    } while (0);
    return bRet;
}

void CSellerShopDialog::setCloseHandler(CCObject *inTarget, SEL_CallFuncO inSelector)
{
    m_pCloseHandler = inTarget;
    m_pCloseSelector = inSelector;
    
}

void CSellerShopDialog::initCSellerShopUI(const char *inSellerName)
{
    // create currency spriteframe:
    if (m_sSellData.priceType == 1)
    {
        m_pCurrency = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cash.png");
    }else
    {
        m_pCurrency = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("coin.png"); 
    }
    
    
    // add bg:
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite * sellerShopBg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "sellershop_bg.png"));
    sellerShopBg->setPosition(ccp(winSize.width/2, winSize.height/2+30));
    CCSize bgSize = sellerShopBg->getContentSize();

    // add close button:
    CCSprite * cancelBtn = CCSprite::createWithSpriteFrameName("closeNormal.png");
    cancelBtn->setAnchorPoint(ccp(1,1));
    cancelBtn->setPosition(ccp(bgSize.width-30, bgSize.height-30));
    sellerShopBg->addChild(cancelBtn);
    m_pCloseBtn = cancelBtn;
    
    //add shop name:
    
    CCLabelTTF * shopName = CCLabelTTF::create(inSellerName, "Arial", 18);
    shopName->setPosition(ccp(bgSize.width/2, bgSize.height-45));
    sellerShopBg->addChild(shopName);
    
    
        // 道具商人
        CCNode * node = NULL;
        for (int i = 0; i < 3; i++)
        {
            node= createShopItem(m_sSellData.sellerShops[i],i);
            CCSize nodeSize = node->getContentSize();
            node->setPosition(ccp(nodeSize.width/2+i*nodeSize.width+ 45, nodeSize.height/2+60));
            sellerShopBg->addChild(node);
        }
    
    
    // add player's cash or coin:
    CCSprite * currency = CCSprite::createWithSpriteFrame(m_pCurrency);
    int total = getPalyerMoney();
    char buff[50] = {0};
    sprintf(buff, "%d", total);
    m_pTotalMoney = CCLabelTTF::create(buff, "Arial", 13);
    
    currency->setPosition(ccp(winSize.width-230, 45));
    m_pTotalMoney->setPosition(ccp(winSize.width-180, 45));
    
    sellerShopBg->addChild(currency);
    sellerShopBg->addChild(m_pTotalMoney);

    addChild(sellerShopBg);
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1000);
    
    
    
}

CCNode * CSellerShopDialog::createShopItem(const SELLER_SHOP &shopItem, int inIndex)
{
    
    CCNode *node = CCNode::create();
    CPtProp * item = m_pPropData->getPropById(shopItem.propId);
    if (item)
    {
        CCSprite *sprite=CCSprite::createWithSpriteFrameName("shangchengzixiang.png");
        node->addChild(sprite,0,1);
        node->setContentSize(sprite->getContentSize());
    }
    
    CCLabelTTF *labelTTF=CCLabelTTF::create(item->getPropName().c_str(), "Arial", 15);
    node->addChild(labelTTF, 1, 10);
    labelTTF->setPosition(ccp(0,85));
    //    const CCSize& dimensions, CCTextAlignment hAlignment
    CCLabelTTF *labelTTFTip=CCLabelTTF::create(item->getTips().c_str(), "Arial", 15,CCSizeMake(130, 100),kCCTextAlignmentLeft);
    addChild(labelTTFTip, 1, 11);
    labelTTFTip->setAnchorPoint(ccp(0,0.5));
    labelTTFTip->setPosition(ccp(0,0));
    
    CCSprite *itemPng=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, item->getIconName().c_str()));
    node->addChild(itemPng,1,12);
    itemPng->setPosition(ccp(-70,8));
    char priceData[40];
    
    if (m_nSellerType == 1)
    {
        if(shopItem.limitNum != 0)
        {
            CCSprite *xianliang=CCSprite::createWithSpriteFrameName("xianggou.png");
            node->addChild(xianliang,1,13);
            xianliang->setPosition(ccp(-110, 60));
            //设置一个限量的比例
            int value=shopItem.limitNum;
            sprintf(priceData, "现货供应量:%d",value);
            CCLabelTTF *xianglianggeshu=CCLabelTTF::create(priceData, "Arial", 15);
            node->addChild(xianglianggeshu,1,21);
            xianglianggeshu->setAnchorPoint(ccp(0, 0));
            xianglianggeshu->setPosition(ccp(-120, -90));
            xianglianggeshu->setColor(g_custom_color[17]);
            m_pLimitLabel[inIndex] = xianglianggeshu;
        }
    }
       
   
    CCSprite *zuanshidazhenow= CCSprite::createWithSpriteFrame(m_pCurrency); //CCSprite::createWithSpriteFrameName("zuanshi.png");
    node->addChild(zuanshidazhenow,1,16);
    zuanshidazhenow->setPosition(ccp(80,-40));
    sprintf(priceData, "%d",shopItem.price);
    CCLabelTTF *labelttfnew=CCLabelTTF::create(priceData, "Arial", 15);
    node->addChild(labelttfnew,1,17);
    labelttfnew->setPosition(ccp(110, -40));
    labelttfnew->setColor(g_custom_color[13]);
    string word;
    if (m_nSellerType == 1)
    {
       word = Utility::getWordWithFile("word.plist", "goumai");
    }else if(m_nSellerType == 2)
    {
        word = "抽取";
    }
    
    CGameButtonControl *pGamebutton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "normal.png", "pressed.png","disabled.png");
    node->addChild(pGamebutton,1,20);
    pGamebutton->setFontColor(g_custom_color[17]);
    pGamebutton->setPosition(ccp(80,-80));
    m_pBuyBtn[inIndex] = pGamebutton;
    
    CCLabelTTF *tips = CCLabelTTF::create(item->getTips().c_str(), "Arial", 13);
    tips->setPosition(ccp(55, 20));
    node->addChild(tips);
    return node;
   
}


int CSellerShopDialog::getPalyerMoney()
{
    int total = 0;
    if (m_sSellData.priceType == 1)
    {
        total = m_pPlayer->getPlayerCash();
    }else
    {
        total = m_pPlayer->getCoin();
    }
    return total;

}

void CSellerShopDialog::onClickBuyBtn(int inIndex)
{
    if (inIndex >= 0 && inIndex <3)
    {

        const SELLER_SHOP &shop = m_sSellData.sellerShops[inIndex];
        CStructShopSellItem shopSellItem = CStructShopSellItem(shop.propId);
        shopSellItem.setGroupNum(shop.teamNum);
        shopSellItem.setOldValue(shop.price);
        shopSellItem.setValue(shop.price);
        if (shop.limitNum != 0)
        {
            if (shop.limitNum == -1)
            {
                shopSellItem.setItemSellMaxNum(0);
                
            }else
            {
                shopSellItem.setItemSellMaxNum(shop.limitNum);
            }
 
        }
        
        if (shop.price > getPalyerMoney())
        {
            //money 不够
            //  1--> cash
            //  2--> coin
            if (m_sSellData.priceType == 1)
            {
                CGameRechargeTip *layer = CGameRechargeTip::create();
                addChild(layer, 1000);
            }else
            {
                CCMessageBox("王老板，游戏币不够", "Tip");
            }
           
            return;
        }else if(m_pPlayer->getPropMaxCountAddToBag(shop.propId)== 0)
        {
            //无法添加的背包
            CGameArrageBackpackTip *layer = CGameArrageBackpackTip::create();
            addChild(layer, 1000);
            return;
        }
        
        CGameRechargeLayer *layer = CGameRechargeLayer::create(&shopSellItem, m_sSellData.priceType);
        layer->setHanlder(this, callfuncO_selector(CSellerShopDialog::onSendBuyRequest), NULL);
        addChild(layer, 1000);
    
    }
    
}

void CSellerShopDialog::onClickExtract(int inIndex)
{
    if (inIndex>= 0 && inIndex < 3)
    {
        const SELLER_SHOP &shop = m_sSellData.sellerShops[inIndex];
        CStructShopSellItem shopSellItem = CStructShopSellItem(shop.propId);
        shopSellItem.setOldValue(shop.price);
        shopSellItem.setValue(shop.price);
        m_nSelectPropId = shop.propId;
        if (shop.price > getPalyerMoney())
        {
            //money 不够
            //  1--> cash
            //  2--> coin
            if (m_sSellData.priceType == 1)
            {
                CGameRechargeTip *layer = CGameRechargeTip::create();
                addChild(layer, 1000);
            }else
            {
                CCMessageBox("王老板，游戏币不够", "Tip");
            }
            
            return;
        }else
        {
            onSendBuyRequest(NULL);
        }
    }
  
    
}

void CSellerShopDialog::onClickClose()
{
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)(NULL);
    }
    removeFromParentAndCleanup(true);
}

// connect server:

//api.php?m=shop&a=buy&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&shop_id=1&item_id=1(物品ID)&num=1(数量)

void CSellerShopDialog::onSendBuyRequest(CCObject *pObject)
{
    CGameRechargeLayer* layer = (CGameRechargeLayer*) pObject;
    if (layer)
    {
        int shopId = m_nShopId;
        int propId = layer->getPropId();
        int num = layer->getBuyNum();
        int sellerType = m_nSellerType;
        if (m_nTouchTag>=0 && m_nTouchTag <3)
        {
          m_nLimitCount = m_sSellData.sellerShops[m_nTouchTag].limitNum;
          if (m_nLimitCount!=0)
          {
              m_nLimitCount -= num;
              m_nLimitCount = m_nLimitCount== 0 ? -1 : m_nLimitCount; // user over:
          }
            
        }else
        {
            CCAssert(false, "error...");
        }
        
        
        CCLog("seller event : shopId--> %d, propId--> %d, buy number---> %d,", shopId, propId, num);
        
        char buf[200]={0};
        sprintf(buf, "&sig=%s&shop_id=%d&item_id=%d&num=%d&seller_shop_type=%d",m_pPlayer->getUserSig(),shopId, propId, num,sellerType);
        CCLog("send data : %s", buf);
        ADDHTTPREQUESTPOSTDATA(STR_URL_GETSHOPBUY(194), "CALLBACK_CSellerShopDialog_onSendBuyRequest", "REQUEST_CSellerShopDialog_onSendBuyRequest",buf,callfuncO_selector(CSellerShopDialog::onReceiveBuyRequest));

    }
    else
    {
//        CCMessageBox("功能尚未开放", "请稍后!");
//        return;
        int shopId = m_nShopId;
        int num = 1;
        int propId = m_nSelectPropId;
        int sellerType = m_nSellerType;
        CCLog("seller event : shopId--> %d, propId--> %d, buy number---> %d", shopId, propId, num);
        
        char buf[200]={0};
        sprintf(buf, "&sig=%s&shop_id=%d&item_id=%d&num=%d&seller_shop_type=%d",m_pPlayer->getUserSig(),shopId, propId, num,sellerType);
        CCLog("send data : %s", buf);
        ADDHTTPREQUESTPOSTDATA(STR_URL_GETSHOPBUY(194), "CALLBACK_CSellerShopDialog_onSendBuyRequest", "REQUEST_CSellerShopDialog_onSendBuyRequest",buf,callfuncO_selector(CSellerShopDialog::onReceiveBuyRequest));
       
    }

}

void CSellerShopDialog::onReceiveBuyRequest(CCObject *pObject)
{
    static int i = 0;
    CCLog("the i: %d", i++);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSellerShopDialog_onSendBuyRequest");
  
    const char *buffer = (char*) pObject;
    if (buffer)
    {
        CCLog("the server post data: %s", buffer);
        CCDictionary * tmpDict = PtJsonUtility::JsonStringParse(buffer);
        delete []buffer;
        CCAssert(tmpDict, "tmpDict null");
        if(tmpDict)
        {
            int code = GameTools::intForKey("code", tmpDict);
          
            if (code == 0)
            {
                CCDictionary * resultDict = (CCDictionary*) tmpDict->objectForKey("result");
                if (resultDict)
                {
                    onHandlerBuySuccess(resultDict);
                    
                }else
                {
                    CCMessageBox("the result is null", "Server ERROR");
                }
                
            }else
            {
                CCMessageBox(CCString::createWithFormat("the error code : %d",code)->getCString(), "CSellerShopDialog :error code");
                
            }
          
        }

    }else
    {
        CCMessageBox("破服务端发送数据为空", "破服务端");
        return;
        
    }
 
}

void CSellerShopDialog::onHandlerBuySuccess(CCDictionary *inResultDict)
{
    if (m_nSellerType == 1)
    {
        onHandlerBuyProp(inResultDict);
    }else if(m_nSellerType == 2)
    {
        onHandlerExtractCard(inResultDict);
    }
}

void CSellerShopDialog::onHandlerBuyProp(CCDictionary *inResultDict)
{
    if (m_nLimitCount == -1)
    {
        m_pBuyBtn[m_nTouchTag]->setDisable();
    }
    if (m_nTouchTag >= 0 && m_nTouchTag <3)
    {
        updateLimitNum(m_pLimitLabel[m_nTouchTag], m_nLimitCount);
        m_sSellData.sellerShops[m_nTouchTag].limitNum = m_nLimitCount;
    }
    updateTotalNum();
    
    CCDictionary *rewardDict=(CCDictionary*)(inResultDict->objectForKey("reward"));
    if(rewardDict)
    {
        CReward *reward =NULL;
        if (rewardDict->objectForKey("add") && ((CCDictionary*)rewardDict->objectForKey("add"))->objectForKey("shop") )
        {
            reward = CReward::create((CCDictionary *)((CCDictionary*)rewardDict->objectForKey("add"))->objectForKey("shop"));
            reward->excuteReward(ADD);
            
        }
        if(rewardDict->objectForKey("dec") &&((CCDictionary*)rewardDict->objectForKey("dec"))->objectForKey("shop"))
        {
            reward = CReward::create((CCDictionary *)((CCDictionary*)rewardDict->objectForKey("dec"))->objectForKey("shop"));
            reward->excuteReward(DEC);
        }
        
    }

}
void CSellerShopDialog::onHandlerExtractCard(CCDictionary *inResultDict)
{
    CCDictionary *rewardDict=NULL;
    
    rewardDict = (CCDictionary*)inResultDict->objectForKey("reward");
    if(rewardDict)
    {
        CReward *reward =NULL;
        CReward *addReward =NULL;
        if (rewardDict->objectForKey("add") && ((CCDictionary*)rewardDict->objectForKey("add"))->objectForKey("event") )
        {
            // add card:
            addReward = CReward::create((CCDictionary *)((CCDictionary*)rewardDict->objectForKey("add"))->objectForKey("event"));
            addReward->excuteReward(ADD);
            
        }
        if(rewardDict->objectForKey("dec") &&((CCDictionary*)rewardDict->objectForKey("dec"))->objectForKey("shop"))
        {
            reward = CReward::create((CCDictionary *)((CCDictionary*)rewardDict->objectForKey("dec"))->objectForKey("shop"));
            reward->excuteReward(DEC);
    
            reward = CReward::create(reward, addReward);  
            char buffer[200] = {0};
            reward->getRewardContent(buffer, 200);
            CCMessageBox(buffer, "卡牌商人tip");
        }
        
    }

    
}

void CSellerShopDialog::updateLimitNum(CCLabelTTF * inLimitLabel, int inLimitNumber)
{
    if (inLimitLabel && inLimitNumber != 0)
    {
        char buffer[50]={0};
        if (inLimitNumber == -1)
        {
            inLimitNumber = 0;
        }
        sprintf(buffer, "现货供应量:%d", inLimitNumber);
        inLimitLabel->setString(buffer);
    }
}

void CSellerShopDialog::updateTotalNum()
{
    int total = getPalyerMoney();
    char buff[50] = {0};
    sprintf(buff, "%d", total);
    m_pTotalMoney->setString(buff);
}

void CSellerShopDialog::loadResouce()
{
   CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "shangchenganniu.png"));
   CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "shangchenganniu.plist"), texture);
   CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "backpack.plist"), CSTR_FILEPTAH(g_mapImagesPath,"backpack.png"));
}