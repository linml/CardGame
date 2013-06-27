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
#include <queue>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
//m=Platform&a=selectServer&puid=194&sig=2ac2b1e302c46976beaab20a68ef95
#define SERVER_INF_ADR(JSON) "http://cube.games.com/cube_test/trunk/web/gateway.php?m=Platform&a=selectServer&puid=194&sig=2ac2b1e302c46976beaab20a68ef95"

#define SERVER_SID "sid"
#define SERVER_NAME "name"
#define SERVER_IP "ip"
#define SERVER_ISUSE "is_use"

#define ADDHTTPREQUEST(URL,NOTIFICATIONTAG,HTTPREQUESTTAG,CALLBACK)\
{\
    stcRequestInf inf;\
    inf.m_pchURL = URL; \
    cout<<inf.m_pchURL<<endl; \
    inf.m_RequestType=CCHttpRequest::kHttpPost;\
    inf.m_pSelector = NOTIFICATIONTAG;\
    inf.m_pchTag = HTTPREQUESTTAG;\
    CPtHttpClient::sharePtHttpClient()->addRequest(inf);\
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,CALLBACK, inf.m_pSelector, NULL);\
}

typedef struct STC_HTTPINF {
    const char* m_pchURL;
    const char* m_pSelector;
    const char* m_pchTag;
    string jsonstr;
    CCHttpRequest::HttpRequestType m_RequestType;
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
    
    void addRequest(const char* pchUrl, const char* pchSelector, const char* pchTag,CCHttpRequest::HttpRequestType requestType = CCHttpRequest::kHttpGet);
    
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
