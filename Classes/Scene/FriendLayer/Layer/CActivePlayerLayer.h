//
//  CActivePlayerLayer.h
//  91.cube
//
//  Created by linminglu on 13-12-17.
//
//

#ifndef ___1_cube__CActivePlayerLayer__
#define ___1_cube__CActivePlayerLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class ActivePlayer;
class  CActivePlayerLayer :public CCLayer
{
public:
    ~CActivePlayerLayer();
    bool init();
    CREATE_FUNC(CActivePlayerLayer);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
    void createGetNext();
    void handleTouchTag(int tag);
    void getNextData();
    void decodeAddFriend(CCObject *object);
    void runLabelTTFAction(string str,bool flag);
    void removeTTF();
public:
    CCPoint getIndexPostion(int index);
    void createItem(ActivePlayer *pFriend,int index);
    void createFriendItem();
    void sendUpdateActiveFriend();
    void callBackSendUpdate(CCObject *object);
public:
    vector<ActivePlayer *> m_vActiverFriendList;
    CCSize size;
    CCArray *m_vTouchArray;
    int m_nSendUidIndex;
};



#endif /* defined(___1_cube__CActivePlayerLayer__) */
