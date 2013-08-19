//
//  CFightingCardLayerLogic.h
//  91.cube
//
//  Created by linminglu on 13-7-15.
//
//
/**
 *该类为计算打斗时候的界面逻辑类
 *
 */

#ifndef ___1_cube__CFightingCardLayerLogic__
#define ___1_cube__CFightingCardLayerLogic__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "gameConfig.h"
#include <list>

using namespace cocos2d;
using namespace std;
class CFightCard;
class SEveryATKData;
class CAnimationSpriteGameFight;
class CCardBufferStatus;
class CFightCardFightingBuffer;
class CFightingCardLayerLogic :public CCObject
{
public:
    CFightingCardLayerLogic();
    ~CFightingCardLayerLogic();
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    int m_iFightCardIndex;
    int m_iMonsterCardIndex;
    int  m_iTotalHuihe;    //总的回合数字
    vector<string> m_vSskillFile;
    vector<string> m_vNeedAnimate;
    
public:
    void initFightLogic(int  loadTeamIndex);
private:
    bool loadFromServerTest(int  loadTeamIndex);    //!<     读取当前卡牌阵容，
    bool loadFromCardTeamTest();                    //!<     获得怪物的信息
private:
    bool checkIsDead();                             //!<     判断对方是否死亡
    bool checkSendZengfu();                         //!<     判断是否发送增幅技能
    bool checkFighting();
    bool checklogicBuffAndDead();
    bool checklogicCheckIsCanSendAtk();
    bool whenDeadSendDeadSkillAndMove();
    void appendUpdateAction();
    void appendHpAngryUpdate();
    void appendUpdateBuffer();
    int  getNextFightCard(int index,bool isLeftFightTeam);
   
public:
    bool logicFighting();
    void loadAnimatePlist();                        //!<    加载animation的plist文件里面的帧动画
public:
    EN_GAMEFIGHTSTATUS  getWinStatus();
    EN_ATKFIGHT_INDEX   getHuiHeIndex();
private:
    EN_ATKFIGHT_INDEX m_enHuiheIndex;                   //记录打斗者的索引。
    EN_GAMEFIGHTSTATUS  m_enWinStatus;
private:
    list<CCardBufferStatus *> m_lpFightBuffer;
    list<CCardBufferStatus *> m_lpMonsterBuffer;
};

#endif /* defined(___1_cube__CFightingCardLayerLogic__) */
