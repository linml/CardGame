//
//  CPlayerBufferManager.h
//  91.cube
//
//  Created by phileas on 13-10-25.
//
//

#ifndef ___1_cube__CPlayerBufferManager__
#define ___1_cube__CPlayerBufferManager__
#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
using namespace std;

struct PLAYERBUFFERDATA
{
    PLAYERBUFFERDATA(int inEffectId, int inKeepTime): effectId(inEffectId), keepTime(inKeepTime){};
    int effectId;
    int keepTime;
};
enum ALTARBUFFERTYPE
{
     KEEPNONE = 0,
     KEEPTIMES = 1, //时间
     KEEPTIME = 2 //次数
    };
class AltarBuffer
{
public:
    AltarBuffer();
    AltarBuffer(int inSkillEffectId, ALTARBUFFERTYPE inBufferType, int inKeepTime);
    ~AltarBuffer();
    
    void subKeepTime(int subTime =1);

    CC_SYNTHESIZE(int, m_nSkillEffectId, SkillEffectId);
    CC_SYNTHESIZE(ALTARBUFFERTYPE, m_eAlarBufferType, AltarBufferType);
    CC_SYNTHESIZE(int, m_nKeep, Keep); // 当AltarBufferType == KEEPTIME 时，表示为倒计的时间； 当 AltarBufferType == KEEPTIMES时，表示为遇到战斗的次数，无论成败都减1
};

/*
 * @class : this class is in single thread, no mutxt
 */
class CPlayerBufferManager
{
    
public:
    static CPlayerBufferManager* getInstance();
    static  void releaseBufferManager();
protected:
    static CPlayerBufferManager *s_pBufferManger;
    
public:
    void addBufferById(int inEffectId, int inKeepTime);
    void subBufferById(int inEffectId);
    int subBufferKeepTime(int inEffectId, int inSubTime);
    int getBufferKeepTimeById(int inEffectId);
    void clearPlayerBuffer();
    
    
    void addAltarBufferById(int inEffectId, ALTARBUFFERTYPE inBufferType, int inKeepTime);
    void clearAltarBufferById(int inEffectId);
    
    int subAltarBufferKeepTime(int inEffectId, int inSubTime = 1, bool inRemoveZero = true);
    int getAltarBufferKeepTime(int inEffectId);
    
    void clearAllAltarBufferes();
    vector<AltarBuffer> &getAllAltarBuffer(){return m_cAltarBuffercontainer;};
    ALTARBUFFERTYPE getAltarBufferTypeById(int inEffectId);
    
protected:
    int m_nNewAddAltarBufferId;
    CPlayerBufferManager();
    ~CPlayerBufferManager();
    
    CC_SYNTHESIZE(int, m_nLastAddEffectId, LastAddEffectId);

   
protected:
   // map<int, int> m_cBufferContainer; // store buffer --> skill_effect_id & and keep time
    vector<PLAYERBUFFERDATA> m_cBufferContainer;
    vector<AltarBuffer> m_cAltarBuffercontainer;
};


#endif /* defined(___1_cube__CPlayerBufferManager__) */
