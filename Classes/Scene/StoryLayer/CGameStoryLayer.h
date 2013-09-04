//
//  CGameStoryLayer.h
//  91.cube
//
//  Created by linminglu on 13-9-4.
//
//

#ifndef ___1_cube__CGameStoryLayer__
#define ___1_cube__CGameStoryLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class CGameTalkDialog;
class CGameStoryLayer :public CCLayer
{
public:
    CGameStoryLayer();
    ~CGameStoryLayer();
static CGameStoryLayer *CreateStoryLayer(int storyId,CCNode *node=NULL);
private:
bool initCreateStory(int storyId,CCNode *node);
void createKuaiJing();
string getStoryTalkList(int storyID);
void  updateTimeToShow(float t);
void  endTalk();
void  loadDialogList();
void  displayDialog(CGameTalkDialog *gameDialog);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
private:
    bool m_bIsEndDialog;
    int m_vDialogTalkIndex;
    vector<int >m_vDialogTalk;
    vector<CGameTalkDialog *>m_vGameTalkDialog;
    CGameTalkDialog *m_oldGameDialog; //用来判断是否要做动画
    CCSize wndSize;
};

#endif /* defined(___1_cube__CGameStoryLayer__) */
