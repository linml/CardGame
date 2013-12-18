//
//  CSceneFriendMainLayer.h
//  91.cube
//
//  Created by linminglu on 11/14/13.
//
//

#ifndef ___1_cube__CSceneFriendMainLayer__
#define ___1_cube__CSceneFriendMainLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CSceneFriendMainLayer :public CCLayer
{
public:
    CSceneFriendMainLayer();
    ~CSceneFriendMainLayer();
    bool init();
    CREATE_FUNC(CSceneFriendMainLayer);
    void createTabControl();
    void createBackGround();
    void createIDText();
    bool addTab(const char* label,int index);
    void updateFriend(CCObject *object);
    void updateActiveFriend(CCObject *object);
    void createFriendText();
    void createBottomButton();
    void createQuitButton();
    void createHaoyouLieBiao();
    void createActiveFriend();
    void handleTagCallBack(int tag);
    
    CCNode*  getContainer();
    CCSize getContainerContentSize();
    void setContainerContentSize(const CCSize& size);
    CCPoint getContainerPosition();
    void setContainerPosition(const CCPoint& pos);
    unsigned int   getTabCount();
 
    CCPoint getTabPositionByIndex(int index);
    bool  switchToTab(int index);
    int checkTouchTableIndex(CCPoint point);
    int checkTouchBottomButton(CCPoint point);
    int checkTouchQuitButton(CCPoint point);
public:
    void selectFriendByUserId();
    void sendAllZanMei();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
protected:
    void onClickApplyList();
    void onClickPriaseList();
protected:
    CCArray*          m_tabs;
    CCLayer*          m_container;
    int               m_currentTabIndex;
    CCSize            size;
    int               m_nFriendMax;
    CCObject *pObject;
    SEL_CallFunc callback[2];
    
};


#endif /* defined(___1_cube__CSceneFriendMainLayer__) */
