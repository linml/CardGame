//
//  CPtLevelConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-7-25.
//
//

#include "CPtLevelConfigData.h"
#include "gameConfig.h"
#include "gameTools.h"

CPtLevelConfigData::CPtLevelConfigData()
{
    m_pConfigData = CCDictionary::createWithContentsOfFile(g_str_level_up_config.c_str());
    m_nLevel = -1;
    if (m_pConfigData)
    {
         m_pConfigData->retain();
    }
   
}
CPtLevelConfigData::~CPtLevelConfigData()
{
    if (m_pConfigData)
    {
        m_pConfigData->release();
    }
}

bool CPtLevelConfigData::update(int level)
{
    if (level == m_nLevel)
    {
        CCLog("don't need update : %d", level);
        return true;
    }
    char buffer[6] = {0};
    sprintf(buffer, "%d", level);
    if (m_pConfigData)
    {
         CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
        if (tmp == NULL)
        {
            return false;
        }
        
         m_nLevel = GameTools::intForKey("level", tmp);
         m_nAttak = GameTools::intForKey("attack", tmp);
         m_nDefend = GameTools::intForKey("defend", tmp);
         m_nHp = GameTools::intForKey("hp", tmp);
         m_nExp = GameTools::intForKey("exp", tmp);
         m_nConin = GameTools::intForKey("conin", tmp);
         m_fStarOne = GameTools::floatForKey("star_param_1", tmp);
         m_fStarTow = GameTools::floatForKey("star_param_2", tmp);
         m_fStarThree = GameTools::floatForKey("star_param_3", tmp);
         m_fStarForth =GameTools::floatForKey("star_param_4", tmp);
         m_fStarFive = GameTools::floatForKey("star_param_5", tmp);
         m_fStarSix = GameTools::floatForKey("star_param_6", tmp);
         m_fStarSeven = GameTools::floatForKey("star_param_7", tmp);
         m_fCorrect = GameTools::floatForKey("correct_param", tmp);
         m_fCorrectOne = GameTools::floatForKey("correct_value_1", tmp);
         m_fCorrectTow = GameTools::floatForKey("correct_value_2", tmp);
        
        return true;
    }
    return false;
}

float CPtLevelConfigData::getStarParamter(int star)
{
    float result = 0.0f;
    switch (star)
    {
        case 1:
            result = m_fStarOne;
            break;
        case 2:
             result = m_fStarTow;
            break;
        case 3:
            result = m_fStarThree;
            break;
        case 4:
             result = m_fStarForth;
            break;
        case 5:
             result = m_fStarFive;
            break;
        case 6:
            result = m_fStarSix;
            break;
        case 7:
            result = m_fStarSeven;
            break;
        default:
            break;
    }
    return result;
    
}

int CPtLevelConfigData::getLevelExp(int level)
{
    int result = 0;
    char buffer[6] = {0};
    sprintf(buffer, "%d", level);
    if (m_pConfigData)
    {
        CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
        if (tmp)
        {
            result = GameTools::intForKey("exp", tmp);
        }
    }
    return result;

}

/*
 * 
 * @return -1: levelcount < beginLevel
 */

int CPtLevelConfigData::getCurrentLevel(int exp, int beginLevel)
{
    int result = beginLevel;
    int tmpValue = 0;
    if (m_pConfigData)
    {
        char buffer[10]={0};
        int count = m_pConfigData->allKeys()->count()+1;
        if (beginLevel < count)
        {
            for (int i = beginLevel; i <= count+1; i++)
            {
                sprintf(buffer, "%d",i);
                CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
                if (tmp)
                {
                    tmpValue = GameTools::intForKey("exp", tmp);
                    if (exp < tmpValue)
                    {
                        result = i-1;
                        break;
                    }else
                    {
                        result = i;
                    }
                }

            }
            
            
            
        }else
        {
            return  -1;
        }
    }

    return  result;
}