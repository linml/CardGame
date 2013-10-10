//
//  LoginLayer.h
//  91.cube
//
//  Created by phileas on 13-6-5.
//
//

#ifndef ___1_cube__LoginScene__
#define ___1_cube__LoginScene__

#include <string>
#include <vector>

using namespace std;
#include "cocos2d.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"


#define REGITER_SUCCESS "RegiterSuccess"


// it should move ot one macro file
#define LOGO_ZORDER (5)
#define LOGIN_ZORDER (0)
#define BUTTON_PLAY_TAG (2001)



USING_NS_CC;


class CLoginScene : public CCLayer
{
public:
    CREATE_FUNC(CLoginScene);
    static  CCScene *scene();
    
public:
    CLoginScene();
    ~CLoginScene();

public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void loginCallBack(CCNode * pSender);
    void setLogoOverCallBack();
    void callBackObject(CCObject *obj);
    void msgCallback(CCObject* obj);
    void serverInf(CCDictionary* dic);
    
    void scheudoLoadGameConfig(); //异步加载配置
    void addFunctionInitGames(float t);
    bool addLabelToShowPrecessInfo();
    void setText(const char *str);
    
    void playGame();
    
    void notificationRegiterRecevice(CCObject* obj);
    
    void doLogin();
    void onReceiveLoginMsg(CCObject* obj);
    
protected:
    bool initLogin();
    bool handleTouchSpritePool(CCPoint point);
    bool isTouchPlayerGame;
    void doingCheckComplateTask();
    void dointAddTask();
    void dealWithCompleteTask(CCObject *object);
    void dealWithAddTask(CCObject *object);
    
protected:
    bool m_bLogoOver;
    LayoutLayer *maps;
    vector<TouchRect> touchRect; 
    vector<stcServerInf> vSerInf;
    bool isLoadCardBag;
    bool isLoadEndConfig;
    bool isLoadTeam;
    bool isGameInit;
    bool isLoadBackPack;
    
};

#endif /* defined(___1_cube__LoginScene__) */
