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
#include "Utility.h"
#include "PtHttpClient.h"
#include "CConfigResourceLoad.h"
#include "PtJsonUtility.h"
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
    isLoadServer=false;
    
    for (int i=0; i<m_vvBattleArray.size(); i++) {
        DELETE_POINT_VECTOR(m_vvBattleArray[i],vector<CFightCard*> );
    }
    m_vvBattleArray.clear();
    
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
    G_SingleCConfigResourceLoad::instance()->loadCardInfo(m_hashmapAllCard, cardFileName);
}

void CGamePlayer::clearPlayerTable()
{
    DELETE_POINT_VECTOR(m_gvPlayerLevel, vector<SLevelPlayer *>)
}

void CGamePlayer::initPlayerTable(const char *playerFileName)
{
    G_SingleCConfigResourceLoad::instance()->loadPlayerLevelInfo(&m_gvPlayerLevel, playerFileName);
}

void CGamePlayer::initByServerDictorny(cocos2d::CCDictionary *dict)
{
    isLoadServer=true;
    if(dict)
    {
        m_iCurrentExp=GameTools::intForKey("exp",dict);
        m_iCurrentHp=GameTools::intForKey("hp",dict);
        m_iCurrentMp=GameTools::intForKey("mp", dict);
        m_iCurrentLeader=GameTools::intForKey("mp", dict);
        m_iCardBagNum=GameTools::intForKey("card_bag_num", dict);
        m_sLevelPlayer=m_gvPlayerLevel[GameTools::intForKey("level", dict)];
        CCArray *aary=GameTools::arrayForKey("card_info", dict);
        initFightingCardByserverDictorny(aary);
    }
}

void CGamePlayer::initFightingCardByserverDictorny(CCArray *array)
{
    for (int i=0; i<array->count(); i++) {
        CCDictionary *temp=(CCDictionary *)array->objectAtIndex(i);
        int value=GameTools::intForKey("card_id",temp);
        int level=GameTools::intForKey("card_level", temp);
        m_hashmapFightingCard.push_back(new CFightCard(m_hashmapAllCard[value],level));
        
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

void CGamePlayer::loadServerCardBag()
{
    isLoadCardBagEnd=false;
    ADDHTTPREQUEST("http://cube.games.com/api.php?m=Card&a=getCardItem&uid=194&sig=2ac2b1e302c46976beaab20a68ef95", "merlin", "merlinaskplayerinfo", callfuncO_selector(CGamePlayer::parseCardBagJson));

}

void CGamePlayer::parseCardBagJson(cocos2d::CCObject *obj)
{ 
    clearServerCardBag();
    //添加card  字符串
    char *tempdata=(char *)obj;
    CCLog("%s",tempdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    delete []tempdata;
    tempdata=NULL;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "merlinaskplayerinfo");
    assert(dict&&"aaaaaaaaa");
    if(GameTools::intForKey("code", dict)!=0)
    {
        CCLog("error");
        return ;
    }
    CCDictionary *directory =(CCDictionary*)dict->objectForKey("result");
    CCArray *array=directory->allKeys();;
    for (int i=0; i<array->count(); i++) {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CCDictionary *detail=(CCDictionary*)(directory->objectForKey(key->m_sString));

        int level=GameTools::intForKey("level", detail);
        int card_id=GameTools::intForKey("card_id", detail);
        int cardexp=GameTools::intForKey("exp", detail);
        int card_item_id=GameTools::intForKey("card_item_id", detail);
        CFightCard *fightCard=new CFightCard(m_hashmapAllCard[card_id],level);
        fightCard->m_iCurrExp=cardexp;
        fightCard->m_User_Card_ID=card_item_id;
        m_vCardBag.push_back(fightCard);
    }
    
    isLoadCardBagEnd=true;
}

void CGamePlayer::clearServerCardBag()
{
     DELETE_POINT_VECTOR(m_vCardBag, vector<CFightCard *>);
}

bool CGamePlayer::isCardBagContainUserCardList(vector<int>User_CardId)
{
    for (int i=0; i<User_CardId.size(); i++) {
        bool isExsit=false;
        for (vector<CFightCard *>::iterator it=m_vCardBag.begin(); it!=m_vCardBag.end();)
        {
            if((*it)->m_User_Card_ID==User_CardId[i])
            {
                isExsit=true;
                break;
            }
        }
        if(!isExsit)
        {
            return false;
        }
    }
    return true;
}

void CGamePlayer::deleteFromCardBag(vector<int>user_CardId)
{
    for (int i=0; i<user_CardId.size(); i++)
    {
        for (vector<CFightCard *>::iterator it=m_vCardBag.begin(); it!=m_vCardBag.end();)
        {
            if((*it)->m_User_Card_ID==user_CardId[i])
            {
                CFightCard * temp=*it;
                it=m_vCardBag.erase(it);
                delete temp;
                temp=NULL;
                break;
            }
            else{
                it++;
            }
        }
    }    
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
                cout<<"card id:"<<it->first<<endl;
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

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
