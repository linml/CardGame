//
//  CCard.cpp
//  91.cube
//
//  Created by linminglu on 13-6-26.
//
//

#include "CCard.h"

CCard::CCard()
{
    m_icard_id=0;
    m_scard_name.clear();
    m_ccard_color=0;;
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
    m_scard_groud.clear();
    
}

CFightCard::CFightCard(CCard *card)
{
    tag=-1;
    if(card)
    {
        m_pCard=new CCard(*card);
        this->m_iCurrHp=m_pCard->m_icardhp;
    }
    else
    {
        m_pCard=NULL;
    }
}

CFightCard::~CFightCard()
{
    if(m_pCard)
    {
        delete  m_pCard;
        m_pCard=NULL;
    }
    
}

