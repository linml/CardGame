//
//  CFightingCardLayerScene.h
//  91.cube
//
//  Created by linminglu on 13-7-11.
//
//

#ifndef ___1_cube__CFightingCardLayerScene__
#define ___1_cube__CFightingCardLayerScene__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"
using namespace cocos2d;
using namespace std;
class CFightCard;
class CAnimationSpriteGameFight;
class SEveryATKData;

class CFightingCardLayerScene :public CCLayer
{
public:
    static CCScene *scene();
public:
    CFightingCardLayerScene();
    ~CFightingCardLayerScene();
public:
    bool init();
    CREATE_FUNC(CFightingCardLayerScene);
public:
    void locgicSchudel(float t);   ///抽离出来 让其在这边做回调
    void appendHpAngryUpdate();
    EN_GAMEFIGHTSTATUS  getWinStatus();
    EN_ATKFIGHT_INDEX   getHuiHeIndex();
    int  m_iTotalHuihe;
    void animationSchudel(float t);
    int  animationAndex;
    bool isAnimationEnd;
    int  hpUpdateIndex;
    int m_itotalAnimation;
private:
    void textSkillInfo(CAnimationSpriteGameFight *fight);
private:
    void logicFighting();
    EN_ATKFIGHT_INDEX m_enHuiheIndex;                   //记录打斗者的索引。
    EN_GAMEFIGHTSTATUS  m_enWinStatus;
private:
    void createFightCard();
    void createMonsterCard();
    bool initBggroudPng();
    bool initAtkPng();
    bool initText();
    void createHero();
    bool initHitText();
    void yinCangRenWu(vector<CCSprite *>vsprite,CCSprite *sprite=NULL);
    void skillByHeloSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void initSetUpdateHp(int iCurrHp,int TotalHp,int currEngry,bool isLeft);
    
    // 创建显示文本比如，怒气，Hp信息;
    void createEngryText();
    void createHpText();
    void updateHpAndAngry();
    void initHpEngry();

    
    //
    CCPoint getCardPoint(int  index, bool isLeftCard);
    void animationSwf( CAnimationSpriteGameFight *fightAnimation);
    void skillAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void skillHelpAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSpriteGameFight *fightAnimation);
private:
   void  winDialog();
    void loseDialog();
private:
    void loadFromServerTest();
    void initGame(); 
    bool checkIsDead(); //判断对方是否死亡
    bool checkSendZengfu(); //判断是否发送增幅技能
    bool checkFighting();
    CAnimationSpriteGameFight *m_currCAnimationHP;
    void showHpAnimation(CCObject *object);
    void AnimaitonEnd(CCObject *object);
    void showHp(int leftHp,int RightHp); //回调该方法显示血液
    void moveCardSprite(vector<CFightCard *> &vCard,int goIndex,bool isLeft);
    
private: 
    void  animationShouShang(CCNode *node,void *tag);
    
public:
   
    vector<CCSprite *>m_vFightHero;
    vector<CCSprite *>m_vMonsterHero;
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    int m_iFightCardIndex;
    int m_iMonsterCardIndex;
    vector<CAnimationSpriteGameFight *>m_vAnimation;
    vector<SEveryATKData*>m_vHpAngry;
};

#endif /* defined(___1_cube__CFightingCardLayerScene__) */
