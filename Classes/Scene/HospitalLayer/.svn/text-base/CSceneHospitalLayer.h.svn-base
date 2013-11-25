//
//  CSceneHospitalLayer.h
//  91.cube
//
//  Created by linminglu on 11/14/13.
//
//

#ifndef ___1_cube__CSceneHospitalLayer__
#define ___1_cube__CSceneHospitalLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CGamePlayer ;

class CSceneHospitalLayer :public cocos2d::CCLayer {
public:
    CSceneHospitalLayer();
    ~CSceneHospitalLayer();
    bool init();
    CREATE_FUNC(CSceneHospitalLayer);
public:
    void createBackGroud();
    void createTitle();
    void createPeople();
    void createButton();
    void createQuitButton();
    void createColorLayer();
    void createContext();
    void updateHospitolData(float t);
    void sendToserverToAddGp();
    void decodeServerAddGp(CCObject *object);
    void setAddGpText(string str);
    void sendHttpToAskTimer();
    void decodeSubTimer(CCObject *object);
    void updateContext();
    
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
private:
    CCSize size;
    CGamePlayer *m_tempGamePlayer;
    CC_SYNTHESIZE(int, m_vYiWuShiShiJian, YiWuShiShiJian);
    CC_SYNTHESIZE(bool, m_bCanUseHostpital, CanUseHospital);
    bool isCanTouch;
    
};

#endif /* defined(___1_cube__CSceneHospitalLayer__) */
