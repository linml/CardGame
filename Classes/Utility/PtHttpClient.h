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

typedef struct STC_HTTPINF {
    const char* m_pchURL;
    const char* m_pSelector;
    const char* m_pchTag;
    CCHttpRequest::HttpRequestType m_RequestType;
} stcRequestInf;


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
