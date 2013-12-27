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
#include "CGameDialogLayer.h"
#include "CSceneShowOtherUidTeam.h"
#include "CReward.h"

#define  GOLDPLACE_TOUCH_PRORITY -2
CSceneFriendHaoyouliebiao::CSceneFriendHaoyouliebiao()
{
    m_nUpdateValue=0;
    m_tempGamePlayer=SinglePlayer::instance();
    m_vNewFriendList=NULL;
    isUpdateTextRunning=false;
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
//    if (m_vNewFriendList) {
//        for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); it++) {
//            if(*it)
//            {
//                CFriend *tempData=(*it);
//                CC_SAFE_DELETE(tempData);
//            }
//        }
//        m_vNewFriendList->erase(m_vNewFriendList->begin(), m_vNewFriendList->end());
//        CC_SAFE_DELETE(m_vNewFriendList);
//  
//    }
   removeTableView();
}

void CSceneFriendHaoyouliebiao::startUpdateText()
{
    if (!isUpdateTextRunning) {
        schedule(schedule_selector(CSceneFriendHaoyouliebiao::updateText), 1.0);
    }
}


bool CSceneFriendHaoyouliebiao::checkCanSendSingleFriendZan(int idx)
{
    //空链表 , 区间上，区间下， 已经赞过
    if (!m_vNewFriendList || idx>=m_vNewFriendList->size() || idx<0  || m_vNewFriendList->at(idx)->getState()) {
        return false;
    }
    return true;
}

void CSceneFriendHaoyouliebiao::stopUpdateText()
{
    isUpdateTextRunning=false;
    unschedule(schedule_selector(CSceneFriendHaoyouliebiao::updateText));
}

int CSceneFriendHaoyouliebiao::getFriendCount()
{
    if (m_vNewFriendList) {
        return m_vNewFriendList->size();
    }
    return 0;
}



void CSceneFriendHaoyouliebiao::updateText(float t)
{
    isUpdateTextRunning=true;
    if (m_nUpdateValue>0) {
        string TextValue=CPtTool::stringForObjectValue(m_nUpdateValue);
        //便利tableview的每个cell
        if (m_vNewFriendList&& getChildByTag(999))
        {
            CCustomerTableView *tableView=(CCustomerTableView *)(getChildByTag(999));
            for (int index=0; index<m_vNewFriendList->size(); index++)
            {
                if (m_vNewFriendList->at(index)->getState()) {
                    CCTableViewCell * cell= tableView->cellAtIndex(index/2);
                    if (cell->getChildByTag(index)) {
                        CGameFriedItem *item=(CGameFriedItem *)(cell->getChildByTag(index));
                        item->checkAndStartUpdateText(TextValue.c_str());
                    }
                }
            }

        }
        m_nUpdateValue--;
    }
    else{
        stopUpdateText();
        cleanFriendVector();
        sendPostGetFriend();
    }
}

void CSceneFriendHaoyouliebiao::removeTableView()
{
    if (getChildByTag(999)) {
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

int  CSceneFriendHaoyouliebiao::deleteVectorByFriendId(int Uid)
{
    int resultIndex=-1;
    for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); ) {

        if ((*it)->getFriendUid()==Uid) {
            CFriend *temp=(*it);
            CC_SAFE_DELETE(temp);
            m_vNewFriendList->erase(it);
            return resultIndex;
        }
        else{
            it++;
        }
       resultIndex++;
    }
    return -1;
}

void CSceneFriendHaoyouliebiao::sendDeleteFriendUid(int Uid)
{
    if (m_vNewFriendList&&m_vNewFriendList->size()==0) {
        return ;
    }
    string  str="&sig=";
    m_nTouchFriendUid=Uid;
    str+=m_tempGamePlayer->getUserSig();
    str+="&friend_uid="+ConvertToString(Uid);
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_DELFRIEND(194), "CALLBACK_CSceneFriendHaoyouliebiao_sendDeleteFriendUid", "REQUEST_CSceneFriendHaoyouliebiao_sendDeleteFriendUid",str.c_str(),callfuncO_selector(CSceneFriendHaoyouliebiao::decodeGetServerDeleteFriend));
    
    
}

void CSceneFriendHaoyouliebiao::decodeGetServerDeleteFriend(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneFriendHaoyouliebiao_sendDeleteFriendUid");
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
    tempdata = NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *resultDict=(CCDictionary*)(dict->objectForKey("result"));
    if (GameTools::intForKey("info", resultDict)==1) {
        //成功添加的话需要删除该ID;
      deleteVectorByFriendId(m_nTouchFriendUid);
      ((CCustomerTableView *)(getChildByTag(999)))->reloadData();
    }
    else
    {
        //失败的话
        CCMessageBox("删除好友失败", "info");
    }
}

void CSceneFriendHaoyouliebiao::sendPostGetFriend()
{
    string connectData="sig=";
    connectData += m_tempGamePlayer->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETFRIEND(194), "CALLBACK_CSceneFriendHaoyouliebiao_sendPostGetFriend", "REQUEST_CGamePlayer_sendUpdataHospital",connectData.c_str(),callfuncO_selector(CSceneFriendHaoyouliebiao::decodeGetFriend))
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
        m_nUpdateValue=GameTools::intForKey("leave_time", resultDict);
        if (m_nUpdateValue>0)
        {
            startUpdateText();
        }
        CCDictionary *active_friendirct=(CCDictionary *)(resultDict->objectForKey("random_friend"));
        CFriendManager::getInstance()->paraseActivePlayerListByDict(active_friendirct); //解析
    }
    else
    {
        m_nUpdateValue=0;
    }
    if (m_vNewFriendList &&m_vNewFriendList->size()>0) {
        
        createTableView();
        CCNotificationCenter::sharedNotificationCenter()->postNotification("CallBACK_CSceneFriendMainLayer_FRIENDDATA",(CCObject *)(new int(getFriendCount())));
       
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification("CallBACK_CSceneFriendMainLayer_updateActiveFriend",NULL);
    
    
}

void CSceneFriendHaoyouliebiao::sendPostSendZanMei(int Uid)
{
    if (m_vNewFriendList&&m_vNewFriendList->size()==0) {
        return ;
    }
    string  str="&sig=";
    str+=m_tempGamePlayer->getUserSig();
    str+="&friend_uid=[";
    string friendUserID;
    if (!isSendServerSingleFriend)
    {
        for (int i=0; i<m_vNewFriendList->size(); i++) {
            if (!m_vNewFriendList->at(i)->getState()) {
                friendUserID+=ConvertToString(m_vNewFriendList->at(i)->getFriendUid());
                friendUserID+=",";
            }
        }
        if (friendUserID.length()==0) {
            return;
        }
    }
    else{
        m_nTouchFriendUid=Uid;
        friendUserID=ConvertToString(m_nTouchFriendUid)+",";
    }
    str.append(friendUserID, 0, friendUserID.length()-1);
    str+="]";

    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_GETFRIENDLY(194), "CALLBACK_CSceneFriendHaoyouliebiao_sendPostSendZanMei", "REQUEST_CSceneFriendHaoyouliebiao_SendZanMei",str.c_str(),callfuncO_selector(CSceneFriendHaoyouliebiao::decodeDataGetFriendLy))
}



void CSceneFriendHaoyouliebiao::decodeDataGetFriendLy(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneFriendHaoyouliebiao_sendPostSendZanMei");
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
        CCDictionary * tmp = (CCDictionary*) resultDict->objectForKey("reward");
        if (tmp)
        {
            CReward *reward =NULL;
            if (tmp->objectForKey("add") &&((CCDictionary*)tmp->objectForKey("add"))->objectForKey("friend") )
            {
                reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("add"))->objectForKey("friend"));
                reward->excuteReward(ADD);
                
            }
        }
        m_nUpdateValue=GameTools::intForKey("leave_time", resultDict);
        if (m_nUpdateValue!=0) {
            if (!isSendServerSingleFriend) {
                for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); it++)
                {
                    (*it)->setState(true); //已经领取的标志位。
                }
            }
            else{
                for (vector<CFriend *>::iterator it=m_vNewFriendList->begin(); it!=m_vNewFriendList->end(); it++)
                {
                    if ((*it)->getFriendUid()==m_nTouchFriendUid)
                    {
                        (*it)->setState(true);
                    }
                }
            }
            startUpdateText();  
        }
        else{
            stopUpdateText();
            cleanFriendVector();
            sendPostGetFriend();
        }
    }
    else{
        m_nUpdateValue=0;
        stopUpdateText();
    }
}

void CSceneFriendHaoyouliebiao::createTableView()
{
    CCustomerTableView *tableView = CCustomerTableView::create(this, CCSizeMake(650, 400),NULL);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(200,160));
    tableView->setDelegate(this);
    tableView->setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,999);
    setContentSize(CCSizeMake(650, 400));
}

CCSize CSceneFriendHaoyouliebiao::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(650, 200);
}

void  CSceneFriendHaoyouliebiao::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    CCLOG("idx====%d",idx);
    int width=0;
    for (int i=0; i<2; i++) {
        int vIndex=idx*2+i;
        CCLOG("vIndex %d",vIndex);
        if (vIndex<m_vNewFriendList->size())
        {
            CGameFriedItem *gameItem=CGameFriedItem::create(m_vNewFriendList->at(vIndex), vIndex);
            cell->addChild(gameItem,1,vIndex);
            gameItem->setPosition(ccp(width+gameItem->getContentSize().width*0.5, gameItem->getContentSize().height*0.5));
            gameItem->setButtonText("查看", "赞美");
            width+=gameItem->getContentSize().width+10;
            CCLOG("width %d",width);
        }
    }
}

CCTableViewCell* CSceneFriendHaoyouliebiao::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
//    table->dequeueCell();
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
        //return m_vNewFriendList->size();
        int cellCount= (int)((m_vNewFriendList->size()+1)*0.5);
        return cellCount;
    }
    return 0;
}

void CSceneFriendHaoyouliebiao::createChaKanHaoyou(int Uid)
{
    CSceneShowOtherUidTeam *pLayer=CSceneShowOtherUidTeam::CreateLayer(Uid);
    addChild(pLayer,7);
}

void CSceneFriendHaoyouliebiao::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int nCellIndex =cell->getIdx();
    CCLog("you touch cell at %d", nCellIndex);
    CCTouch *pTouch=((CCustomerTableView *)table)->getTouchSet();
    for (int i=0; i<2; i++)
    {
        if(cell->getChildByTag(nCellIndex*2+i))
        {
            CGameFriedItem *pGameItem=(CGameFriedItem *)(cell->getChildByTag(nCellIndex*2+i));
            int nValue=pGameItem->getTouchTag(pTouch);
            if (nValue==-1) {
                continue;
            }
            switch (nValue) {
                case 1:
                    //查看团队
                    CCLOG("11111111111");
                    createChaKanHaoyou(pGameItem->getFriendInfo()->getFriendUid());
                    break;
                case 2:
                    //赞美
                    CCLOG("22222222222");
                {
                    int idx=nCellIndex*2+i ;
                    if (checkCanSendSingleFriendZan(idx)) {
                        isSendServerSingleFriend=true;
                        sendPostSendZanMei(pGameItem->getFriendInfo()->getFriendUid());
                    }
                }
                    break;
                case 3:
                {
                    CCLOG("333333333");
                    createSureDeleteFriend(pGameItem->getFriendInfo());
                }
                    break;
                default:
                    break;
            }
        }
    }
}

void CSceneFriendHaoyouliebiao::createSureDeleteFriend(CFriend *pFriend)
{
    string str;
    str="确定抛弃基友"+ConvertToString(pFriend->getFriendUid())+":"+pFriend->getNickName();
    CPtDialog *dialog=CPtDialog::create(str, this, NULL ,callfuncO_selector(CSceneFriendHaoyouliebiao::sureDeleteFriend) , NULL, (CCObject *)pFriend, NULL);
    dialog->setButtonText("菊花已烂", "点错了");
    addChild(dialog,7);
}

void CSceneFriendHaoyouliebiao::sureDeleteFriend(CCObject *object)
{
    sendDeleteFriendUid(((CFriend *)object)->getFriendUid());
}

void CSceneFriendHaoyouliebiao::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CSceneFriendHaoyouliebiao::scrollViewDidZoom(CCScrollView* view)
{
    
}

