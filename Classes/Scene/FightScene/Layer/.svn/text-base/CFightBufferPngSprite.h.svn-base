//
//  CFightBufferPngSprite.h
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#ifndef ___1_cube__CFightBufferPngSprite__
#define ___1_cube__CFightBufferPngSprite__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"
using namespace std;
using namespace cocos2d;
class CImapact;
enum EN_EFFECTBUFFER_TYPE {
    EN_EFFECTBUFFER_TYPE_APPEND = 0,
    EN_EFFECTBUFFER_TYPE_REST = 1,
    EN_EFFECTBUFFER_TYPE_REMOVE = 2
    };
class CFightBufferPngSprite :public CCSprite
{
public:
    CFightBufferPngSprite();
    ~CFightBufferPngSprite();
  static  CFightBufferPngSprite *Create(int effectId,EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex); //是否是左边的人 同时是给哪个人的附带的效果是给哪一个人的
    bool initCreate(int effectId,EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex);
    void resetValue(EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex);
    void subValue(EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex);
    void remove(EN_LEFTTEAMORRIGHTTEAM isLeft,int effectId); //最好不要调用 否则很难管理
    void updateNumber();
private:
    CC_SYNTHESIZE(int , m_nEffectTime, EffectTime);
    CC_SYNTHESIZE(int , m_nKeepTime, KeepTime);
    CC_SYNTHESIZE(string, m_sBufferPng, BufferPng);
    CC_SYNTHESIZE(int, m_nBackEffectTime, BackEffectTime);
    CC_SYNTHESIZE(int, m_nBackKeepTime, BackKeepTime);
    CC_SYNTHESIZE(int , m_nAtkIndex, nAtkIndex);
    CC_SYNTHESIZE(EN_LEFTTEAMORRIGHTTEAM , m_enTeamPostion, TeamPostion);
    CC_SYNTHESIZE(int , m_nEffectId,EffectId);
};

#endif /* defined(___1_cube__CFightBufferPngSprite__) */
