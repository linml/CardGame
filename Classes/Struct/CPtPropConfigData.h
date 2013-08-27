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

class CPtProp : public CCObject
{
public:

    CC_SYNTHESIZE(int , m_nPropId,PropId);
    CC_SYNTHESIZE(std::string, m_strPropName, PropName);
    CC_SYNTHESIZE(int, m_nPropType, PropType);
    CC_SYNTHESIZE(int, m_nUnLockLevel, UnLockLevel);
    CC_SYNTHESIZE(int, m_nUseDelete, UseDelete);
    CC_SYNTHESIZE(int, m_nCoin, Coin);
    CC_SYNTHESIZE(int, m_nPrice, Price);
    CC_SYNTHESIZE(int, m_nSkillId, SkillId);
    CC_SYNTHESIZE(int, m_nSellCoin, SellCoin);
    CC_SYNTHESIZE(std::string, m_strIconName, IconName);
    CC_SYNTHESIZE(int, m_nLimitNum, LimitNum);
    CC_SYNTHESIZE(int, m_nIsOnly, IsOnlyNum);
    CC_SYNTHESIZE(std::string, m_strTips, Tips);
};

class CPtPropConfigData : public CCObject
{
    

public:
    CPtPropConfigData();
    virtual ~CPtPropConfigData();
    
public:
    bool getPropDataById(const int &inPropId);

    CPtProp *getPropById(const int &inPropId);
    
    map<int, CPtProp*> &getProps(){return m_pAllProps;};
    
    CC_SYNTHESIZE_READONLY(std::string, m_strPropName, PropName);
    CC_SYNTHESIZE_READONLY(int, m_nPropType, PropType);
    CC_SYNTHESIZE_READONLY(int, m_nUnLockLevel, UnLockLevel);
    CC_SYNTHESIZE_READONLY(int, m_nUseDelete, UseDelete);
    CC_SYNTHESIZE_READONLY(int, m_nCoin, Coin);
    CC_SYNTHESIZE_READONLY(int, m_nPrice, Price);
    CC_SYNTHESIZE_READONLY(int, m_nSkillId, SkillId);
    CC_SYNTHESIZE_READONLY(int, m_nSellCoin, SellCoin);
    CC_SYNTHESIZE_READONLY(std::string, m_strIconName, IconName);
    CC_SYNTHESIZE_READONLY(int, m_nLimitNum, LimitNum);
    CC_SYNTHESIZE_READONLY(int, m_nIsOnly, IsOnlyNum);
    CC_SYNTHESIZE_READONLY(std::string, m_strTips, Tips);
    
protected:
    void loadPropToMap(CCDictionary *inConfigData);
protected:
    int m_nPropId;
    map<int, CPtProp*> m_pAllProps;
};

typedef Singleton<CPtPropConfigData> SinglePropConfigData;

#endif /* defined(___1_cube__CPtPropConfigData__) */
