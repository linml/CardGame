//
//  CRankRewardManager.cpp
//  91.cube
//
//  Created by phileas on 13-12-20.
//
//

#include "CRankRewardManager.h"
#include "gameConfig.h"



CRankRewardManager * CRankRewardManager::s_pInstance = NULL;

CRankRewardManager* CRankRewardManager::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CRankRewardManager();
    }
    return s_pInstance;
}

void CRankRewardManager::releaseDataManager()
{
    CC_SAFE_DELETE(s_pInstance);
    s_pInstance = NULL;
}


int CRankRewardManager::getMedalKind(int inType, int inRank)
{
    int nRes = -1;
    RankReward *tmpPoint = NULL;
    getWhichRank(inType, tmpPoint);
    if (tmpPoint)
    {
        for (int i = 0;  i < LEVELCOUNT; i++)
        {
            if (inRank >= tmpPoint[i].startIndex && inRank <= tmpPoint[i].endIndex)
            {
                nRes = i+1;
                break;
            }
        }
    }
    
    return nRes;
}
/*
 * @notice:
 *   1.if CRankRewardManager's instance is released ,you can't use the return object
 *   2.you need relase the return object by yourself.
 */
const RankReward* CRankRewardManager::getRankRewardByRank(int inType, int inRank)
{
    RankReward *pRes = NULL;
    RankReward *tmpPoint = NULL;
    getWhichRank(inType, tmpPoint);
    if (tmpPoint)
    {
        for (int i = 0;  i < LEVELCOUNT; i++)
        {
            if (inRank >= tmpPoint[i].startIndex && inRank <= tmpPoint[i].endIndex)
            {
                pRes = tmpPoint+i;
                break;
            }
        }
    }
    
    return pRes;
}

/*
 * @param inIndex from 1 to 5
 */
const RankReward* CRankRewardManager::getRankRewardByIndex(int inType, int inIndex)
{
    RankReward *pRes = NULL;
    RankReward *tmpPoint = NULL;
    getWhichRank(inType, tmpPoint);
    if (tmpPoint && inIndex <= LEVELCOUNT && inIndex > 0)
    {
        pRes = tmpPoint+(inIndex-1);
    }
    
    return pRes;

}

CRankRewardManager::CRankRewardManager()
{
    parseDataFromFile();
}
CRankRewardManager::~CRankRewardManager()
{
    
}

void CRankRewardManager::parseDataFromFile()
{
    CCDictionary *tempDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "charts_reward_config.plist"));
    CCAssert(tempDict, "can't read charts_reward_config.plist file");
    /*
     <key>599911</key>
     <dict>
     <key>id</key>
     <string>599911</string>
     <key>type</key>
     <string>1</string>
     <key>high</key>
     <string>1</string>
     <key>low</key>
     <string>1</string>
     <key>coin</key>
     <string>47700</string>
     <key>exp</key>
     <string>500</string>
     <key>item</key>
     <string>907050</string>
     <key>num</key>
     <string>5</string>
     </dict>
     */
    
    CCDictionary *elementDict = NULL;
    CCDictElement *element = NULL;
    int type = 0;
    int key = 0;
    
    RankReward * tmpPoint = NULL;
    CCDICT_FOREACH(tempDict, element)
    {
        elementDict = (CCDictionary*)element->getObject();
        key = atoi(element->getStrKey());
        key %= 10;
        key--;
        type = GameTools::intForKey("type", elementDict);
        getWhichRank(type, tmpPoint);
        tmpPoint[key].setData(GameTools::intForKey("high", elementDict), GameTools::intForKey("low", elementDict), GameTools::intForKey("exp", elementDict), GameTools::intForKey("coin", elementDict), GameTools::intForKey("item", elementDict), GameTools::intForKey("num", elementDict));
    }
    
}

void CRankRewardManager::getWhichRank(int inType, RankReward * &outPointer)
{
    if (inType == LEVELRANKREWARD)
    {
        outPointer = m_aLRankReward;
    }
    else if(inType == FIGHTRANKREWARD)
    {
        outPointer = m_aFRankReward;
    }
    else
    {
        outPointer = m_aRRankReward;
    }

    
}