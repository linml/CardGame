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
    cardproperty->m_cardData.m_vSkillList.push_back(1);
    cardproperty->m_cardData.m_vSkillList.push_back(2);
    cardproperty->m_cardData.m_vSkillList.push_back(3);
    //cardproperty->m_cardData.m_pBasicSkill=new CCraftsmanship
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="奥比里西斯";
    cardproperty->m_cardData.m_unCardLevel=4;
    cardproperty->m_cardData.m_unPlayerCardAttack=14;
    cardproperty->m_cardData.m_unPlayerCardDefence=13;
    cardproperty->m_cardData.m_unHp=10;
    cardproperty->m_cardData.m_unCurrentHp=10;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_vSkillList.push_back(4);
    cardproperty->m_cardData.m_vSkillList.push_back(5);
    cardproperty->m_cardData.m_vSkillList.push_back(6);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="芙蕾雅";
    cardproperty->m_cardData.m_unCardLevel=2;
    cardproperty->m_cardData.m_unPlayerCardAttack=17;
    cardproperty->m_cardData.m_unPlayerCardDefence=7;
    cardproperty->m_cardData.m_unHp=6;
    cardproperty->m_cardData.m_unCurrentHp=6;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_vSkillList.push_back(7);
    cardproperty->m_cardData.m_vSkillList.push_back(8);
    cardproperty->m_cardData.m_vSkillList.push_back(9);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="阿尔维斯";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_unPlayerCardAttack=15;
    cardproperty->m_cardData.m_unPlayerCardDefence=8;
    cardproperty->m_cardData.m_unHp=7;
    cardproperty->m_cardData.m_unCurrentHp=7;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_vSkillList.push_back(10);
    cardproperty->m_cardData.m_vSkillList.push_back(11);
    cardproperty->m_cardData.m_vSkillList.push_back(12);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="霜牙";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_unPlayerCardAttack=19;
    cardproperty->m_cardData.m_unPlayerCardDefence=10;
    cardproperty->m_cardData.m_unHp=8;
    cardproperty->m_cardData.m_unCurrentHp=8;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_vSkillList.push_back(13);
    cardproperty->m_cardData.m_vSkillList.push_back(14);
    cardproperty->m_cardData.m_vSkillList.push_back(15);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
