//
//  CFightLayer.h
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#ifndef ___1_cube__CFightLayer__
#define ___1_cube__CFightLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class CCardSprite;
struct SFightCardSprite {
    CCardSprite *cardsprite;
    bool isDead;
    int  tag;
};


class CFightLayer :public CCLayer{
public:
    CFightLayer();
    ~CFightLayer();
    CREATE_FUNC(CFightLayer);
    void  updateGetGameDataToGetServerRandow();
public:
    bool init();
private:
    void createOwnFightCardPosition();
    void setHp(CCardSprite *pCardSprite,CCLabelTTF *labelttf);
    void createMonster();
    void updateHp(CCardSprite *pCardSprite,CCardSprite *pMonsterCardSprite);
    void setText(const char *data);
    int  getWinStats();
    void dealWithFight();
private:
    vector<SFightCardSprite *>m_vfightCardSprite;
    vector<SFightCardSprite *>m_vMonsterCardSprite;
    class CCardLayer  *currOwnCardSprite;
    class CCardLayer  *monsterCardSprite;
    int currIndexBegin;
    int currIndexEnd;
};

#endif /* defined(___1_cube__CFightLayer__) */
