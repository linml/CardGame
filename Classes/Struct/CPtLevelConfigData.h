//
//  CPtLevelConfigData.h
//  91.cube
//
//  Created by phileas on 13-7-25.
//
//

#ifndef ___1_cube__CPtLevelConfigData__
#define ___1_cube__CPtLevelConfigData__

#include "cocos2d.h"
#include "CSingleton.h"
#include "CPtTool.h"
using namespace cocos2d;

class CPtLevelConfigData : public CCObject
{
    
    CCDictionary * m_pConfigData;
public:
    bool update(int level);
    
    CPtLevelConfigData();
    virtual ~CPtLevelConfigData();
    
    
    CC_SYNTHESIZE_READONLY(int, m_nLevel, Level);
    CC_SYNTHESIZE_READONLY(int, m_nAttak, Attack);
    CC_SYNTHESIZE_READONLY(int, m_nDefend, Define);
    CC_SYNTHESIZE_READONLY(int, m_nHp, Hp);
    CC_SYNTHESIZE_READONLY(int, m_nExp, Exp);
    CC_SYNTHESIZE_READONLY(int, m_nConin, Conin);
    CC_SYNTHESIZE_READONLY(float, m_fStarOne, StarOne);
    CC_SYNTHESIZE_READONLY(float, m_fStarTow, StarTow);
    CC_SYNTHESIZE_READONLY(float, m_fStarThree, StarThree);
    CC_SYNTHESIZE_READONLY(float, m_fStarForth, StarForth);
    CC_SYNTHESIZE_READONLY(float, m_fStarFive, StarFive);
    CC_SYNTHESIZE_READONLY(float, m_fStarSix, StarSix);
    CC_SYNTHESIZE_READONLY(float, m_fStarSeven, StarSeven);
    CC_SYNTHESIZE_READONLY(float, m_fCorrect, Correct);
    CC_SYNTHESIZE_READONLY(float, m_fCorrectOne, CorrectOne);
    CC_SYNTHESIZE_READONLY(float, m_fCorrectTow, CorrectTow);
    
public:
    float getStarParamter(int start);
    float getStarParamter(int start, int level);
    int getLevelExp(int level);
    int getCurrentLevel(int exp, int star, int beginLevel=1);
    int getValeWithLevel(int inLevel, int inType);
    
    int getTopExpByStar(int inLevel, int inStar);
    
};

typedef Singleton<CPtLevelConfigData> SingleLevleConfigData;
#endif /* defined(___1_cube__CPtLevelConfigData__) */
