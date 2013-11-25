//
//  CPtSellerConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-10-16.
//
//

#include "CPtSellerConfigData.h"
#include "gameConfig.h"


// implement of class CPtSeller

CPtSeller *CPtSeller::create()
{
    CPtSeller * seller = new CPtSeller();
    seller->autorelease();
    return seller;
}

CPtSeller::CPtSeller()
{
    
}

CPtSeller::~CPtSeller()
{
}


// implement of class CPtSellerConfigData


CPtSellerConfigData::CPtSellerConfigData()
{
    initData();
}

CPtSellerConfigData::~CPtSellerConfigData()
{
    releaseSource();
}

CPtSeller * CPtSellerConfigData::getSellerById(int inSellerId)
{
    CPtSeller * seller = NULL;
    if (inSellerId == m_nCurrentSellerId)
    {
        seller = m_pCurrentSeller;
    }
    else
    {
        seller = getSellerFromFile(inSellerId);
        CC_SAFE_RETAIN(seller);
        CC_SAFE_RELEASE_NULL(m_pCurrentSeller);
        m_pCurrentSeller=seller;
        m_nCurrentSellerId = inSellerId;
    }
    
    
    return seller;
}

// protected method:

void CPtSellerConfigData::releaseSource()
{
    CC_SAFE_RELEASE_NULL(m_pCurrentSeller);
}

void CPtSellerConfigData::initData()
{
    m_nCurrentSellerId = -1;
    m_pCurrentSeller = NULL;
}

CPtSeller * CPtSellerConfigData::getSellerFromFile(int inSellerId)
{
    CCDictionary *dict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "seller_config.plist"));
    int  sellId = 0;
    if (dict)
    {
        CCDictElement *element = NULL;
        CCDictionary *tmpDict = NULL;
        CCDICT_FOREACH(dict, element)
        {
            tmpDict = (CCDictionary*) element->getObject();
            sellId = GameTools::intForKey("seller_id", tmpDict);
            if (tmpDict && inSellerId == sellId)
            {
                CPtSeller *seller = CPtSeller::create();
                seller->setSellerID(sellId);
                seller->setSellerName(GameTools::valueForKey("name", tmpDict));
                seller->setSellerType(GameTools::intForKey("type", tmpDict));
                seller->setSellerIcon(GameTools::valueForKey("icon", tmpDict));
                seller->setSellerDecriptionId(GameTools::intForKey("decription", tmpDict));
                seller->setShopId(GameTools::intForKey("shop_id", tmpDict));
                return seller;
            }
        }
    }
    return NULL;
}
