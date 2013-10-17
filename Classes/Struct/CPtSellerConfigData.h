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

};

#endif /* defined(___1_cube__CPtSellerConfigData__) */
