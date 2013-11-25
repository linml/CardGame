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


class CLoginScene : public cocos2d::CCLayer,public cocos2d::extension::CCTableViewDataSource,public cocos2d::extension::CCTableViewDelegate
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
    void doLogin(const char* usrName,const char* password);
    void onReceiveLoginMsg(CCObject* obj);
    
    void getServerInf();
    void onReceiveServerMsg(CCObject* obj);
    void onReceiveRegisterServerMsg(CCObject* obj);
    
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};

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
    CCTableView* m_serverTable;
    CCEditBox* m_pEditName;
    set<int> m_serverSet;
    int m_nSelectServerId;
    
};

#endif /* defined(___1_cube__LoginScene__) */
