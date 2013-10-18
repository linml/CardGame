//
//  CPtRecharge.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-10-14.
//
//

#include "CPtRecharge.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "gameMiddle.h"
#include "PtJsonUtility.h"

bool CPtRecharge::init()
{
    
    PtMapUtility::addChildFromScript(this,CSTR_FILEPTAH(plistPath, "recharge.plist"));
    PtMapUtility::addTouchRectFromScript(CSTR_FILEPTAH(plistPath, "recharge.plist"), this, &vTouchRect);
    
    CCLabelTTF* ttfCoin = (CCLabelTTF*)getChildByTag(2001)->getChildByTag(6)->getChildByTag(31)->getChildByTag(12);
    ttfCoin->setString(ConvertToString(SinglePlayer::instance()->getCoin()).c_str());
    
    CCLabelTTF* ttfCash = (CCLabelTTF*)getChildByTag(2001)->getChildByTag(6)->getChildByTag(30)->getChildByTag(12);
    ttfCash->setString(ConvertToString(SinglePlayer::instance()->getPlayerCash()).c_str());
    
    
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "recharge_config.plist"));
    
    for(int i=0; i<6;i++)
    {
        char key[3] ="";
        sprintf(key, "%c",'1'+i);
        CCSprite* temp = createIAPItem((CCDictionary*)dic->objectForKey(key));
        getChildByTag(2001)->getChildByTag(6)->getChildByTag(24+i)->addChild(temp);        
    }
    
    if(SinglePlayer::instance()->getPlayrHadRecharged())
    {
        for(int i=0; i<6;i++)
        {
            CCNode* temp = getChildByTag(2001)->getChildByTag(6)->getChildByTag(6+i);
            temp->setVisible(false);
        }
    }

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(-20000);

    return true;
}

bool CPtRecharge::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CPtRecharge::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint pos=pTouch->getLocation();
    handleTouchSpritePool(pos);
}

bool CPtRecharge::handleTouchSpritePool(CCPoint point)
{
    CCSprite* btn = CCSprite::create();
    int tag=TouchRect::SearchTouchTag(point, vTouchRect, &btn);
    if (tag == -1) {
        return false;
    }
    Utility::handleBtnCallBack(btn, this, NULL);
    switch (tag) {
        case 20000:
            removeFromParentAndCleanup(true);
            break;
        case 30001:
        case 30002:
        case 30003:
        case 30004:
        case 30005:
        case 30006:
            setTouchEnabled(false);
            sendIapItem(tag-30000);
            break;
        default:
            break;
    }
    return false;
}

CCSprite* CPtRecharge::createIAPItem(CCDictionary* dicItem)
{
    CCSprite* item = CCSprite::create();
    item->setAnchorPoint(ccp(0, 0.5));
    CCString* cash_base = (CCString*)dicItem->objectForKey("cash");
    CCLabelTTF* ttfCash = CCLabelTTF::create(cash_base->m_sString.c_str(),"",24);
    ttfCash->setAnchorPoint(ccp(0, 0.5));
    ttfCash->setPosition(ccp(30, 15));
    item->addChild(ttfCash);
    string inf = "";
    if(GameTools::intForKey("state",dicItem) == 2)
    {
        CCString* sale_text = (CCString*)dicItem->objectForKey("sale_text");
        CCString* sale = (CCString*)dicItem->objectForKey("sale_percent");
        inf += sale_text->m_sString;
        inf += "(";
        inf += ConvertToString(atoi(cash_base->m_sString.c_str())*atoi(sale->m_sString.c_str())/100);
        inf += ")";
        CCLabelTTF* ttfScale = CCLabelTTF::create(inf.c_str(),"",22);
        ttfScale->setAnchorPoint(ccp(0, 0.5));
        ttfScale->setPosition(ccp(0, -22));
        item->addChild(ttfScale);

    }
    CCString* rmb_base = (CCString*)dicItem->objectForKey("rmb");
    inf = "￥";
    inf += rmb_base->m_sString;
    CCLabelTTF* ttfRmb = CCLabelTTF::create(inf.c_str(),"",28);
    ttfRmb->setAnchorPoint(ccp(1, 0.5));
    item->addChild(ttfRmb);
    ttfRmb->setPosition(ccp(140,-50));
    
    CCSprite* icon = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "recharge.png"));
    icon->setPosition(ccp(-56, -11));
    item->addChild(icon);
    
    return item;
}

void CPtRecharge::sendIapItem(int itemId)
{
    CCLog("item %d",itemId);
    char achData[256]="";
    sprintf(achData, "sig=%s&recharge_id=%d&type=1",SinglePlayer::instance()->getUserSig(),itemId);
    ADDHTTPREQUESTPOSTDATA(STR_URL_IAPBUY(194),
                           "CALLBACK_CPtRecharge::sendIapItem",
                           "REQUEST_CPtRecharge::sendIapItem",
                           achData,
                           callfuncO_selector(CPtRecharge::onReceiveIAPBuyMsg));
}

void CPtRecharge::onReceiveIAPBuyMsg(CCObject *pOject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPtRecharge::sendIapItem");
    char* data = (char*)pOject;
    printf("msg:%s\n",data);
    if(!data)
    {
        Middle::showAlertView("网络异常,请重试");
    }
    else if(strstr(data,"1901"))
    {
        Middle::showAlertView("参数不足");
    }
    else if(strstr(data,"1902"))
    {
        Middle::showAlertView("请求创建订单失败");
    }
    else if(strstr(data,"1903"))
    {
        Middle::showAlertView("确认订单失败->之前客户端没有来请求,第三方直接来请求");
    }
    else if(strstr(data,"1904"))
    {
        Middle::showAlertView("充值ID错误(不在充值ID范围内)");
    }
    else if(strstr(data,"1905"))
    {
        Middle::showAlertView("充值失败");
    }
    else if(strstr(data,"1906"))
    {
        Middle::showAlertView("第一次双倍充值记录失败(unlock记录失败)");
    }
    else if(strstr(data,"1907"))
    {
        Middle::showAlertView("订单验证失败(recharge_id,type与缓存中的不一致)");
    }
    else
    {
        if(!SinglePlayer::instance()->getPlayrHadRecharged())
        {
            SinglePlayer::instance()->setPlayrHadRecharged(true);
            for(int i=0; i<6;i++)
            {
                CCNode* temp = getChildByTag(2001)->getChildByTag(6)->getChildByTag(6+i);
                temp->setVisible(false);
            }
        }

        Middle::showAlertView(data);
        CCDictionary* dic=(CCDictionary*)PtJsonUtility::JsonStringParse(data);
        CCDictionary* dic_result=(CCDictionary*)dic->objectForKey("result");
        CCDictionary* dic_info=(CCDictionary*)dic_result->objectForKey("user_info");
        SinglePlayer::instance()->setPlayerCash(GameTools::intForKey("cash", dic_info));
        CCLabelTTF* ttfCash = (CCLabelTTF*)getChildByTag(2001)->getChildByTag(6)->getChildByTag(30)->getChildByTag(12);
        ttfCash->setString(ConvertToString(GameTools::intForKey("cash", dic_info)).c_str());

    }
    delete []data; //POST出来的数据是NEW出来的。 需要自己去删除下。 MERLIN
    data=NULL;
    setTouchEnabled(true);
}

