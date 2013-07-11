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
class CConfigResourceLoad :public CCObject
{
public:
    bool loadCardInfo(map<int,CCard *> &hashmapAllCard,const char *fileName);
    bool loadPlayerLevelInfo(vector<SLevelPlayer *>*vPlayerLevel,const char *fileName);
public:
    
};
typedef Singleton<CConfigResourceLoad> G_SingleCConfigResourceLoad;
#endif /* defined(___1_cube__CConfigResourceLoad__) */