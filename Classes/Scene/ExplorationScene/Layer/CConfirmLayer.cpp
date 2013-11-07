//
//  CConfirmLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-14.
//
//

#include "CConfirmLayer.h"
#include "gameConfig.h"
#include "AsgardLayer.h"
#include "ExplorationScene.h"
#include "SceneManager.h"
#include "PtJsonUtility.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "gamePlayer.h"

CConfirmLayer::CConfirmLayer()
{
    
}

CConfirmLayer::~CConfirmLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CConfirmLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125,125,125,125)));
        
        initConfirm();
        bRet  = true;
    } while (0);
    return  bRet;
}

bool CConfirmLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CConfirmLayer::ccTouchBegan");

    return true;
}
void CConfirmLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CConfirmLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    handlerTouch();
}
void CConfirmLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CConfirmLayer::initConfirm()
{
    // init data:
    m_nTouchTag  = -1;
 
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    
    m_cMaps->getElementByTags("2,1")->setVisible(false);
    m_cMaps->getElementByTags("2,2")->setVisible(false);
    
   }


void CConfirmLayer::handlerTouch()
{
    
 
        switch (m_nTouchTag)
        {
            case 2002:
                onClickFight();
                break;
            default:
                break;
        }

    
}

void CConfirmLayer::onClickFight()
{
    onSendRequest();
}
// url-> /api.php?m=Fight&a=setTeam&uid=194&sig=2ac2b1e302c46976beaab20a68ef95&team=1
void CConfirmLayer::onSendRequest()
{
    char buffer[300] = {0};
    int teamId = 1;
    sprintf(buffer, "&sig=%s&team=%d",STR_USER_SIG, teamId);
    CCLog("the send :%s", buffer);
    
    ADDHTTPREQUESTPOSTDATA(STR_URL_SETFIGHTTEAM(196),"CALLBACK_CConfirmLayer::onSendRequest", "REQUEST_CConfirmLayer::onSendRequest",buffer, callfuncO_selector(CConfirmLayer::onReceiveResponse));
    
}
void CConfirmLayer::onReceiveResponse(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this
                                                                     , "CALLBACK_CConfirmLayer::onSendRequest");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCLog("the receive data: %s", buffer);
        CCDictionary *dict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (dict)
        {
            CCString *tmp = (CCString*)dict->objectForKey("code");
            CCAssert(tmp, "no code ");
            int code = tmp->intValue();
            if (code == 0)
            {
                onHandlerSuccess((CCDictionary*)dict->objectForKey("result"));
            }else
            {
                CCMessageBox(CCString::createWithFormat("the error code : %d", code)->getCString(), "setTeam Error");
            }
        }
    }
}
void CConfirmLayer::onHandlerSuccess(CCDictionary *inResult)
{
    if(inResult)
    {
        CCString *tmp = (CCString*) inResult->objectForKey("info");
        if (tmp)
        {
            int info = tmp->intValue();
            if (info == 1)
            {
                // set success
                setFightTeamBuffer(inResult->objectForKey("chapter_buff"));
                
                CCArray *inRandomData = (CCArray*) inResult->objectForKey("random_data");
                SinglePlayer::instance()->parseRandomData(inRandomData);
                
                SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_FIGHTSCENE);
                removeFromParentAndCleanup(true);
                
            }else
            {
                CCMessageBox(CCString::createWithFormat("set fightTeam error: %d", info)->getCString(), "Fight Start Error");
            }
        }
    }
}

void CConfirmLayer::setFightTeamBuffer(CCObject *inBuffers)
{
    SinglePlayer::instance()->m_vPlayerBufferList.clear();
    if (!inBuffers) {
        return ;
    }
    std::string typeName = typeid(*inBuffers).name();
    if (typeName.find("CCDictionary") != std::string::npos)
    {
        CCDictionary *dict =(CCDictionary*) inBuffers;
        CCDictElement *element = NULL;
        CCDictionary *tmpDict = NULL;
        CCDICT_FOREACH(dict, element)
        {
            tmpDict =(CCDictionary*) element->getObject();
            int objectId = GameTools::intForKey("object_id", tmpDict);
            CCLog("objectid Value:%d",objectId);
            if (objectId != 0)
            {
                SinglePlayer::instance()->m_vPlayerBufferList.push_back(objectId);
            }
           
        }
    }
    
}