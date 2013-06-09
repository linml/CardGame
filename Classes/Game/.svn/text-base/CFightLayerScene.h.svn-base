//
//  CFightLayer.h
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#ifndef ___1_cube__CFightLayerScene__
#define ___1_cube__CFightLayerScene__

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
struct SATKLIST {
    int gongjiZheindex;
    int gongjiShanghai;
    int gongjiJiNeng;
    int gongjiZijijiaxue;
    int gongji;
};


class CFightLayerScene :public CCLayer{
public:
    CFightLayerScene();
    ~CFightLayerScene();
    CREATE_FUNC(CFightLayerScene);
    void  updateGetGameDataToGetServerRandow();
public:
    static CCScene *scene();
    bool init();
private:
    void createOwnFightCardPosition();
    void setHp(CCardSprite *pCardSprite,CCLabelTTF *labelttf);
    void createMonster();
    void updateHp(CCardSprite *pCardSprite,CCardSprite *pMonsterCardSprite);
    void setText(const char *data);
    int  getWinStats();
    void dealWithFight(CCObject *object);
private:
    vector<SFightCardSprite *>m_vfightCardSprite;
    vector<SFightCardSprite *>m_vMonsterCardSprite;
    class CCardLayer  *currOwnCardSprite;
    class CCardLayer  *monsterCardSprite;
    int currIndexBegin;
    int currIndexEnd;
};

#endif /* defined(___1_cube__CFightLayer__) */
