//
//  CAddFriendLayer.cpp
//  91.cube
//
//  Created by linminglu on 11/15/13.
//
//

#include "CAddFriendLayer.h"
#include "gamePlayer.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "gameTools.h"

CAddFriendLayer::CAddFriendLayer()
{
    
}
CAddFriendLayer::~CAddFriendLayer()
{
    
}
bool CAddFriendLayer::init()
{
    if (CCLayer::init())
    {
        createBackGround();
        createTitle();
        createEdit();
        createSearchButton();
        setTouchPriority(-7);
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void CAddFriendLayer::sendAddFriend()
{
    string connectData="sig=";
    connectData += SinglePlayer::instance()->getUserSig();
    connectData +=string("&friend_uid=")+m_pEditUid->getText();
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_SEARCHFRIEND(194), "CALLBACK_CAddFriendLayer_sendAddFriend", "REQUEST_CAddFriendLayer_sendAddFriend",connectData.c_str(),callfuncO_selector(CAddFriendLayer::decodeAddFriend));
}

void CAddFriendLayer::decodeAddFriend(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CAddFriendLayer_sendAddFriend");
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
        runLabelTTFAction("申请发送成功",true);
    }
    else
    {
        runLabelTTFAction("没有该ID",false);
    }
}

void CAddFriendLayer::createTitle()
{
    CCLabelTTF *label=CCLabelTTF::create("查找", "Arial",25);
    CCSize nSize=CCDirector::sharedDirector()->getWinSize();
    label->setPosition(ccp(nSize.width*0.5-200, nSize.height*0.5));
    addChild(label,1);
}

bool checkIsAllDiagal(const char *pStr)
{
    char *p = 0;
    strtol(pStr, &p, 10);
    if (*p == 0)
        return true;
    else
        return false;
}

void CAddFriendLayer::menuAddUser(CCObject* pSender)
{
    if (strlen(m_pEditUid->getText())==0) {
        return;
    }
    if (checkIsAllDiagal(m_pEditUid->getText())) {
        sendAddFriend();
    }
}

void CAddFriendLayer::menuQuit(CCObject *pSender)
{
    
}

void CAddFriendLayer::createSearchButton(){
    CCMenuItemImage* pSureItem = CCMenuItemImage::create(
                                                         "resource_cn/img/tabButton_normal.png",
                                                         "resource_cn/img/tabButton_selected.png",
                                                         this,
                                                         menu_selector(CAddFriendLayer::menuAddUser) );
    CCLabelTTF* inf = CCLabelTTF::create();
    inf->setString("添加");
    inf->setFontSize(25);
    inf->setPosition(ccp(63, 17));
    pSureItem->setPosition( ccp(432, 200) );
    pSureItem->addChild(inf);
    CCMenu* pMenu = CCMenu::create(pSureItem,NULL);
    pMenu->setPosition( ccp(0, 100) );
    this->addChild(pMenu, 1);
}

void CAddFriendLayer::createEdit()
{
    float x=CCDirector::sharedDirector()->getWinSize().width*0.5;
    float y=CCDirector::sharedDirector()->getWinSize().height*0.5;
    m_pEditUid = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditUid->setPosition(ccp(x,y));
    m_pEditUid->setPlaceHolder("UID");
    m_pEditUid->setInputMode(kEditBoxInputModeNumeric);
    addChild(m_pEditUid,1);
}

void CAddFriendLayer::createBackGround()
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    CCLayerColor *pLayerColor=CCLayerColor::create(ccc4(0, 0, 0, 200), 500, 300);
    addChild(pLayerColor,1,0);
    pLayerColor->setPosition(ccp(size.width*0.5-250, size.height*0.5-150));
}

void CAddFriendLayer::removeAction()
{
    
}

void CAddFriendLayer::createLabelTip()
{
    
}
void CAddFriendLayer::removeTTF()
{
    if (getChildByTag(55))
    {
        removeChildByTag(55, true);
    }
}
void CAddFriendLayer::runLabelTTFAction(string str,bool )
{
    CCLabelTTF *pLabel=CCLabelTTF::create(str.c_str(), "Arial", 40);
    addChild(pLabel,2,55);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    pLabel->setPosition(ccp(size.width *0.5, size.height*0.5-200));
    pLabel->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(2.0, ccp(0,300)),CCFadeOut::create(2.0),NULL),CCCallFunc::create(this, callfunc_selector(CAddFriendLayer::removeTTF)),NULL));
}

bool CAddFriendLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CAddFriendLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CAddFriendLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayerColor *layerColor=(CCLayerColor *)getChildByTag(0);
    if (!layerColor->boundingBox().containsPoint(pTouch->getLocation())) {
        removeFromParentAndCleanup(true);
    }
}
void CAddFriendLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CAddFriendLayer::registerWithTouchDispatcher(void)
{
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CAddFriendLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}