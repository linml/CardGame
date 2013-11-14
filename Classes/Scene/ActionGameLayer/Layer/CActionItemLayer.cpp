//
//  CActionItemLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-30.
//
//

#include "CActionItemLayer.h"
#include "CStructGameActionData.h"
#include "Utility.h"
#include "CPtPropConfigData.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "CPtTool.h"
#include "CStructGameActionData.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CGameArrageBackpackTip.h"
#include "CReward.h"
#include "SceneManager.h"
#include "HallScene.h"
#include "CPtRecharge.h"
#include "CActivityEncounterManager.h"


CActionItemLayer::CActionItemLayer()
{
    size=CCDirector::sharedDirector()->getWinSize();
    height=380.0f;
    m_pSingleButton = NULL;
    m_nRewardButtonCount = 0;
    m_pRewardButtons = NULL;
    m_nTouchTag = -1;
}
CActionItemLayer::~CActionItemLayer()
{
    CC_SAFE_DELETE_ARRAY(m_pRewardButtons);
}

CActionItemLayer *CActionItemLayer::Created(CStructGameActionData *data)
{
    CActionItemLayer *layer=new CActionItemLayer();
    if (!layer || !layer->initCreate(data)) {
        delete layer;
        layer=NULL;
        return NULL;
    }
    layer->autorelease();
    return layer;
}


bool CActionItemLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return  handlerTouchEvent(pTouch);

}

void CActionItemLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

    if (m_pData)
    {
        if(m_nTouchTag != -1)
        {
            recoverButtons(pTouch);
            switch (m_pData->getActionShowType())
            {
                case 1 :
                    handlerGetReward();
                    break;
                case 2:
                    handlerGoRecharge();
                    break;
                case 3:
                    handlerGoEncounter();
                    break;
                case 4:
                    break;
                default:
                    break;
            }

        }
       
    }
}

bool CActionItemLayer::handlerTouchEvent(cocos2d::CCTouch *pTouch)
{
    bool bRet = false;
    if (m_pData)
    {
        /**
         * switch (data->getActionShowType()) {
         case 1:
         //领奖
         createTips(data);
         break;
         case 2:
         //充值 //做一个充值界面
         createPng(data->getActionPic());
         createChongZhi(data);
         break;
         case 3:
         //创建副本 //需要添加的内容
         createPng(data->getActionPic());
         createHuben(data);
         break;
         case 4:
         //说明 什么都不加
         break;
         default:
         break;
         }

         */
        switch (m_pData->getActionShowType())
        {
            case 1 :
                bRet = handlerMultiButtons(pTouch);
                break;
            case 2:
            case 3:
                bRet = handlerSingleButton(pTouch);
                break;
            case 4:
                break;
            default:
                break;
        }
    
    }
    return bRet;
}

bool CActionItemLayer::handlerSingleButton(CCTouch *pTouch)
{
    bool bRet = false;
    m_nTouchTag = isTouchEvent(pTouch, 3);
    if (m_nTouchTag != -1)
    {
        m_pSingleButton->selected();
        bRet = true;
        
    }
    return bRet;
}

bool CActionItemLayer::handlerMultiButtons(CCTouch *pTouch)
{
    bool bRet =false;
    m_nTouchTag = isTouchEvent(pTouch, 1);
    if (m_nTouchTag != -1)
    {
        m_pRewardButtons[m_nTouchTag]->selected();
        bRet = true;
    }
    return bRet ;

}

void CActionItemLayer::recoverButtons(CCTouch *pTouch)
{
    if(m_nTouchTag != -1)
    {
        if (m_nTouchTag == 10)
        {
            m_pSingleButton->unselected();
        }else if(m_nTouchTag >= 0 && m_nTouchTag < m_nRewardButtonCount)
        {
            if (m_pRewardButtons[m_nTouchTag])
            {
                m_pRewardButtons[m_nTouchTag]->unselected();
            }
  
        }
    }
}

void CActionItemLayer::handlerGoEncounter()
{
    CCLog("CActionItemLayer::handlerGoEncounter");
    if(!SingleActivityEncounterManager::instance()->haveEncounterBySectionId(m_pData->getActionPartId()))
    {
        CCMessageBox("该活动今天不开放", "");
        return;
    }
    onClickGoEncounter();
    
}
void CActionItemLayer::handlerGetReward()
{
    if(m_nTouchTag >= 0 && m_nTouchTag < m_nRewardButtonCount)
    {
       Present *present =(Present*)m_pData->getAllRewardItems()->objectAtIndex(m_nTouchTag);
        if (present && present->getHasGet() == false)
        {
            onClickGetItems(present->getItemId());
        }else
        {
            CCMessageBox("该道具已领取", "CActionItemLayer");
        }
    }
   
}
void CActionItemLayer::handlerGoRecharge()
{

    onClickGoRecharge();

}

/*
 * @return -1: no touch 0-2 --> getReward 10 -->(go encounter or go recharge)
 */
int CActionItemLayer::isTouchEvent(CCTouch *pTouch , int inType)
{
    int nRet = -1;
    if (inType == 1)
    {
        for (int  i = 0; i <  m_nRewardButtonCount; i++)
        {
           if(CPtTool::isInNode(m_pRewardButtons[i], pTouch))
           {
               nRet = i;
               break;
           }
        }
    }else
    {
        if (CPtTool::isInNode(m_pSingleButton, pTouch))
        {
            nRet = 10;
        }
    }
    return nRet;
}

bool  CActionItemLayer::initCreate(CStructGameActionData *data)
{
    if (!CCLayer::init()) {
        return false;
    }
    m_pData=data;
    createHead(data->getActionShowTime());
    createContext(data->getContent());
    switch (data->getActionShowType()) {
        case 1:
            //领奖
            createTips(data);
            break;
        case 2:
            //充值 //做一个充值界面
            createPng(data->getActionPic());
            createChongZhi(data);
            break;
        case 3:
            //创建副本 //需要添加的内容
            createPng(data->getActionPic());
            createFuben(data);
            break;
        case 4:
            //说明 什么都不加
            break;
        default:
            break;
    }
    if (height<0) {
        height=400;
    }
    else{
        height=380-height;
    }
    setContentSize(CCSizeMake(400,height ));
    return true;
}


void CActionItemLayer::onClickGoEncounter()
{
    CHallScene* hallScene = (CHallScene*)  CCDirector::sharedDirector()->getRunningScene()->getChildByTag(100);
    hallScene->getAsgardLayer()->createBiforestLayer(ACTIVITYEXPLORATION, m_pData->getActionPartId());
 
}
void CActionItemLayer::onClickGoRecharge()
{
    CPtRecharge *layer = CPtRecharge::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 1000);
}
void CActionItemLayer::onClickGetItems(int inItemId)
{
    //
    int result =   SinglePlayer::instance()->getPropMaxCountAddToBag(inItemId);
    if (result > 0)
    {
        onSendGetItemRequest(inItemId);
    }
    else if(result == 0)
    {
        CGameArrageBackpackTip *layer = CGameArrageBackpackTip::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 1000);
        return;
        
    }
    else
    {
        CCMessageBox("不入包的", "CActionItemLayer: error");
    }
}

//eg. cube.games.com/api.php?m=Activity&a=getActivityItem&uid=194&sig=2ac2b1e302c46976beaab20a68ef95&board_id=1&item_id=10001
void CActionItemLayer::onSendGetItemRequest(int inItemId)
{
    char buffer[200] = {0};
    sprintf(buffer,"&sig=%s&board_id=%d&item_id=%d", STR_USER_SIG, m_pData->getActionID(), inItemId);
    CCLog("the buffer: %s", buffer);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETREWARDITEM(196),"CALLBACK_CActionItemLayer::onSendGetItemRequest", "REQUEST_CActionItemLayer::onSendGetItemRequest",buffer, callfuncO_selector(CActionItemLayer ::onReceiveGetItemMsg));
}
void CActionItemLayer::onReceiveGetItemMsg(CCObject* pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this , "CALLBACK_CActionItemLayer::onSendGetItemRequest");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if(resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                // success:
                resultDict = (CCDictionary*) resultDict->objectForKey("result");
                if (resultDict &&   GameTools::intForKey("info", resultDict)==1)
                {
                    CCDictionary *rewardDict = (CCDictionary*)resultDict->objectForKey("reward");
                    if (rewardDict)
                    {
                        rewardDict = (CCDictionary*)rewardDict->objectForKey("add");
                        if (rewardDict)
                        {
                            rewardDict =(CCDictionary*) rewardDict->objectForKey("activity");
                            CReward * reward = CReward::create(rewardDict);
                            if (reward)
                            {
                                reward->excuteReward(ADD);
                                char tmp[300]={0};
                                reward->getRewardContent(tmp, 300);
                                CCMessageBox(tmp, "activity");
                            }
                            Present *present = (Present*)m_pData->getAllRewardItems()->objectAtIndex(m_nTouchTag);
                            present->setHasGet(true);
                        }
                       
                    }
                }
               
            }else
            {
            
            }
            
        }else
        {
            
        }
    }
}

void CActionItemLayer::createHead(string strHead)
{
    if (!getChildByTag(1)) {
        string word=Utility::getWordWithFile("word.plist", "huodongshijian");
        CCLabelTTF *labelTTF=CCLabelTTF::create(word.c_str(), "Arial", 20);
        labelTTF->setPosition(ccp(300,height));
        addChild(labelTTF,1,1);
        labelTTF->setColor(g_custom_color[13]);
        
    }
    if (!getChildByTag(2)) {
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 15);
        labelTTF->setPosition(ccp(300,height-30));
        addChild(labelTTF,1,2);
    }
    ((CCLabelTTF *)getChildByTag(2))->setString(strHead.c_str());
}

void CActionItemLayer::createContext(string  strContext)
{
    if (!getChildByTag(3)) {
        string word=Utility::getWordWithFile("word.plist", "huodongneirong");
        CCLabelTTF *labelTTF=CCLabelTTF::create(word.c_str(), "Arial", 20);
        labelTTF->setPosition(ccp(300,height-60));
        addChild(labelTTF,1,1);
        labelTTF->setColor(g_custom_color[13]);
    }
    if (!getChildByTag(4)) {
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 15,CCSizeMake(400, 0),kCCTextAlignmentCenter);
        labelTTF->setPosition(ccp(300,height-90));
        addChild(labelTTF,1,4);
    }
    ((CCLabelTTF *)getChildByTag(4))->setString(strContext.c_str());
    height-=(150+((CCLabelTTF *)getChildByTag(4))->getContentSize().height*0.5);
}

void CActionItemLayer::createTips(CStructGameActionData *data)
{
    CCArray * array = data->getAllRewardItems();
    if (array)
    {
        m_nRewardButtonCount = array->count();
        m_pRewardButtons = new CGameButtonControl*[m_nRewardButtonCount];
        
    }
    Present *present = NULL;
    string word=Utility::getWordWithFile("word.plist", "lingqujiangpin");
    for (int i = 0; i < m_nRewardButtonCount; i++)
    {
        m_pRewardButtons[i] = NULL;
        present = (Present*)array->objectAtIndex(i);
        if (present && present->getItemId() != 0)
        {
            
            height-=80;
            //获取物品表里面的物品图片；
            CPtProp *m_itemProp=SinglePropConfigData::instance()->getPropById(present->getItemId());
            CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath,m_itemProp->getIconName().c_str()));
            addChild(sprite,1,20+12*i);
            sprite->setPosition(ccp(100, height));
            //描述
            CCLabelTTF *labelTTF=CCLabelTTF::create(present->getTips().c_str(), "Arial", 15);
            addChild(labelTTF,1,21+12*i);
            labelTTF->setPosition(ccp(180, height));
            CGameButtonControl *gamebutton =CGameButtonControl::createButton(TEXTMID, word.c_str(), "anniu2_Normal.png", "anniu2_Pressed.png");
            gamebutton->setPosition(ccp(400, height));
            addChild(gamebutton,1,22+12*i);
            m_pRewardButtons[i] = gamebutton;
        }
    }
  
     

}
void CActionItemLayer::createChongZhi(CStructGameActionData *data)
{
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, "进入", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(gamebutton,1,171);
    gamebutton->setPosition(ccp(200, height-60));
    m_pSingleButton = gamebutton;
    height-=100;
}

void CActionItemLayer::createFuben(CStructGameActionData *data)
{
   
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, "进入", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(gamebutton,1,61);
    gamebutton->setPosition(ccp(200, height-60));
    height-=100;
    m_pSingleButton = gamebutton;

}

void CActionItemLayer::createPng(string strPngName)
{
    CCLog("strPngName:%s",strPngName.c_str());
    if (!strPngName.empty()|| !strcmp(strPngName.c_str(),0)) {
        
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,strPngName.c_str()));
        if(sprite)
        {
            addChild(sprite,1,100);
            sprite->setPosition(ccp(180 , height-200));
            height-=(200+sprite->getContentSize().height*0.5);
        }
        else{
            CCLog("后台传输的PNG图片为NULL");
        }
    }
}
