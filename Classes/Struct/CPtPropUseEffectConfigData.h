//
//  CPtPropUseEffectConfigData.h
//  91.cube
//
//  Created by phileas on 13-10-30.
//
//

#ifndef ___1_cube__CPtPropUseEffectConfigData__
#define ___1_cube__CPtPropUseEffectConfigData__

#include "cocos2d.h"
#include <map>

using namespace cocos2d;
using namespace std;

class PtPropUseEffectData : public CCObject
{

public:
    PtPropUseEffectData();
    
    CC_SYNTHESIZE(int, m_nItemEffectId, ItemEffectId);
    CC_SYNTHESIZE(int, m_nItemEffectTipId, ItemEffectTipId);
    CC_SYNTHESIZE(std::string, m_sSpecialEffect, SpecialEffect);
    CC_SYNTHESIZE(std::string, m_sIcon, Icon);
};

class CPtPropUseEffectConfigData
{
public:
    static CPtPropUseEffectConfigData* getInstance();
    static void releaseData();
    static CPtPropUseEffectConfigData *s_pInstance;
    
public:
    CPtPropUseEffectConfigData();
    ~CPtPropUseEffectConfigData();
    
    PtPropUseEffectData* getPropUseDataById(int inItemEffectId);
    
protected:
    void loadDataFromFile();
    
protected:
    map<int, PtPropUseEffectData*> m_cPropUseEffectContainer;
};

#endif /* defined(___1_cube__CPtPropUseEffectConfigData__) */
