//
//  gamePlayer.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-5-24.
//
//

#include "gamePlayer.h"
#include "gameTools.h"
#include "gameConfig.h"
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
    initAllCard((resRootPath+"card.plist").c_str());
    forTestCard();
}

CGamePlayer::~CGamePlayer()
{
    clearAllCard();
    //DELETE_POINT_VECTOR(m_vZhangdouCardSprite, vector<CCardSprite *>);
    
   // DELETE_POINT_VECTOR(m_vZhangdouCardSprite, vector<CCardSprite *>);
    
}

void CGamePlayer::forTestDeleteMonster()
{
   // DELETE_POINT_VECTOR(m_vMonsterCardSprite,vector<CCardSprite *>);
    m_vMonsterCardSprite.erase(m_vMonsterCardSprite.begin(), m_vMonsterCardSprite.end());
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
}

void CGamePlayer::forTestCard()
{
    //获取当前所有卡佩里面的5张牌面给原先的
    srand(time(0));
    for (int i=0;i<5;i++)
    {
        int index=rand()%m_hashmapAllCard.size();
        int tempindex=0;
        for (map<int, CCard *> ::iterator it=m_hashmapAllCard.begin(); it!=m_hashmapAllCard.end();it++) {
            if(tempindex==index)
            {
                cout<<"card id:"<<it->first<<endl;;
              m_hashmapFight.push_back(it->second);
                
                break;
            }
            tempindex++;
        }
    }
}

void CGamePlayer::forTestMonsterCard()
{
    //srand(time(0));
    for (int i=0;i<5;i++)
    {
        int index=rand()%m_hashmapAllCard.size();
        int tempindex=0;
        for (map<int, CCard *> ::iterator it=m_hashmapAllCard.begin(); it!=m_hashmapAllCard.end();it++) {
            if(tempindex==index)
            {
                cout<<"card Monster id:"<<it->first<<endl;
                m_hashmapMonster.push_back(it->second);
                break;
            }
            tempindex++;
        }
    }
}

void CGamePlayer::clearAllCard()
{
    map<int ,CCard *>::iterator it;
    for(it=m_hashmapAllCard.begin();it!=m_hashmapAllCard.end();++it)
    {
        if(it->second)
        {
            delete it->second;
            it->second=NULL;
        }
    }
    m_hashmapAllCard.erase(m_hashmapAllCard.begin(),m_hashmapAllCard.end());
}

void CGamePlayer::initAllCard(const char *cardFileName)
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(cardFileName);
    CCArray *vKeyArray=directory->allKeys();
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *cardDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CCard *card=new CCard;
        card->m_icard_id=GameTools::intForKey("card_id", cardDirector);
        card->m_scard_name=string(GameTools::valueForKey("card_name", cardDirector));
        card->m_ccard_next=GameTools::intForKey("card_next", cardDirector);  ///背景底色
        card->m_sicard_star=GameTools::intForKey("card_star", cardDirector);
        card->m_icard_stirps=GameTools::intForKey("card_stirps", cardDirector);   //种族
        card->m_icard_suit=GameTools::intForKey("card_suit", cardDirector);      //随机数值
        card->m_icard_leadership=GameTools::intForKey("card_leadership", cardDirector);
        card->m_icard_exp=GameTools::intForKey("card_exp", cardDirector);    //吃掉该卡牌的 经验
        card->m_icard_price=GameTools::intForKey("card_price", cardDirector);  //卖掉该卡佩的 价格
        card->m_ileve_max=GameTools::intForKey("card_leve_max", cardDirector);   //等级最高级别
        card->m_icard_attack=GameTools::intForKey("card_attack", cardDirector); //攻击力
        card->m_icard_defend=GameTools::intForKey("card_defend", cardDirector); //防御力
        card->m_icardhp=GameTools::intForKey("card_hp",cardDirector);// 卡牌的总的HP
        card->m_iusually_attack=GameTools::intForKey("usually_attack", cardDirector);
        card->m_iskillLine=GameTools::intForKey("skill_anger", cardDirector);
        card->m_iskillHelp=GameTools::intForKey("skill_help", cardDirector);
        card->m_iskillDead=GameTools::intForKey("skill_dead", cardDirector);
        card->m_iskillBuff=GameTools::intForKey("skill_buff", cardDirector);
        card->m_scard_tips=string(GameTools::valueForKey("card_tips", cardDirector));
        card->m_scard_resources=GameTools::valueForKey("card_resources", cardDirector);
        card->m_scard_head=GameTools::valueForKey("card_head", cardDirector);
        card->m_scard_groud=GameTools::valueForKey("card_groud", cardDirector);
        m_hashmapAllCard[key->intValue()]=card;
    }
    cout<<"=========>>>>"<<m_hashmapAllCard.size()<<endl;
}
bool CGamePlayer::isCanUpdateLevel(int nExp)
{
    if(m_iCurrentExp+nExp>=m_sLevelPlayer->m_iexp)
    {
        return true;
    }
    return false;
}

bool CGamePlayer::UpdateLevel()
{
//    do {
//        <#statements#>
//    } while (<#condition#>);
}

int CGamePlayer::getTotoalHp()
{
    
}

int CGamePlayer::getCurrentHp()
{
    
}

int CGamePlayer::getTotoalMp()
{
    
}

int CGamePlayer::getCurrentMp()
{
    
}

int CGamePlayer::getTotalExp()
{
    
}

int CGamePlayer::getCurrentExp()
{
    
}
void CGamePlayer::setCurrentHp(int nHp)
{
    
    
}
void CGamePlayer::setCurrentMp(int nMp)
{
    
}
void CGamePlayer::setCurrentExp(int nExp)
{
    
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
