//
//  CCardEnchantConfig.h
//  91.cube
//
//  Created by phileas on 14-1-8.
//
//

#ifndef ___1_cube__CCardEnchantConfig__
#define ___1_cube__CCardEnchantConfig__

#include "cocos2d.h"
#include "CCard.h"
#include <vector>
using namespace cocos2d;
#define DataExtentCount 5

typedef struct
{
    int startIndex = 0;
    int count = 0;
    
}DataExtent;;
typedef struct
{
    int cardid;
    int star;
    int mergeValue;
    int splitValue;
    
}CardEnchantData;

typedef struct
{
    CardEnchantData *echantCards = NULL;
    //std::vector<int> * haveCards = NULL;
    std::vector<CFightCard*>* haveCards = NULL;
    void addCard(CFightCard* inCardData)
    {
        haveCards = haveCards ? haveCards : new std::vector<CFightCard*>();
        haveCards->push_back(inCardData);
    }
    void removeCardDataByIndex(int inIndex)
    {
        if(haveCards && inIndex < haveCards->size())
        {
            haveCards->erase(haveCards->begin()+inIndex);
        }
    }
}EnchantCards;

typedef struct
{
    EnchantCards * head = NULL;
    int count = 0;
}EnchantCardsContaner;


class CCardEnchantConfig
{
public:
    static CCardEnchantConfig* getInstance();
    static void releaseCCardEnchantData();
    static void releaseEnchant(EnchantCardsContaner *inEnchantCards);
protected:
    static CCardEnchantConfig* s_pInstance;
    
public:
    void getEnchantCards(EnchantCardsContaner * outEnchantCards);
    DataExtent getDataExtentByStar(int inStar);
protected:
    CCardEnchantConfig();
    ~CCardEnchantConfig();
    
    void loadDataFromFile();
    
protected:
    CardEnchantData *m_pCardEnchantContainer;
    int m_nEnchantCardKinds;
    DataExtent m_cExtent[DataExtentCount];
};

#endif /* defined(___1_cube__CCardEnchantConfig__) */
