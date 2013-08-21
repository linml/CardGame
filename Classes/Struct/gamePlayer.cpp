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

//#define AAAAFOROSMACHINE 1

using namespace std;
string  readFileName(const char *filename)
{
    string filpat=CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filename);
    ifstream out;
    out.open(filpat.c_str(), ios::in);
    static string result;
    result.clear();
    string line;
    if(!out)
    {
        cout<<"aaaa";
    }
    while(!out.eof())
    {
        std::getline(out,line);
        result+=line;
    }
    out.close();
    return result;
}

#define SKILLPUTONGGONGJIID 100000
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
    // test begin:
    testPlayInfoData();
    // test end:
    
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
    initAllCard((resRootPath+"card_config.plist").c_str());
    initPlayerTable((resRootPath +"level_config.plist").c_str());
    loadAllSkillInfo((resRootPath+"skill_config.plist").c_str());
    loadAllEffectInfo((resRootPath + "skill_effect_config.plist").c_str());
    G_FightSkillManager::instance()->initSkill();//加载列表
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
//读取技能表格
void CGamePlayer::loadAllSkillInfo(const char *skillFileName)
{
    G_SingleCConfigResourceLoad::instance()->loadSkillLogicInfo(m_vSkillInfo, skillFileName);
}

void CGamePlayer::clearAllSkillInfo()
{
    DELETE_POINT_VECTOR(m_vSkillInfo, vector<CSkillData *>);

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
    for (int i=0; i<m_vSkillInfo.size(); i++) {
        if(m_vSkillInfo[i]->skill_id==skillId)
        {
            return m_vSkillInfo[i];
        }
    }
    return NULL;
    
}

CSkillData *CGamePlayer::getPutongGongji()
{
    for (int i=0; i<m_vSkillInfo.size(); i++) {
        if(m_vSkillInfo[i]->skill_id ==SKILLPUTONGGONGJIID)
        {
            return m_vSkillInfo[i];
        }
    }
    return NULL;
}


//读取效果表格
void CGamePlayer::loadAllEffectInfo(const char *effectFileName)
{
     G_SingleCConfigResourceLoad::instance()->loadEffectLogicInfo(m_vImpactInfo,effectFileName);
}

void CGamePlayer::clearAllEffectInfo()
{
    DELETE_POINT_VECTOR(m_vImpactInfo, vector<CImapact *>);
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
        m_iCurrentExp=GameTools::intForKey("exp",dict);
        m_iCurrentHp=GameTools::intForKey("hp",dict);
        m_iCurrentMp=GameTools::intForKey("mp", dict);
        m_iCurrentLeader=GameTools::intForKey("mp", dict);
        m_iCardBagNum=GameTools::intForKey("card_bag_num", dict);
        m_sLevelPlayer=m_gvPlayerLevel[GameTools::intForKey("level", dict)];
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
    m_iCurrentExp=0;
    m_iCurrentHp=m_sLevelPlayer->m_iHP_max;
    m_iCurrentMp=m_sLevelPlayer->m_iExp_max;
}

bool CGamePlayer::isCanUpdateLevel(int nExp)
{
    if(m_iCurrentExp+nExp>=m_sLevelPlayer->m_iExp_max)
    {
        return true;
    }
    return false;
}

bool CGamePlayer::UpdateLevel(int nExp)
{
    while(isCanUpdateLevel(nExp))
    {
        nExp-=m_sLevelPlayer->m_iExp_max-m_iCurrentExp;
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
    
    isLoadCardBagEnd=ERROR_MSG_NONE;

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
    isLoadFightTeam=false;
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
                DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard*>);
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
    CCLog("Value====:%d",value);
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
    return  m_nCoin;
}

int CGamePlayer::getRVC()
{
    return  m_nRvc;
}

void CGamePlayer::addRVC(const int &inAddValue)
{
    m_nRvc += inAddValue;
}
void CGamePlayer::addCoin(const int &inAddValue)
{
    m_nCoin += inAddValue;
}
void CGamePlayer::ReduceRVC(const int &inReduceRVC)
{
    m_nRvc -= inReduceRVC;
}
void CGamePlayer::ReduceCoin(const int &inReduceCoin)
{
    m_nCoin -= inReduceCoin;
}

void CGamePlayer::testPlayInfoData()
{
    m_nCoin = 500000;
    m_nRvc = 100000;
    
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
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_vHpAngry, vector<SEveryATKData *>);
    DELETE_POINT_VECTOR(m_vCFightCardFightingBuffer, vector<CFightCardFightingBuffer *>);
}

void CGamePlayer::deleteFightMonsterCard()
{
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *>);

}
void CGamePlayer::onFightExitScene()
{
    DELETE_POINT_VECTOR(m_hashmapFightingCard, vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_vHpAngry, vector<SEveryATKData *>);
    DELETE_POINT_VECTOR(m_vCFightCardFightingBuffer, vector<CFightCardFightingBuffer *>);
}


/*
 * @breif: 获取道具背包信息：
 */

void CGamePlayer::loadPropsInfo()
{
 
    
    
}
void CGamePlayer:: parsePropsInfo(CCObject *pObject)
{
    // reset props package
    m_vProps.clear();
    
    // test code:
    
}


/*
 * 获取玩家基本信息
 */

//获取Uid
int CGamePlayer::getUserId()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("uid");
}
//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)
