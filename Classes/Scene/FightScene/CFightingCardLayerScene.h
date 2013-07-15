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
using namespace cocos2d;
using namespace std;
class CFightCard;

enum EN_GAMEFIGHTSTATUS {
    EN_GAMEFIGHTSTATUS_WIN,   //胜利
    EN_GAMEFIGHTSTATUS_LOSE,  //失败，
    EN_GAMEFIGHTSTATUS_NONE,  //需要继续战斗，
};

enum  EN_ATKFIGHT_INDEX
{
    EN_ATKFIGHT_INDEX_NONE,
    EN_ATKFIGHT_INDEX_LEFT_LORD,
    EN_ATKFIGHT_INDEX_LEFT_SUPPORT,
    EN_ATKFIGHT_INDEX_RIGHT_LORD,
    EN_ATKFIGHT_INDEX_RIGHT_SUPPORT
};

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
    void locgicSchudel(float t);
    EN_GAMEFIGHTSTATUS  getWinStatus();
    int  m_iTotalHuihe;
private:
    void logicFighting();
    EN_ATKFIGHT_INDEX m_enHuiheIndex;//记录是否是拥护打斗的。
    EN_GAMEFIGHTSTATUS  m_enWinStatus;
private:
    void createFightCard();
    void createMonsterCard();
    bool initBggroudPng();
    bool initAtkPng();
    bool initText();
    bool initHitText();
    CCPoint getCardPoint(int  index, bool isLeftCard);
private:
    void loadFromServerTest();
    void initGame();
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    int m_iFightCardIndex;
    int m_iMonsterCardIndex;
private:
    friend class CFightingCardLayerLogic ;
};

#endif /* defined(___1_cube__CFightingCardLayerScene__) */
