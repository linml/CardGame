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

class PropItem : public CCObject
{
public:
    int propId;
    int propCount;
};

    

class CPtStarConfigData :public CCObject
{

    CCDictionary * m_pConfigData;
public:
    CPtStarConfigData();
    virtual ~CPtStarConfigData();
    int getConstConin(const int &inCardId);
    CCArray *getPropArrays(const int&inCardId);
    
protected:
    bool updateWithCardId(const int &inCardId);
    
protected:
    int m_nCurrentCardId;

    int m_nCoin;
    CCArray *m_pProps;

};

typedef Singleton<CPtStarConfigData> SingleStarConfigData;

#endif /* defined(___1_cube__CPtStarConfigData__) */
