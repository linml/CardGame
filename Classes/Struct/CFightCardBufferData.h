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
class CFightCard;
using namespace std;
struct  BufferList
{
    int iBufferEffectId;
    int iValue;
};
class CFightCardBufferData
{
public:
    void appendBuffer(int iBufferEffectId,int Ivalue);
    void destory();
public:
    vector <BufferList *> m_vBufferList;
};

class CFightCardBufferDataEveryFight
{
public:
    CFightCardBufferDataEveryFight();
    ~CFightCardBufferDataEveryFight();
    void appendBuffer(CFightCard*pFightCard,CFightCard *pCardMonster);
    void destoryBufferData(CFightCardBufferData *data);
    CFightCardBufferData *m_arrayFight[2]; //记录双方的状态
};

#endif /* defined(___1_cube__CFightCardBufferData__) */
