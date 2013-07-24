//
//  CCard.cpp
//  91.cube
//
//  Created by linminglu on 13-6-26.
//
//

#include "CCard.h"
#include "CCardBufferStatus.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CCard::CCard()
{
    m_icard_id=0;
    m_scard_name.clear();
    m_ccard_next=0;;
    m_sicard_star=1;
    m_icard_stirps=0;
    m_icard_suit=0;
    m_icard_leadership=0;
    m_icard_exp=0;    //吃掉该卡牌的 经验
    m_icard_price=0;  //卖掉该卡佩的 价格
    m_ileve_max=0;   //等级最高级别
    m_icard_attack=0; //攻击力
    m_icard_defend=0; //防御力
    m_icardhp=0;    //卡牌的总的HP
    m_iskillLine=0;
    m_iskillHelp=0;
    m_iskillDead=0;
    m_iskillBuff=0;
    m_scard_tips.clear();
    m_scard_resources.clear();
    m_scard_head.clear();
    m_scard_ground.clear();
    
    
}
CFightCard::CFightCard()
{
    m_pCard=NULL;
}


CFightCard::CFightCard(CCard *card,int level)
{
    tag=-1;
    m_iSuit=1;
    if(card)
    {
        
        m_pCard=card;
        this->m_iCurrHp=m_pCard->m_icardhp;
        m_iHp=m_iCurrHp;
        m_attack=card->m_icard_attack;
        m_defend=card->m_icard_defend;
        isDead=false;
        m_iSuit=card->m_icard_suit;
    }
    else
    {
        m_pCard=NULL;
    }
    initFighting();
}

void CFightCard::initFighting()
{
    isSendZengfu=false;
    isDead=false;
    m_iCurrEngry=0;
    DELETE_POINT_VECTOR(m_vBuffer,list<CCardBufferStatus*>);
}

CFightCard::~CFightCard()
{
    
}

void CFightCard::needRebackAtkAndBuf(CCardBufferStatus *buffer)
{
    
    switch (buffer->m_enBuffer_Field) {
        case  EN_BUFF_FIELD_TYPE_ATTACK:   //影响攻击力
            this->m_attack+=-buffer->m_iValue;
            break;
        case  EN_BUFF_FIELD_TYPE_DEFEND:   //影响防御力:
            this->m_defend +=-buffer->m_iValue;
            break;
        default:
            break;
    }
}

void CFightCard::appendBuffer(CCardBufferStatus *buffer)
{
    if(m_vBuffer.size()==0)
    {
        m_vBuffer.push_back(buffer);
    }
    else{
        list<CCardBufferStatus *>::iterator it;
        for (it=m_vBuffer.begin(); it!=m_vBuffer.end(); it++)
        {
            //如果mutex的需要 一样的话， 并且字段的类型都是一样的话。
            if((*it)->m_mutex==buffer->m_mutex && (*it)->m_enBuffer_Field==buffer->m_enBuffer_Field)
            {
                if ((*it)->m_mutexlevel > buffer->m_mutexlevel) {
                    delete buffer;
                    buffer=NULL;
                }
                else
                {
                    needRebackAtkAndBuf(*it);
                    m_vBuffer.erase(it);
                    delete *it;
                    *it=NULL;
                    m_vBuffer.push_back(buffer);
                    break;
                }
            }
        }
    }
    
}
