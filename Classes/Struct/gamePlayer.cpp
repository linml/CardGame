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

static vector<CCardSprite *>g_FightCard;
static void  FightCarderInit()
{
    CCardSprite *cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="雷娜斯";
     cardproperty->m_cardData.m_sResourceName="green_raynas";
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_unCardLevel=5;
    cardproperty->m_cardData.m_unPlayerCardAttack=20;
    cardproperty->m_cardData.m_unPlayerCardDefence=9;
    cardproperty->m_cardData.m_unHp=9;
    cardproperty->m_cardData.m_unCurrentHp=9;
    cardproperty->m_cardData.initJineng(1, -1, 2, 3);
    //cardproperty->m_cardData.m_pBasicSkill=new CCraftsmanship
    g_FightCard.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="奥比里西斯";
    cardproperty->m_cardData.m_sResourceName="blue_aobraysies";
    cardproperty->m_cardData.m_unCardLevel=4;
    cardproperty->m_cardData.m_unPlayerCardAttack=14;
    cardproperty->m_cardData.m_unPlayerCardDefence=13;
    cardproperty->m_cardData.m_unHp=10;
    cardproperty->m_cardData.m_unCurrentHp=10;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(4, -1, 5, 6);
    g_FightCard.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="芙蕾雅";
    cardproperty->m_cardData.m_unCardLevel=2;
     cardproperty->m_cardData.m_sResourceName="purple_princess";
    cardproperty->m_cardData.m_unPlayerCardAttack=17;
    cardproperty->m_cardData.m_unPlayerCardDefence=7;
    cardproperty->m_cardData.m_unHp=6;
    cardproperty->m_cardData.m_unCurrentHp=6;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(7, -1, 8, 9);
    g_FightCard.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="阿尔维斯";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_sResourceName="gold_aerwise";
    cardproperty->m_cardData.m_unPlayerCardAttack=15;
    cardproperty->m_cardData.m_unPlayerCardDefence=8;
    cardproperty->m_cardData.m_unHp=7;
    cardproperty->m_cardData.m_unCurrentHp=7;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(10, -1, 11, 12);
    g_FightCard.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="霜牙";
    cardproperty->m_cardData.m_sResourceName="red_wolf";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_unPlayerCardAttack=19;
    cardproperty->m_cardData.m_unPlayerCardDefence=10;
    cardproperty->m_cardData.m_unHp=8;
    cardproperty->m_cardData.m_unCurrentHp=8;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(13, -1, 14, 15);
    g_FightCard.push_back(cardproperty);
    
    
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="狼";
    cardproperty->m_cardData.m_unCardLevel=1;
    cardproperty->m_cardData.m_unPlayerCardAttack=17;
    cardproperty->m_cardData.m_unPlayerCardDefence=4;
    cardproperty->m_cardData.m_unHp=4;
    cardproperty->m_cardData.m_sResourceName="test_card";
    cardproperty->m_cardData.m_unCurrentHp=4;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(16, -1, 17, 0);
    g_FightCard.push_back(cardproperty);
}
CGamePlayer::CGamePlayer()
{
    m_nFaceTo = 0;
    m_stcCurrenPos.x = 10;
    m_stcCurrenPos.y = 10;
    m_stcCurrenPos.z = 10;
    FightCarderInit();
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
    
    //随机获取卡牌 5 张
    srand(time(0));
    for(int i=0; i<5; i++)
    {
        int index=rand()%(g_FightCard.size());
        this->m_vMonsterCardSprite.push_back(g_FightCard[index]);
    }

}

void CGamePlayer::forTest()
{
    for (int i=0 ; i<5; i++) {
        m_vZhangdouCardSprite.push_back(g_FightCard[i]);
    }
    
    return ;
    
  /*
    CCardSprite *cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="雷娜斯";
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.m_unCardLevel=5;
    cardproperty->m_cardData.m_unPlayerCardAttack=20;
    cardproperty->m_cardData.m_unPlayerCardDefence=9;
    cardproperty->m_cardData.m_unHp=9;
    cardproperty->m_cardData.m_unCurrentHp=9;
    cardproperty->m_cardData.initJineng(1, -1, 2, 3);
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
    cardproperty->m_cardData.initJineng(4, -1, 5, 6);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="芙蕾雅";
    cardproperty->m_cardData.m_unCardLevel=2;
    cardproperty->m_cardData.m_unPlayerCardAttack=17;
    cardproperty->m_cardData.m_unPlayerCardDefence=7;
    cardproperty->m_cardData.m_unHp=6;
    cardproperty->m_cardData.m_unCurrentHp=6;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(7, -1, 8, 9);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="阿尔维斯";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_unPlayerCardAttack=15;
    cardproperty->m_cardData.m_unPlayerCardDefence=8;
    cardproperty->m_cardData.m_unHp=7;
    cardproperty->m_cardData.m_unCurrentHp=7;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(10, -1, 11, 12);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
    
    cardproperty=new CCardSprite ();
    cardproperty->m_cardData.m_sPlayerCardName="霜牙";
    cardproperty->m_cardData.m_unCardLevel=3;
    cardproperty->m_cardData.m_unPlayerCardAttack=19;
    cardproperty->m_cardData.m_unPlayerCardDefence=10;
    cardproperty->m_cardData.m_unHp=8;
    cardproperty->m_cardData.m_unCurrentHp=8;
    cardproperty->m_cardData.m_eCardCategory=(EN_CARDCATEGORY)(rand()%4+1);
    cardproperty->m_cardData.initJineng(13, -1, 14, 15);
    this->m_vZhangdouCardSprite.push_back(cardproperty);
   */
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
