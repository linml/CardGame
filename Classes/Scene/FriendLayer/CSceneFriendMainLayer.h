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
    bool  addTab(const char* label,int index);
    void updateFriend();
    void createFriendText();
    void createBottomButton();
    
    void  setTabMargin(float marginLeft,float marginTop);
    
    CCNode*  getContainer();
    CCSize getContainerContentSize();
    void setContainerContentSize(const CCSize& size);
    CCPoint getContainerPosition();
    void setContainerPosition(const CCPoint& pos);
    unsigned int   getTabCount();
 
    CCPoint getTabPositionByIndex(int index);
    bool  switchToTab(int index);
    int touchTableIndex(CCPoint point);
    int touchBottomButton(CCPoint point);
    int touchQuitButton(CCPoint *point);

protected:
    CCArray*          m_tabs;
    CCLayer*          m_container;
    CCSprite*         m_bgSprite;
    CCSpriteFrame*    m_tabNormalFrame;
    CCSpriteFrame*    m_tabSelectedFrame;
    std::string       m_fontName;
    int               m_fontSize;
    int               m_currentTabIndex;
    CCSize size;
    int m_nFriendMax;
};


#endif /* defined(___1_cube__CSceneFriendMainLayer__) */
