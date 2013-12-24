//
//  CGameRankLayer.cpp
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#include "CGameRankLayer.h"
#include "gameStruct.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "gamePlayer.h"
#include "CRankRewardManager.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CReward.h"

#define TAG_TABBEGIN 1000
#define TAG_FRIENDCOUNT 12
#define TAG_BOTTOMALLBUTTON 14
#define TAG_QUITBUTTON 20
#define TAG_TABLEVIEW 2
#define TAG_BACKGROUDMAP 1

CGameRankLayer::CGameRankLayer()
{
    m_pRankRewardData = NULL;
    m_pRankManager = CRankDataManager::getInstance();
    m_nPlayerUid = atoi(SinglePlayer::instance()->getUserId());
    m_pPlayerRankInfo = NULL;
    m_currentTabIndex=-1;
    m_vTableViewData = NULL;
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    size=CCDirector::sharedDirector()->getWinSize();
    m_tabs = new CCArray();
    if (m_tabs && m_tabs->init())
    {
        m_tabs->retain();
    }
    else
    {
        CC_SAFE_DELETE(m_tabs);
    }
    tempSprite=CCSprite::create((g_mapImagesPath+"rankpagecell.png").c_str());
    tempSprite->retain();
    
    m_pPlayerRankRewardInfo = NULL;
    m_pGetRankRewardBtn = NULL;

}

CGameRankLayer::~CGameRankLayer()
{
    CC_SAFE_RELEASE(tempSprite);
    CC_SAFE_RELEASE(m_tabs);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}

void CGameRankLayer::callBackFunc(void)//让对方去回调的数据;
{
    m_vTableViewData = m_pRankManager->getDataByType(m_currentTabIndex);
    if (m_vTableViewData)
    {
        switch (m_currentTabIndex)
        {
            case 0:
                onClickZongZhandouTabItem();
                break;
            case 1:
                onClickDengJiTabItem();
                break;
            case 2:
                onClickFuhaoTabItem();
                break;
            default:
                break;
        }
        
    }

}

bool CGameRankLayer::init()
{
    if (!CCLayer::init()) {
        return  false;
    }
    createBackGround();
    createRankTypeButton();
    createQuitButton();
    createRankReward();
    createPlayerRankInfo();
    setGunDongTiaoPtr();
    setTouchEnabled(true);
    setTouchPriority(-9);
   if(switchToTab(0))
   {
        m_vTableViewData = m_pRankManager->sendRequest(FIGHTRANK, this, callfunc_selector(CGameRankLayer::callBackFunc));
       if (m_vTableViewData)
       {
           onClickZongZhandouTabItem();
       }
       
       
   }
    return true;
}

void CGameRankLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_BACKGROUDMAP);

}



void CGameRankLayer::createRankTypeButton()
{
    
    const string str[3]={Utility::getWordWithFile("word.plist", "zongzhandoulipaihang"),Utility::getWordWithFile("word.plist", "dengjipaihang"),Utility::getWordWithFile("word.plist", "fubushipaihang")};
    for (int i=0; i<3; i++) {
        addTab(str[i].c_str(),i);
    }
    
}

CCPoint CGameRankLayer::getTabPositionByIndex(int index)
{
    return CCPointMake(size.width *0.5+index*130-280, size.height *0.5+210);
}

bool  CGameRankLayer::addTab(const char* label,int index)
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, label, "tabButton_normal.png", "tabButton_selected.png");
    addChild(gameButton,2,TAG_TABBEGIN+index);
    gameButton->setPosition(getTabPositionByIndex(index));
    m_tabs->addObject(gameButton);
    return true;
}
void CGameRankLayer::onClickZongZhandouTabItem()
{
    m_pRankRewardData = m_pRankManager->getRankRewardByType(FIGHTRANK);
    updateRankReward();
    updatePlayerRankInfo();
    updateRankCount();
    if(getChildByTag(TAG_TABLEVIEW))
    {
        CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
        tableView->reloadDataView();
    }
    else{
        createTableView(0);
    }
}

void CGameRankLayer::onClickDengJiTabItem()
{
    m_pRankRewardData = m_pRankManager->getRankRewardByType(LEVELRANK);
    updateRankReward();
    updatePlayerRankInfo();
    updateRankCount();
    if(getChildByTag(TAG_TABLEVIEW))
    {
        CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
        tableView->reloadDataView();
    }
    else{
        createTableView(0);
    }

}

void CGameRankLayer::onClickFuhaoTabItem()
{
    m_pRankRewardData = m_pRankManager->getRankRewardByType(RECHARGERANK);
    updateRankReward();
    updatePlayerRankInfo();
    updateRankCount();
    if(getChildByTag(TAG_TABLEVIEW))
    {
        CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
        tableView->reloadDataView();
    }
    else
    {
        createTableView(0);
    }
    
}

bool CGameRankLayer::switchToTab(int index)
{
    
    if(m_currentTabIndex !=-1 && m_currentTabIndex == index)
    {
        return false;
    }
    if (m_currentTabIndex!=-1)
    {
        CGameButtonControl *pTempButton=((CGameButtonControl *)m_tabs->objectAtIndex(m_currentTabIndex));
        if(pTempButton)
        {
            pTempButton->unselected();
        }
    }
    CGameButtonControl *pTempButton=((CGameButtonControl *)m_tabs->objectAtIndex(index));
    if(pTempButton)
    {
        pTempButton->selected();
    }
    m_currentTabIndex=index;
    return true;
}

void CGameRankLayer::setGunDongTiaoPtr()
{
    m_pScrollViewGuanDongTiao=(CCSprite *)Utility::getNodeByTag(this, "1,1,7,0");
    m_fOldScrollBarPosiontYtop=m_pScrollViewGuanDongTiao->getPosition().y;
    m_foldscrollBarPosiontylow=Utility::getNodeByTag(this, "1,1,7,26")->getPosition().y;
}

void CGameRankLayer::scrollViewBar(CCTableView* table)
{
    CCNode* bar = Utility::getNodeByTag(this, "1,1,7,40");
    if(bar == NULL)
    {
        return;
    }
    
    CCSize tableSize = table->getContentSize();
    CCSize tableViewSize = table->getViewSize();
    
    CCPoint contOffsetPos = table->getContentOffset();
    float maxOff = tableViewSize.height - tableSize.height;
    //CCLog("tableViewSize.height:%f,tableSize.height:%f,table->getContentOffset():%f",tableViewSize.height,tableSize.height,table->getContentOffset().y);
    float curOff = contOffsetPos.y - maxOff;
    float percentage = fabs(curOff)/fabs(maxOff);
    printf("curOff:%f, maxOff:%f, per:%f\n", curOff, maxOff, percentage);
    if(percentage < 0.1f)
    {
        percentage = 0;
    }
    if(percentage > 1.0f)
    {
        percentage = 1.0f;
    }
    float barTopPosY = m_fOldScrollBarPosiontYtop;
    float barLowPosY = m_foldscrollBarPosiontylow;
    float h = barTopPosY - percentage*(barTopPosY- barLowPosY);
    bar->setPosition(ccp(bar->getPosition().x, h));
}

void CGameRankLayer::tableScrolBarView(CCTableView *view)
{
     scrollViewBar((CCTableView*)view);
}

void CGameRankLayer::tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch)
{
    //判断具体点击哪一个按钮
}

void CGameRankLayer::scrollViewDidScroll(CCScrollView* view)
{
      scrollViewBar((CCTableView*)view);
}

void CGameRankLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CGameRankLayer::createTableView(int)
{
    if (!getChildByTag(TAG_TABLEVIEW)) {
        CCPoint p=ccp(200,150);
        CCSize s=CCSizeMake(650, 400);
        m_pTableCellSize=CCSizeMake(650, 100);
        CUtilityTableView * tableView = CUtilityTableView::Create(this, s,this);
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(p);
        tableView->setDelegate(this);
        tableView->setTouchPriority(-10);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        this->addChild(tableView,2,TAG_TABLEVIEW);
    }
    else{
        ((CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW)))->reloadDataView();
    }
    
}

void CGameRankLayer::removeTableView(int)
{
    if (getChildByTag(TAG_TABLEVIEW)) {
        removeChildByTag(TAG_TABLEVIEW, true);
    }
    
}

void CGameRankLayer::reloadTableViewData(int index)
{
    if (getChildByTag(TAG_TABLEVIEW)) {
        ((CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW)))->reloadDataView();
    }
}

unsigned int CGameRankLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_nRankCount;//m_vTableViewData->size();
}

void CGameRankLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    CCSprite *sprite=CCSprite::createWithTexture(tempSprite->getTexture());
    sprite->setPosition(CCPointMake(0, 0));
    sprite->setAnchorPoint(CCPointMake(0, 0));
    cell->addChild(sprite,0);
    string imgIcon=ConvertToString(idx%5+1)+".png";
    CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, imgIcon));
    cell->addChild(icon);
    icon->setPosition(CCPointMake(50, 50));
    string strName;
    switch (m_currentTabIndex) {
        case 0:
        {
           strName="玩家战斗力:";
            
        }
            break;
        case 1:
            strName="玩家等级:";
            break;
        case 2:
            strName="累积充值额度:";
            break;
        default:
            break;
    }
    CRankData * data =  m_vTableViewData->at(idx);
    data->name.c_str();
    char buffer[100]={0};
    snprintf(buffer, 100, "玩家姓名: %s", data->name.c_str());
    CCLabelTTF *labelTTF=CCLabelTTF::create(buffer, "Arial", 15);
    labelTTF->setPosition(CCPointMake(150, 40));
    labelTTF->setAnchorPoint(CCPointZero);
    cell->addChild(labelTTF);
    snprintf(buffer,100, "%d", data->value);
    labelTTF=CCLabelTTF::create(strName.append(buffer).c_str(), "Arial", 20);
    labelTTF->setAnchorPoint(CCPointZero);
    labelTTF->setPosition(CCPointMake(150, 10));
    cell->addChild(labelTTF);
    
    CCSprite * iconSprite = CCSprite::create(CSTR_FILEPTAH(g_rankIconPath,data->getRankChangeIconPath().c_str()));
    iconSprite->setAnchorPoint(CCPointZero);
    iconSprite->setPosition(ccp(340, 20));
    cell->addChild(iconSprite);
    // add rank label
    snprintf(buffer,100, "第%d名", idx+1);
    labelTTF=CCLabelTTF::create(buffer, "Arial", 20);
    labelTTF->setAnchorPoint(CCPointZero);
    labelTTF->setPosition(CCPointMake(400, 20));
    cell->addChild(labelTTF);
    
    
    snprintf(buffer,100, "第%d勋章", data->stage);
    labelTTF=CCLabelTTF::create(buffer, "Arial", 20);
    labelTTF->setAnchorPoint(CCPointZero);
    labelTTF->setPosition(CCPointMake(460, 20));
    cell->addChild(labelTTF);
}

CCTableViewCell* CGameRankLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        this->initCellItem(cell, idx);
    }
    else
    {
        scrollViewBar(table);
    }
    return cell;
}

CCSize CGameRankLayer::cellSizeForTable(CCTableView *table)
{
    return m_pTableCellSize;
}

void CGameRankLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if (cell && m_vTableViewData)
    {
        int idx = cell->getIdx();
        int index = m_vTableViewData->at(idx)->stage;
        int inType = m_currentTabIndex == 0 ? FIGHTRANKREWARD : m_currentTabIndex == 1 ? LEVELRANKREWARD : RECHARGRANKREWARD;
        const RankReward* tmpData =  CRankRewardManager::getInstance()->getRankRewardByIndex(inType, index);
        if (tmpData)
        {
            char buffer[200] = {0};
            snprintf(buffer, sizeof(char)*200, "第 %d 名 获得奖励: \n金币: %d, 经验:%d \n道具: id: %d * %d",idx+1,tmpData->exp, tmpData->coin, tmpData->item.propId, tmpData->item.propCount);
            CCMessageBox(buffer, "排名奖励");
        }
    }
}

//CCSize CGameRankLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
//{
//    
//}



bool CGameRankLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CGameRankLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CGameRankLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));
    
}
int CGameRankLayer::checkTouchQuitButton(CCPoint point)
{
    if (getChildByTag(TAG_QUITBUTTON)) {
        CGameButtonControl *pGameButton=(CGameButtonControl *)getChildByTag(TAG_QUITBUTTON);
        if(pGameButton->boundingBox().containsPoint(point))
        {
            return TAG_QUITBUTTON;
        }
    }
    return -1;
}

void CGameRankLayer::updateRankCount()
{
    m_nRankCount = 0;
    if (m_vTableViewData)
    {
        m_nRankCount = m_vTableViewData->size();
        int leftValue = m_nRankCount;
        int rightValue = 0;
        int middleValue = 0;

        while (rightValue != leftValue)
        {
            if (m_vTableViewData->at(rightValue))
            {
                if (m_vTableViewData->at(leftValue-1))
                {
                    m_nRankCount = leftValue;
                    break;
                }
                else
                {
                    middleValue =(leftValue+rightValue)/2;
                    if (m_vTableViewData->at(middleValue))
                    {
                        rightValue = middleValue;
                    }else
                    {
                        leftValue = middleValue;
                    }
                }
            }else
            {
                m_nRankCount = 0;
                break;
            }
        }
       
        
    }
}
int CGameRankLayer::checkTouchTableIndex(CCPoint point)
{
    CCObject *tempButton=NULL;
    CCARRAY_FOREACH(m_tabs, tempButton)
    {
        if (tempButton) {
            if(((CGameButtonControl *)tempButton)->boundingBox().containsPoint(point))
            {
                return ((CGameButtonControl *)tempButton)->getTag();
            }
        }
    }
    return -1;
}
void CGameRankLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pGetRankRewardBtn, pTouch))
    {
        onClickGetRankReward();
    }
    CCPoint point =pTouch->getLocation();
    int tag;
    if ((tag=checkTouchTableIndex(point))!=-1)
    {
        handleTagCallBack(tag);
    }
    else if(checkTouchQuitButton(point)!=-1)
    {
        removeFromParentAndCleanup(true);
    }
}

void CGameRankLayer::handleTagCallBack(int tag)
{
    switch (tag) {
        case TAG_TABBEGIN:
            if(switchToTab(0))
            {
                m_vTableViewData = m_pRankManager->sendRequest(FIGHTRANK, this, callfunc_selector(CGameRankLayer::callBackFunc));
                if (m_vTableViewData)
                {
                    onClickZongZhandouTabItem();
                }

            }
            break;
        case TAG_TABBEGIN+1:
            if(switchToTab(1))
            {
                m_vTableViewData = m_pRankManager->sendRequest(LEVELRANK, this, callfunc_selector(CGameRankLayer::callBackFunc));
                if (m_vTableViewData)
                {
                    onClickDengJiTabItem();
                }
                
            }
            break;
        case TAG_TABBEGIN+2:
            if(switchToTab(2))
            {
                m_vTableViewData = m_pRankManager->sendRequest(RECHARGERANK, this, callfunc_selector(CGameRankLayer::callBackFunc));
                if (m_vTableViewData)
                {
                    onClickFuhaoTabItem();
                }
                
            }
            break;
        default:
            break;
    }
}

void CGameRankLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameRankLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
void CGameRankLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CGameRankLayer::createPlayerRankInfo()
{
    char buffer[200] = {0};
    snprintf(buffer, 200, "player uid: %d\nfight : %d\nrank : %d ", 0, 0, 0);
    m_pPlayerRankInfo = CCLabelTTF::create("", "Arial", 13);
    m_pPlayerRankInfo->setPosition(ccp(size.width*0.5+240, size.height*0.5+220));
    m_pPlayerRankInfo->setString(buffer);
    m_pPlayerRankInfo->setHorizontalAlignment(kCCTextAlignmentLeft);
    addChild(m_pPlayerRankInfo,2);
}
void CGameRankLayer::updatePlayerRankInfo()
{
    int rank = -1;
    int value  = 0;
    char buffer[200] = {0};
    if (m_currentTabIndex == 0)
    {
        value = SinglePlayer::instance()->getTotalFightPoint();
        snprintf(buffer, 200, "我的uid: %d\n我的战力: %d\n", m_nPlayerUid, value);
    }
    else if(m_currentTabIndex == 1)
    {
        value = SinglePlayer::instance()->getPlayerLevel();
        snprintf(buffer, 200, "我的uid: %d\n我的等级: %d\n", m_nPlayerUid, value);
    }
    else if(m_currentTabIndex == 2)
    {
        value = SinglePlayer::instance()->getTotalRechargeValue();
        snprintf(buffer, 200, "我的uid: %d\n我的充值: %d\n", m_nPlayerUid, value);
    }
    
    CRankData *data  =m_pRankManager->getRankDataByUid(m_nPlayerUid, m_currentTabIndex, rank);
    char *tmpPointer = buffer+strlen(buffer);
    if (data == NULL)
    {
        snprintf(tmpPointer, 200-strlen(buffer), "未上榜");
    }else
    {
        snprintf(tmpPointer, 200-strlen(buffer), "我的排名 : %d", rank);
    }
    //


     m_pPlayerRankInfo->setString(buffer);
    
}

//领取排名奖励
void CGameRankLayer::createRankReward()
{
    if (m_pRankRewardData == NULL)
    {
        return;
    }
    if (m_pGetRankRewardBtn == NULL)
    {
        m_pGetRankRewardBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "anniu2_Normal.png"));
      
        addChild(m_pGetRankRewardBtn,2);
        m_pGetRankRewardBtn->setPosition(ccp(size.width*0.5+400, size.height*0.5+220));
  
        CCLabelTTF * tip = CCLabelTTF::create("", "Arial", 14);
        tip->setPosition(ccp(m_pGetRankRewardBtn->getContentSize().width/2, m_pGetRankRewardBtn->getContentSize().height/2));
        m_pGetRankRewardBtn->addChild(tip, 0, 1);
        m_pGetRankRewardBtn->setVisible(false);
    }
    if (m_pPlayerRankRewardInfo == NULL)
    {
        m_pPlayerRankRewardInfo = CCLabelTTF::create("", "Arial", 13);
        m_pPlayerRankRewardInfo->setPosition(ccp(size.width*0.5+125, size.height*0.5+220));
        m_pPlayerRankRewardInfo->setHorizontalAlignment(kCCTextAlignmentLeft);
        addChild(m_pPlayerRankRewardInfo,2);
        m_pPlayerRankRewardInfo->setVisible(false);
        
    }
}
void CGameRankLayer::updateRankReward()
{
    if (m_pRankRewardData->rankRewardData)
    {
        createRankReward();
        const char *buffer = m_pRankRewardData->hasGet ? "已领取" : "未领取";
        m_pGetRankRewardBtn->setVisible(true);
        ((CCLabelTTF*)m_pGetRankRewardBtn->getChildByTag(1))->setString(buffer);
        const RankReward* tmpData =  m_pRankRewardData->rankRewardData;
        if (tmpData)
        {
            char buffer[200] = {0};
            snprintf(buffer, sizeof(char)*200, "第 %d - %d 名 获得奖励: \n金币: %d, 经验:%d \n道具: id: %d * %d",tmpData->startIndex, tmpData->endIndex,tmpData->exp, tmpData->coin, tmpData->item.propId, tmpData->item.propCount);
            m_pPlayerRankRewardInfo->setString(buffer);
            m_pPlayerRankRewardInfo->setVisible(true);
        }
    
    }else
    {
        if (m_pGetRankRewardBtn)
        {
            m_pGetRankRewardBtn->setVisible(false);
        }
        if (m_pPlayerRankRewardInfo)
        {
            m_pPlayerRankRewardInfo->setVisible(false);
        }
         
        
    }
    
}

void CGameRankLayer::onClickGetRankReward()
{
    if (m_pRankRewardData && m_pRankRewardData->rankRewardData)
    {
        if (m_pRankRewardData->hasGet)
        {
            CCMessageBox("奖励已经领完，还想要，找王老板！", "排名奖励");
        }else
        {
            onSendGetRankReward();
        }
        
    }
}
void CGameRankLayer::onSendGetRankReward()
{
    int inType = m_currentTabIndex == 0 ? 2 : m_currentTabIndex == 1 ? 0 : 3;
    char buffer[200] = {0};
    snprintf(buffer,sizeof(char)*200,"sig=%s&type=%d",SinglePlayer::instance()->getUserSig(), inType);

    ADDHTTPREQUESTPOSTDATA(STR_URL_GETRANKREWARD(SinglePlayer::getUserId), "CALLBACK_CGameRankLayer::onSendGetRankReward", 
                           "REQUEST_CGameRankLayer::onSendGetRankReward", buffer, callfuncO_selector(CGameRankLayer::onReceiveGetRankRewardMsg));
}
void CGameRankLayer::onReceiveGetRankRewardMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGameRankLayer::onSendGetRankReward");
    char *buffer = (char*)pObject;
    if (buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if(code == 0)
            {
                resultDict = (CCDictionary*) resultDict->objectForKey("result");
                if (resultDict)
                {
                    SinglePlayer::instance()->addEmails((CCDictionary*)resultDict->objectForKey("inbox_info"));
                    CCDictionary *rewardDict = (CCDictionary*) resultDict->objectForKey("reward");
                    if (rewardDict)
                    {
                        rewardDict = (CCDictionary*) rewardDict->objectForKey("add");
                        if (rewardDict)
                        {
                            rewardDict = (CCDictionary*) rewardDict->objectForKey("task");
                            CReward *reward = CReward::create(rewardDict);
                            if (reward)
                            {
                                reward->excuteReward(ADD);
                                char buffer[200] = {0};
                                const RankReward *tmpData = m_pRankRewardData->rankRewardData;
                                
                                snprintf(buffer, sizeof(char)*200, "获得上期奖励: \n金币: %d, 经验:%d \n道具: id: %d * %d",tmpData->coin, tmpData->exp, tmpData->item.propId, tmpData->item.propCount);
                                CCMessageBox(buffer, "排名奖励");
                            }

                        }
                    }
                                    }
                 m_pRankRewardData->hasGet = true;
                 updateRankReward();
            }
            else
            {
                CCMessageBox(CCString::createWithFormat("error code :%d",code)->getCString(), "error code");
            }
        }
    }
}
