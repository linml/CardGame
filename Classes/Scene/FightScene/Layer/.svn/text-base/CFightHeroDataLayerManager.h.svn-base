//
//  CFightHeroDataLayerManager.h
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#ifndef ___1_cube__CFightHeroDataLayerManager__
#define ___1_cube__CFightHeroDataLayerManager__

#include <iostream>
#include <list>
#include "cocos2d.h"
#include "gameConfig.h"
using namespace std;
using namespace cocos2d;
class CFightBufferPngSprite;
//该处 管理的 HP,ENGRY,BUFFER
struct CFightBufferPngValue
{
    int effectid;
    EN_LEFTTEAMORRIGHTTEAM enType;
    int nAtkIndex;
    bool isAddChild;
    int tag;
};
class CFightHeroDataLayerManager :public cocos2d::CCLayer {
public:
    CFightHeroDataLayerManager();
    ~CFightHeroDataLayerManager();
    void appendBuffer(int effectid,int nAtk,EN_LEFTTEAMORRIGHTTEAM enType);
    void everyHuiheDealWith(int nAtk,EN_LEFTTEAMORRIGHTTEAM enType);
    bool init();
    void setCurrentTeamAtkIndex(EN_LEFTTEAMORRIGHTTEAM enTeamPostion, int nLeftAtkIndex,int nRightAtkIndex);
    CREATE_FUNC(CFightHeroDataLayerManager);
    CCPoint getPngFilePostion(EN_LEFTTEAMORRIGHTTEAM enTeamPostion,int currPngCount);
    void initUpdateBuffer(EN_LEFTTEAMORRIGHTTEAM enTeamPostion);
public:
    EN_LEFTTEAMORRIGHTTEAM m_enTeamPostion;
    int m_nLeftAtk;
    int m_nRightAtk;
    list<CFightBufferPngValue *>m_listBufferList;
    int m_nLeftBufferCount;
    int m_nRightBufferCount;
};

#endif /* defined(___1_cube__CFightHeroDataLayerManager__) */
