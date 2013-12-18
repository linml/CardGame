//
//  CActivePlayerLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-12-17.
//
//

#include "CActivePlayerLayer.h"
#include "CFriend.h"
#include "gameTools.h"
#include "gamePlayer.h"
#include "CGameButtonControl.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"

CActivePlayerLayer::~CActivePlayerLayer()
{
    CC_SAFE_RELEASE(m_vTouchArray);
}


bool CActivePlayerLayer::init()
{
    m_nSendUidIndex=-1;
    m_vTouchArray=CCArray::create();
    m_vTouchArray->retain();
    size=CCDirector::sharedDirector()->getWinSize();
    setContentSize(CCSizeMake(300,400));

    CCLayerColor *layercolor=CCLayerColor::create(ccc4(255, 0, 0, 100), 300, 400);
    addChild(layercolor,1);
    layercolor->ignoreAnchorPointForPosition(false);
    createGetNext();
    setTouchEnabled(true);
    setTouchPriority(-4);
    return true;
}

void CActivePlayerLayer::createFriendItem()
{
    
    for (int index=0; index<10; index++) {
        if (getChildByTag(100+index)) {
            removeChildByTag(100+index, true);
        }
        if (getChildByTag(index)) {
            removeChildByTag(index, true);
        }
    }
    for (int i=1; i<m_vTouchArray->count(); i++)
    {
            m_vTouchArray->removeObjectAtIndex(i);
    }
    for (int i=0; i<m_vActiverFriendList.size(); i++)
    {
            ActivePlayer *pSctiveFriend=m_vActiverFriendList[i];
            createItem(pSctiveFriend,i);
    }
}


void CActivePlayerLayer::getNextData()
{
    CFriendManager::getInstance()->getActivePlayerListFromLocal(&m_vActiverFriendList);

    createFriendItem();
}


void CActivePlayerLayer::createGetNext()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "刷新", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(gameButton,2,11);
    gameButton->setPosition(CCPointMake(0,-180));
    m_vTouchArray->addObject(gameButton);
    
}
CCPoint CActivePlayerLayer::getIndexPostion(int index)
{
        return CCPointMake(0, 200-40*index);
}
void CActivePlayerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point =pTouch->getLocation();
    point=convertToNodeSpace(point);
    for (int i=0; i<m_vTouchArray->count(); i++) {
        CGameButtonControl * touchGameButton=(CGameButtonControl *)m_vTouchArray->objectAtIndex(i);
        if(touchGameButton &&touchGameButton->boundingBox().containsPoint(point))
        {
            handleTouchTag(touchGameButton->getTag());
            return;
            
        }
    }
}

void CActivePlayerLayer::createItem(ActivePlayer *pFriend,int index)
{
    if (pFriend) {
        string showStr="Lv"+ConvertToString(pFriend->level);
        string userName=pFriend->username;
        string showValue=showStr+":"+userName;
        CCLabelTTF *label=CCLabelTTF::create(showValue.c_str(), "Arail", 15);
        label->setAnchorPoint(CCPointMake(0, 0.5));
        label->setPosition(CCPointMake(-100, getIndexPostion(index).y));
        addChild(label,1,100+index);
        string buttonStr;
        if (pFriend->isFriend) {
            buttonStr="已申请";
        }
        else{
            buttonStr="申请";
        }
        CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, buttonStr.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
        addChild(gameButton,1,index);
        gameButton->setPosition(CCPointMake(100, getIndexPostion(index).y));
        m_vTouchArray->addObject(gameButton);
    }
    
    
}

bool CActivePlayerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    CCRect rect=boundingBox();
    rect.origin.x-=getContentSize().width/2;
    rect.origin.y-=getContentSize().height/2;
    CCPoint pointLabel=rect.origin;

    if (rect.containsPoint(point)) {
        return true;
    }
    return false;
}

void CActivePlayerLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CActivePlayerLayer::sendUpdateActiveFriend()
{
    callBackSendUpdate(NULL);
}

void CActivePlayerLayer::callBackSendUpdate(CCObject *object)
{
    CFriendManager::getInstance()->getActivePlayerListFromLocal(&m_vActiverFriendList);
    createFriendItem();
}

void CActivePlayerLayer::handleTouchTag(int tag)
{
    switch (tag) {
        case 11:
            getNextData();
            break;
        default:
        {
            int index=tag;
            if (index>=0 && index<m_vActiverFriendList.size())
            {
                ActivePlayer *pFriend=m_vActiverFriendList[index];
                if (pFriend && !pFriend ->isFriend) {
                    //发送数据
                        string connectData="sig=";
                        connectData += SinglePlayer::instance()->getUserSig();
                        connectData +=string("&friend_uid=")+ConvertToString(pFriend->friend_uid);
                        ADDHTTPREQUESTPOSTDATA(STR_URL_SEARCHFRIEND(194), "CALLBACK_CActivePlayerLayer_sendAddFriend", "REQUEST_CActivePlayerLayer_sendAddFriend",connectData.c_str(),callfuncO_selector(CActivePlayerLayer::decodeAddFriend));
                        m_nSendUidIndex=index;
                }

            }
            
        }
            break;
    }
    
}

void CActivePlayerLayer::decodeAddFriend(cocos2d::CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CActivePlayerLayer_sendAddFriend");
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
    if (GameTools::intForKey("info", resultDict)==1)
    {
        if (m_nSendUidIndex>=0 && m_nSendUidIndex<m_vActiverFriendList.size())
        {
            CCLOG("m_nSendUidIndex:%d",m_nSendUidIndex);
            m_vActiverFriendList[m_nSendUidIndex]->isFriend=true;
            ((CGameButtonControl *)getChildByTag(m_nSendUidIndex))->setText("已申请");

        }
        runLabelTTFAction("申请发送成功",true);
    }

}

void CActivePlayerLayer::runLabelTTFAction(string str, bool flag)
{
    CCLabelTTF *pLabel=CCLabelTTF::create(str.c_str(), "Arial", 40);
    addChild(pLabel,2);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    pLabel->setPosition(ccp(-400, 0));
    pLabel->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(1.0, ccp(0,100)),CCFadeOut::create(1.0),NULL),CCCallFuncN::create(this, callfuncN_selector(CActivePlayerLayer::removeTTF)),NULL));
}


void CActivePlayerLayer::removeTTF(CCNode *node)
{
    if (node) {
        removeChild(node,true);
    }

}

void CActivePlayerLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CActivePlayerLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -4, true);
}
void CActivePlayerLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}