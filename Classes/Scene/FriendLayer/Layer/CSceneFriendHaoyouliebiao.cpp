//
//  CSceneFriendHaoyouliebiao.cpp
//  91.cube
//
//  Created by linminglu on 11/15/13.
//
//

#include "CSceneFriendHaoyouliebiao.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "gamePlayer.h"
#include "gameTools.h"
#include "PtJsonUtility.h"
#include "CFriend.h"
#include "CGameFriedItem.h"
#include "CCustomerTableView.h"
#include "CAddFriendLayer.h"

#define  GOLDPLACE_TOUCH_PRORITY -2
CSceneFriendHaoyouliebiao::CSceneFriendHaoyouliebiao()
{
    m_nUpdateValue=0;
    m_tempGamePlayer=SinglePlayer::instance();
    m_vNewFriendList=NULL;
}

CSceneFriendHaoyouliebiao::~CSceneFriendHaoyouliebiao()
{
    
}

bool CSceneFriendHaoyouliebiao::init()
{
    if (CCLayer::init()) {
        sendPostGetFriend();
        return true;
    }
    return false;
}

void CSceneFriendHaoyouliebiao::cleanFriendVector()
{
    if (m_vNewFriendList) {
        for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); it++) {
            if(*it)
            {
                CFriend *tempData=(*it);
                CC_SAFE_DELETE(tempData);
            }
        }
        m_vNewFriendList->erase(m_vNewFriendList->begin(), m_vNewFriendList->end());
        CC_SAFE_DELETE(m_vNewFriendList);
    }
}


void CSceneFriendHaoyouliebiao::createTableView()
{
    CCustomerTableView *tableView = CCustomerTableView::create(this, CCSizeMake(650, 400),NULL);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0.0));
    tableView->setPosition(ccp(200,150));
    tableView->setDelegate(this);
    tableView->setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,999);
    setContentSize(CCSizeMake(650, 400));
}

void CSceneFriendHaoyouliebiao::removeTableView()
{
    if(getChildByTag(999))
    {
        removeChildByTag(999, true);
    }
}

void CSceneFriendHaoyouliebiao::selectButton()
{
    if(!getChildByTag(7777))
    {
        CAddFriendLayer *layer=CAddFriendLayer::create();
        addChild(layer,5,7777);
    }
}

void CSceneFriendHaoyouliebiao::allZan()
{
    if(m_tempGamePlayer->getPlayrHadRecharged())
    {
        if (m_vNewFriendList&&m_vNewFriendList->size()>0) {
             isSendServerSingleFriend=false;
            sendPostSendZanMei(0);
        }
    }
    else{
        CCMessageBox("快去充值下就可以免费使用该功能", "info");
    }
}


void CSceneFriendHaoyouliebiao::sendPostGetFriend()
{
    string connectData="sig=";
    connectData += m_tempGamePlayer->getUserSig();
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_GETFRIEND(194), "CALLBACK_CSceneFriendHaoyouliebiao_sendPostGetFriend", "REQUEST_CGamePlayer_sendUpdataHospital",connectData.c_str(),callfuncO_selector(CSceneFriendHaoyouliebiao::decodeGetFriend))
}

void CSceneFriendHaoyouliebiao::decodeGetFriend(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneFriendHaoyouliebiao_sendPostGetFriend");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
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
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict) {
        cleanFriendVector();
        CCDictionary *friend_info=(CCDictionary *)(resultDict->objectForKey("friend_info"));
        m_vNewFriendList=CFriendManager::getInstance()->getFriendListByDict(friend_info);
        m_nUpdateValue=GameTools::intForKey("leave_time", dict);
    }
    else{
        m_nUpdateValue=0;
    }
    if (m_vNewFriendList &&m_vNewFriendList->size()>0) {
        
        createTableView();
    }
}

void CSceneFriendHaoyouliebiao::sendPostSendZanMei(int Uid)
{
    string  str="&sig";
    str+=m_tempGamePlayer->getUserSig();
    str+="&friend_uid=[";
    string friendUserID;
    if (!isSendServerSingleFriend)
    {
        for (int i=0; i<m_vNewFriendList->size(); i++) {
            if (m_vNewFriendList->at(i)->getState()) {
                friendUserID+=m_vNewFriendList->at(i)->getFriendUid();
                friendUserID+=",";
            }
        }
        if (friendUserID.length()==0) {
            return;
        }
    }
    str.append(friendUserID, 0, friendUserID.length()-1);
    str+="]";

    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_GETFRIEND(194), "CALLBACK_CSceneFriendHaoyouliebiao_sendPostGetFriend", "REQUEST_CGamePlayer_sendUpdataHospital",str.c_str(),callfuncO_selector(CSceneFriendHaoyouliebiao::decodeDataGetFriendLy))
}

void CSceneFriendHaoyouliebiao::decodeDataGetFriendLy(CCObject *object)
{CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneFriendHaoyouliebiao_sendPostGetFriend");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
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
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict)
    {
        m_nUpdateValue=GameTools::intForKey("leave_time", dict);
        if (!isSendServerSingleFriend) {
            for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); it++) {
                if (<#condition#>) {
                    <#statements#>
                }
                (*it)->setState(true); //已经领取的标志位。
            }
        }
    }
    else{
        m_nUpdateValue=0;
    }
    
}

CCSize CSceneFriendHaoyouliebiao::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(400, 200);
}

void  CSceneFriendHaoyouliebiao::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    int width=0;
    for (int i=0; i<2; i++) {
        if (idx*2+i<m_vNewFriendList->size())
        {
            CGameFriedItem *gameItem=CGameFriedItem::create(  m_vNewFriendList->at(idx*2+i), idx*2+i);
            cell->addChild(gameItem);
            gameItem->setPosition(ccp(width+gameItem->getContentSize().width *0.5, 0));
            width=gameItem->getContentSize().width+10;
        }
    }
}

CCTableViewCell* CSceneFriendHaoyouliebiao::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int CSceneFriendHaoyouliebiao::numberOfCellsInTableView(CCTableView *table)
{
    if (m_vNewFriendList) {
        return (int)(m_vNewFriendList->size()/2+0.5);
    }
    return 0;
}

void CSceneFriendHaoyouliebiao::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

void CSceneFriendHaoyouliebiao::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CSceneFriendHaoyouliebiao::scrollViewDidZoom(CCScrollView* view)
{
    
}

