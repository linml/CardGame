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
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
private:
    int m_iFightingCardIndex;
    int m_iMonsterCardIndex;
    int m_iHuihe;
public:
    
};

#endif /* defined(___1_cube__CCardFightingLayerScene__) */
