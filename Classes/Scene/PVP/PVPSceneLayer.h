//
//  PVPSceneLayer.h
//  91.cube
//
//  Created by phileas on 14-1-7.
//
//

#ifndef ___1_cube__PVPSceneLayer__
#define ___1_cube__PVPSceneLayer__

#include "cocos2d.h"
#include <string>
#include "gamePlayer.h"

#define  PVPSCENETOUCH_PRIORITY -30001
using namespace cocos2d;

#define PVPSCENEBTNCOUNT 9
#include "CPVPPublicStruct.h"
class CPVPMonsterData;
class PVPSceneLayer : public CCLayer
{
public:
    static PVPSceneLayer * create();
public:
    PVPSceneLayer();
    virtual ~PVPSceneLayer();
    virtual bool init();
    static cocos2d::CCScene* scene();

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initPVPSceneLayerUI();
    void createPlayerPVPInfo();
    void createPVPRankInfo();
    void createPlayePVPRankReward(PVPRankReward * inRewardData, int inTime = -1);
    void createTiaoZhanCountInfo();
    
    CCNode* createRankNode(PVPRankData * inRankData);
    
    
    // 点击事件:
    void onClickRule(); // 规则
    void onClickBattleLog(); // 战报
    void onClickChallengeReward(); //挑战奖励
    void onClickHuDian(); //魂殿
    void onClickSearchOpponent(); // 搜索对手
    void onClickAddPVPCount(); //增加挑战次数
    void onClickPVPRankReward(); //领取排名奖励
    void onClickBack();
    void onClickPVPRank(); //排行榜
    
    ////回调搜索对手时候的东西 bymerlin
    void callBackValue(CCObject *object);
    void addPvpSearchLayer(CPVPMonsterData *data);

    
    
protected:
    CGamePlayer *m_pPlayer;
    int m_nTouchTag;
    CCSprite *m_pBgContainer;

    // btn:
    CCNode* m_pBtns[PVPSCENEBTNCOUNT]; //0->规则; 1->战报; 2->挑战奖励; 3->魂殿; 4->搜索对手;5->增加挑战次数;6->排行榜;7->返回;8->领取排名奖励;
public:
        static int m_nProtect_time;
};

#endif /* defined(___1_cube__PVPSceneLayer__) */
