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

float CPtLevelConfigData::getStarParamter(int star, int level)
{
    float tmpValue = 0;
    if (m_pConfigData)
    {
        int count = m_pConfigData->allKeys()->count()+1;
        if (count < level)
        {
            level = count;
        }
        char buffer[10]={0};
        sprintf(buffer, "%d",level);
        CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
        if (tmp)
        {
            switch(star)
            {
                case 1:
                    tmpValue = GameTools::floatForKey("star_param_1", tmp);
                    break;
                case 2:
                    tmpValue = GameTools::floatForKey("star_param_2", tmp);
                    break;
                case 3:
                    tmpValue = GameTools::floatForKey("star_param_3", tmp);
                    break;
                case 4:
                    tmpValue =GameTools::floatForKey("star_param_4", tmp);
                    break;
                case 5:
                    tmpValue = GameTools::floatForKey("star_param_5", tmp);
                    break;
                case 6:
                    tmpValue = GameTools::floatForKey("star_param_6", tmp);
                    break;
                case 7:
                    tmpValue = GameTools::floatForKey("star_param_7", tmp);
                    break;
                default:
                    break;
            }
            
            
        }
        
    }
    return tmpValue;

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

int CPtLevelConfigData::getCurrentLevel(int exp, int star,  int beginLevel)
{
    int result = beginLevel;
    float start_param = 0;
    int tmpValue = 0;
    if (m_pConfigData)
    {
        char buffer[30]={0};
        int count = m_pConfigData->allKeys()->count()+1;
        if (beginLevel < count)
        {
            for (int i = beginLevel; i <= count; i++)
            {
                sprintf(buffer, "%d",i);
                CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
                if (tmp)
                {
                
                    sprintf(buffer, "star_param_%d", star);
                    start_param = GameTools::floatForKey(buffer, tmp);
                    if (start_param == 0)
                    {
                        CCLog("star_param : error");
                        return  -1;
                    }
                    tmpValue = GameTools::intForKey("exp", tmp);
                    CCLog("%d", start_param);
                    // 向下取整：
                    if (exp < (int)(tmpValue*start_param))
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

/*
 * @param inLevel: 从 level_up_config.plist中获取level的inType值,当level大于level_up_config.plist中的最大值时，取最大值
 * @param inType: 1: coin, 2: exp
 */
int CPtLevelConfigData::getValeWithLevel(int inLevel, int inType)
{
    int tmpValue = 0;
    if (m_pConfigData)
    {
        int count = m_pConfigData->allKeys()->count()+1;
        if (count < inLevel)
        {
            inLevel = count;
        }
        char buffer[10]={0};
        sprintf(buffer, "%d",inLevel);
        CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
        if (tmp)
        {
            switch(inType)
            {
                case 1:
                    tmpValue = GameTools::intForKey("conin", tmp);
                    break;
                case 2:
                    tmpValue = GameTools::intForKey("exp", tmp);
                    break;
                default:
                    break;
            }
           
           
        }

    }
    return tmpValue;
}
/*
 * 向下取整
 */

int CPtLevelConfigData::getTopExpByStar(int inLevel, int inStar)
{
    char buffer[30]={0};
    sprintf(buffer, "%d",inLevel);
    CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
    if (tmp)
    {
                
        sprintf(buffer, "star_param_%d", inStar);
        float start_param = GameTools::floatForKey(buffer, tmp);
        int tmpValue = GameTools::intForKey("exp", tmp);
        return (((float)tmpValue)*start_param);
    }
    return 0;
}
