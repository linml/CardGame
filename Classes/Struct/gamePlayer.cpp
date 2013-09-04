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
#include "CSkillData.h"
#include "CFightSkillManager.h"
#include "SEveryATKData.h"
#include <fstream>
#include "PtHttpURL.h"
#include "CFightCardBufferData.h"
#include "CGameEmailManager.h"
#include "SGamePlayerData.h"

//#define AAAAFOROSMACHINE 1

using namespace std;
#define SKILLPUTONGGONGJIID 100000
#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
    for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
        __TYPECLASSNAME__ *temp=*it; \
        delete temp; \
        temp=NULL; \
    } \
    VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}



CGamePlayer::CGamePlayer() : m_rAllProps(SinglePropConfigData::instance()->getProps())
{

    m_strSig = "";
    m_strUid = "";
    isLoadServer=false;
    for (int i=0; i<m_vvBattleArray.size(); i++) {
        DELETE_POINT_VECTOR(m_vvBattleArray[i],vector<CFightCard*> ,CFightCard );
    }
    m_vvBattleArray.clear();
    loadGamesConfig();
    m_gGamePlayerData=new CGamePlayerData();
    initPlayerStatusZero();
}

CGamePlayer::~CGamePlayer()
{
    onExitGameApp();
    if(m_gGamePlayerData)
    {
        delete m_gGamePlayerData;
        m_gGamePlayerData=NULL;
    }
}

void CGamePlayer::loadGamesConfig()
{
    initAllCard((resRootPath+"card_config.plist").c_str());
    initPlayerTable((resRootPath +"level_config.plist").c_str());
    loadAllSkillInfo((resRootPath+"skill_config.plist").c_str());
    loadAllEffectInfo((resRootPath + "skill_effect_config.plist").c_str());
    G_FightSkillManager::instance()->initSkill();//加载列表
}

void CGamePlayer::onExitGameApp()
{
    clearAllEffectInfo();
    clearAllSkillInfo();
    clearAllCard();
    clearPlayerTable();
    
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

CCard *CGamePlayer::getCardByCardId(int cardid)
{
    return m_hashmapAllCard[cardid];
}
void CGamePlayer::clearPlayerTable()
{
    DELETE_POINT_VECTOR(m_gvPlayerLevel, vector<SLevelPlayer *> ,SLevelPlayer)
}

void CGamePlayer::initPlayerTable(const char *playerFileName)
{
    G_SingleCConfigResourceLoad::instance()->loadPlayerLevelInfo(&m_gvPlayerLevel, playerFileName);
}
//读取技能表格
void CGamePlayer::loadAllSkillInfo(const char *skillFileName)
{
    G_SingleCConfigResourceLoad::instance()->loadSkillLogicInfo(m_vSkillInfo, skillFileName);
}

void CGamePlayer::clearAllSkillInfo()
{

    for (map<int , CSkillData *>::iterator it=m_vSkillInfo.begin(); it!= m_vSkillInfo.end(); it++)
    {
            delete it->second; 
            it->second=NULL;
                m_vSkillInfo.erase(it);
    }
        m_vSkillInfo.erase(m_vSkillInfo.begin(),m_vSkillInfo.end());
}
string CGamePlayer::getBufferPlistByEffectId(int effectID)
{
    for (int i=0; i<m_vImpactInfo.size(); i++) {
        if(m_vImpactInfo[i]->m_ieffect_id==effectID)
        {
            vector<string>tempstr=GameTools::splitString(m_vImpactInfo[i]->m_sEffectFile.c_str(), ",");
            if(tempstr.size()>1)
            {
                return tempstr[0];
            }
            else{
                return "";
            }
            
        
        }
    }
    return "";
}
string CGamePlayer::getBufferPngByEffectId(int effectID)
{
    for (int i=0; i<m_vImpactInfo.size(); i++) {
        if(m_vImpactInfo[i]->m_ieffect_id==effectID)
        {
            vector<string>tempstr=GameTools::splitString(m_vImpactInfo[i]->m_sEffectFile.c_str(), ",");
            if(tempstr.size()>1)
            {
                return tempstr[1];
            }
            else{
                return "";
            }
            
            
        }
    }
    return "";
    
}

CImapact *CGamePlayer::getEffectTableByEffectId(int effectId)
{
    for (int i=0; i<m_vImpactInfo.size(); i++) {
        if(m_vImpactInfo[i]->m_ieffect_id==effectId)
        {
            return m_vImpactInfo[i];
        }
    }
    return  NULL;
}

CSkillData *CGamePlayer::getSkillBySkillId(int skillId)
{
    return m_vSkillInfo[skillId];
}

CSkillData *CGamePlayer::getPutongGongji()
{

    return m_vSkillInfo[SKILLPUTONGGONGJIID];
}


//读取效果表格
void CGamePlayer::loadAllEffectInfo(const char *effectFileName)
{
     G_SingleCConfigResourceLoad::instance()->loadEffectLogicInfo(m_vImpactInfo,effectFileName);
}

void CGamePlayer::clearAllEffectInfo()
{
    DELETE_POINT_VECTOR(m_vImpactInfo, vector<CImapact *> ,CImapact);
}

CImapact *CGamePlayer::findByImpactId(int tempImpactId)
{
    for (int i=0 ; i<m_vImpactInfo.size(); i++) {
        if(tempImpactId==m_vImpactInfo[i]->m_ieffect_id)
        {
            return m_vImpactInfo[i];
        }
    }
    return NULL;
}

void CGamePlayer::initByServerDictorny(cocos2d::CCDictionary *dict)
{
    if(dict)
    {
        m_gGamePlayerData->decodeDictnory(dict);
        
        CCArray *aary=GameTools::arrayForKey("card_info", dict);
        initFightingCardByserverDictorny(aary);
    }
   isLoadServer=true;
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
    m_gGamePlayerData->setLevelConfig(m_gvPlayerLevel[0]);
}

void CGamePlayer::getSeverPlayerInfo(cocos2d::CCObject *object)
{
    
}

void CGamePlayer::loadServerCardBag()
{
    isLoadCardBagEnd=0;
#ifndef AAAAFOROSMACHINE
    //http://cube.games.com/api.php?m=Card&a=getCardItem&uid=194&sig=2ac2b1e302c46976beaab20a68ef95
    ADDHTTPREQUESTPOSTDATA(STR_URL_GET_ITEM(194), "merlin", "merlinaskplayerinfo","sig=2ac2b1e302c46976beaab20a68ef95" ,callfuncO_selector(CGamePlayer::parseCardBagJson));
#else
    char *data=new char [5];
    parseCardBagJson((CCObject *)data);
#endif
    
}

void CGamePlayer::parseCardBagJson(cocos2d::CCObject *obj)
{
    clearServerCardBag();
    //添加card  字符串
    char *tempdatat=(char *)obj;
    CCLog("%s",tempdatat);
    if(tempdatat==NULL)
    {
        isLoadCardBagEnd=ERROR_MSG_CONNECTSERVERERROR;
        return;
    }
#ifndef AAAAFOROSMACHINE

    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdatat);
#else
    const char *tempdata=readFileName((resRootPath+"cardbg.txt").c_str()).c_str();
     CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
#endif
    delete []tempdatat;
    tempdatat=NULL;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "merlinaskplayerinfo");
    assert(dict&&"aaaaaaaaa");
    if(GameTools::intForKey("code", dict)!=0)
    {
        CCLog("error");
        return ;
    }
    CCDictionary *directory =(CCDictionary*)((CCDictionary*)dict->objectForKey("result"))->objectForKey("card_item");
    decodeCardDict(directory);
    isLoadCardBagEnd=ERROR_MSG_NONE;

}

void CGamePlayer::decodeCardDict(cocos2d::CCDictionary *directory)
{
    
 
    CCArray *array=directory->allKeys();;
    for (int i=0; i<array->count(); i++) {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CCDictionary *detail=(CCDictionary*)(directory->objectForKey(key->m_sString));
        
        int level=GameTools::intForKey("level", detail);
        int card_id=GameTools::intForKey("card_id", detail);
        int cardexp=GameTools::intForKey("exp", detail);
        int card_item_id=GameTools::intForKey("card_item_id", detail);
        int card_suit = GameTools::intForKey("suit", detail);
        CFightCard *fightCard=new CFightCard(m_hashmapAllCard[card_id],level);
        fightCard->m_iCurrExp=cardexp;
        fightCard->m_User_Card_ID=card_item_id;
        fightCard->m_iSuit = card_suit;
        fightCard->m_iCurrLevel = level;
        m_vCardBag.push_back(fightCard);
    }
    
}

void CGamePlayer::clearServerCardBag()
{
    DELETE_POINT_VECTOR(m_vCardBag, vector<CFightCard *> ,CFightCard);
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

void CGamePlayer::loadServerPlayerInfo()
{
    isLoadPlayerInfoEnd=false;
    ADDHTTPREQUESTPOSTDATA("http://cube.games.com/api.php?m=GameBegin&a=init&uid=229&sig=9d377f6c3e440ac6c9623c55a6f4f9d0&sid=1", "GetServerPlayerInfo", "merlinaskplayerinfo1", "sig=2ac2b1e302c46976beaab20a68ef95",callfuncO_selector(CGamePlayer::loadServerPlayerInfoCallBack));
    
}

void CGamePlayer::loadServerPlayerInfoCallBack(cocos2d::CCObject *obj)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetServerPlayerInfo");
    char *data=(char *)obj;
    CCLog("%s",data);
    CCDictionary *dirct=PtJsonUtility::JsonStringParse(data);
    if(GameTools::intForKey("code",dirct)==0)
    {
        CCDictionary *userinfodirct=(CCDictionary *)(((CCDictionary *)dirct->objectForKey("result"))->objectForKey("user_info"));
        SinglePlayer::instance()->initByServerDictorny(userinfodirct);
    }
    isLoadPlayerInfoEnd=true;
    delete []data;
}


void CGamePlayer::loadCardTeamInfo()
{
    isLoadEndCardTeam=false;
 #ifndef AAAAFOROSMACHINE
    ADDHTTPREQUESTPOSTDATA(STR_URL_GET_TEAM(194), "GetLoadCardItem", "merlinaskplayerinfo1","sig=2ac2b1e302c46976beaab20a68ef95", callfuncO_selector(CGamePlayer::loadCardTeamInfoCallBack));
#else
    char *data=new char [5];
    loadCardTeamInfoCallBack((CCObject *)data);
#endif
}

void CGamePlayer::loadCardTeamInfoCallBack(CCObject *obj)
{
    m_vvBattleArray.resize(3);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetLoadCardItem");
    char *datat=(char *)obj;
#ifndef  AAAAFOROSMACHINE
        CCDictionary *dirct=PtJsonUtility::JsonStringParse(datat);
#else
    const  char *data=readFileName((resRootPath +"cardteam.txt").c_str()).c_str();
        CCDictionary *dirct=PtJsonUtility::JsonStringParse(data);
#endif
    CCLog("%s",datat);

    delete [] datat;
    datat=NULL;
    if(GameTools::intForKey("code",dirct)==0)
    {
        CCDictionary *dictresult=(CCDictionary *)((CCDictionary *)dirct->objectForKey("result"))->objectForKey("card_team");
        if (!dictresult)
        {
            return;
        }
        CCArray *vKeyArrayresult=dictresult->allKeys();
        for (int i=0; i<vKeyArrayresult->count(); i++)
        {
            CCString *key=(CCString *)vKeyArrayresult->objectAtIndex(i);
            CCDictionary *cardDirector=(CCDictionary*)(dictresult->objectForKey(key->m_sString));
            if(cardDirector)
            {
                vector<CFightCard *>tempVcard(5);
                CCDictionary *cardtemp=(CCDictionary *)cardDirector->objectForKey("team");
                CCArray *vKeyArraytemp=cardtemp->allKeys();
                for (int j=0; j<vKeyArraytemp->count(); j++)
                {
                    CCString *keytemp=(CCString *)vKeyArraytemp->objectAtIndex(j);
                    CCDictionary *cardDirectorDetail=(CCDictionary*)(cardtemp->objectForKey(keytemp->m_sString));
                    int card_item_id=GameTools::intForKey("card_item_id", cardDirectorDetail);
                    int position=GameTools::intForKey("position", cardDirectorDetail);
                    tempVcard[position-1]=findFightCardByCard_User_ID(card_item_id);
                    if(tempVcard[position-1])
                    {
                        tempVcard[position-1]->setInBattleArray(i+1);//(true);
                    }
               
            
                }
                int index=atoi(key->m_sString.c_str())-1;
                CCLog("index=========%d",index);
                m_vvBattleArray[index]=tempVcard;
            }
        }
        for (int i=0; i<m_vvBattleArray.size(); i++) {
            if(m_vvBattleArray[i].size()==0)
            {
                vector<CFightCard *>tempVectory(5);
                m_vvBattleArray[i]=tempVectory;
            }
        }
    }
    isLoadEndCardTeam=true;
    
}

void CGamePlayer::loadCardTeamInfoCallBackByDict(CCDictionary *dictresult)
{
    m_vvBattleArray.resize(3);
   if (!dictresult)
    {
        for (int i=0; i<m_vvBattleArray.size(); i++) {
            if(m_vvBattleArray[i].size()==0)
            {
                vector<CFightCard *>tempVectory(5);
                m_vvBattleArray[i]=tempVectory;
            }
        }
        return;
    }
    CCArray *vKeyArrayresult=dictresult->allKeys();
    for (int i=0; i<vKeyArrayresult->count(); i++)
    {
            CCString *key=(CCString *)vKeyArrayresult->objectAtIndex(i);
            CCDictionary *cardDirector=(CCDictionary*)(dictresult->objectForKey(key->m_sString));
            if(cardDirector)
            {
                vector<CFightCard *>tempVcard(5);
                CCDictionary *cardtemp=(CCDictionary *)cardDirector->objectForKey("team");
                CCArray *vKeyArraytemp=cardtemp->allKeys();
                for (int j=0; j<vKeyArraytemp->count(); j++)
                {
                    CCString *keytemp=(CCString *)vKeyArraytemp->objectAtIndex(j);
                    CCDictionary *cardDirectorDetail=(CCDictionary*)(cardtemp->objectForKey(keytemp->m_sString));
                    int card_item_id=GameTools::intForKey("card_item_id", cardDirectorDetail);
                    int position=GameTools::intForKey("position", cardDirectorDetail);
                    tempVcard[position-1]=findFightCardByCard_User_ID(card_item_id);
                    if(tempVcard[position-1])
                    {
                        tempVcard[position-1]->setInBattleArray(i+1);//(true);
                    }
                    
                    
                }
                int index=atoi(key->m_sString.c_str())-1;
                CCLog("index=========%d",index);
                m_vvBattleArray[index]=tempVcard;
            }
        }
        for (int i=0; i<m_vvBattleArray.size(); i++) {
            if(m_vvBattleArray[i].size()==0)
            {
                vector<CFightCard *>tempVectory(5);
                m_vvBattleArray[i]=tempVectory;
            }
        } 
}



CFightCard *CGamePlayer::findFightCardByCard_User_ID(int carduserid)
{
    if(m_vCardBag.size()==0)
    {
        return NULL;
    }
    for (int i=0; i<m_vCardBag.size(); i++) {
        if(m_vCardBag[i]->m_User_Card_ID==carduserid)
        {
            return m_vCardBag[i];
        }
    }
    return NULL;
}

void CGamePlayer::loadRival(int  usid,int  troops)
{
    isLoadFightTeam=0;
#ifndef AAAAFOROSMACHINE
    char data[50];
    sprintf(data, "%d",usid);
    string str=string("info={\"uid\":")+ data;
    sprintf(data, "%d",troops);
    str +=string(",\"troops\":")+data+"}";
    string connectData="sig=2ac2b1e302c46976beaab20a68ef95";
    connectData+="&"+str;
    //http://cube.games.com/api.php?m=Fight&a=getTeamInfo&uid=194&sig=2ac2b1e302c46976beaab20a68ef95
    ADDHTTPREQUESTPOSTDATA(STR_URL_CHOOSE_TEAM(connectData), "GetFightTeam", "merlinaskplayerinfo1",connectData.c_str(),callfuncO_selector(CGamePlayer::parseRival));
#else
    char *data=new char[5];
    parseRival((CCObject *)data);
#endif
}

void CGamePlayer::parseRival(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetFightTeam");
    char *datat=(char *)object;
    CCLog("%s",datat);
#ifndef AAAAFOROSMACHINE
   CCDictionary *dirct=PtJsonUtility::JsonStringParse(datat);
#else
    const  char *data=readFileName((resRootPath +"team.txt").c_str()).c_str();
    CCDictionary *dirct=PtJsonUtility::JsonStringParse(data);
#endif
    delete [] datat;
    datat=NULL;
    if(GameTools::intForKey("code",dirct)==0)
    {
        CCDictionary *dictresult=(CCDictionary *)((CCDictionary *)dirct->objectForKey("result"))->objectForKey("card_team");
        CCArray *vKeyArrayresult=dictresult->allKeys();
        for (int i=0; i<vKeyArrayresult->count(); i++)
        {
            CCString *key=(CCString *)vKeyArrayresult->objectAtIndex(i);
            CCDictionary *cardDirector=(CCDictionary*)(dictresult->objectForKey(key->m_sString));
            if(cardDirector)
            {
                DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard*> ,CFightCard);
                m_hashmapMonsterCard.resize(5);
                string teamStrType=typeid(*cardDirector->objectForKey("team")).name();
                if(teamStrType.find("CCDictionary")!=std::string::npos)
                {
                    CCDictionary *cardtemp=((CCDictionary *)cardDirector->objectForKey("team"));
                    CCArray *vKeyArraytemp=cardtemp->allKeys();
                    for (int i=0; i<vKeyArraytemp->count(); i++)
                    {
                        CCString *keytemp=(CCString *)vKeyArraytemp->objectAtIndex(i);
                        CCDictionary *cardDirectorDetail=(CCDictionary*)(cardtemp->objectForKey(keytemp->m_sString));
                        int card_id=GameTools::intForKey("card_id", cardDirectorDetail);
                        int position=GameTools::intForKey("position", cardDirectorDetail);
                        position=(position-1<0?0:position-1); //后台的数据的postion 是1开始的。
                        int level=GameTools::intForKey("level", cardDirectorDetail);
                        //需要显示一个花色的问题。
                        int suit=GameTools::intForKey("suit", cardDirectorDetail); //花色
                        CFightCard *pFightCard=new CFightCard(m_hashmapAllCard[card_id],level);
                        pFightCard->m_iSuit=suit;
                        m_hashmapMonsterCard[position]=pFightCard;
                    }
                }
                else if(teamStrType.find("CCArray")!=std::string::npos)
                {
                    CCArray *vKeyArraytemp=(CCArray *)(cardDirector->objectForKey("team"));
                    for (int i=0; i<vKeyArraytemp->count(); i++)
                    {
                        CCDictionary *cardDirectorDetail=(CCDictionary *)vKeyArraytemp->objectAtIndex(i);
                        int card_id=GameTools::intForKey("card_id", cardDirectorDetail);
                        int position=GameTools::intForKey("position", cardDirectorDetail);
                        position=(position-1<0?0:position-1); //后台的数据的postion 是1开始的。
                        int level=GameTools::intForKey("level", cardDirectorDetail);
                        //需要显示一个花色的问题。
                        int suit=GameTools::intForKey("suit", cardDirectorDetail); //花色
                        CFightCard *pFightCard=new CFightCard(m_hashmapAllCard[card_id],level);
                        pFightCard->m_iSuit=suit;
                        m_hashmapMonsterCard[position]=pFightCard;
                    }

                }
                CCArray *vKeyArraytempBBB=(CCArray *)(cardDirector->objectForKey("random_data"));
                m_getRandom_data.clear();
                m_currRandRomIndex=0;
                for (int i=0; i<vKeyArraytempBBB->count(); i++) {
                    CCString* strtemp=   (CCString *)vKeyArraytempBBB->objectAtIndex(i);
                    m_getRandom_data.push_back(strtemp->intValue());
                }
                
            }
       }
        
    }
    isLoadFightTeam=true;

}

void CGamePlayer::randRomIndexAdd()
{
        m_currRandRomIndex++;
}

void CGamePlayer::logicRandValue(int &value,bool needAdd)
{
    if(value==0)
    {
        return;
    }
    if(m_currRandRomIndex<0|| m_currRandRomIndex>=m_getRandom_data.size())
        m_currRandRomIndex=0;
    // value =value+ value*0.1*m_getRandom_data[m_currRandRomIndex]/100;
    if(value>0)
    {
        value =value + m_getRandom_data[m_currRandRomIndex];
    }
    else{
        value =value - m_getRandom_data[m_currRandRomIndex];
    }
    CCLog("Value====:%d,random=%d",value,m_getRandom_data[m_currRandRomIndex]);
}
void CGamePlayer::backUpFightTeam(int index)
{
    for (int i=0; i<m_vvBattleArray[index].size();i++)
    {
        if(m_vvBattleArray[index][i])
        {
            m_hashmapFightingCard.push_back(new CFightCard(*(m_vvBattleArray[index][i])));
        }
        else
        {
            m_hashmapFightingCard.push_back(NULL);
        }
    }
}


// player info
int CGamePlayer::getCoin()
{
    return  m_gGamePlayerData->m_icoin;
}

int CGamePlayer::getRVC()
{
    return  m_gGamePlayerData->m_irvc;
}

int CGamePlayer::getPlayerExp()
{
    return m_gGamePlayerData->m_iexp;
}

void CGamePlayer::addPalyerExp(int inAddExp)
{
    m_gGamePlayerData->m_iexp += inAddExp;
}
void CGamePlayer::subPlayerExp(int inSubExp)
{
    m_gGamePlayerData->m_iexp -= inSubExp;
}

void CGamePlayer::addRVC(const int &inAddValue)
{
     m_gGamePlayerData->m_irvc += inAddValue;
}
void CGamePlayer::addCoin(const int &inAddValue)
{
    m_gGamePlayerData->m_icoin += inAddValue;
}
void CGamePlayer::ReduceRVC(const int &inReduceRVC)
{
    m_gGamePlayerData->m_irvc -= inReduceRVC;
}
void CGamePlayer::ReduceCoin(const int &inReduceCoin)
{
    m_gGamePlayerData->m_icoin  -= inReduceCoin;
}


void CGamePlayer::appendCFightCardFightingBuffer(CFightCardFightingBuffer *data)
{
    this->m_vCFightCardFightingBuffer.push_back(data);
}

void CGamePlayer::appendAtkData(SEveryATKData * data)
{
    m_vHpAngry.push_back(data);
}

void CGamePlayer::onFightInterScene()
{
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *> ,CFightCard);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *> ,CFightCard);
    DELETE_POINT_VECTOR(m_vHpAngry, vector<SEveryATKData *> ,SEveryATKData);
    DELETE_POINT_VECTOR(m_vCFightCardFightingBuffer, vector<CFightCardFightingBuffer * > ,CFightCardFightingBuffer);
}

void CGamePlayer::deleteFightMonsterCard()
{
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *> ,CFightCard);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *>,CFightCard);

}
void CGamePlayer::onFightExitScene()
{
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *> ,CFightCard);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *> ,CFightCard);
    DELETE_POINT_VECTOR(m_vHpAngry, vector<SEveryATKData *> ,SEveryATKData);
    DELETE_POINT_VECTOR(m_vCFightCardFightingBuffer, vector<CFightCardFightingBuffer * > ,CFightCardFightingBuffer);
}


/*
 * @breif: 获取道具背包信息：
 */

void CGamePlayer::loadPropsInfo()
{
  m_bLoadProps = false;
  ADDHTTPREQUESTPOSTDATA(STR_URL_BAG_PROP(194), "loadProps", "loadProps","sig=2ac2b1e302c46976beaab20a68ef95" ,callfuncO_selector(CGamePlayer::parsePropsInfo));
    
}

void CGamePlayer::parseProsInfoByDict(CCDictionary *tmpDictionary)
{
    if (tmpDictionary)
    {
        int propId = 0;
        int propCount = 0;
        

            CCArray * items =(CCArray*) tmpDictionary ;
            for (int i = 0;  items!= NULL && i < items->count(); i++)
            {
                tmpDictionary = (CCDictionary*) items->objectAtIndex(i);
                
                if (tmpDictionary)
                {
                    propId = GameTools::intForKey("item_id", tmpDictionary);
                    propCount = GameTools::intForKey("num", tmpDictionary);
                    m_vProps.insert(map<int,int>::value_type(propId, propCount));
                }
                
            }
            CCLog("prop %d",m_vProps.size());
       }
}

void CGamePlayer:: parsePropsInfo(CCObject *pObject)
{
    m_bLoadProps = true;
    char *buffer = (char*) pObject;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "loadProps");
    // reset props package
    m_vProps.clear();
    
    CCAssert(buffer!=NULL, "server Error");
    
    // parse data to m_vProps:
    CCDictionary *tmpDictionary = PtJsonUtility::JsonStringParse(buffer);
    delete [] buffer;
    if (tmpDictionary)
    {
        int resultCode = GameTools::intForKey("code", tmpDictionary);
        int propId = 0;
        int propCount = 0;
        
        if (resultCode == 0)
        {
            tmpDictionary = (CCDictionary*) tmpDictionary->objectForKey("result");
        
            CCArray * items = tmpDictionary == NULL? NULL : (CCArray*) tmpDictionary->objectForKey("items");
            
            for (int i = 0;  items!= NULL && i < items->count(); i++)
            {
                tmpDictionary = (CCDictionary*) items->objectAtIndex(i);
                
                if (tmpDictionary)
                {
                    propId = GameTools::intForKey("item_id", tmpDictionary);
                    propCount = GameTools::intForKey("num", tmpDictionary);
                    m_vProps.insert(map<int,int>::value_type(propId, propCount));
                }
            
            }
        CCLog("prop %d",m_vProps.size());     
            
        }else
        {
            CCLog("load prop error...");
        }
    }

}
/*
 * @breif: 获取可以添加到背包的邮件队列
 * @param: inEmailDatas: 要判断的邮件队列
 * @return: 可以添加的背包的邮件ID队列
 */

void CGamePlayer::updateProps()
{
    for (map<int, int>::iterator i = m_vProps.begin(); i!=m_vProps.end(); i++)
    {
        if (i->second == 0)
        {
            m_vProps.erase(i);
        }
    }
}
vector<int>CGamePlayer::getCanAddToBackPackEmals(vector<EMAIL_DATA> inEmailDatas)
{
    int useGridCount =getUseGridCount();
    vector<int> emailIds;
    map<int, int> allProps = m_vProps;
    int tmpAddCount = 0;
    
    for (int i = 0; i <inEmailDatas.size(); i++)
    {
        tmpAddCount = isCanAddToBackPack(allProps, inEmailDatas.at(i).props, useGridCount);
        if (tmpAddCount != -1)
        {
            emailIds.push_back(inEmailDatas.at(i).emailId);
            useGridCount += tmpAddCount;
            mergeProps(allProps, inEmailDatas.at(i).props);
        }
    }
    return emailIds;
}

/*
 * @breif: 判断是否可以添加到背包队列中
 * @param: tmpProps: 背包队列
 * @param: inAddProps: 要添加的道具队列 (包含特殊的道具：金币和经验)
 * @param: inUserGridCount: tmpProps使用的格子数
 * @return: 返回新增的格子数，－1:无法添加的背包队列中
 */
int CGamePlayer::isCanAddToBackPack(map<int, int> &tmpProps, map<int, int>& inAddProps, int inUserGridCount)
{
    int m_nOpenGridCount = m_gGamePlayerData->m_nOpenGridCount;
    int bRet = -1;
    int key = 0;
    int count = 0;
    int isOnly = 0;
    int limitCount;
    CPtProp *tmp = NULL;
    for (map<int, int>::iterator i = inAddProps.begin(); i != inAddProps.end() && (inUserGridCount+count) <= m_nOpenGridCount; i++)
    {
        
        key = i->first;
        // filter exp and coin:
        if (key == EXPID || key == COINSID)
        {
            continue;
        }
        
        tmp = m_rAllProps.at(key);
        CCAssert(tmp != NULL, "no this prop");
        isOnly = tmp->getIsOnlyNum();
        if (isOnly == 1)
        {
            //已存在
            if (tmpProps.find(key)!= tmpProps.end())
            {
              //  return bRet;
                continue;
            }else
            {
                count++;
            }
        }else
        {
            int addCount = i->second;
            limitCount = tmp->getLimitNum();
            if (tmpProps.find(key)!= tmpProps.end())
            {
                int addCount2 =tmpProps.at(key);
                addCount += addCount2;
                addCount = (addCount/limitCount + addCount%limitCount == 0 ? 0 : 1);
                addCount2 = (addCount2/limitCount + addCount2%limitCount == 0 ? 0 : 1);
                count += (addCount-addCount2);
            
            }else
            {
               
                count += (addCount/limitCount + addCount%limitCount == 0 ? 0 : 1);
            }
        }
    }
    
    if((inUserGridCount+count) <= m_nOpenGridCount)
    {
        bRet = count;
        
    }
    
    return bRet;
}


/*
 * @breif: 将要添加的道具队列中数据和背包队列中的数据融合
 * @param: tmpProps 背包队列
 * @param: inAddProps 添加的道具队列
 */
void CGamePlayer::mergeProps(map<int, int> &tmpProps, map<int, int> &inAddProps)
{
     CPtPropConfigData *propData =   SinglePropConfigData::instance();
    for (map<int, int>::iterator i = inAddProps.begin(); i != inAddProps.end(); i++)
    {
        if (i->first == EXPID || i->first == COINSID)
        {
            continue;
        }
        
        if (tmpProps.find(i->first) == tmpProps.end())
        {
            tmpProps.insert(map<int, int>::value_type (i->first, i->second));
        }else
        {
            if(propData->getPropById(i->first)->getIsOnlyNum() == 1)
            {
                
            }else
            {
                tmpProps.at(i->first) += i->second;
            }
            
        }
    }
}

int CGamePlayer::getOpenGridCount()
{
    return m_gGamePlayerData->m_nOpenGridCount;
}
int CGamePlayer::AddOpenGridCount(int inAddCount)
{
    int &m_nOpenGridCount = m_gGamePlayerData->m_nOpenGridCount;
    m_nOpenGridCount += inAddCount;
    m_nOpenGridCount = m_nOpenGridCount < 0 ? 0 : (m_nOpenGridCount%3==0 ? m_nOpenGridCount : (m_nOpenGridCount/3)*3);
    return m_nOpenGridCount;
}

/*
 * 获取使用的格子数：
 */

int CGamePlayer::getUseGridCount()
{
    int limitMax;
    int isOnly;
    int useGridCount = 0;
    int keyId = 0;
    int itemCount = 0;
    CPtProp *tmp = NULL;
    map<int,int> &allProps = m_vProps;
    for (map<int, int>::iterator i = allProps.begin(); i != allProps.end(); i++)
    {
        keyId = i->first;
        itemCount = i->second;
        tmp =  m_rAllProps.at(keyId);
        CCAssert(tmp!=NULL, "key is null!");
        
        isOnly = tmp->getIsOnlyNum();
        if (isOnly == 1)
        {
            useGridCount++;
        }else
        {
           limitMax = tmp->getLimitNum();
            if (limitMax == 0)
            {
                continue;
            }
            useGridCount += ((itemCount/limitMax) + (itemCount % limitMax) == 0 ? 0 : 1);
        }
        
    }
    
    return useGridCount;
    
}

void CGamePlayer::receiveEmail(CGameEmailData * inEmailData)
{
    receiveEmail(inEmailData->m_mapDataProp, inEmailData->getGameEmailExp(), inEmailData->getGameEmailCoins());
}
void CGamePlayer::receiveEmail(map<int, int> inProps, int inAddPlayerExp, int inAddPlayerCoin)
{
    // add props:
    if (inProps.size()!= 0)
    {
         mergeProps(m_vProps, inProps);
    }
   
    // add play exp and coin:
    addPalyerExp(inAddPlayerExp);
    addCoin(inAddPlayerCoin);
    
}
// interface of prop:
/*
 * @breif: 添加道具到背包中，不进行检测，由服务端进行检测
 * @param: inPropId, 道具的ID
 * @param: inAddNum, 添加的数量
 */

void CGamePlayer::addProp(int inPropId, int inAddNum)
{
    map<int, int>::iterator it =  m_vProps.find(inPropId);
    if (it != m_vProps.end())
    {
        it->second += inAddNum;
    }else
    {
        m_vProps.insert(map<int, int>::value_type(inPropId, inAddNum));
    }
}
/*
 * @breif: 删除道具, 当道具的数目减为0时，将自动缩进
 * @param: inPropId, 道具ID
 * @param: inSubNum, 删除数目
 * @return: -1 --> 没有该道具， －2 --> 道具的数目不够删除， 0 --> 删除成功
 */

int CGamePlayer::subProp(int inPropId, int inSubNum)
{
    int resultCode = -1;
    map<int, int>::iterator it =  m_vProps.find(inPropId);
    if (it != m_vProps.end())
    {
        if (it->second >= inSubNum)
        {
            it->second -= inSubNum;
            if (it->second == 0)
            {
                m_vProps.erase(it);
            }
            
            resultCode = 0;
        }else
        {
            resultCode = -2;
        }

    }
    return  resultCode;
}

/*
 * @breif: 返回用户道具列表， struct： key--> propId, value --> propNum
 */
map<int,int> CGamePlayer::getPlayerProps()
{
    return m_vProps;
}


/*
 * 获取玩家基本信息
 */

//获取Uid
const char* CGamePlayer::getUserId()
{
    return m_strUid.c_str();
}

void CGamePlayer::setUserId(string strUid)
{
    m_strUid = strUid;
}
//获取Sig
const char* CGamePlayer::getUserSig()
{
    return m_strSig.c_str();
}

void CGamePlayer::setUserSig(string sig)
{
    m_strSig = sig;
}

void CGamePlayer::onGameBegin()
{
    //xianbei modify
    gameInitStatus=0;
    int getEmailMax=G_GAMESINGEMAIL::instance()->getCurrentTotalEmail();
    char data[50];
    sprintf(data, "&info={\"max_id\":%d}",getEmailMax);
    string connectData="sig=2ac2b1e302c46976beaab20a68ef95";
    connectData+=data;
    ADDHTTPREQUESTPOSTDATA(STR_URL_GAMEINIT(194), "GameBegin", "merlinaskplayerinfo1",connectData.c_str(),callfuncO_selector(CGamePlayer::onGameBeginCallBack));
    
}

void CGamePlayer::onGameBeginCallBack(CCObject *object)
{
    if (!object) {
        gameInitStatus=ERROR_MSG_CONNECTSERVERERROR;
        return;
    }

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GameBegin");
    char *strdata=(char *)object;
    CCLog("%s",strdata);    
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut!=0)
    {
        gameInitStatus=codeReslut;
    }
    else{
        CCDictionary *dictresult=(CCDictionary *)dict->objectForKey("result");
        string teamStrType=typeid(*dictresult->objectForKey("inbox_info")).name();
        if(teamStrType.find("CCDictionary")!=std::string::npos)
        {
            CCDictionary *dicinfobox_info=(CCDictionary *)dictresult->objectForKey("inbox_info");
            G_GAMESINGEMAIL::instance()->decodeEmap(dicinfobox_info);
        }
            
        teamStrType=typeid(*dictresult->objectForKey("user_info")).name();
        if(teamStrType.find("CCDictionary")!=std::string::npos)
        {
               CCDictionary *dicuserinfo=(CCDictionary *)dictresult->objectForKey("user_info");
             parseJsonUserInfo(dicuserinfo);
            gameInitStatus=1;
        }
        CCDictionary *getCardItem=(CCDictionary *)dictresult->objectForKey("card_team");
        loadCardTeamInfoCallBackByDict(getCardItem);
        
    }
}

void CGamePlayer::parseJsonUserInfo(CCDictionary *dict)
{
    if(m_gGamePlayerData)
    {
        m_gGamePlayerData->decodeDictnory(dict);
    }
        CCDictionary *cardinfo=(CCDictionary *)(dict->objectForKey("card_item"));
            if (cardinfo) {
                        decodeCardDict(cardinfo);
                }
    
    CCDictionary *bag_info=(CCDictionary *)(dict->objectForKey("bag_info"));
    if(bag_info)
    {
        parseProsInfoByDict(bag_info);
    }
    
}
