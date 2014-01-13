//
//  CCardEnchantConfig.cpp
//  91.cube
//
//  Created by phileas on 14-1-8.
//
//

#include "CCardEnchantConfig.h"
#include "gameConfig.h"
#include "gameTools.h"
#include "gamePlayer.h"

// tool method:
int compare(const void *, const void *);
// static method:
CCardEnchantConfig* CCardEnchantConfig::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CCardEnchantConfig();
    }
    return s_pInstance;
}
void CCardEnchantConfig::releaseCCardEnchantData()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

void CCardEnchantConfig::releaseEnchant(EnchantCardsContaner *inEnchantCards)
{
    if (inEnchantCards)
    {
        for (int i = 0; i < inEnchantCards->count; i++)
        {
            CC_SAFE_DELETE(inEnchantCards->head[i].haveCards);
            inEnchantCards->head[i].haveCards = NULL;
        }
        CC_SAFE_DELETE_ARRAY(inEnchantCards->head);
        inEnchantCards->head = NULL;
        inEnchantCards->count = 0;
    }
    
}

CCardEnchantConfig* CCardEnchantConfig::s_pInstance = NULL;

// public method:
void CCardEnchantConfig::getEnchantCards(EnchantCardsContaner * outEnchantCards)
{
    releaseEnchant(outEnchantCards);
    if (m_pCardEnchantContainer && m_nEnchantCardKinds > 0)
    {
        
        outEnchantCards->head = new EnchantCards[m_nEnchantCardKinds];
        outEnchantCards->count = m_nEnchantCardKinds;
        vector<CFightCard*> & haveCards = SinglePlayer::instance()->getCardBagVector();
        vector<vector<CFightCard*> > &fights =SinglePlayer::instance()->getCardBattleArray();
        vector<int> cardsInFight;
        for (int i = 0; i < fights.size(); i++)
        {
            for (int j = 0; j < fights.at(i).size(); j++)
            {
                if (fights.at(i).at(j))
                {
                    cardsInFight.push_back(fights.at(i).at(j)->m_User_Card_ID);
                }
            }
        }
        vector<int>::iterator it;
        EnchantCards* tmp = NULL;
        for (int i = 0; i < m_nEnchantCardKinds; i++)
        {
            tmp = outEnchantCards->head+i;
            tmp->echantCards = m_pCardEnchantContainer+i;
            tmp->haveCards = NULL;
            int usercardId;
            
            for (int j = 0; j < haveCards.size(); j++)
            {
                usercardId = haveCards[j]->m_User_Card_ID;
                if (tmp->echantCards->cardid == haveCards[j]->m_pCard->m_icard_id)
                {
                    it = find(cardsInFight.begin(), cardsInFight.end(), usercardId);
                    if (it != cardsInFight.end())
                    {
                        continue;
                    }
                    if (tmp->haveCards)
                    {
                        tmp->haveCards->push_back(haveCards[j]);
                    }else
                    {
                        tmp->haveCards = new vector<CFightCard*>();
                        tmp->haveCards->push_back(haveCards[j]);
                    }
                }
            }
            
        }
    }
}

DataExtent CCardEnchantConfig::getDataExtentByStar(int inStar)
{
    DataExtent dataExtent;
    inStar -= 3;
    if (inStar>=0 && inStar < DataExtentCount)
    {
        dataExtent = m_cExtent[inStar];
    }
    return dataExtent;
}

// protected method:
CCardEnchantConfig::CCardEnchantConfig()
{
    m_pCardEnchantContainer = NULL;
    m_nEnchantCardKinds = 0;
    loadDataFromFile();
}
CCardEnchantConfig::~CCardEnchantConfig()
{
    CC_SAFE_DELETE_ARRAY(m_pCardEnchantContainer);
    m_pCardEnchantContainer = NULL;
}

void CCardEnchantConfig::loadDataFromFile()
{
    CCDictElement *element ;
    CCDictionary *elementDict;
    CCDictionary * tmpDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "place_soul_config.plist"));
    if (tmpDict)
    {
        m_nEnchantCardKinds = tmpDict->count();
        if (m_nEnchantCardKinds > 0)
        {
            CC_SAFE_DELETE_ARRAY(m_pCardEnchantContainer);
            m_pCardEnchantContainer = new CardEnchantData[m_nEnchantCardKinds];
            memset(m_pCardEnchantContainer, 0, sizeof(CardEnchantData)*m_nEnchantCardKinds);
            int  index = 0;
            CCDICT_FOREACH(tmpDict, element)
            {
              
                elementDict = (CCDictionary*)element->getObject();
                m_pCardEnchantContainer[index].cardid = GameTools::intForKey("card_id", elementDict);
                m_pCardEnchantContainer[index].mergeValue = GameTools::intForKey("merge_num", elementDict);
                m_pCardEnchantContainer[index].splitValue = GameTools::intForKey("split_num", elementDict);
                m_pCardEnchantContainer[index].star = GameTools::intForKey("star", elementDict);
                index++;
            }
            qsort(m_pCardEnchantContainer, m_nEnchantCardKinds, sizeof(CardEnchantData), compare);

            int j = 0;
            if (m_nEnchantCardKinds>0)
            {
                m_cExtent[j].count = 1;
                for (int i = 1; i < m_nEnchantCardKinds; i++)
                {
                    if (m_pCardEnchantContainer[i-1].star != m_pCardEnchantContainer[i].star)
                    {
                        m_cExtent[++j].startIndex = i;
                        m_cExtent[j].count = 1;
                    }else
                    {
                        m_cExtent[j].count++;
                    }
                }

            }
        }
       
    }
}

int compare(const void *param1, const void * param2)
{
   return  ((CardEnchantData*)param1)->star > ((CardEnchantData*)param2)->star;

}
