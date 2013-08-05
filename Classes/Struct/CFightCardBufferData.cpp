//
//  CFightCardBufferData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-5.
//
//

#include "CFightCardBufferData.h"
void CFightCardBufferData::appendBuffer(int iBufferEffectId,int Ivalue)
{
    for (vector <BufferList *>::iterator it=m_vBufferList.begin();it !=m_vBufferList.end(); it++)
    {
        delete *it;
        *it=NULL;
    }
    m_vBufferList.erase(m_vBufferList.begin(),m_vBufferList.end());
}

void CFightCardBufferData::destory()
{
    for (vector <BufferList *>::iterator it=m_vBufferList.begin();it !=m_vBufferList.end(); it++)
    {
        delete *it;
        *it=NULL;
    }
    m_vBufferList.erase(m_vBufferList.begin(),m_vBufferList.end());
}

CFightCardBufferDataEveryFight::CFightCardBufferDataEveryFight()
{
    m_arrayFight[0]=NULL;
    m_arrayFight[1]=NULL;
    
}
void CFightCardBufferDataEveryFight::appendBuffer(CFightCard *pFightCard, CFightCard *pCardMonster)
{
    if(pFightCard)
    {
        
    }
    if(pCardMonster)
    {
        
    }
}
void CFightCardBufferDataEveryFight::destoryBufferData(CFightCardBufferData *data)
{
    if(data)
    {
        data->destory();
        delete data;
        data=NULL;
    }

}
CFightCardBufferDataEveryFight::~CFightCardBufferDataEveryFight()
{
    destoryBufferData(m_arrayFight[0]);
    destoryBufferData(m_arrayFight[1]);
}