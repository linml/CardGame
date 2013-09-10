//
//  CGameNpcCard.h
//  91.cube
//
//  Created by linminglu on 13-9-10.
//
//

#ifndef ___1_cube__CGameNpcCard__
#define ___1_cube__CGameNpcCard__

#include <iostream>
#include "CCard.h"
class CNpcCard :public CCard
{
public:
    CNpcCard();
    ~CNpcCard(){};
    int m_npcCardLevel;
};
class CGameNpcCard :public CFightCard
{
public:
    CGameNpcCard(CNpcCard *card);
    ~CGameNpcCard();
private:
    int  getAddValue(int level, int type);
private:
    int m_iNpcAttack;
    int m_iNpcDefine;
    int m_iNpcHp;



};

#endif /* defined(___1_cube__CGameNpcCard__) */
