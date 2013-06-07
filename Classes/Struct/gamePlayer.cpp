//
//  gamePlayer.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-5-24.
//
//

#include "gamePlayer.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
    delete *it; \
    *it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CGamePlayer::CGamePlayer()
{
    m_nFaceTo = 0;
    m_stcCurrenPos.x = 10;
    m_stcCurrenPos.y = 10;
    m_stcCurrenPos.z = 10;
    forTest();
}

CGamePlayer::~CGamePlayer()
{
    DELETE_POINT_VECTOR(m_vZhangdouCardSprite, vector<CCardSprite *>);
    
    DELETE_POINT_VECTOR(m_vZhangdouCardSprite, vector<CCardSprite *>);
    
}

void CGamePlayer::forTestDeleteMonster()
{
    DELETE_POINT_VECTOR(m_vMonsterCardSprite,vector<CCardSprite *>);
}

void CGamePlayer::forTestMonster()
{
    forTestDeleteMonster();
    srand(time(0));
    int monsterCount=rand()%5+1;
    for (int i=0; i<monsterCount; i++)
    {
        CCardSprite *cardproperty=new CCardSprite ();
        cardproperty->m_cardData.m_sPlayerCardName="****";
        cardproperty->m_cardData.m_unCardLevel=3;
        cardproperty->m_cardData.m_unPlayerCardAttack=rand()%1000+100;
        cardproperty->m_cardData.m_unPlayerCardDefence=rand()%200+100;
        cardproperty->m_cardData.m_unHp=cardproperty->m_cardData.m_unCurrentHp=rand()%2000+500;
        m_vMonsterCardSprite.push_back(cardproperty);
    }
}

void CGamePlayer::forTest()
{
    CCardSprite *cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="雷娜斯";
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_unCardLevel=5;
    cardproperty->m_cardData.m_unPlayerCardAttack=20;
    cardproperty->m_cardData.m_unPlayerCardDefence=9;
    cardproperty->m_cardData.m_unHp=9;
    cardproperty->m_cardData.m_unCurrentHp=9;
    //cardproperty->m_cardData.m_pBasicSkill=new CCraftsmanship
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="dada";
    cardproperty->m_cardData.m_unCardLevel=4;
    cardproperty->m_cardData.m_unPlayerCardAttack=100;
    cardproperty->m_cardData.m_unPlayerCardDefence=200;
    cardproperty->m_cardData.m_unHp=1000;
    cardproperty->m_cardData.m_unCurrentHp=1000;
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="caca";
    cardproperty->m_cardData.m_unCardLevel=1;
    cardproperty->m_cardData.m_unPlayerCardAttack=100;
    cardproperty->m_cardData.m_unPlayerCardDefence=200;
    cardproperty->m_cardData.m_unHp=1000;
    cardproperty->m_cardData.m_unCurrentHp=1000;
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="cucu";
    cardproperty->m_cardData.m_unCardLevel=5;
    cardproperty->m_cardData.m_unPlayerCardAttack=100;
    cardproperty->m_cardData.m_unPlayerCardDefence=200;
    cardproperty->m_cardData.m_unHp=4000;
    cardproperty->m_cardData.m_unCurrentHp=2000;
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="cici";
    cardproperty->m_cardData.m_unCardLevel=2;
    cardproperty->m_cardData.m_unPlayerCardAttack=100;
    cardproperty->m_cardData.m_unPlayerCardDefence=200;
    cardproperty->m_cardData.m_unHp=3000;
    cardproperty->m_cardData.m_unCurrentHp=3000;
    this->m_vZhangdouCardSprite.push_back(cardproperty);
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
