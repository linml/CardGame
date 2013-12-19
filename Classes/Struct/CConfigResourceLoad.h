//
//  CConfigResourceLoad.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CConfigResourceLoad__
#define ___1_cube__CConfigResourceLoad__

#include <iostream>
#include "cocos2d.h"
#include "CSingleton.h"
#include <map>
#include <vector>
using namespace cocos2d;
using namespace std;
class CCard;
class SLevelPlayer;
class CImapact;
class CSkillData;
class CStructShopInfo;
class CStructStrips;
class CStructShopInfoManager;
class CConfigResourceLoad :public CCObject
{
public:
    bool loadCardInfo(map<int,CCard *> &hashmapAllCard,const char *fileName);
    bool loadPlayerLevelInfo(map <int ,SLevelPlayer *> &vPlayerLevel,const char *fileName);
    bool loadSkillLogicInfo(map<int, CSkillData *> &vPlayerLevel,const char *fileName);
    bool loadEffectLogicInfo(vector<CImapact *> &vPlayerLevel,const char *fileName);
    bool loadNPCCardInfo(map<int ,CCard *>&hashMapNpcCardAll,const char *fileName);
    bool loadShopSellItem(CStructShopInfoManager *shopInfoManager,const char *fileName);
    bool loadSkillStripTable(map<int ,CStructStrips *>&mapStrip,const char *fileName);
public:
    
};
typedef Singleton<CConfigResourceLoad> G_SingleCConfigResourceLoad;
#endif /* defined(___1_cube__CConfigResourceLoad__) */
