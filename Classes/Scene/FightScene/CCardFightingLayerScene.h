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
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    int m_iFightingCardIndex;
    int m_iMonsterCardIndex;
    int m_iHuihe;
private:
    bool m_bIsAnimationEnd;
    int  m_iAnimationIndex;
    vector<SAnimationFps *>m_listAnimationlist;
public:
    void AnimaitonEnd(CCObject *obj);
};

#endif /* defined(___1_cube__CCardFightingLayerScene__) */
