//
//  CEveryDayLoginLayer.cpp
//  91.cube
//
//  Created by linminglu on 12/9/13.
//
//

#include "CEveryDayLoginLayer.h"
#include "gamePlayer.h"
#include "CEveryDayLoginData.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CReward.h"
#include "CEveryDayLoginRewardInfoLayer.h"

#define TAG_QUITBUTTON 200
#define TAG_QIANDAOBUTTON 202222
#define TAG_MSGLABEL 202
#define TAG_ITEMINFO 203


CEveryDayLoginLayer::CEveryDayLoginLayer()
{
    m_vTouchArray=new CCArray;
    m_vTouchArray->retain();
    size=CCDirector::sharedDirector()->getWinSize();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "seller.plist"), CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));
    m_tempPlayer=SinglePlayer::instance();
    toDay=CPtTool::getToDayByTimet(m_tempPlayer->m_pEveryDataLogin->m_nServerTime);
    m_pQianDaoManager=new CEveryDayLoginRewardDataContainer;
    m_pQianDaoManager->initWithLoginData(m_tempPlayer->m_pEveryDataLogin);
}

CEveryDayLoginLayer::~CEveryDayLoginLayer()
{
    CC_SAFE_RELEASE(m_vTouchArray);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_plistPath, "seller.plist"));//, CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));
    CC_SAFE_DELETE(m_pQianDaoManager);
}


bool CEveryDayLoginLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    createColorBackGroud();
    createLayer();
    createQuitbutton();
    createQianDaoButton();
    CEveryDayLoginDataStruct *item = m_pQianDaoManager->getLoginDataStructByIndex(toDay-1);
    createTodayInfo(item);
    setTouchEnabled(true);
    setTouchPriority(-99);
    return true;
}
void CEveryDayLoginLayer::createColorBackGroud()
{
    CCLayerColor *layer=CCLayerColor::create(ccc4(0, 0, 255, 120), 800, 600);
    addChild(layer,0);
    layer->ignoreAnchorPointForPosition(false);
    
    layer->setPosition(ccp(size.width *0.5,size.height*0.5));
    
}

void CEveryDayLoginLayer::sendQianDao()
{
    if (!m_tempPlayer->isTodayHaveGet()) {
        string connectData="sig=";
        connectData += m_tempPlayer->getUserSig();
        ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_QIANDAO(194), "CALLBACK_CEveryDayLoginLayer_sendQianDao", "REQUEST_CEveryDayLoginLayer_sendQianDao",connectData.c_str(),callfuncO_selector(CEveryDayLoginLayer::callBackGetLingQu));
    }
}
void CEveryDayLoginLayer::runCCLayerBel()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create("签到成功", "Arial", 40);
    addChild(labelTTF,10,TAG_MSGLABEL);
    labelTTF->setPosition(CCPointMake(size.width *0.5, size.height*0.5));
    labelTTF->runAction(CCSequence::create(CCSpawn::create(CCFadeOut::create(2.0),CCMoveBy::create(2.0, CCPointMake(0, 100)),NULL),CCCallFunc::create(this, callfunc_selector(CEveryDayLoginLayer::removeCCLayerber)),NULL));
}
void CEveryDayLoginLayer::removeCCLayerber()
{
    if (getChildByTag(TAG_MSGLABEL)) {
        removeChildByTag(TAG_MSGLABEL, true);
    }
}
void CEveryDayLoginLayer::callBackGetLingQu(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CEveryDayLoginLayer_sendQianDao");
    if(!object)
    {
        CCMessageBox("fuck 服务端 签到数据是NULL的", "error");
        return;
    }
    char *strdata=(char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    strdata=NULL;
    if (GameTools::intForKey("code", dict)==0) {
        //可能签到的数据被再次签到。
        CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
        if(resultDict)
        {
            int valueInfo=GameTools::intForKey("info", resultDict);
            if (valueInfo==1) {
                string strDateArray=GameTools::valueForKey("sign_info", resultDict);
                m_tempPlayer->initDayInfo(strDateArray);
                CCDictionary *rewardDict = (CCDictionary*)resultDict->objectForKey("reward");
                if (rewardDict)
                {
                    rewardDict = (CCDictionary*)rewardDict->objectForKey("add");
                    if (rewardDict)
                    {
                        rewardDict =(CCDictionary*) rewardDict->objectForKey("login_reward");
                        CReward * reward = CReward::create(rewardDict);
                        if (reward)
                        {
                            reward->excuteReward(ADD);
                        }
                    }
                    
                }
                runCCLayerBel();
                updateLoad();
            }
        }
    }
    else{
        CCLog("error==============");
    }

}

void CEveryDayLoginLayer::updateLoad()
{
    string word;
    if (m_tempPlayer->isTodayHaveGet())
    {
        word=Utility::getWordWithFile("word.plist", "yiqiandao");
    }
    else
    {
        word=Utility::getWordWithFile("word.plist", "qiandao");
    }
    CCLog("====>%s",word.c_str());
    CGameButtonControl *temp=(CGameButtonControl *)getChildByTag(TAG_QIANDAOBUTTON);
    temp->setText(word.c_str());
    m_pQianDaoManager->initWithLoginData(m_tempPlayer->m_pEveryDataLogin);
    for (int i=0; i<m_pQianDaoManager->getDataSize(); i++) {
        CCLayer *layerItem=(CCLayer *)getChildByTag(100+i);
        CCLayerColor *layerColorItem=(CCLayerColor *)layerItem->getChildByTag(1);
        layerColorItem->setColor(getItemBackGroundColor(m_pQianDaoManager->getLoginDataStructByIndex(i)));
    }


}



CCPoint CEveryDayLoginLayer::getItemPostion(int index)
{
    int row =index/5;
    int cloum=index%5;
    float pointX= size.width *0.5-200+70*cloum;
    float pointY=size.height *0.5+200-70*row;
    return CCPointMake(pointX, pointY);
}

CCLayer *CEveryDayLoginLayer::createItemSprite(CEveryDayLoginDataStruct *pSEvery)
{
    CCLayer *layerResult=CCLayer::create();
    CCLayerColor *layer=CCLayerColor::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->setColor(getItemBackGroundColor(pSEvery));
    layer->setContentSize(CCSizeMake(60, 60));
    layerResult->addChild(layer,1,1);
    layer->setOpacity(255);
    layerResult->setContentSize(CCSizeMake(70, 70));
    string pngValuePath=pSEvery->getIconFullPathByType(0);
    if (!pngValuePath.empty()) {
        CCSprite *sprinte=CCSprite::create(pngValuePath.c_str());
        layerResult->addChild(sprinte,2);
    }
    CCLabelTTF *labelTTF=CCLabelTTF::create(ConvertToString(pSEvery->getDayInMonth()).c_str(), "Arial", 15);
    labelTTF->setColor(ccc3(255, 0, 0));
    labelTTF->setPosition(ccp(20,10));
    layerResult->addChild(labelTTF,2);
    return layerResult;
}


void CEveryDayLoginLayer::createLayer()
{
    CCLog("toDay %d",toDay);
    for (int i=0; i<m_pQianDaoManager->getDataSize(); i++) {
        CCLayer * sprite=createItemSprite(m_pQianDaoManager->getLoginDataStructByIndex(i));
        addChild(sprite,2,100+i);
        sprite->setPosition(getItemPostion(i));
    }
}

void CEveryDayLoginLayer::createQuitbutton()
{
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, "", "closeNormal.png", "closeSelected.png");
    addChild(gamebutton,2,TAG_QUITBUTTON);
    gamebutton->setPosition(CCPointMake(size.width*0.5+200, size.height *0.5+200));
    m_vTouchArray->addObject(gamebutton);
}

void CEveryDayLoginLayer::createTodayInfo(CEveryDayLoginDataStruct *item)
{
    CEveryDayLoginRewardInfoLayer *layer=CEveryDayLoginRewardInfoLayer::create(item);
    addChild(layer,2,TAG_ITEMINFO);
    layer->setPosition(CCPointMake(size.width*0.5+250, size.height *0.5));
}

void CEveryDayLoginLayer::createQianDaoButton()
{
    string word;
    if (m_tempPlayer->isTodayHaveGet()) {
        word=Utility::getWordWithFile("word.plist", "yiqiandao");
    }
    else{
         word=Utility::getWordWithFile("word.plist", "qiandao");
    }
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "goShopNormal.png", "goShopPressed.png");
    addChild(gamebutton,2,TAG_QIANDAOBUTTON);
    gamebutton->setPosition(CCPointMake(size.width*0.5+200, size.height *0.5-200));
    m_vTouchArray->addObject(gamebutton);
}

int  CEveryDayLoginLayer::getTouchindex(CCPoint point)
{
    for (int i=0; i<31; i++) {
        CCRect rect=getChildByTag(100+i)->boundingBox();
        rect.origin.x-=20;
        rect.origin.y-=20;
        rect.size.width-=20;
        rect.size.height-=20;
        if(rect.containsPoint(point))
        {
            return i;
        }
    }
    return -1;
}

ccColor3B  CEveryDayLoginLayer::getItemBackGroundColor(CEveryDayLoginDataStruct *item)
{
    /*
     {209,209,209},  //21每日签到灰色
     {0,255,0},      //22每日签到绿色
     {236,250,163},  //23每日签到黄色
     {255,255,255},  //24每日签到白色
     */
    if (item->IsSignIn()) { //绿色
        return g_custom_color[21];
    }
    else if(toDay==item->getDayInMonth() && !item->IsSignIn()) {
        return g_custom_color[22];//黄色
    } else if(toDay>item->getDayInMonth() && !item->IsSignIn())
    {
        return g_custom_color[20];
    }
    return g_custom_color[23];
    
}
void  CEveryDayLoginLayer::handTouchTag(int tag)
{
    switch (tag) {
        case TAG_QIANDAOBUTTON:
            //发送签到的数据
            if(getChildByTag(TAG_QIANDAOBUTTON)->isVisible())
            {
                sendQianDao();
            }
            break;
        case TAG_QUITBUTTON:
        {
            removeFromParentAndCleanup(true);
        }
        default:
            break;
    }
}
int  CEveryDayLoginLayer::checkTouchTag(CCPoint point)
{
    for (int i=0; i<m_vTouchArray->count(); i++) {
        if(((CGameButtonControl *)m_vTouchArray->objectAtIndex(i))->boundingBox().containsPoint(point))
        {
            return ((CGameButtonControl *)m_vTouchArray->objectAtIndex(i))->getTag();
        }
    }
    return -1;
}
bool CEveryDayLoginLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CEveryDayLoginLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CEveryDayLoginLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int itemIndex=0;
    if ((itemIndex=getTouchindex(pTouch->getLocation()))!=-1) {
        CCLog("itemIndex=%d",itemIndex);
        CEveryDayLoginRewardInfoLayer *pLayer=(CEveryDayLoginRewardInfoLayer *)(getChildByTag(TAG_ITEMINFO));
        pLayer->updateView(m_pQianDaoManager->getLoginDataStructByIndex(itemIndex));
        if (itemIndex!=toDay-1) {
            getChildByTag(TAG_QIANDAOBUTTON)->setVisible(false);
        }
        else{
            getChildByTag(TAG_QIANDAOBUTTON)->setVisible(true);
        }
    }else{
        handTouchTag(checkTouchTag(pTouch->getLocation()));
    }
}
void CEveryDayLoginLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CEveryDayLoginLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
void CEveryDayLoginLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}