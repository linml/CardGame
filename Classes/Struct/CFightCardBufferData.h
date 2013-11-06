//
//  CFightCardBufferData.h
//  91.cube
//
//  Created by linminglu on 13-8-5.
//
//

#ifndef ___1_cube__CFightCardBufferData__
#define ___1_cube__CFightCardBufferData__

#include <iostream>
#include <string>
#include <vector>
#include "gameConfig.h"
class CFightCard;
using namespace std;
struct  BufferList
{
public:
    int m_iBufferEffectId;
    int m_iValue;
};
class CFightCardBufferData
{
public:
    void appendBufferPngList(int iBufferEffectId,int Ivalue);
    void destory();
public:
    vector <BufferList *> m_vBufferList;
};

class CFightCardBufferDataEveryFight
{
public:
    CFightCardBufferDataEveryFight();
    ~CFightCardBufferDataEveryFight();
    void appendBufferPngList(CFightCard*pFightCard,CFightCard *pCardMonster);
    void destoryBufferData(CFightCardBufferData *data);
    CFightCardBufferData *m_arrayFight[2]; //记录双方的状态
};
enum EN_FIGHTBUFFER {
    EN_FIGHTBUFFER_NONE,
    EN_FIGHTBUFFER_ADD,
    EN_FIGHTBUFFER_SUB,
    };
    struct  CBufferIcon {
    int m_iEffectid; //icon id
    int m_iValue;
    bool isLeft;
    EN_FIGHTBUFFER m_enFightBuffer;
};



class CFightCardFightingBuffer
{
public:
    CFightCardFightingBuffer();
    ~CFightCardFightingBuffer();
public:
    int m_nActionSkillIndex;
    void append(int EffectId,int  Value,bool isLeft);
    int m_index;
    vector<CBufferIcon *>m_vbufferList;
};



#endif /* defined(___1_cube__CFightCardBufferData__) */
