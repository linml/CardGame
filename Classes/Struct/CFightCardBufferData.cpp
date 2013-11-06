//
//  CFightCardBufferData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-5.
//
//

#include "CFightCardBufferData.h"
void CFightCardBufferData::appendBufferPngList(int iBufferEffectId,int Ivalue)
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
    m_arrayFight[0]=new CFightCardBufferData ;
    m_arrayFight[1]=new CFightCardBufferData;
    
}
void CFightCardBufferDataEveryFight::appendBufferPngList(CFightCard *pFightCard, CFightCard *pCardMonster)
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

CFightCardFightingBuffer::CFightCardFightingBuffer()
{
    m_index=0;
    m_nActionSkillIndex=-1;
    this->m_vbufferList.clear();
 
    
}
void CFightCardFightingBuffer::append(int EffectId, int Value, bool isLeft)
{
    CBufferIcon *bufferIcon=new CBufferIcon;
    bufferIcon->m_iEffectid=EffectId;
    bufferIcon->m_iValue=Value;
    bufferIcon->isLeft=isLeft;
    this->m_vbufferList.push_back(bufferIcon);
}

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CFightCardFightingBuffer::~CFightCardFightingBuffer()
{

    DELETE_POINT_VECTOR(m_vbufferList,vector<CBufferIcon *>);
    vector<CBufferIcon *>tempbufferList;
    m_vbufferList.swap(tempbufferList);
}