//
//  PtHttpClient.h
//  91.cube
//
//  Created by xianbei1987 on 13-6-5.
//
//

#ifndef _1_cube_PtHttpClient_h
#define _1_cube_PtHttpClient_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "PtHttpURL.h"
#include <queue>
#include "CPanelGameLog.h"
#include "CPanelLoadingLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;


#define SERVER_SID "sid"
#define SERVER_NAME "name"
#define SERVER_IP "ip"
#define SERVER_ISUSE "is_use"

//需要在CALLBACK的函数里面删除传递的参数。 库函数使用malloc 后传递给回调函数 但没做删除处理 必须使用者自己来

#define ADDHTTPREQUEST(URL,NOTIFICATIONTAG,HTTPREQUESTTAG,CALLBACK)\
{\
    stcRequestInf inf;\
    inf.m_pchURL = URL; \
    cout<<inf.m_pchURL<<endl; \
    inf.m_RequestType=CCHttpRequest::kHttpPost;\
    inf.m_pSelector = NOTIFICATIONTAG;\
    inf.m_pchTag = HTTPREQUESTTAG;\
    inf.m_pchData = NULL;\
    CPtHttpClient::sharePtHttpClient()->addRequest(inf);\
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,CALLBACK, inf.m_pSelector, NULL);\
}

#define ADDHTTPREQUESTPOSTDATA(URL,NOTIFICATIONTAG,HTTPREQUESTTAG,__POSTSTR__,CALLBACK)\
{\
CCScene *pScene=CCDirector::sharedDirector()->getRunningScene(); \
CCLayer *pLayer=CPanelLoadingLayer::create(); \
pLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width *0.5,    CCDirector::sharedDirector()->getWinSize().height*0.5)); \
if(pScene) \
    pScene->addChild(pLayer,7,8888888); \
stcRequestInf inf; \
inf.m_pchURL = URL; \
cout<<inf.m_pchURL<<endl; \
inf.m_RequestType=CCHttpRequest::kHttpPost;\
inf.m_pSelector = NOTIFICATIONTAG;\
inf.m_pchTag = HTTPREQUESTTAG;\
inf.m_pchData = __POSTSTR__;\
appendFileLog(URL); \
appendFileLog(__POSTSTR__); \
 \
CPtHttpClient::sharePtHttpClient()->addRequest(inf);\
CCNotificationCenter::sharedNotificationCenter()->addObserver(this,CALLBACK, inf.m_pSelector.c_str(), NULL);\
}




#define ADDHTTPREQUESTPOSTDATABYOWNCCCLASS(URL,NOTIFICATIONTAG,HTTPREQUESTTAG,__POSTSTR__,__CCOBJECTPOINT__,CALLBACK)\
{\
CCScene *pScene=CCDirector::sharedDirector()->getRunningScene(); \
CCLayer *pLayer=CPanelLoadingLayer::create(); \
pLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width *0.5,    CCDirector::sharedDirector()->getWinSize().height*0.5)); \
if(pScene) \
pScene->addChild(pLayer,7,8888888); \
stcRequestInf inf;\
inf.m_pchURL = URL; \
cout<<inf.m_pchURL<<endl; \
inf.m_RequestType=CCHttpRequest::kHttpPost;\
inf.m_pSelector = NOTIFICATIONTAG;\
inf.m_pchTag = HTTPREQUESTTAG;\
inf.m_pchData = __POSTSTR__;\
appendFileLog(URL); \
appendFileLog(__POSTSTR__); \
CPtHttpClient::sharePtHttpClient()->addRequest(inf);\
CCNotificationCenter::sharedNotificationCenter()->addObserver(__CCOBJECTPOINT__,CALLBACK, inf.m_pSelector.c_str(), NULL);\
}



typedef struct STC_HTTPINF {
    string m_pchURL;
    string m_pSelector;
    string m_pchTag;
    CCHttpRequest::HttpRequestType m_RequestType;
    string m_pchData;
} stcRequestInf;

typedef struct STC_SERVER_INF
{
    int m_nSid;
    string m_strName;
    string m_strIp;
    bool m_bIsUse;
} stcServerInf;


class CPtHttpClient : public CCObject{
public:
    CPtHttpClient();
    
    ~CPtHttpClient();
    static CPtHttpClient* sharePtHttpClient();
    
    void addRequest(stcRequestInf tInf);
    
    void addRequest(const char* pchUrl, const char* pchSelector, const char* pchTag,const char* pchData = NULL,CCHttpRequest::HttpRequestType requestType = CCHttpRequest::kHttpPost);
    
    int getRequestQueueSize();
    
private:
    
    void send(stcRequestInf tInf);
    
    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
    
    void notificationMsgRecevice(CCObject* obj);
    
private:
    queue<stcRequestInf> m_qRequestInf;
    
    bool m_bIsSending;
};

#endif
