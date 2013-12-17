//
//  CSceneFriendMainLayer.cpp
//  91.cube
//
//  Created by linminglu on 11/14/13.
//
//

#include "CSceneFriendMainLayer.h"
#include "CGameButtonControl.h"
#include "gameConfig.h"
#include "LayoutLayer.h"
#include "gameTools.h"
#include "gamePlayer.h"
#include "CPtApplyListLayer.h"
#include "CPtPriaseListLayer.h"
#include "CSceneFriendHaoyouliebiao.h"
#include <string>

using namespace std;
#define TAG_TABBEGIN 1000
#define TAG_FRIENDCOUNT 12
#define TAG_BOTTOMALLBUTTON 14
#define TAG_QUITBUTTON 20


CSceneFriendMainLayer::CSceneFriendMainLayer()
{
    m_container=NULL;
    pObject=NULL;
    memset(callback, 0, 2);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    size=CCDirector::sharedDirector()->getWinSize();
    m_nFriendMax=SinglePlayer::instance()->getFriendMax();
    m_tabs=CCArray::create();
    m_tabs->retain();
    m_currentTabIndex=-1;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CSceneFriendMainLayer::updateFriend), "CallBACK_CSceneFriendMainLayer_FRIENDDATA", NULL);
}
CSceneFriendMainLayer::~CSceneFriendMainLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CallBACK_CSceneFriendMainLayer_FRIENDDATA");
    CC_SAFE_RELEASE(m_tabs);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
}
bool CSceneFriendMainLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    m_container = NULL;
    createBackGround();
    createTabControl();
    createIDText();
    //createFriendText();
    createBottomButton();
    createQuitButton();
    switchToTab(0);
    setTouchPriority(-3);
    setTouchEnabled(true);
    return true;
}

void CSceneFriendMainLayer::createTabControl()
{
    string data[3]={Utility::getWordWithFile("word.plist", "haoyouliebiao"),
                    Utility::getWordWithFile("word.plist", "shenqingliebiao"),
        Utility::getWordWithFile("word.plist", "shouhuozanmei")};
    for (int i=0; i<3; i++) {
        addTab(data[i].c_str(), i);
    }
}

void CSceneFriendMainLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,1);
}

void CSceneFriendMainLayer::updateFriend(CCObject *object)
{
    int *p=(int *)object;
    
    if (!getChildByTag(TAG_FRIENDCOUNT))
    {
        createFriendText();
    }
    if (p) {
        CCLabelTTF *pLabelTTF=(CCLabelTTF *)getChildByTag(TAG_FRIENDCOUNT);
        if (pLabelTTF)
        {
            string value;
            value +=Utility::getWordWithFile("word.plist", "haoyoushuliang")+ConvertToString(*p)+"/"+ConvertToString(m_nFriendMax);
            pLabelTTF->setString(value.c_str());
        }
        delete p;
        p=NULL;
    }
    
}


void CSceneFriendMainLayer::createFriendText()
{
    string value;
    value +=Utility::getWordWithFile("word.plist", "haoyoushuliang")+"0/"+ConvertToString(m_nFriendMax);
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(value.c_str(), "Arial", 15);
    pLabelTTF->setPosition(ccp(size.width *0.5-290 , size.height*0.5-230));
    addChild(pLabelTTF,1,TAG_FRIENDCOUNT);

}

void CSceneFriendMainLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));
    
}


void CSceneFriendMainLayer::createBottomButton()
{
    for (int i=0; i<2; i++)
    {
        CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, " ", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
        addChild(gameButton,2,TAG_BOTTOMALLBUTTON+i);
        gameButton->setPosition(ccp(size.width *0.5+120+120*i, size.height *0.5-230));
    }
}


void CSceneFriendMainLayer::createIDText()
{
    CGamePlayer *tempPlayer=SinglePlayer::instance();
    string IdText;
    IdText=Utility::getWordWithFile("word.plist", "wodeid");
    IdText += GameTools::ConvertToString(tempPlayer->getUserId());
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(IdText.c_str(), "Arial", 20);
    pLabelTTF->setPosition(ccp(size.width*0.5+300, size.height*0.5+200));
    pLabelTTF->setAnchorPoint(ccp(1.0, 0.0));
    addChild(pLabelTTF, 2);
}

                                          
CCNode*  CSceneFriendMainLayer::getContainer()
{
    return m_container;
}

CCSize CSceneFriendMainLayer::getContainerContentSize()
{
    return m_container->getContentSize();
}

void CSceneFriendMainLayer::setContainerContentSize(const CCSize& size)
{
    m_container->setContentSize(size);
}

CCPoint CSceneFriendMainLayer::getContainerPosition()
{
    return m_container->getPosition();
}

void CSceneFriendMainLayer::setContainerPosition(const CCPoint& pos)
{
    m_container->setPosition(pos);
}

unsigned int   CSceneFriendMainLayer::getTabCount()
{
    return m_tabs->count();
}

CCPoint CSceneFriendMainLayer::getTabPositionByIndex(int index)
{
    return CCPointMake(size.width *0.5+index*130-280, size.height *0.5+210);
}

bool  CSceneFriendMainLayer::addTab(const char* label,int index)
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, label, "tabButton_normal.png", "tabButton_selected.png");
    addChild(gameButton,2,1000+index);
    gameButton->setPosition(getTabPositionByIndex(index));
    m_tabs->addObject(gameButton);
    return true;
}
int CSceneFriendMainLayer::checkTouchTableIndex(CCPoint point)
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
int CSceneFriendMainLayer::checkTouchBottomButton(CCPoint point)
{
    for (int i=0; i<2; i++)
    {
        if (getChildByTag(i+TAG_BOTTOMALLBUTTON))
        {
            CGameButtonControl *tempGameButton=(CGameButtonControl *)(getChildByTag(i+TAG_BOTTOMALLBUTTON));
            if (tempGameButton && tempGameButton->boundingBox().containsPoint(point))
            {
               return  i+TAG_BOTTOMALLBUTTON;
            }
        }
    }
    return -1;
}

int CSceneFriendMainLayer::checkTouchQuitButton(CCPoint point)
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

void CSceneFriendMainLayer::selectFriendByUserId()
{
    
}

void CSceneFriendMainLayer::sendAllZanMei()
{
    
}

bool  CSceneFriendMainLayer::switchToTab(int index)
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
    CCLog("current tab index %d",m_currentTabIndex);
    if (m_container)
    {
        removeChild(m_container, true);
        m_container=NULL;
        pObject=NULL;
        memset(callback, 0, 2);
    }
    switch (m_currentTabIndex) {
        case 0:
        {
            CCLayer *layer=CSceneFriendHaoyouliebiao::create();
            addChild(layer,2,33333);
            m_container=layer;
            pObject=layer;
            callback[0]=callfunc_selector(CSceneFriendHaoyouliebiao::selectButton);
            callback[1]=callfunc_selector(CSceneFriendHaoyouliebiao::allZan);
            const string data[2]={"添加好友","全部赞"};
            for (int i=0; i<2; i++)
            {
                CGameButtonControl *gameButton=(CGameButtonControl*)(getChildByTag(TAG_BOTTOMALLBUTTON+i));
                if( i== 0)
                {
                    gameButton->setVisible(true);
                }
                if(gameButton->getTextLabel())
                {
                    gameButton->getTextLabel()->setString(data[i].c_str());
                }
            }
        }
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return  true;
}

bool CSceneFriendMainLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CSceneFriendMainLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSceneFriendMainLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point =pTouch->getLocation();

    if (checkTouchTableIndex(point)!=-1)
    {
        handleTagCallBack(checkTouchTableIndex(point));
        
    } else if(checkTouchBottomButton(point)!=-1)
    {
        handleTagCallBack(checkTouchBottomButton(point));
    }
    else if(checkTouchQuitButton(point)!=-1)
    {
        removeFromParentAndCleanup(true);
    }
}

void CSceneFriendMainLayer::handleTagCallBack(int tag)
{
    switch (tag) {
        case (TAG_BOTTOMALLBUTTON+1):
        {
            if (pObject&&callback[1]) {
                (pObject->*callback[1])();
            }

        }
            break;
        case TAG_BOTTOMALLBUTTON:
        {
            if (pObject&&callback[0]) {
                (pObject->*callback[0])();
            }
            CCLOG("touch the bottom left button");
        }
        break;
        case TAG_TABBEGIN:
            switchToTab(0);
            break;
        case TAG_TABBEGIN+1:
            if(switchToTab(1))
            {
                onClickApplyList();
            }
            break;
        case TAG_TABBEGIN+2:
            if(switchToTab(2))
            {
                onClickPriaseList();
            }
            break;
        default:
            break;
    }
}

void CSceneFriendMainLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CSceneFriendMainLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
void CSceneFriendMainLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CSceneFriendMainLayer::onClickApplyList()
{
    if (m_container)
    {
        m_container->removeFromParentAndCleanup(true);
    }
    
    callback[0]=callfunc_selector(CPtApplyListLayer::onClickAllCancel);
    callback[1]=callfunc_selector(CPtApplyListLayer::onClickAllConsent);
    const string data[2]={"全部忽略","全部同意"};
    for (int i=0; i<2; i++)
    {
        CGameButtonControl *gameButton=(CGameButtonControl*)(getChildByTag(TAG_BOTTOMALLBUTTON+i));

        if( i== 0)
        {
            gameButton->setVisible(true);
        }
        if(gameButton->getTextLabel())
        {
            gameButton->getTextLabel()->setString(data[i].c_str());
        }
    }
    
    m_container = CPtApplyListLayer::create();
    pObject = m_container;
    addChild(m_container, 100000);
}

void CSceneFriendMainLayer::onClickPriaseList()
{
    if (m_container)
    {
        m_container->removeFromParentAndCleanup(true);
    }
    
    callback[0]= NULL;//callfunc_selector(CPtApplyListLayer::onClickAllCancel);
    callback[1]=callfunc_selector(CPtPriaseListLayer::onClickAllGanEn);
    const string data[2]={"","全部感恩"};
    for (int i=0; i<2; i++)
    {
        CGameButtonControl *gameButton=(CGameButtonControl*)(getChildByTag(TAG_BOTTOMALLBUTTON+i));
        if ( i== 0)
        {
            gameButton->setVisible(false);
        }
        if(gameButton->getTextLabel())
        {
            gameButton->getTextLabel()->setString(data[i].c_str());
        }
    }

    m_container = CPtPriaseListLayer::create();
    pObject = m_container;
    addChild(m_container, 100000);

}