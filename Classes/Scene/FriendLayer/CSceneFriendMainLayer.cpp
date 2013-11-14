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
#include <string>
using namespace std;
#define TAG_TABBEGIN 1000
#define TAG_FRIENDCOUNT 12
#define TAG_BOTTOMALLBUTTON 14

CSceneFriendMainLayer::CSceneFriendMainLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    size=CCDirector::sharedDirector()->getWinSize();
    m_nFriendMax=SinglePlayer::instance()->getFriendMax();
    m_tabs=CCArray::create();
    m_tabs->retain();
    m_currentTabIndex=-1;
}
CSceneFriendMainLayer::~CSceneFriendMainLayer()
{
    CC_SAFE_RELEASE(m_tabs);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
}
bool CSceneFriendMainLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    createBackGround();
    createTabControl();
    createIDText();
    createFriendText();
    createBottomButton();
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
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
    addChild(layer,1,1);
}

void CSceneFriendMainLayer::updateFriend()
{
    if (!getChildByTag(TAG_FRIENDCOUNT))
    {
        createFriendText();
    }
    CCLabelTTF *pLabelTTF=(CCLabelTTF *)getChildByTag(TAG_FRIENDCOUNT);
    if (pLabelTTF)
    {
        string value;
        value +=Utility::getWordWithFile("word.plist", "haoyoushuliang")+ConvertToString(5)+"/"+ConvertToString(m_nFriendMax);
        pLabelTTF->setString(value.c_str());
    }
}


void CSceneFriendMainLayer::createFriendText()
{
    string value;
    value +=Utility::getWordWithFile("word.plist", "haoyoushuliang")+ConvertToString(5)+"/"+ConvertToString(m_nFriendMax);
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(value.c_str(), "Arial", 15);
    pLabelTTF->setPosition(ccp(size.width *0.5-290 , size.height*0.5-230));
    addChild(pLabelTTF,1,TAG_FRIENDCOUNT);

}

void CSceneFriendMainLayer::createBottomButton()
{
    for (int i=0; i<2; i++)
    {
        CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, " ", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
        addChild(gameButton,2,TAG_BOTTOMALLBUTTON+1);
        gameButton->setPosition(ccp(size.width *0.5+120+120*i, size.height *0.5-230));
    }
}


void CSceneFriendMainLayer::createIDText()
{
    CGamePlayer *tempPlayer=SinglePlayer::instance();
    string IdText;
    IdText += "+" +GameTools::ConvertToString(tempPlayer->getUserId());
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(IdText.c_str(), "Arial", 20);
    pLabelTTF->setPosition(ccp(size.width*0.5+300, size.height+200));
    pLabelTTF->setAnchorPoint(ccp(1.0, 0.0));
    addChild(pLabelTTF, 2);
}

void  CSceneFriendMainLayer::setTabMargin(float marginLeft,float marginTop)
{
    //setMarginLeft(marginLeft);
    //setMarginTop(marginTop);
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
int CSceneFriendMainLayer::touchTableIndex(CCPoint point)
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
int CSceneFriendMainLayer::touchBottomButton(CCPoint point)
{
    for (int i=1; i<3; i++) {
        if (getChildByTag(i+TAG_BOTTOMALLBUTTON)) {
            CGameButtonControl *tempGameButton=(CGameButtonControl *)(getChildByTag(i+TAG_BOTTOMALLBUTTON));
            if (tempGameButton) {
                tempGameButton->getTag();
            }
        }
    }
    return -1;
}

int CSceneFriendMainLayer::touchQuitButton(CCPoint *point)
{
    return -1;
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
    return  true;
}