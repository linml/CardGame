//
//  CPVPReportLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#include "CPVPReportLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CCustomerTableView.h"
#include "CGameButtonControl.h"
#include "CPVPReportItemSprite.h"
#include "CReward.h"
#include "CPVPReportItemData.h"
#include "CPtTool.h"
#include "CPVPMonsterData.h"
#include "CPVPMonsterPlayerLayer.h"
#define TAG_PVP_BACKGROUD 0
#define TAG_TASKPVP_TOUCH_PRORITY -30002
#define TAG_TASKPVP_QUITBUTTON 2


CPVPReportLayer::CPVPReportLayer()
{
    m_nCellIndex=0;
    m_nVectorIndex=0;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}
bool CPVPReportLayer::init()
{
    size=CCDirector::sharedDirector()->getWinSize();
    m_nTaskId=0;
    m_pCustomTable=NULL;
    createBackGround();
    createQuitButton();
    sendInitTable();
    setTouchEnabled(true);
    return true;
}

CPVPReportLayer::~CPVPReportLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
}

void CPVPReportLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_PVP_BACKGROUD);
    
    
}


void CPVPReportLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_TASKPVP_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));
}


void CPVPReportLayer::createTableView()
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
    // m_pCustomTable->setContentSize(CCSizeMake(650, 400));
}


void CPVPReportLayer::createInfo()
{
    CCLabelTTF *pLabelTTF=CCLabelTTF::create("服务端没有响应关闭该对话框后再打开", "Arial", 30);
    addChild(pLabelTTF,2);
    //CCSize size=CCDirector::sharedDirector()->getWinSize();
    pLabelTTF->setPosition(ccp(size.width *0.5 ,size.height *0.5));
    
}



void CPVPReportLayer::sendInitTable()
{
    string str;
    str+="&sig=";
    str+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPZHANBAO(194), "CALLBACK_CPVPReportLayer_sendInitTable", "REQUEST_CPVPReportLayer_sendInitTable",str.c_str(),callfuncO_selector(CPVPReportLayer::callBackInitTableValue))
}

void CPVPReportLayer::callBackInitTableValue(CCObject *object)
{
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPVPReportLayer_sendInitTable");
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
        CCDictElement *element = NULL;
        CCDictionary *onlyCard=NULL;
        CCDICT_FOREACH(resultDict, element)
        {
            CCObject * object = element->getObject();
            
            if (object && (onlyCard = (CCDictionary *)(object)))
            {
                CPVPReportItemData *tempdata=new CPVPReportItemData;
                tempdata->status=GameTools::intForKey("status", onlyCard);
                tempdata->time=GameTools::intForKey("time", onlyCard);
                tempdata->fuchouID=atoi(element->getStrKey());
                tempdata->m_pvpRankData.uid=GameTools::intForKey("uid", onlyCard);
                tempdata->m_pvpRankData.name=GameTools::valueForKey("username", onlyCard);
                tempdata->m_pvpRankData.fightpointer=GameTools::intForKey("power", onlyCard);
                tempdata->m_pvpRankData.level=GameTools::intForKey("level", onlyCard);
                tempdata->m_pvpRankData.rank=GameTools::intForKey("pvp_num", onlyCard);
                tempdata->m_pvpRankData.credits=GameTools::intForKey("score", onlyCard);
                tempdata->m_vCardList.resize(5);
                CCDictionary *temp_info2=(CCDictionary *)onlyCard->objectForKey("team_info");
                if (CPtTool::isDictionary(temp_info2))
                {
                    CCDictionary *team=(CCDictionary *)temp_info2->objectForKey("team");
                    CCDictElement *element2 = NULL;
                    CCDictionary *teamonlyCard=NULL;
                    CCDICT_FOREACH(team, element2)
                    {
                        CCObject * object = element->getObject();
                        
                        if (object && (teamonlyCard = (CCDictionary *)(object)))
                        {
                            int postion=GameTools::intForKey("position", teamonlyCard);
                            int cardid=GameTools::intForKey("card_id", teamonlyCard);
                            int level=GameTools::intForKey("level", teamonlyCard);
                            CCard *card=SinglePlayer::instance()->getCardByCardId(cardid);
                            if(card)
                            {
                                CFightCard *pFightCard=new CFightCard(card,level);
                                tempdata->m_vCardList[postion]=pFightCard;
                            }
                        }
                    }

                }
              m_vPvpReport.push_back(tempdata);
         }
        }
        createTableView();
        
    }
    
}

void CPVPReportLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CPVPReportLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
bool CPVPReportLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CPVPReportLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPReportLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    if (getChildByTag(TAG_TASKPVP_QUITBUTTON)->boundingBox().containsPoint(point))
    {
        removeFromParentAndCleanup(true);
    }
}

void CPVPReportLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

CCSize CPVPReportLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(650, 120);
}

CCTableViewCell* CPVPReportLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    //table->dequeueCell();
    CCTableViewCell* cell = table->cellAtIndex(idx);
    CCLOG("cell %d ,%d",(int)cell,idx);
    if (!cell)
    {
        
        CCLOG("cell====");
        cell = new CCTableViewCell();
        cell->autorelease();
        initCellItem(cell, idx);
    }
    else
    {
        //  CCLOG("cell==%x",cell);
        //scrollBar(table);
    }
    return cell;
}

unsigned int CPVPReportLayer::numberOfCellsInTableView(CCTableView *table)
{
    //return  (unsigned int)(m_pManager->m_vtaskRewordManager.size()+1)/2;
    return (unsigned int)(m_vPvpReport.size()+1)/2;
}

/*
 * @brief: 判断，道具，卡牌入包
 * @return: false(不行，则提示)； true(可以)
 */
bool CPVPReportLayer::checkisCanGetData(CPVPReportItemData *pvp)
{
//    if (<#condition#>) {
//        <#statements#>
//    }
    return true;
}

void CPVPReportLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int nCellIndex =cell->getIdx();
    CCLog("you touch cell at %d", nCellIndex);
    CCTouch *pTouch=((CCustomerTableView *)table)->getTouchSet();
    for (int i=0; i<2; i++)
    {
        int tempIndex=nCellIndex*2+i;
        if(cell->getChildByTag(tempIndex))
        {
            CPVPReportItemSprite *pGameItem=(CPVPReportItemSprite *)(cell->getChildByTag(tempIndex));
            
            CCRect rect=pGameItem->boundingBox();
            if (pGameItem &&CPtTool::isInNode(pGameItem, pTouch))
            {
                if (m_vPvpReport[tempIndex]->status==0) {
                    m_nCellIndex=nCellIndex;
                    m_nVectorIndex=tempIndex;
                    CPVPMonsterData *pvpdata=new CPVPMonsterData();
                    pvpdata->autorelease();
                    pvpdata->retain();
                    PVPRankData *rankData=new PVPRankData;
                    rankData->uid=m_vPvpReport[tempIndex]->m_pvpRankData.uid;
                    rankData->name=m_vPvpReport[tempIndex]->m_pvpRankData.name;
                    rankData->rank=m_vPvpReport[tempIndex]->m_pvpRankData.rank;
                    rankData->level=m_vPvpReport[tempIndex]->m_pvpRankData.level;
                    rankData->fightpointer=m_vPvpReport[tempIndex]->m_pvpRankData.fightpointer;
                    rankData->credits =m_vPvpReport[tempIndex]->m_pvpRankData.credits;
                    pvpdata->setPVPUserData(rankData);
                    pvpdata->fuchouID=m_vPvpReport[tempIndex]->fuchouID;
                    pvpdata->m_vCardList.resize(5);
                    for (int i=0; i<m_vPvpReport[tempIndex]->m_vCardList.size();i++ ) {
                        if (m_vPvpReport[tempIndex]->m_vCardList[i]) {
                            pvpdata->m_vCardList[i]=new CFightCard(*m_vPvpReport[tempIndex]->m_vCardList[i]);
                        }
                    }
                    CCLayer *pLayer=CPVPMonsterPlayerLayer::CreateByUserID(pvpdata, false);
                    pLayer->setTouchPriority(getTouchPriority()-1);
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer,101);
                    break;
                }
                
            }
       }
    }
    
}

void CPVPReportLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CPVPReportLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CPVPReportLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    int width=50;
    for (int i=0; i<2; i++) {
        int vIndex=idx*2+i;
        if (vIndex<m_vPvpReport.size())
        {
            CPVPReportItemSprite *gameItem=CPVPReportItemSprite::CreateByPvpData(m_vPvpReport[vIndex]);
            cell->addChild(gameItem,1,vIndex);
            gameItem->setPosition(ccp(width, 0));
            width+=gameItem->getContentSize().width+50;
            //CCLOG("width %d",width);
        }

    }
    
}