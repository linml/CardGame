//
//  CPtPropConfigData.h
//  91.cube
//
//  Created by phileas on 13-8-12.
//
//

#ifndef ___1_cube__CPtPropConfigData__
#define ___1_cube__CPtPropConfigData__

#include "cocos2d.h"
#include "CSingleton.h"
using namespace cocos2d;

class CPtPropConfigData : public CCObject
{
    
    CCDictionary * m_pConfigData;
public:
    CPtPropConfigData();
    virtual ~CPtPropConfigData();
    
public:
    bool getPropDataById(const int &inPropId);

    CC_SYNTHESIZE_READONLY(std::string, m_strPropName, PropName);
    CC_SYNTHESIZE_READONLY(int, m_nPropType, PropType);
    CC_SYNTHESIZE_READONLY(int, m_nUnLockLevel, UnLockLevel);
    CC_SYNTHESIZE_READONLY(int, m_nUseDelete, UseDelete);
    CC_SYNTHESIZE_READONLY(int, m_nQuality, Quality);
    CC_SYNTHESIZE_READONLY(int, m_nCoin, Coin);
    CC_SYNTHESIZE_READONLY(int, m_nPrice, Price);
    CC_SYNTHESIZE_READONLY(int, m_nSkillId, SkillId);
    CC_SYNTHESIZE_READONLY(int, m_nSellCoin, SellCoin);
    CC_SYNTHESIZE_READONLY(std::string, m_strIconName, IconName);
    CC_SYNTHESIZE_READONLY(int, m_nLimitNum, LimitNum);
    CC_SYNTHESIZE_READONLY(int, m_nMaxNum, MaxNum);
    CC_SYNTHESIZE_READONLY(std::string, m_strTips, Tips);
    
protected:
    int m_nPropId;
   
};

typedef Singleton<CPtPropConfigData> SinglePropConfigData;

#endif /* defined(___1_cube__CPtPropConfigData__) */
