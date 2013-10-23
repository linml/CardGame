//
//  CPtSellerConfigData.h
//  91.cube
//
//  Created by phileas on 13-10-16.
//
//

#ifndef ___1_cube__CPtSellerConfigData__
#define ___1_cube__CPtSellerConfigData__

#include "cocos2d.h"
#include "CSingleton.h"
using namespace cocos2d;

class CPtSeller : public CCObject
{
public:
    static CPtSeller *create();
public:
    CPtSeller();
    ~CPtSeller();
    
    CC_SYNTHESIZE(int , m_nSellerId, SellerID);
    CC_SYNTHESIZE(std::string, m_sSellerName, SellerName);
    CC_SYNTHESIZE(int, m_nSellerType, SellerType);
    CC_SYNTHESIZE(std::string, m_sSellerIcon, SellerIcon);
    CC_SYNTHESIZE(int, m_sSellerDescriptionId, SellerDecriptionId);
    CC_SYNTHESIZE(int, m_nShopId, ShopId);

};


class CPtSellerConfigData
{
public:
    CPtSellerConfigData();
    virtual ~CPtSellerConfigData();

    CPtSeller *getSellerById(int inSellerId);
protected:
    void releaseSource();
    void initData();

    CPtSeller *getSellerFromFile(int inSellerId);

protected:
    CPtSeller *m_pCurrentSeller;
    int m_nCurrentSellerId;
};

typedef Singleton<CPtSellerConfigData> SingleSellerConfigData;

#endif /* defined(___1_cube__CPtSellerConfigData__) */
