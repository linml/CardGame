//
//  CHallScene.h
//  91.cube
//
//  Created by phileas on 13-6-7.
//
//

#ifndef ___1_cube__HallScene__
#define ___1_cube__HallScene__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "AsgardLayer.h"


#define HALL_TOUCH_PRORITY (-1)

using namespace std;
USING_NS_CC;

class CHallScene : public CCLayer {
    
public:
    static CCScene *scene(int pType = 0);
    static CHallScene *create(int pType = 0);

public:
    CHallScene();
    ~CHallScene();

public:
    virtual bool init(int inType = 0);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void createEmailNumberUnread();
    void updateEmailNumber(CCObject *object);
    
    CAsgardLayer* getAsgardLayer(){return m_pAsgardLayer;};
    
protected:
    bool initHall(int inType = 0);
    void handlerTouch();
    void createEmailLayer();
    void showBackBag();
    void setHallSceneLayerCanTouch(void *tag);
    void showBackNotice(CCObject *object);
    void callBackToShowRechargeDialog(CCObject *object);
    void showRechargeView();
    void showNiuDanJieMian();
    void createPropBufferTips();
protected:
    int m_nTouchTag;
    CCSprite* m_touchSprite;
    LayoutLayer* m_cplist;
    CAsgardLayer *m_pAsgardLayer;
    vector<TouchRect> m_cTouches;
    

};

#endif /* defined(___1_cube__CHallScene__) */
