//
//  CEveryDayLoginLayer.h
//  91.cube
//
//  Created by linminglu on 12/9/13.
//
//

#ifndef ___1_cube__CEveryDayLoginLayer__
#define ___1_cube__CEveryDayLoginLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CGameButtonControl;
class CGamePlayer;
class CEveryDayLoginDataStruct;
class CEveryDayLoginRewardDataContainer;
class CEveryDayLoginLayer :public CCLayer
{
public:
    CEveryDayLoginLayer();
    ~CEveryDayLoginLayer();
    bool init();
    CREATE_FUNC(CEveryDayLoginLayer);

    void updateLoad();
    CCPoint getItemPostion(int index);
    int  getTouchindex(CCPoint point);
    ccColor3B  getItemBackGroundColor(CEveryDayLoginDataStruct *item);
protected:
    void createColorBackGroud();
    void createLayer();
    void createQuitbutton();
    void createQianDaoButton();
    void createTodayInfo(CEveryDayLoginDataStruct *item);
    CCLayer *createItemSprite(CEveryDayLoginDataStruct *);
    void sendQianDao();
    void callBackGetLingQu(CCObject *object);
    
    void runCCLayerBel();
    void removeCCLayerber();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
    void  handTouchTag(int tag);
    int  checkTouchTag(CCPoint point);
private:
    CGamePlayer *m_tempPlayer;
    int toDay;
    CEveryDayLoginRewardDataContainer *m_pQianDaoManager;
    CCSize size;
    CCArray * m_vTouchArray;
    
};

#endif /* defined(___1_cube__CEveryDayLoginLayer__) */
