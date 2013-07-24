//
//  PtHttpClient.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-6-5.
//
//

#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "support/CCNotificationCenter.h"

#define MSG_HTTPCLIENT "msgHttpClient"

static CPtHttpClient *s_pPtHttpClient = NULL;

CPtHttpClient::CPtHttpClient()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CPtHttpClient::notificationMsgRecevice), MSG_HTTPCLIENT, NULL);
    m_bIsSending=false;
}

CPtHttpClient::~CPtHttpClient()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_HTTPCLIENT);
    CCHttpClient::getInstance()->destroyInstance();
}

CPtHttpClient* CPtHttpClient::sharePtHttpClient()
{
    if (s_pPtHttpClient == NULL)
    {
        s_pPtHttpClient = new CPtHttpClient();
    }
    
    return s_pPtHttpClient;
}

void CPtHttpClient::send(stcRequestInf tInf)
{
    printf("send %s\n",tInf.m_pchURL);
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(tInf.m_pchURL);
    request->setRequestType(tInf.m_RequestType);
    request->setResponseCallback(this, callfuncND_selector(CPtHttpClient::onHttpRequestCompleted));
    if (tInf.m_RequestType ==  CCHttpRequest::kHttpPost && tInf.m_pchData) {
        printf("post data:%s\n",tInf.m_pchData);
        request->setRequestData(tInf.m_pchData, strlen(tInf.m_pchData));
    }
    request->setTag(tInf.m_pchTag);
    CCHttpClient::getInstance()->send(request);
    request->release();
    m_bIsSending = true ;

}

void CPtHttpClient::addRequest(stcRequestInf tInf)
{
    printf("addRequest %s\n",tInf.m_pchTag);
    m_qRequestInf.push(tInf);
    if(!m_bIsSending)
    {
        send(m_qRequestInf.front());
        printf("addRequest send\n");
    }
}

void CPtHttpClient::addRequest(const char* pchUrl, const char* pchSelector, const char* pchTag, const char* pchData ,CCHttpRequest::HttpRequestType requestType)
{
    //stcRequestInf inf = {pchUrl,pchSelector,pchTag,requestType,pchData};
    //addRequest(inf);
}

void CPtHttpClient::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    const char* pchReciveTag = response->getHttpRequest()->getTag();
    if (0 != strlen(pchReciveTag) && !strcmp(m_qRequestInf.front().m_pchTag, pchReciveTag))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
        m_bIsSending = false;
        
    }
    
//    int statusCode = response->getResponseCode();
//    char statusString[64] = {};
//    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
//    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char* pchData=new char[(*buffer).size()+1];
    memset(pchData, 0, (*buffer).size()+1);
    //printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        pchData[i]=(*buffer)[i];
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_HTTPCLIENT, (CCObject*)pchData);    
}

void CPtHttpClient::notificationMsgRecevice(CCObject* obj)
{
//    PtJsonUtility::JsonStringParse((char*)obj);
    stcRequestInf inf = m_qRequestInf.front();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(inf.m_pSelector, (CCObject*)obj);
    m_qRequestInf.pop();
    if(!m_qRequestInf.empty())
    {
        send(m_qRequestInf.front());
        printf("notificationMsgRecevice send\n");
    }
}

int CPtHttpClient::getRequestQueueSize()
{
    return m_qRequestInf.size();
}
