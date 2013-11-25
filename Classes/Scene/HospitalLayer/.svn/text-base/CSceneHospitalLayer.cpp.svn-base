//
//  CSceneHospitalLayer.cpp
//  91.cube
//
//  Created by linminglu on 11/14/13.
//
//

#include "CSceneHospitalLayer.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "Utility.h"
#include "CReward.h"

#define TAG_QUITBUTTON 12
#define TAG_GETGPBUTTON 11


CSceneHospitalLayer::CSceneHospitalLayer()
{
    size=CCDirector::sharedDirector()->getWinSize();
    m_tempGamePlayer=SinglePlayer::instance();
}

CSceneHospitalLayer::~CSceneHospitalLayer()
{
    
}

bool CSceneHospitalLayer::init()
{
    if (CCLayer::init()) {
        createBackGroud();
        createTitle();
        createButton();
        createQuitButton();
        createContext();
        sendHttpToAskTimer();
        setTouchPriority(-3);
        setTouchEnabled(true);
        isCanTouch=false;
    }
    return true;
}

void CSceneHospitalLayer::createBackGroud()
{
    CCSprite *bgSprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "hospital/yiwushibeijing.png"));
    addChild(bgSprite,1,0);
    bgSprite->setPosition(ccp(size.width*0.5 ,size.height *0.5));

}

void CSceneHospitalLayer::updateContext()
{
    CCLabelTTF *label=(CCLabelTTF *)getChildByTag(10);
    if (m_tempGamePlayer->getPlayerGp()==m_tempGamePlayer->getGpMax())
    {
        label->setString("你还很健康呢？不需要我吧~");
    }
    else
    {
        label->setString("你怎么又受伤了？让我来爱抚你吧~");
    }
}


void CSceneHospitalLayer::createContext()
{
    CCLabelTTF *label=CCLabelTTF::create("", "Arial", 15, CCSizeMake(200, 200), kCCTextAlignmentLeft);
    label->setPosition(ccp(size.width *0.5, size.height*0.5-150));
    label->setColor(g_custom_color[14]);
    label->setAnchorPoint(CCPointZero);
    addChild(label,2,10);
    label->setColor(ccc3(255, 0, 0));
    updateContext();
}


void CSceneHospitalLayer::createTitle()
{
    string word = Utility::getWordWithFile("word.plist", "yiwushi");
    CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 45);
    label->setPosition(ccp(size.width *0.5, size.height*0.5+200));
    label->setColor(g_custom_color[14]);
    addChild(label,2,1);
}

void CSceneHospitalLayer::createPeople()
{
    
}

void CSceneHospitalLayer::createButton()
{
    CCSprite* pHospital= CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "hospital/shijiananniu.png"));
    pHospital->setPosition(ccp(size.width *0.5, 200));
    addChild(pHospital, 200, TAG_GETGPBUTTON);
    CCLabelTTF *label=CCLabelTTF::create("", "Arial", 20);
    label->setPosition(ccp(pHospital->getContentSize().width*0.5, pHospital->getContentSize().height *0.5));
    pHospital->addChild(label,2,999);
}

void CSceneHospitalLayer::updateHospitolData(float t)
{
    //这种调用方法比较偏门， 这个地方的 成员就是调用者的指针
    CCLabelTTF *labelttf=(CCLabelTTF *)(getChildByTag(TAG_GETGPBUTTON)->getChildByTag(999));
    if(labelttf)
    {

        if (m_vYiWuShiShiJian>0)
        {
            m_vYiWuShiShiJian--;
            labelttf->setString(CPtTool::stringForObjectValue(m_vYiWuShiShiJian));
        }
        else
        {
            unschedule(schedule_selector(CSceneHospitalLayer::updateHospitolData));
            if (!m_bCanUseHostpital)
            {
                sendHttpToAskTimer();
            }
        }
    }

}

void CSceneHospitalLayer::createQuitButton()
{
    CCSprite *pSprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "hospital/tuichuanniu.png"));
    addChild(pSprite,1,TAG_QUITBUTTON);
    pSprite->setPosition(ccp(size.width *0.5+300, size.height *0.5+220));
                                
}



void CSceneHospitalLayer::sendHttpToAskTimer(){
    isCanTouch=false;
    string connectData="sig=";
    connectData += m_tempGamePlayer->getUserSig();
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_GPLEVELTIME(194), "CALLBACK_CGamePlayer_sendUpdataHospital", "REQUEST_CGamePlayer_sendUpdataHospital",connectData.c_str(),callfuncO_selector(CSceneHospitalLayer::decodeSubTimer));
}
void CSceneHospitalLayer::decodeSubTimer(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_sendUpdataHospital");
    if(!object)
    {
        CCMessageBox("fuck 服务端 传递医务室数据的数据是NULL的", "error");
        CCNotificationCenter::sharedNotificationCenter()->postNotification("Layer_CAsgardLayer_createHospitolButton");
        return;
    }
    char *strdata=(char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    strdata=NULL;
    if (GameTools::intForKey("code", dict)!=0) {
        
        CCLog("后台有错误");
        CCNotificationCenter::sharedNotificationCenter()->postNotification("Layer_CAsgardLayer_createHospitolButton");
        m_vYiWuShiShiJian=10800;
        m_bCanUseHostpital=false;
        schedule(schedule_selector(CSceneHospitalLayer::updateHospitolData), 1.0);
        return ;
    }
    CCDictionary *result=(CCDictionary *)dict->objectForKey("result");
    if(result)
    {
        m_vYiWuShiShiJian=GameTools::intForKey("over_time", result);
        if (m_vYiWuShiShiJian!=0) {
            m_bCanUseHostpital=false;
            schedule(schedule_selector(CSceneHospitalLayer::updateHospitolData), 1.0);
        }
        else{
            m_bCanUseHostpital=true;
            string word=Utility::getWordWithFile("word.plist", "dayizhen");
            CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(TAG_GETGPBUTTON)->getChildByTag(999);
            if(labelttf)
            {
                labelttf->setString(word.c_str());
            }
            updateContext();
        }
        isCanTouch=true;
    }
}



void CSceneHospitalLayer::setAddGpText(string str)
{
    CCLabelTTF *labelTTF=(CCLabelTTF *)getChildByTag(10);
    if (labelTTF) {
        labelTTF->setString(str.c_str());
    }
    
}

void CSceneHospitalLayer::sendToserverToAddGp()
{
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_HOSPITALACTITY(194), "CALLBACK_CAsgardLayer_sendToserverToAddGp", "REQUEST_CAsgardLayer_sendToserverToAddGp",postStrdata.c_str(),callfuncO_selector(CSceneHospitalLayer::decodeServerAddGp));
}

void CSceneHospitalLayer::decodeServerAddGp(CCObject *object)
{
    isCanTouch=true;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CAsgardLayer_sendToserverToAddGp");
    char *resualtData=(char *)object;
    if (!resualtData) {
        CCMessageBox("小护士返回数据为空", "错误");
        return ;
    }
    CCDictionary *dictReuslt=PtJsonUtility::JsonStringParse(resualtData);
    delete [] resualtData;
    resualtData=NULL;
    if (dictReuslt)
    {
        if (GameTools::intForKey("code",dictReuslt)!=0)
        {
            CCMessageBox("code错误", "error");
            return ;
        }
        int reStatValue=((CCString *)((CCDictionary *)dictReuslt->objectForKey("result"))->objectForKey("over_time"))->intValue();
        if (reStatValue>0)
        {
            this->m_vYiWuShiShiJian=reStatValue;
            m_bCanUseHostpital=false;
            CCLabelTTF *labelttf=(CCLabelTTF *)(getChildByTag(TAG_GETGPBUTTON)->getChildByTag(999));
            if(labelttf)
            {
                labelttf->setString(ConvertToString(reStatValue).c_str());
            }
            schedule(schedule_selector(CSceneHospitalLayer::updateHospitolData), 1.0);
        }
        if (dictReuslt)
        {
            CCDictionary * resultDict=(CCDictionary*)(dictReuslt->objectForKey("result"));
            CCDictionary * rewardDict = (CCDictionary*)((CCDictionary *)resultDict->objectForKey("reward"))->objectForKey("add");
            if (dictReuslt)
            {
                rewardDict =(CCDictionary*) rewardDict->objectForKey("clinic");
                CReward * reward = CReward::create(rewardDict);
                if (reward)
                {
                    reward->excuteReward(ADD);
                }
                string value="我已爱抚了你，感觉如何？";
                setAddGpText(value);
            }
        }
    }
}

bool CSceneHospitalLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CSceneHospitalLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSceneHospitalLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isCanTouch)
    {
        return ;
    }
        CCSprite  *node=(CCSprite *)getChildByTag(TAG_QUITBUTTON);
        if (node && node->boundingBox().containsPoint(pTouch->getLocation()))
        {   Utility::handleBtnCallBack(node, this, NULL);
             PtSoundTool::playSysSoundEffect("UI_click.wav");
            removeFromParentAndCleanup(true);
        }
        else
        {
            CCSprite  *node=(CCSprite *)getChildByTag(TAG_GETGPBUTTON);
            if (node && node->boundingBox().containsPoint(pTouch->getLocation())) {
            if (m_tempGamePlayer->getPlayerGp()<m_tempGamePlayer->getGpMax() && m_bCanUseHostpital) {
                 Utility::handleBtnCallBack(node, this, NULL);
                 PtSoundTool::playSysSoundEffect("UI_click.wav");
                sendToserverToAddGp();
            }
            else if(m_tempGamePlayer->getPlayerGp()==m_tempGamePlayer->getGpMax())
            {
                setAddGpText("你还很健康呢？不需要我吧~");
            }
        }
    }
    
}

void CSceneHospitalLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSceneHospitalLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CSceneHospitalLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}