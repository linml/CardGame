//
//  CardSprite.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "CardSprite.h"


#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}


CCardSprite::CCardSprite()
{
    
}

CCardSprite::CCardSprite(CCardSprite &t1)
{
    this->m_cardData=t1.m_cardData;
    
}
CCardSprite::~CCardSprite()
{
    
}

string CCardSprite::getName()
{
    return "";
}

SFightCardSprite::~SFightCardSprite()
{
        if(cardsprite)
        {
            delete cardsprite;
            cardsprite=NULL;
        }
        DELETE_POINT_VECTOR(m_vbuffSkill,  vector<SkillBuff *>);
}


void SFightCardSprite::initShangHai()
{
    m_iJianMianShanghai=0;
    m_iHuduanshanghai=0;
    if (m_iXuanYun>=1)
    {
        m_iXuanYun--;

    }
    if(m_ibingdong>=1)
    {
      m_ibingdong--;
    }
    
}

void SFightCardSprite::fantanShanghai(SFightCardSprite **sfightCardSprite)
{
    (*sfightCardSprite)->cardsprite->m_cardData.m_unCurrentHp-=m_iHuduanshanghai;
}


void SFightCardSprite::dealLastHp()
{
    this->cardsprite->m_cardData.m_unCurrentHp  += m_iJianMianShanghai;
    this->cardsprite->m_cardData.m_unCurrentHp  +=m_iHuduanshanghai;
}


bool SFightCardSprite::isCannotATK()
{
    return m_ibingdong||m_iXuanYun||m_iShuiMian;
}

