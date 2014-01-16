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
#include "PVPDataManager.h"
#include "PVPRankRewardConfig.h"
#include "CPVPAddTiaoZhanCountLayer.h"
#include "CPVPPublicStruct.h"
class CPVPMonsterData;
#define  PVPSCENETOUCH_PRIORITY -10001
using namespace cocos2d;

#define PVPSCENEBTNCOUNT 8


typedef struct
{
    int date = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
}TIMESTRUCT;



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
    
    
    void callBack();
protected:
    void initPVPSceneLayerUI();
    void createPlayerPVPInfo(PVPRankData *inUserRankInfo);
    void createPVPRankInfo(PVPRankData *inHeadRankInfo, int inCount);
    void createPlayePVPRankReward(PVPRankReward * inRewardData);
    void createTiaoZhanCountInfo();
    
    void updatePVPRankReward();
    
    CCNode* createRankNode(PVPRankData * inRankData);
    
    void updateTime(float dt);
    void updateTimeTip();
    void reloadData();
    // 点击事件:
    void onClickRule(); // 规则
    void onClickBattleLog(); // 战报
    void onClickChallengeReward(); //挑战奖励
    void onClickHuDian(); //魂殿
    void onClickSearchOpponent(); // 搜索对手
    void onClickPVPRankReward(); //领取排名奖励
    void onClickBack();
    void onClickPVPRank(); //排行榜

    //get pvp rank reward:
    void sendRequestGetPVPRankReward();
    void receiveGetPVPRankRewardMsg(CCObject* pObject);
    

    ////回调搜索对手时候的东西 bymerlin
    void callBackValue(CCObject *object);
    void addPvpSearchLayer(CPVPMonsterData *data);

    
    

protected:
    CGamePlayer *m_pPlayer;
    PVPDataManager *m_pDataManager;
    int m_nTouchTag;
    bool m_bCanTouch;
    bool m_bCanGetPVPRankReward;
    CCSprite *m_pBgContainer;

    // btn:
    CCNode* m_pBtns[PVPSCENEBTNCOUNT]; //0->规则; 1->战报; 2->挑战奖励; 3->魂殿; 4->搜索对手;5->增加挑战次数;6->排行榜;7->返回;8->领取排名奖励;

    CCLabelTTF *m_pTimeLabel;
    CCLabelTTF *m_pCoinLabel;
    CCLabelTTF *m_pLabel[3];
    TIMESTRUCT m_cOverTime;
    
    CCNode *m_pPVPRankRewardPanel;
    CCNode *m_pPlayerPVPPanel;
    CPVPAddTiaoZhanCountLayer *m_pAddTiaoZhanCountPanel;


public:
        static int m_nProtect_time;

};

#endif /* defined(___1_cube__PVPSceneLayer__) */
