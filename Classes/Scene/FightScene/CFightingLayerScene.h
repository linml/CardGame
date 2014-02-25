//
//  CFightingLayerScene.h
//  91.cube
//
//  Created by linminglu on 11/5/13.
//
//

#ifndef ___1_cube__CFightingLayerScene__
#define ___1_cube__CFightingLayerScene__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"
using namespace cocos2d;
using namespace std;
class CFightCard;
class CAnimationSctrip;
class SEveryATKData;
class CGameCardBuffer;
class SFightResultData;
class CGamePlayer;
class CGameCardFactory;
class CFightingLayerScene :public CCLayer
{
public:
    static CCScene *scene();
public:
    CFightingLayerScene();
    ~CFightingLayerScene();
public:
    bool init();
    CREATE_FUNC(CFightingLayerScene);
public:
    EN_GAMEFIGHTSTATUS  getWinStatus();
    EN_ATKFIGHT_INDEX   getHuiHeIndex();
    void animationSchudel(float t);
    int  m_nAnimationAndex; //当前动画做到第几帧
    bool m_bAnimationEnd;
    int  m_nTotalAnimation ;//总共多个动画
    int  m_nHpEngryIndex;
    int  m_nBufferIndex;
   
private:
    void textSkillInfo(CAnimationSctrip *fight);
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
    void skillByHeloSwf(CAnimationSctrip *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void initSetUpdateHp(int iCurrHp,int TotalHp,int currEngry,bool isLeft);
    void actionReorderZorder(CCNode  *object,void *tag);
    void actionReorderZorderNode(CCObject *object);
    void delayToSchude(CCObject *object);
    void createShowFightUid();
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
    void clearAllHeroTexiao();
private:
    CCPoint getBufferIconPostion(int index,bool isLeft);//读取buffer图标的位置
    CCPoint getCardPoint(int index, bool isLeftCard);
    void skillAnimationSwf(CAnimationSctrip *fightAnimation,CCSprite *pFight,CCSprite *pMonster);
    void animationSwf(CAnimationSctrip *fightAnimation);
    void showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSctrip *fightAnimation);
    void showHpShanghai(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSctrip *fightAnimation);
private:
    void  winDialog();
    void  loseDialog();
private:
    void loadFromServerTest();
    CAnimationSctrip *m_currCAnimationHP;
    void showHpAnimation();
    void AnimaitonEnd();
    void showHp(int leftHp,int RightHp); //回调该方法显示血液
    void moveCardSprite(vector<CFightCard *> &vCard,int goIndex,bool isLeft);
    void SetCardOnGameBeiginFirstPosition();
    
    
public:
    vector<CCSprite *>m_vFightHero;
    vector<CCSprite *>m_vMonsterHero;
    
    CCSprite *HeroFightCard[2];  //准备做一个方式调用的精灵图片只有一张的方式
    
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    vector<CGameCardBuffer *>m_leftBuffer;
    vector<CGameCardBuffer *>m_rightBuffer;
    
private:
    SFightResultData *m_pSFightData;
    CGamePlayer *m_gamePlayer;
    CCActionManager *actionManager2;
    CCActionManager *actionManager1;
    CCScheduler *schudel1;
    CCScheduler *schudel2;
    CGameCardFactory *m_pTotalFrightMana;
    
};
#endif /* defined(___1_cube__CFightingLayerScene__) */
