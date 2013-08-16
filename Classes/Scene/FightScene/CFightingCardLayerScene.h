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
class CGameCardBuffer;

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
    EN_GAMEFIGHTSTATUS  getWinStatus();
    EN_ATKFIGHT_INDEX   getHuiHeIndex();
    void animationSchudel(float t);
    int  animationAndex;
    bool isAnimationEnd;
    int  hpUpdateIndex;
    int bufferIndex;
    int m_itotalAnimation;
private:
    void textSkillInfo(CAnimationSpriteGameFight *fight);
private:
    EN_ATKFIGHT_INDEX m_enHuiheIndex;                   //记录打斗者的索引。
    EN_GAMEFIGHTSTATUS  m_enWinStatus;
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
private:
    void onEnter();
    void onExit();
    void rePlayerRecode();
private:
    void createFightCard();
    void createMonsterCard();
    void deleteBackGamePlayerFightMonsterCard();
    bool initBggroudPng();
    bool initAtkPng();
    bool initText();
    void createHero();
    bool initHitText();
    void yinCangRenWu(vector<CCSprite *>vsprite,CCSprite *sprite=NULL);
    void skillByHeloSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void initSetUpdateHp(int iCurrHp,int TotalHp,int currEngry,bool isLeft);
    void actionReorderZorder(CCNode  *object,void *tag);
    void actionReorderZorderNode(CCObject *object);
    void delayToSchude(CCObject *object);
   
    
    // 创建显示文本比如，怒气，Hp信息;
    void createEngryText();
    void createHpText();
    void updateHpAndAngry();
    void hideAllHero();
    void initHpEngry();
    void updateBuffer();
    void clearUpVectorBuffer();
    void showVectorBuffer();
    void createKuaiJin();
    void resetCardPosition();

    
    //
    CCPoint getCardPoint(int  index, bool isLeftCard);
    void animationSwf( CAnimationSpriteGameFight *fightAnimation);
    void skillAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void skillHelpAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSpriteGameFight *fightAnimation);
    void showSkillBuffer(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSpriteGameFight *fightAnimation);
    CCPoint getBufferIconPostion(int index,bool isLeft);
    void actionPFightSkill(const char *fightName,CCSprite *pFight,CCSprite *pMonster);
    void actionHelpSprite(const char *fightName,CCSprite *pFight,CCSprite *pMonster);
private:
   void  winDialog();
    void loseDialog();
private:
    void loadFromServerTest();
    CAnimationSpriteGameFight *m_currCAnimationHP;
    void showHpAnimation();
    void AnimaitonEnd();
    void showHp(int leftHp,int RightHp); //回调该方法显示血液
    void moveCardSprite(vector<CFightCard *> &vCard,int goIndex,bool isLeft);
    
private: 
    void  animationShouShang(CCNode *node,void *tag);
    
public:
    vector<CCSprite *>m_vFightHero;
    vector<CCSprite *>m_vMonsterHero;

    CCSprite *HeroFightCard[2];  //准备做一个方式调用的精灵图片只有一张的方式
    
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    vector<CGameCardBuffer *>m_leftBuffer;
    vector<CGameCardBuffer *>m_rightBuffer;
};

#endif /* defined(___1_cube__CFightingCardLayerScene__) */
