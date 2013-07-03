//
//  CCardFightingLayerScene.h
//  91.cube
//
//  Created by linminglu on 13-6-27.
//
//

#ifndef ___1_cube__CCardFightingLayerScene__
#define ___1_cube__CCardFightingLayerScene__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
class CFightCard;
class SAnimationFps;

class CCardFightingLayerScene : public CCLayer
{
public:
    static CCScene *scene();   
public:
    CCardFightingLayerScene();
    ~CCardFightingLayerScene();
public:
    bool init();
    CREATE_FUNC(CCardFightingLayerScene);
public:
    void fSchudelUpdate(float t);
    void locgicSchudel(float t);
private:
    void createFightCard();
    void createMonsterCard();
    int getWinStats();
    void fightLogic(int &iHuihe);
    void checkOwnIsDeadAndMove();
    void checkMonsterIsDeadAndMove();
    void animationSwf(int skillIndex,vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs);
    void MoveCardSprite(vector<CFightCard *> &card);
    void basicAnimation(vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs);
    
    void showJuqi(CCNode *node,void *tag);
    void animationShouShang(CCNode *node,void *tag);
    void setVistablHit(CCObject *object);
    void hideJuqi(CCNode *node,void *tag);
    void animationHpFight(CCNode *node,void *tag);
    void animationHpMonster(CCNode *node,void *tag);
    void updateSetHp(CCObject *object);
    void setHideHpAnimation(CCObject *obj);
    void AnimaitonEnd(CCObject *obj);
    void animationCardPanel(class CCNode *card, void *tag);
    
private:
    void winDialog();
    void loseDialog();
    
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    int m_iFightingCardIndex;
    int m_iMonsterCardIndex;
    int m_iHuihe;
    SAnimationFps * m_sfps;
private:
    bool m_bIsAnimationEnd;
    int  m_iAnimationIndex;
    vector<SAnimationFps *>m_listAnimationlist;
    int  m_iWinStatus;
public:
   
};

#endif /* defined(___1_cube__CCardFightingLayerScene__) */
