//
//  CPtPropUseManager.h
//  91.cube
//
//  Created by phileas on 13-10-29.
//
//

#ifndef ___1_cube__CPtPropUseManager__
#define ___1_cube__CPtPropUseManager__

#include "cocos2d.h"
#include "gamePlayer.h"
#include <map>
#include <vector>

using namespace std;
using namespace cocos2d;

#define EXTENDCARDPROPID 900004

enum USERTYPE
{
    NONE = 0,
    HUICHENG = 1,
    TANLU,
    XIAOBUFFER,
    HUIGP,
    HUIAP
};


class CPtPropUserManager
{
public:
    static CPtPropUserManager *getInstance();
    static void releaseManager();
protected:
    static CPtPropUserManager *s_pInstance;
public:
    vector<int>* getUserPropsBy(USERTYPE inUserType);
    int getGetHavePropIdByFunctionId(USERTYPE inUserType);
    int getFirstPropIdByFunctionId(USERTYPE inUserType);
protected:
    CPtPropUserManager();
    ~CPtPropUserManager();
    void loadDataFromFile();
    
protected:
    map<USERTYPE, vector<int> > m_cUserPropContainer;
    CGamePlayer *m_pPlayer;
};

#endif /* defined(___1_cube__CPtPropUseManager__) */
