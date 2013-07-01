//
//  LoginLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-5.
//
//

#include "HallScene.h"

#include "LoginScene.h"
#include "LogoLayer.h"
#include "SceneManager.h"
#include "Utility.h"
#include "PtJsonUtility.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"

// implement of the CLoginScene:

CCScene* CLoginScene::scene()
{
    CCScene *scene = CCScene::create();
   
    do {
       
        CLoginScene *login = CLoginScene::create();
        scene->addChild(login, LOGIN_ZORDER);
    } while (0);
    
    return scene;
}


CLoginScene::CLoginScene()
{
    
}
CLoginScene::~CLoginScene()
{
    maps->release();
}

bool CLoginScene::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        CC_BREAK_IF(!initLogin());
        bRet = true;
    } while (0);
    return bRet;
}

bool CLoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bLogoOver==false) {
        return false;
    }
   // CCLog("LoginLayer::ccTouchBegan");
    
    return handleTouchSpritePool(pTouch->getLocation());
}
void CLoginScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CLoginScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("LoginLayer::ccTouchEnded");
}
void CLoginScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


bool CLoginScene::handleTouchSpritePool(CCPoint point)
{
    bool bRet = false;
   
    int tag = -1;
    tag = TouchRect::SearchTouchTag(point, touchRect);
  
    switch (tag) {
        case -1:
            
            break;
        case BUTTON_PLAY_TAG:
            
//            CCDirector::sharedDirector()->replaceScene(
//                                                       CCTransitionFade::create(1.5f, CHallScene::scene())
//            );
              SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
            return true;
        default:
            break;
    }
    return bRet;
}


// implement of the CLoginScene's callback method:

void CLoginScene::loginCallBack(cocos2d::CCNode *pSender)
{
    // to do the call back 
}

void CLoginScene::setLogoOverCallBack()
{
    m_bLogoOver = true;
}

// implement ofthe  protected methods of CLoginScene:

bool CLoginScene::initLogin()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool bRet = false;
    do {
        // to do init
        // init variable
         m_bLogoOver = false;
        
        // add logo
        LogoLayer *logo = LogoLayer::create();
        this->addChild(logo, LOGO_ZORDER);;
        
       
        // note: remember release maps
        maps = LayoutLayer::create();
        maps->retain();
        maps->initWithFile(this, CSTR_FILEPTAH(plistPath, "login.plist"));
        
        
        CCNode* map = maps->getElementByTags("0");
        if (map)
        {
           CCSprite* bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "bg.png"));
           bg->setPosition(ccp(winSize.width/2, winSize.height/2));
            map->addChild(bg, -1);
        }

        
        // set layer touche enable
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
      
        maps->getTouchRects(touchRect);
        bRet = true;
        ADDHTTPREQUEST(SERVER_INF_ADR("dd"),"xianbeiTest0","menuCloseCallback0",callfuncO_selector(CLoginScene::msgCallback));
        
    } while (0);
    return bRet;
    
}

void CLoginScene::serverInf(CCDictionary* dic)
{
    CCDictElement* ele = NULL;
    STC_SERVER_INF inf;
    CCDICT_FOREACH(dic,ele)
    {
        CCDictionary* item = (CCDictionary*)ele->getObject();
        inf.m_nSid = item->valueForKey("sid")->intValue();
        inf.m_strName = item->valueForKey("name")->m_sString;
        inf.m_strIp = item->valueForKey("ip")->m_sString;
        inf.m_bIsUse = item->valueForKey("is_use")->intValue();
        vSerInf.push_back(inf);
    }
    
}

void CLoginScene::msgCallback(CCObject* obj)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "xianbeiTest0");
    CCDictionary* dic = PtJsonUtility::JsonStringParse((char*)obj);
    
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic,ele)
    {
        string key = ele->getStrKey();
        if (key == "result") {
            CCDictionary* childDic = (CCDictionary*)ele->getObject();
            serverInf(childDic);
        }
    }
    printf("xianbei msg:%s\n",(char*)obj);
    CCArray * array = CCArray::create();
    for (int i = 0; i < vSerInf.size(); i++)
    {
        CCLabelTTF * label = CCLabelTTF::create(vSerInf[i].m_strName.c_str(), "arial", 12.0f);
        CPtTableItem *item = CPtTableItem::create(CSTR_FILEPTAH(g_mapImagesPath, "HelloWorld.png"), CCRectMake(0, 0 , 100, 40));
        item->addChild(label);
        label->setPosition(ccp(0, 0));
        label->setAnchorPoint(CCPointZero);
        array->addObject(item);
       // array->addObject(label);
        //item->setTouchNode(label);
        
    }
    
    CPtListViewWidget * listServer = CPtListViewWidget::create(array, CCSizeMake(300, 60), kCCScrollViewDirectionHorizontal, CCSizeMake(2, 5), 1);
    this->addChild(listServer);
    listServer->setAnchorPoint(CCPointZero);
    listServer->setPosition(ccp(30, 40));
    listServer->setBackGround(CCLayerColor::create(ccc4(125, 0, 0, 125)));
    
}




