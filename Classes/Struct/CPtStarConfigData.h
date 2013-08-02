//
//  CPtStarConfigData.h
//  91.cube
//
//  Created by phileas on 13-8-2.
//
//

#ifndef ___1_cube__CPtStarConfigData__
#define ___1_cube__CPtStarConfigData__

#include "cocos2d.h"
#include "CSingleton.h"
using namespace cocos2d;

class CPtStarConfigData :public CCObject
{

    CCDictionary * m_pConfigData;
public:
    CPtStarConfigData();
    virtual ~CPtStarConfigData();

    int getConstConin(const int &inCardId);

};

typedef Singleton<CPtStarConfigData> SingleStarConfigData;

#endif /* defined(___1_cube__CPtStarConfigData__) */
