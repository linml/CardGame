//
//  FightResultConfirm.h
//  91.cube
//
//  Created by phileas on 13-6-17.
//
//

#ifndef ___1_cube__FightResultConfirm__
#define ___1_cube__FightResultConfirm__

#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace std;
using namespace cocos2d;
// m_iFightType 0 : fight with npc, 1: fight with team
class SFightResultData
{
public:
    SFightResultData(){m_bHuiFang=false; m_iFightResult=0;m_iFightType=0;m_iFightUid=100000+rand()%100000;}
    ~SFightResultData(){};
public:
    CC_SYNTHESIZE(bool, m_bHuiFang, HuiFang);
    CC_SYNTHESIZE(int, m_iFightResult, FightResult);
    CC_SYNTHESIZE(int, m_iFightType, FightType);
    CC_SYNTHESIZE(int, m_iFightUid, FightUid);
};


class FightResultConfirm : public CCLayerColor {
    
public:
    CREATE_FUNC(FightResultConfirm);
    
public:
    FightResultConfirm();
    virtual ~FightResultConfirm();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void postHttpNpc();
    void postHttpTeam();
    void callBackData(CCObject *object);    
protected:
    void initFightResultConfirm();
    void handlerTouch();
    
    void handlerEventReward(CCDictionary * inAllReward);
    // test:
    void test_print(int code);
    void test_print(const char * inMsg);
protected:
    SFightResultData * m_nResult;
    int m_nTouchTag;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
};


#endif /* defined(___1_cube__FightResultConfirm__) */
