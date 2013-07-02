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


CGamePlayer::CGamePlayer()
{
    m_nFaceTo = 0;
    m_stcCurrenPos.x = 10;
    m_stcCurrenPos.y = 10;
    m_stcCurrenPos.z = 10;
}

CGamePlayer::~CGamePlayer()
{
    clearAllCard();
    clearPlayerTable();
}

void CGamePlayer::loadGamesConfig()
{
    initAllCard((resRootPath+"card.plist").c_str());
    initPlayerTable((resRootPath +"level_max.plist").c_str());
}

void CGamePlayer::initGames()
{
   
    if(m_gvPlayerLevel.size()==0)
    {
        CCLog("game  player level  error");
    }
    else
    {
        m_sLevelPlayer=m_gvPlayerLevel[0];
        initPlayerStatusZero();
    }
    
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
    m_hashmapMonster.erase(m_hashmapMonster.begin(),m_hashmapMonster.end());
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
    cout<<"card plist size ="<<m_hashmapAllCard.size()<<endl;
}

void CGamePlayer::clearPlayerTable()
{
    DELETE_POINT_VECTOR(m_gvPlayerLevel, vector<SLevelPlayer *>)
}

void CGamePlayer::initPlayerTable(const char *playerFileName)
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(playerFileName);
    CCArray *vKeyArray=directory->allKeys();
    for (int i=0; i<vKeyArray->count(); i++)
    {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *playerDictionary=(CCDictionary*)(directory->objectForKey(key->m_sString));
        SLevelPlayer *playerLevel=new SLevelPlayer;
        playerLevel->m_iLevel=GameTools::intForKey("level", playerDictionary);
        playerLevel->m_iCard_max=GameTools::intForKey("card_max", playerDictionary);
        playerLevel->m_iLeadership=GameTools::intForKey("leadership", playerDictionary);
        playerLevel->m_iFriend_max=GameTools::intForKey("friend_max",playerDictionary); //好友上线
        playerLevel->m_iHP=GameTools::intForKey("hp",playerDictionary);
        playerLevel->m_iMp=GameTools::intForKey("mp",playerDictionary);
       playerLevel->m_iexp=GameTools::intForKey("exp", playerDictionary);
       this->m_gvPlayerLevel.push_back(playerLevel);
    }
}


void CGamePlayer::initPlayerStatusZero()
{
    m_iCurrentExp=0;
    m_iCurrentHp=m_sLevelPlayer->m_iHP;
    m_iCurrentMp=m_sLevelPlayer->m_iexp;
}

bool CGamePlayer::isCanUpdateLevel(int nExp)
{
    if(m_iCurrentExp+nExp>=m_sLevelPlayer->m_iexp)
    {
        return true;
    }
    return false;
}

bool CGamePlayer::UpdateLevel(int nExp)
{
    while(isCanUpdateLevel(nExp))
    {
        nExp-=m_sLevelPlayer->m_iexp-m_iCurrentExp;
        if(m_sLevelPlayer->m_iLevel+1<m_gvPlayerLevel.size())
        {
            this->m_sLevelPlayer=m_gvPlayerLevel[m_sLevelPlayer->m_iLevel+1];
            initPlayerStatusZero();
            m_iCurrentExp=nExp;
        }
    }
    return  true;
}

void CGamePlayer::getSeverPlayerInfo(cocos2d::CCObject *object)
{
    
}



int CGamePlayer::getTotoalHp()
{
    return m_sLevelPlayer->m_iHP;
}

int CGamePlayer::getCurrentHp()
{
    return this->m_sLevelPlayer->m_iHP;
}

int CGamePlayer::getTotoalMp()
{
    return this->m_sLevelPlayer->m_iHP;
}

int CGamePlayer::getCurrentMp()
{
    return this->m_iCurrentMp;
}

int CGamePlayer::getTotalExp()
{
    return this->m_sLevelPlayer->m_iexp;
}

int CGamePlayer::getCurrentExp()
{
    return this->m_iCurrentExp;
}
void CGamePlayer::setCurrentHp(int nHp)
{
    this->m_iCurrentHp=nHp;
    
}
void CGamePlayer::setCurrentMp(int nMp)
{
    this->m_iCurrentMp=nMp;
}
void CGamePlayer::setCurrentExp(int nExp)
{
    this->m_iCurrentHp=nExp;
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
