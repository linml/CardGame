//
//  CDrawCardLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-18.
//
//

#include "CDrawCardLayer.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "gameTools.h"


CDrawCardLayer::CDrawCardLayer()
{
    m_nDiamondTimer=5;
    m_nFriendTimer=5;
  
}

CDrawCardLayer::~CDrawCardLayer()
{
    
}

bool CDrawCardLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    CCLayerColor *layerColor=CCLayerColor::create(ccc4(255, 255, 0, 125), 1024, 768);
    addChild(layerColor);
    setTouchEnabled(true);
    //建立两个
    setTouchPriority(-9);
    scheduleOnce(schedule_selector(CDrawCardLayer::initSendGetData), 0.5);
    return true;
}


void CDrawCardLayer::decodeParseFriendDraw(CCObject *object)
{
    decodeGetData(object,EN_NIUDANTYPE_FRIEND);
}

void CDrawCardLayer::decodeParseCashDraw(CCObject *object)
{
    decodeGetData(object,EN_NIUDANTYPE_CASH);

}

void CDrawCardLayer::sendHttpToFriendGetValue(float t)
{
    if(!sendGetDataHttp(EN_NIUDANTYPE_FRIEND))
    {
        scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToFriendGetValue), m_stempScanData[0].m_nNextTimer);
    }
}

void CDrawCardLayer::sendHttpToCashGetValue(float t)
{
    if(!sendGetDataHttp(EN_NIUDANTYPE_CASH))
    {
        scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToCashGetValue), m_stempScanData[1].m_nNextTimer);
    }
}

void CDrawCardLayer::initSendGetData(float t)
{
//开始读取服务器的 友情点数时间和 钻石 各免费时间
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), "CALLBACK_CDrawCardLayer_initSendGetData", "REQUEST_CSceneGameShopLayer_getLimitItems",postStrdata.c_str(),callfuncO_selector(CDrawCardLayer::decodeInitEnterDrawCardLayer));
}

bool CDrawCardLayer::getScanData(const char *value,CCDictionary *dict,ScanDrawValue &rebackValue)
{
    CCDictionary *StrcutValue=(CCDictionary *)dict->objectForKey(value);
    if(StrcutValue)
    {
        rebackValue.m_nCount=GameTools::intForKey("temdata", StrcutValue);
        rebackValue.m_nNextTimer=GameTools::intForKey("leave_time", StrcutValue);
        return true;
    }
    return false;
    
}

void CDrawCardLayer::decodeInitEnterDrawCardLayer(CCObject *object)
{
    
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CDrawCardLayer_initSendGetData");
    if(!object)
    {
        CCMessageBox("服务端发送数据为空", "服务端");
        return;
    }
    char *strdata=(char *)object;
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut ==0)
    {
        string teamStrType;
        CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
        if(resualt)
        {

            if(!getScanData("friendly",resualt,m_stempScanData[0]))
            {
                CCMessageBox("返回没有friendly信息 服务端错误", "服务端错误");
                
            }
            if (m_stempScanData[0].m_nNextTimer>0) {
                scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToFriendGetValue), m_stempScanData[0].m_nNextTimer);//,kCCRepeatForever, m_stempScanData[0].m_nNextTimer);
            }

            if(!getScanData("cash", resualt, m_stempScanData[1]))
            {
                CCMessageBox("返回没有cash抽卡信息 服务端错误", "服务端错误");
            }
            if(m_stempScanData[1].m_nNextTimer>0)
            {
                scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToCashGetValue), m_stempScanData[1].m_nNextTimer);//,kCCRepeatForever, m_stempScanData[0].m_nNextTimer);
            }
            
        }
    }
}

bool CDrawCardLayer::sendGetDataHttp(EN_NIUDANTYPE type)
{
    ScanDrawValue *pData=NULL;
    string callBackStr;
    SEL_CallFuncO callBackFunc;
    if (type==EN_NIUDANTYPE_FRIEND) {
        pData=&m_stempScanData[0];
        callBackStr="CALLBACK_CDrawCardLayer_sendHttpToFriendGetValue";
        callBackFunc=callfuncO_selector(CDrawCardLayer::decodeParseFriendDraw);
    }
    else if(type==EN_NIUDANTYPE_CASH)
    {
        pData=&m_stempScanData[1];
        callBackStr="CALLBACK_CDrawCardLayer_sendHttpToCashGetValue";
        callBackFunc=callfuncO_selector(CDrawCardLayer::decodeParseCashDraw);
    }else{
        return  false;
    }
        CCLog("sendHttpToFriendGetValue %d",m_stempScanData[0].m_nCount);
        //开始读取服务器的 友情点数时间和 钻石 各免费时间
        string postStrdata="sig=";
        postStrdata+=SinglePlayer::instance()->getUserSig();
        ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), callBackStr.c_str(), "REQUEST_CDrawCardLayer_NiuDan",postStrdata.c_str(),callBackFunc);
    return true;
}

void CDrawCardLayer::decodeGetData(cocos2d::CCObject *object, EN_NIUDANTYPE type)
{
    ScanDrawValue *pData;
    string callStr;
    string valueStr;
    SEL_SCHEDULE schFunction;
    if (type==EN_NIUDANTYPE_FRIEND) {
        pData=&m_stempScanData[0];
        callStr="CALLBACK_CDrawCardLayer_sendHttpToFriendGetValue";
        valueStr="friendly";
        schFunction=schedule_selector(CDrawCardLayer::sendHttpToFriendGetValue);
    }
    else if(type==EN_NIUDANTYPE_CASH)
    {
        pData=&m_stempScanData[1];
        callStr="CALLBACK_CDrawCardLayer_sendHttpToCashGetValue";
        valueStr="Cash";
        schFunction=schedule_selector(CDrawCardLayer::sendHttpToCashGetValue);
    }
    else
        return;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,callStr.c_str());
    if (!object)
    {
        CCMessageBox("请求服务器扭蛋次数的数据为NULL", "错误");
        return;
    }
    else
    {
        char *strdata=(char *)object;
        CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
        delete []strdata;
        strdata=NULL;
        int codeReslut=GameTools::intForKey("code", dict);
        if(codeReslut ==0)
        {
            string teamStrType;
            CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
            if(resualt)
            {
                
                if(!getScanData(valueStr.c_str(),resualt,*pData))
                {
                    CCMessageBox("返回没有扭蛋信息 服务端错误", "服务端错误");
                }
            }
        }
        if(pData->m_nNextTimer>0)
        {
            scheduleOnce(schFunction, pData->m_nNextTimer);
        }
    }
   
}
                           
void CDrawCardLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,getTouchPriority(), true);
}

void CDrawCardLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CDrawCardLayer::updateFriendTimer(float t)
{

    m_nFriendTimer--;
    
}

void CDrawCardLayer::updateDiamondTimer(float t)
{
    m_nDiamondTimer--;
}

bool CDrawCardLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
