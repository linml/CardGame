//
//  CPtStarConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-8-2.
//
//

#include "CPtStarConfigData.h"
#include "gameConfig.h"
#include "gameTools.h"

CPtStarConfigData::CPtStarConfigData()
{
    m_pConfigData = CCDictionary::createWithContentsOfFile(g_str_star_config.c_str());
    if (m_pConfigData)
    {
        m_pConfigData->retain();
    }
    m_nCurrentCardId = -1;
    m_pProps = CCArray::create();
    if (m_pProps)
    {
        m_pProps->retain();
    }

}

CPtStarConfigData::~CPtStarConfigData()
{
    if (m_pConfigData)
    {
        m_pConfigData->release();
        m_pConfigData = NULL;
    }
    if (m_pProps)
    {
        m_pProps->release();
        m_pProps = NULL;
    }
}

int CPtStarConfigData::getConstConin(const int &inCardId)
{
 
    if (inCardId != m_nCurrentCardId)
    {
        updateWithCardId(inCardId);
    }
    return m_nCoin;
}

CCArray *CPtStarConfigData::getPropArrays(const int&inCardId)
{

    if (inCardId != m_nCurrentCardId)
    {
        updateWithCardId(inCardId);

    }
    return m_pProps;
}

bool CPtStarConfigData::updateWithCardId(const int &inCardId)
{
    bool  bRet = false;
    
    if (m_nCurrentCardId == inCardId)
    {
        return true;
    }
    
    if (m_pConfigData)
    {
        char buff [15]={0};
        sprintf(buff, "%d",inCardId);
        CCDictionary * tmp = (CCDictionary *)m_pConfigData->objectForKey(buff);
        if (tmp)
        {
            m_pProps->removeAllObjects();
            int tmpId = 0;
            int tmpNum = 0;
            m_nCoin = GameTools::intForKey("coin", tmp);
            int count  = 0;
            for (int i= 1; i <8 && count < 5 ; i++)
            {
               
                sprintf(buff, "item_%d",i);
                tmpId = GameTools::intForKey(buff, tmp);
                sprintf(buff, "num_%d",i);
                tmpNum = GameTools::intForKey(buff, tmp);
                if (tmpId != 0 && tmpNum != 0)
                {
                    count++;
                    PropItem * item = new PropItem();
                    item->autorelease();
                    item->propId = tmpId;
                    item->propCount = tmpNum;
                    m_pProps->addObject(item);
                }
                m_nCurrentCardId = inCardId;
                bRet = true;
        
            }
        }
    }

    
    return bRet;
}