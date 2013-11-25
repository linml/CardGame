//
//  CAddFriendLayer.h
//  91.cube
//
//  Created by linminglu on 11/15/13.
//
//

#ifndef ___1_cube__CAddFriendLayer__
#define ___1_cube__CAddFriendLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;

class CAddFriendLayer :public CCLayer
{
public:
    CAddFriendLayer();
    ~CAddFriendLayer();
    bool init();
    void sendAddFriend();
    void decodeAddFriend(CCObject *object);
    void createTitle();
    void createEdit();
    void createBackGround();
    void removeAction();
    void createSearchButton();
    void createLabelTip();
    void menuAddUser(CCObject* pSender);
    void menuQuit(CCObject *pSender);
    CREATE_FUNC(CAddFriendLayer);
    void removeTTF();
private:
    void runLabelTTFAction(string str,bool bSuccess);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
private:
    CCEditBox *m_pEditUid;
    
    
};

#endif /* defined(___1_cube__CAddFriendLayer__) */
