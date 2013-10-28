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
#include <string>
using namespace cocos2d;

#define EXPID 900000
#define COINSID 900001
#define NOPROPIDEND 900100

class CPtProp : public CCObject
{
public:

    std::string getTips();
    CC_SYNTHESIZE(int , m_nPropId,PropId);
    CC_SYNTHESIZE(std::string, m_strPropName, PropName);
    CC_SYNTHESIZE(int, m_nPropType, PropType);
    CC_SYNTHESIZE(int, m_nUnLockLevel, UnLockLevel); //可能用到 的时候到达某个等级后显示该道具
    CC_SYNTHESIZE(int, m_nUseDelete, UseDelete);    //使用跟删除
    CC_SYNTHESIZE(int, m_nCoin, Coin);              //
    CC_SYNTHESIZE(int, m_nCash, Cash);
    CC_SYNTHESIZE(int, m_nUseId, UseId);            //!<
    CC_SYNTHESIZE(int, m_nSellCoin, SellCoin);
    CC_SYNTHESIZE(std::string, m_strIconName, IconName);
    CC_SYNTHESIZE(int, m_nLimitNum, LimitNum);
    CC_SYNTHESIZE(int, m_nIsOnly, IsOnlyNum);
    CC_SYNTHESIZE(std::string, m_stTipKey, TipKey);
    
    //描述
    //没有一个仓库存在多少个
    //商店里面对应的金币或者cash的价格。 
};

class CPtPropConfigData : public CCObject
{
    

public:
    CPtPropConfigData();
    virtual ~CPtPropConfigData();
    
public:
    bool getPropDataById(const int &inPropId);

    CPtProp *getPropById(const int &inPropId);
    std::string getIconName(const int &inPropId);
    
    map<int, CPtProp*> &getProps(){return m_pAllProps;};
    
    bool isCanGoBackPackById(int inPropId){return (inPropId >= EXPID && inPropId < NOPROPIDEND);};
    
    CC_SYNTHESIZE_READONLY(std::string, m_strPropName, PropName);
    CC_SYNTHESIZE_READONLY(int, m_nPropType, PropType);
    CC_SYNTHESIZE_READONLY(int, m_nUnLockLevel, UnLockLevel);
    CC_SYNTHESIZE_READONLY(int, m_nUseDelete, UseDelete);
    CC_SYNTHESIZE_READONLY(int, m_nCoin, Coin);
    CC_SYNTHESIZE_READONLY(int, m_nPrice, Price);
    CC_SYNTHESIZE_READONLY(int, m_nUseId, UseId);
    CC_SYNTHESIZE_READONLY(int, m_nSellCoin, SellCoin);
    CC_SYNTHESIZE_READONLY(std::string, m_strIconName, IconName);
    CC_SYNTHESIZE_READONLY(int, m_nLimitNum, LimitNum);
    CC_SYNTHESIZE_READONLY(int, m_nIsOnly, IsOnlyNum);
    CC_SYNTHESIZE_READONLY(std::string, m_sTipKey, TipKey);
    
protected:
    void loadPropToMap(CCDictionary *inConfigData);
protected:
    int m_nPropId;
    map<int, CPtProp*> m_pAllProps; // 所有道具出了经验和金币以外
    
    std::string m_sExpIcon;
    std::string m_sCoinIcon;
};

typedef Singleton<CPtPropConfigData> SinglePropConfigData;

#endif /* defined(___1_cube__CPtPropConfigData__) */
