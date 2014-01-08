//
//  CPVPStructMissionTaskRewordLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-1-8.
//
//

#include "CPVPStructMissionTaskRewordLayer.h"
#include "CPVPStructMissionTaskReword.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CCustomerTableView.h"
#include "CGameButtonControl.h"
#include "CPVPStructMissionTaskRewordSprite.h"
#include "CReward.h"
#define TAG_PVP_BACKGROUD 0
#define TAG_TASKPVP_TOUCH_PRORITY -2
#define TAG_TASKPVP_QUITBUTTON 2

CPVPStructMissionTaskRewordLayer::CPVPStructMissionTaskRewordLayer()
{
    m_nCellIndex=0;
    m_nVectorIndex=0;
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}
bool CPVPStructMissionTaskRewordLayer::init()
{
    size=CCDirector::sharedDirector()->getWinSize();
    m_nTaskId=0;
    m_pCustomTable=NULL;
    m_pManager=new CPVPStructMissionTaskRewordManager;
    createBackGround();
    createQuitButton();
    sendInitTable();
    return true;
}

CPVPStructMissionTaskRewordLayer::~CPVPStructMissionTaskRewordLayer()
{
     CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
    CC_SAFE_DELETE(m_pManager);
}

void CPVPStructMissionTaskRewordLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_PVP_BACKGROUD);

    
}


void CPVPStructMissionTaskRewordLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_TASKPVP_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));
}


void CPVPStructMissionTaskRewordLayer::createTableView()
{
    if (m_pCustomTable) {
        m_pCustomTable->removeFromParentAndCleanup(true);
    }
    m_pCustomTable = CCustomerTableView::create(this, CCSizeMake(650, 400),NULL);
    m_pCustomTable->setDirection(kCCScrollViewDirectionVertical);
    m_pCustomTable->setPosition(ccp(200,160));
    m_pCustomTable->setDelegate(this);
    m_pCustomTable->setTouchPriority(TAG_TASKPVP_TOUCH_PRORITY-2);
    m_pCustomTable->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(m_pCustomTable,2,999);
    setContentSize(CCSizeMake(650, 400));
}


void CPVPStructMissionTaskRewordLayer::createInfo()
{
    CCLabelTTF *pLabelTTF=CCLabelTTF::create("服务端没有响应关闭该对话框后再打开", "Arial", 30);
    addChild(pLabelTTF,2);
    //CCSize size=CCDirector::sharedDirector()->getWinSize();
    pLabelTTF->setPosition(ccp(size.width *0.5 ,size.height *0.5));
    
}



void CPVPStructMissionTaskRewordLayer::sendInitTable()
{
    string str;
    str+="&sig=";
    str+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETYIJINGLINGQU(194), "CALLBACK_CPVPStructMissionTaskRewordLayer_sendInitTable", "REQUEST_CPVPStructMissionTaskRewordLayer_SendZanMei",str.c_str(),callfuncO_selector(CPVPStructMissionTaskRewordLayer::callBackInitTableValue))
}

void CPVPStructMissionTaskRewordLayer::callBackInitTableValue(CCObject *object)
{
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPVPStructMissionTaskRewordLayer_sendInitTable");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
        createInfo();
        setTouchEnabled(true);
        setTouchPriority(TAG_TASKPVP_TOUCH_PRORITY-1);
        return ;
    }
    char * tempdata=(char *)object;
    //解析字符串。 赋值与vector中
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    CCLog("tempdata:%s",tempdata);
    delete [] tempdata;
    tempdata=NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        setTouchEnabled(true);
        setTouchPriority(TAG_TASKPVP_TOUCH_PRORITY-1);
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict)
    {
        int nCurrPlayerPvpTimes=GameTools::intForKey("pvp_times", resultDict);
        int nCurrPlayerPvpJifen=GameTools::intForKey("pvp_score", resultDict);
        CCArray *arrya=(CCArray *)resultDict->objectForKey("pvp_task");
        if (m_pManager) {
            m_pManager->changeLingQuStatuas(arrya);
            m_pManager->changeKeyiLingqu(nCurrPlayerPvpTimes, nCurrPlayerPvpJifen);
            createTableView();
            setTouchEnabled(true);
            setTouchPriority(TAG_TASKPVP_TOUCH_PRORITY-1);
        }
    }

}
void CPVPStructMissionTaskRewordLayer::sendLingqu()
{
    if(m_nTaskId)
    {
    string str;
    str+="&sig=";
    str+=SinglePlayer::instance()->getUserSig();
    str+="&pvp_task_id=";
    str+=ConvertToString(m_nTaskId);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETLINGQUPVPJIANGLI(194), "CALLBACK_CPVPStructMissionTaskRewordLayer_sendLingqu", "REQUEST_CPVPStructMissionTaskRewordLayer_sendLingqu",str.c_str(),callfuncO_selector(CPVPStructMissionTaskRewordLayer::callBackLingQuValue))
    }
}

void CPVPStructMissionTaskRewordLayer::callBackLingQuValue(CCObject *object)
{
      CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPVPStructMissionTaskRewordLayer_sendLingqu");
    char *data=(char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse(data);
    delete data;
    data=NULL;

    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        setTouchEnabled(true);
        setTouchPriority(TAG_TASKPVP_TOUCH_PRORITY-1);
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict)
    {
        if (GameTools::intForKey("info", resultDict)==1) {
            CCDictionary * tmp = (CCDictionary*) resultDict->objectForKey("reward");
            if (tmp)
            {
                CReward *reward =NULL;
                if (tmp->objectForKey("add") &&((CCDictionary*)tmp->objectForKey("add"))->objectForKey("pvptask") )
                {
                    reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("add"))->objectForKey("pvptask"));
                    reward->excuteReward(ADD);
                    
                }
            }
            m_pManager->m_vtaskRewordManager[m_nVectorIndex]->setMissionTaskLingQu(true);
            m_pCustomTable->updateCellAtIndex(m_nCellIndex);
        }
        else{
            CCMessageBox("error", "error");
        }
    }

}


void CPVPStructMissionTaskRewordLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CPVPStructMissionTaskRewordLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
bool CPVPStructMissionTaskRewordLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CPVPStructMissionTaskRewordLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPStructMissionTaskRewordLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    if (getChildByTag(TAG_TASKPVP_QUITBUTTON)->boundingBox().containsPoint(point))
    {
        removeFromParentAndCleanup(true);
    }
}

void CPVPStructMissionTaskRewordLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

CCSize CPVPStructMissionTaskRewordLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(650, 120);
}

CCTableViewCell* CPVPStructMissionTaskRewordLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        initCellItem(cell, idx);
    }
    else
    {
        //scrollBar(table);
    }
    return cell;
}

unsigned int CPVPStructMissionTaskRewordLayer::numberOfCellsInTableView(CCTableView *table)
{
    return  (unsigned int)(m_pManager->m_vtaskRewordManager.size()+1)/2;
}

bool CPVPStructMissionTaskRewordLayer::checkisCanGetData(CPVPStructMissionTaskReword *pvp)
{
    return true;
}

void CPVPStructMissionTaskRewordLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int nCellIndex =cell->getIdx();
    CCLog("you touch cell at %d", nCellIndex);
    CCTouch *pTouch=((CCustomerTableView *)table)->getTouchSet();
    for (int i=0; i<2; i++)
    {
        int tempIndex=nCellIndex*2+i;
        if(cell->getChildByTag(tempIndex))
        {
            CPVPStructMissionTaskRewordSprite *pGameItem=(CPVPStructMissionTaskRewordSprite *)(cell->getChildByTag(tempIndex));

            CCRect rect=pGameItem->boundingBox();
            if (pGameItem &&CPtTool::isInNode(pGameItem, pTouch))
            {
                if (!m_pManager->m_vtaskRewordManager[tempIndex]->getMissionTaskLingQu()&& m_pManager->m_vtaskRewordManager[tempIndex]->getMissionTaskKeYiLingQu()) {
                    m_nTaskId=pGameItem->getPVPStructMissionTaskReword()->getMissionTaskID();
                    m_nCellIndex=nCellIndex;
                    m_nVectorIndex=tempIndex;
                    if(checkisCanGetData(m_pManager->m_vtaskRewordManager[nCellIndex*2+i]))
                    {
                        PtSoundTool::playSysSoundEffect("UI_click.wav");
                        sendLingqu();
                    }
                    break;
                }
               
            }
        }
    }

}

void CPVPStructMissionTaskRewordLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CPVPStructMissionTaskRewordLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CPVPStructMissionTaskRewordLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
      int width=50;
    for (int i=0; i<2; i++) {
        int vIndex=idx*2+i;
      
        //CCLOG("vIndex %d",vIndex);
        if (vIndex<m_pManager->m_vtaskRewordManager.size())
        {
            CPVPStructMissionTaskRewordSprite *gameItem=CPVPStructMissionTaskRewordSprite::CreateByPvpData(m_pManager->m_vtaskRewordManager[vIndex]);
            cell->addChild(gameItem,1,vIndex);
            gameItem->setPosition(ccp(width, 0));
            width+=gameItem->getContentSize().width+50;
            //CCLOG("width %d",width);
        }
    }

}