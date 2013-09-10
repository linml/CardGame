//
//  CGameNpcCard.cpp
//  91.cube
//
//  Created by linminglu on 13-9-10.
//
//

#include "CGameNpcCard.h"
CNpcCard::CNpcCard()
{
    m_npcCardLevel=0;
};
//~CNpcCard(){};
int CNpcCard::getCardLevel()
{
    return m_npcCardLevel;
}



CGameNpcCard::CGameNpcCard(CCard *card,int level):CFightCard()
{
  m_iCurrLevel=level;
  m_attack=m_iNpcAttack=card->m_icard_attack;
  m_defend=m_iNpcDefine=card->m_icard_defend;
  m_iCurrExp=m_iNpcHp=card->m_icardhp;
  m_iHp=card->m_icardhp;
}

CGameNpcCard::~CGameNpcCard()
{
    
}

int  CGameNpcCard::getAddValue(int level, int type)
{
    switch (type) {
        case 1:
            return  m_iNpcAttack;
        case 2:
            return  m_iNpcDefine;
        case 3:
            return  m_iNpcHp;
        default:
            return 0;
            break;
    }
    
}