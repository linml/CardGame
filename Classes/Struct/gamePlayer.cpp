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
#include "CGameNpcCard.h"
#include "CPtTool.h"
#include "gameMiddle.h"
#include "CGlobalUpdateObject.h"
#include "CStructShopInfo.h"
#include "CPlayerBufferManager.h"
#include "CGameTimerManager.h"
#include "CPlayerBufferManager.h"
#include "CStructStrips.h"



//#define AAAAFOROSMACHINE 1

using namespace std;
#define SKILLPUTONGGONGJIID 100000


inline bool dealWithCallbackObject(CCObject *object)
{
    if (!object) {
        Middle::showAlertView("服务端传输NULL的数据");
        return false;
    }
    return true;
}

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
    CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
    tempClear.clearVector(); \
}



struct mk {
    int type; // 操作类型 1 通关。2 打怪 3 寻宝 （4 吃药 5购买 ，6 遇见佣兵。）
    vector<int >guaiwuid;
    
};

//

CGamePlayer::CGamePlayer() : m_rAllProps(SinglePropConfigData::instance()->getProps())
{
    m_pTaskLogic=new CPtTaskLogic;
    m_bAllTaskCompleted = false;
    m_nMaxSectionId = 1;
    m_nMaxSectionId = 1;
    m_nCurrentTaskId=0;
    m_strSig = "";
    m_strUid = "";
    for (int i=0; i<m_vvBattleArray.size(); i++) {
        DELETE_POINT_VECTOR(m_vvBattleArray[i],vector<CFightCard*> ,CFightCard );
    }
    m_vvBattleArray.clear();
    m_gGamePlayerData=new CGamePlayerData();
    //loaditem表格;
    m_gameShop=new CStructShopInfo;
    loadGamesConfig();
    if ( m_gGamePlayerData->m_sLevelPlayer==NULL) {
        m_gGamePlayerData->settestInit(1);
    }
    m_bFightKuaijin=false;
    if (!m_pTaskLogic)
    {
        CCLog("m_pTaskLogic == NULL");
        appendFileLog("m_pTaskLogic ISnULL");
    }
    m_pUpdateAp=new CGlobalUpdateAp(1);
    m_pUpdateGp=new CGlobalUpdateGp(2);
}

CGamePlayer::~CGamePlayer()
{
    CC_SAFE_DELETE(m_pUpdateGp);
    CC_SAFE_DELETE(m_pUpdateAp);
    CC_SAFE_DELETE(m_pTaskLogic);
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
    loadNpcCard((resRootPath+"npc_config.plist").c_str());
    
    G_SingleCConfigResourceLoad::instance()->loadSkillStripTable(m_haspMapSkillStrips,(resRootPath + "skill_strips_config.plist").c_str());
    
    SinglePropConfigData::instance();
    G_SingleCConfigResourceLoad::instance()->loadShopSellItem(m_gameShop, (resRootPath+"shop.plist").c_str());
    G_FightSkillManager::instance()->initSkill();//加载列表
}

void CGamePlayer::onExitGameApp()
{
    clearAllEffectInfo();
    clearAllSkillInfo();
    clearAllCard();
    clearAllNpcCard();
    clearPlayerTable();
    clearShangchengData();
    clearSkillStrip();    
    CPlayerBufferManager::releaseBufferManager(); // 取出player身上的buff add by phileas

    
}
void CGamePlayer::clearShangchengData()
{
    if(m_gameShop)
    {
        m_gameShop->clearShopItemData();
        CC_SAFE_DELETE(m_gameShop);
    }
}

void CGamePlayer::clearSkillStrip()
{
    map<int ,CStructStrips *>::iterator it;
    for(it=m_haspMapSkillStrips.begin();it!=m_haspMapSkillStrips.end();++it)
    {
        if(it->second)
        {
            delete it->second;
            it->second=NULL;
        }
    }
    m_haspMapSkillStrips.erase(m_haspMapSkillStrips.begin(),m_haspMapSkillStrips.end());
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

//读取NPC效果表格
void CGamePlayer::loadNpcCard(const char *npcCardName)
{
    G_SingleCConfigResourceLoad::instance()->loadNPCCardInfo(m_hashmapNpcAllCard, npcCardName);
}
void CGamePlayer::clearAllNpcCard()
{
    map<int ,CCard *>::iterator it;
    for(it=m_hashmapNpcAllCard.begin();it!=m_hashmapNpcAllCard.end();++it)
    {
        if(it->second)
        {
            delete it->second;
            it->second=NULL;
        }
    }
    m_hashmapNpcAllCard.erase(m_hashmapNpcAllCard.begin(),m_hashmapNpcAllCard.end());
    
}


CCard *CGamePlayer::getCardByCardId(int cardid)
{
    return m_hashmapAllCard[cardid];
}

void CGamePlayer::clearPlayerTable()
{
    if (m_gGamePlayerData) {
        m_gGamePlayerData->clearPlayerData();
    }
}

void CGamePlayer::initPlayerTable(const char *playerFileName)
{
    if(m_gGamePlayerData)
    {
        G_SingleCConfigResourceLoad::instance()->loadPlayerLevelInfo(m_gGamePlayerData->m_gvPlayerLevel, playerFileName);
    }

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
    if(skillId==0)
    {
        return NULL;
    }
    return m_vSkillInfo[skillId];
}

CSkillData *CGamePlayer::getPutongGongji(CFightCard *pCard)
{
    //正确的地方应该调用这个注释的语句
    //return getSkillBySkillId(pCard->m_pCard->m_iusually_attack);
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


void CGamePlayer::getSeverPlayerInfo(cocos2d::CCObject *object)
{
    
}

void CGamePlayer::decodeCardDictAppendCardBag(cocos2d::CCDictionary *directory)
{
    CCArray *array=directory->allKeys();;
    for (int i=0; i<array->count(); i++) {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CCDictionary *detail=(CCDictionary*)(directory->objectForKey(key->m_sString));
        
        int level=GameTools::intForKey("level", detail);
        if (level == 0)
        {
            level = 1;
        }
        int card_id=GameTools::intForKey("card_id", detail);
        int cardexp=GameTools::intForKey("exp", detail);
        int card_item_id= key->intValue(); //GameTools::intForKey("card_item_id", detail);
        int card_suit = GameTools::intForKey("suit", detail);
        
        CFightCard *fightCard=new CFightCard(m_hashmapAllCard[card_id],level);
        fightCard->m_iCurrExp=cardexp;
        fightCard->m_User_Card_ID=card_item_id;
        fightCard->m_iSuit = card_suit;
        fightCard->m_iCurrLevel = level;
        m_vCardBag.push_back(fightCard);
    }
    
}

void CGamePlayer::appendCardToCardBag(const int &inCardItemId, const int &inCardId, int inSuit, int inLevel, int inExp )
{
    CFightCard *fightCard=new CFightCard(m_hashmapAllCard[inCardId],inLevel);
    fightCard->m_iCurrExp=inLevel;
    fightCard->m_User_Card_ID=inCardItemId;
    fightCard->m_iSuit = inSuit;
    fightCard->m_iCurrLevel = inLevel;
    m_vCardBag.push_back(fightCard);
}

void CGamePlayer::subCardByUserId(const int &inUserId)
{
    vector<CFightCard *>::iterator iterator =  m_vCardBag.begin();
    for (; iterator != m_vCardBag.end(); iterator++)
    {
       if((*iterator)->m_User_Card_ID == inUserId)
       {
           delete *iterator;
           m_vCardBag.erase(iterator);
           break;
       }
    }
}

void CGamePlayer::subCardByIndex(const int inIdex)
{
    if (inIdex < m_vCardBag.size())
    {
        vector<CFightCard *>::iterator iterator =  m_vCardBag.begin() + inIdex;
        subCardByIterator(iterator);
    }
}

void CGamePlayer::subCardByIterator(vector<CFightCard *>::iterator inIterator)
{
    if (inIterator != m_vCardBag.end())
    {
        delete  *inIterator;
        m_vCardBag.erase(inIterator);
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
        else
        {
            CCLog("index team is not null %d",i);
        }
    }
    isLoadEndCardTeam=true;
}

bool CGamePlayer::cardBagIsMoreThanConfig()
{
    return m_vCardBag.size()>=m_gGamePlayerData->getCardBagMax();
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
    //#ifndef AAAAFOROSMACHINE
    char data[50];
    sprintf(data, "%d",usid);
    string str=string("info={\"uid\":")+ data;
    sprintf(data, "%d",troops);
    str +=string(",\"troops\":")+data+"}";
    string connectData="sig=";
    connectData += m_strSig;
    connectData+="&"+str;
    //http://cube.games.com/api.php?m=Fight&a=getTeamInfo&uid=194&sig=2ac2b1e302c46976beaab20a68ef95
    ADDHTTPREQUESTPOSTDATA(STR_URL_CHOOSE_TEAM(connectData), "CALLBACK_CGamePlayer_GetFightTeam", "REQUEST_CGamePlayer_GetFightTeam",connectData.c_str(),callfuncO_selector(CGamePlayer::parseRival));
}
void CGamePlayer::loadNpcCardTeam(int zhang, int jie, int bu, int dijige)
{
    isLoadFightTeam=0;
    
#ifdef AAAAFOROSMACHINE
    const  char *data=readFileName((resRootPath +"team.txt").c_str()).c_str();
    CCDictionary *dirct=PtJsonUtility::JsonStringParse(data);
#else
    const  char *data=CPtTool::readFileName((resRootPath +"npccardteam.txt").c_str()).c_str();
    char *datat=new char [strlen(data+1)];
    memset(datat, 0, strlen(data+1));
    memcpy(datat, data, strlen(data));
    parseNpcCard((CCObject *)datat);
#endif
}
/*
 {
 "code": 0,
 "result": {
 "npccardteam": {
 "1": "4000001",
 "2": "4000002",
 "3": "4000003",
 "4": "4000004",
 "5": "4000005"
 },
 "random_data": [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
 }
 }
 */
void CGamePlayer::parseNpcCard(cocos2d::CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetNpcCard");
    char *datat=(char *)object;
    CCLog("%s",datat);
    CCDictionary *dirct=PtJsonUtility::JsonStringParse(datat);
    delete [] datat;
    if(GameTools::intForKey("code",dirct)==0)
    {
        CCDictionary *dictresult=(CCDictionary *)dirct->objectForKey("result");
        if(dictresult)
        {
            parseNpcCard(dictresult);
        }
    }
    isLoadFightTeam=true;
}

void CGamePlayer::parseNpcCard(CCDictionary *resultDictresult)
{
    CCDictionary *dictresult=(CCDictionary*)resultDictresult->objectForKey("npc_team");
    if(!dictresult)
    {
        return ;
    }
//    CCArray *vKeyArrayresult=dictresult->allKeys();
//    for (int i=0; i<vKeyArrayresult->count(); i++)
//    {
//        CCString *key=(CCString *)vKeyArrayresult->objectAtIndex(i);
//        CCDictionary *cardDirector=(CCDictionary*)(dictresult->objectForKey(key->m_sString));
//        if(cardDirector)
//        {
            DELETE_POINT_VECTOR(m_hashmapMonsterCard, vector<CFightCard*> ,CFightCard);
            m_hashmapMonsterCard.resize(5);
            if(dictresult)
            {
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(dictresult, pElement)
                {
                    const char* pchKey = pElement->getStrKey();
                    int position=atoi(pchKey)-1;
                    CCAssert(position>=0, "f**k服务端传输位置为-1的");
                    int card_id=GameTools::intForKey(pchKey, dictresult);
                    if(card_id==0)
                    {
                        m_hashmapMonsterCard[position]=NULL;
                    }else{
                        CFightCard *pFightCard=new CGameNpcCard((CNpcCard *)m_hashmapNpcAllCard[card_id]);
                        m_hashmapMonsterCard[position]=pFightCard;

                    }
                }
            }
            parseRandomData((CCArray*)resultDictresult->objectForKey("random_data"));
//            CCArray *vKeyArraytempBBB=(CCArray *)resultDictresult->objectForKey("random_data");
//            std::vector<int>().swap(m_getRandom_data);
//            m_currRandRomIndex=0;
//            for (int i=0; i<vKeyArraytempBBB->count(); i++) {
//                CCString* strtemp=   (CCString *)vKeyArraytempBBB->objectAtIndex(i);
//                m_getRandom_data.push_back(strtemp->intValue());
//            }
    
//        }
//    }
    isLoadFightTeam=true;

}

void CGamePlayer::parseRandomData(CCArray* inRandomArray)
{
    if(inRandomArray)
    {
        std::vector<int>().swap(m_getRandom_data);
        m_currRandRomIndex=0;
        for (int i=0; i<inRandomArray->count(); i++)
        {
            CCString* strtemp=   (CCString *)inRandomArray->objectAtIndex(i);
            m_getRandom_data.push_back(strtemp->intValue());
        }
    }

}

void CGamePlayer::parseRival(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_GetFightTeam");
    if( !dealWithCallbackObject(object))
    {
        return;
    }
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

int CGamePlayer::getPlayerCash()
{
    return m_gGamePlayerData->m_icash;
}

int CGamePlayer::getRVC()
{
    return  m_gGamePlayerData->m_irvc;
}

int CGamePlayer::getPlayerExp()
{
    return m_gGamePlayerData->m_iexp;
}

int CGamePlayer::getPlayerLevel()
{
    return m_gGamePlayerData->m_ilevel;
}

int CGamePlayer::getPlayerGp()
{
    return m_gGamePlayerData->m_iGp;
}

int CGamePlayer::getGpMax()
{
    return m_gGamePlayerData->getGpMax();
}

int CGamePlayer::getApMax()
{
    return m_gGamePlayerData->getApMax();
}

int CGamePlayer::getExpMax()
{
    return m_gGamePlayerData->getExpMax();
}

int CGamePlayer::getFriendly()
{
    return m_gGamePlayerData->m_nFriendly;
}

int CGamePlayer::getPlayerAp()  //体力
{
    return m_gGamePlayerData->m_iAp;
}

bool CGamePlayer::getPlayrHadRecharged()
{
    return m_gGamePlayerData->m_bFirstRecharge;
}

const char* CGamePlayer::getPlayerName()
{
    return m_gGamePlayerData->m_susername.c_str();
}

bool CGamePlayer::setPlayrHadRecharged(bool var)
{
    m_gGamePlayerData->m_bFirstRecharge=var;
    return var;
}

int CGamePlayer::setPlayerGp(int iValue)
{
     m_gGamePlayerData->m_iGp=iValue;
     return m_gGamePlayerData->m_iGp;
}

int CGamePlayer::setPlayerAp(int iValue)
{
    m_gGamePlayerData->m_iAp=iValue;
    return m_gGamePlayerData->m_iAp;
}

int CGamePlayer::setPlayerCash(int iValue)
{
    m_gGamePlayerData->m_icash=iValue;
    return m_gGamePlayerData->m_icash;
}


void CGamePlayer::addPlayerGp(int inAddHp)
{

    m_gGamePlayerData->logicValue(m_gGamePlayerData->m_iGp,inAddHp,'+');
    if(m_gGamePlayerData->m_iGp>m_gGamePlayerData->getGpMax())
    {
        m_gGamePlayerData->m_iGp=m_gGamePlayerData->getGpMax();
    }

}
void CGamePlayer::subPlayerGp(int inSubHp)
{
    m_gGamePlayerData->logicValue(m_gGamePlayerData->m_iGp,inSubHp,'-');
    if(m_gGamePlayerData->m_iGp<0)
    {
        m_gGamePlayerData->m_iGp=0;
    }
}

void CGamePlayer::addPlayerAp(int inAddEnergy)
{
    m_gGamePlayerData->logicValue(m_gGamePlayerData->m_iAp,inAddEnergy,'+');
    if(m_gGamePlayerData->m_iAp > m_gGamePlayerData->getApMax())
    {
        m_gGamePlayerData->m_iAp=m_gGamePlayerData->getApMax();
    }
    
}
void CGamePlayer::subPlayerAp(int inSubEnergy)
{
    m_gGamePlayerData->logicValue(m_gGamePlayerData->m_iAp,inSubEnergy,'-');
    if(m_gGamePlayerData->m_iAp <0)
    {
        m_gGamePlayerData->m_iAp=0;
    }}

void CGamePlayer::addPlayerCash(int iValue)

{
    m_gGamePlayerData->m_icash += iValue;
}
void CGamePlayer::subPlayerCash(int iValue)
{
    m_gGamePlayerData->m_icash -= iValue;
    m_gGamePlayerData->m_icash = m_gGamePlayerData->m_icash >= 0 ? m_gGamePlayerData->m_icash : 0;
}




bool CGamePlayer::addPalyerExp(int inAddExp)
{
    bool bRet = false;
    m_gGamePlayerData->m_iexp += inAddExp;
    if(m_gGamePlayerData->m_iexp >= m_gGamePlayerData->m_sLevelPlayer->m_iExp_max)
    {
        bRet = true;
    }
    return bRet;
}

void CGamePlayer::subPlayerExp(int inSubExp)
{
    m_gGamePlayerData->m_iexp -= inSubExp;
}

void CGamePlayer::setPlayerExp(int inExpValue)
{
    m_gGamePlayerData->m_iexp = inExpValue;
}

void CGamePlayer::addRVC(const int &inAddValue)
{
    m_gGamePlayerData->m_irvc += inAddValue;
}

void CGamePlayer::subRVC(const int &inSubRVC)
{
    m_gGamePlayerData->m_irvc -= inSubRVC;
    m_gGamePlayerData->m_irvc  =  m_gGamePlayerData->m_irvc >= 0 ? m_gGamePlayerData->m_irvc : 0;
}

void CGamePlayer::addCoin(const int &inAddValue)
{
    m_gGamePlayerData->m_icoin += inAddValue;
}

void CGamePlayer::subCoin(const int &inSubValue)
{
    m_gGamePlayerData->m_icoin -= inSubValue;
    
    m_gGamePlayerData->m_icoin  =  m_gGamePlayerData->m_icoin >= 0 ? m_gGamePlayerData->m_icoin : 0;
}

void CGamePlayer::addTaskOperator(int taskType,vector<int > *targetIdVector)
{
    m_pTaskLogic->addTaskOperator(taskType, targetIdVector);
}

void CGamePlayer::subTaskOperator(int taskType,vector<int > *targetIdVector)
{
    m_pTaskLogic->subTaskOperator(taskType, targetIdVector);
    
}

void CGamePlayer::getNextTaskInfo()
{
    CPtTask *pttask=SingleTaskConfig::instance()->getNextByPreTask(m_nCurrentTaskId);
    if(pttask)
    {
        m_nCurrentTaskId = pttask->getTaskTipId();
        m_nMaxChapterId = pttask->getChapterId();
        m_nMaxSectionId = pttask->getSectionId();
        if (m_pTaskLogic) {
            m_pTaskLogic->setInitDataByCPtTask(pttask);
        }
    }
}

bool CGamePlayer::isHaveSendComplate()
{
    return (m_pTaskLogic->getPtTaskCurrentStatus()==2 ? true:false);
}

void CGamePlayer::setInitCurrentTaskTargetNumber(int number)
{
    if (m_pTaskLogic) {
        m_pTaskLogic->setPtTaskCurrentNumber(number);
    }
}

void CGamePlayer::postAddTask(int taskNextId, cocos2d::CCObject *object, SEL_CallFuncO selector,const char *strCallback)
{
    char data[256];
    sprintf(data, "&task_id=%d",taskNextId);
    string connectData="sig=";
    connectData += m_strSig;
    connectData+=data;
    ADDHTTPREQUESTPOSTDATABYOWNCCCLASS(STR_URL_ADDTASK(194), strCallback, "REQUEST_CGamePlayer_AddTask",connectData.c_str(),object,selector);
}

void CGamePlayer::postCompleteTask(int taskId, cocos2d::CCObject *object, SEL_CallFuncO selector,const char *strCallback)
{
    char data[256];
    sprintf(data, "&task_id=%d",taskId);
    string connectData="sig=";
    connectData += m_strSig;
    connectData+=data;
    ADDHTTPREQUESTPOSTDATABYOWNCCCLASS(STR_URL_COMPLATETASK(194), strCallback, "REQUEST_CGamePlayer_CompleteTask",connectData.c_str(),object,selector);
}

void CGamePlayer::setTaskTotalNumberOnFinishSectionTask(int MaxTaskStep)
{
    m_pTaskLogic->setPtTaskTotalNumber(MaxTaskStep);
}

int CGamePlayer::getCurrentTaskType()
{
    return m_pTaskLogic->getPtTaskType();
}

bool CGamePlayer::isSuccessFinishTask()
{
    
    if (m_pTaskLogic->getPtTaskCurrentNumber() >= m_pTaskLogic->getPtTaskTotalNumber())
    {
        return true;
    }
    return false;
    
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
vector<SEveryATKData*> CGamePlayer::getHpAngryVectory()const
{
    return m_vHpAngry;
}
vector<CFightCardFightingBuffer *> CGamePlayer::getFightCardFightingBuffer()const
{
    return m_vCFightCardFightingBuffer;
}

CStructStrips *CGamePlayer::getSkillStripNeed(int id)
{
    return m_haspMapSkillStrips[id];
}

bool CGamePlayer::isCheckNeedSatisfied(int nlineIndex,EN_LEFTTEAMORRIGHTTEAM enValue)
{
    CStructStrips *pStrip=getSkillStripNeed(nlineIndex);
    if(pStrip)
    {
        int i=0;
        if (enValue==EN_RIGHTTEAM)
        {
            i=4;
        }
        if (zhongzuCount[i++]>=pStrip->m_nStripsOne &&
            zhongzuCount[i++]>=pStrip->m_nStripsTwo &&
            zhongzuCount[i++]>=pStrip->m_nStripsThree  &&
            zhongzuCount[i++]>=pStrip->m_nStripsFour )
        {
            return true;
        }
    }
   return false;
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
    CEmrysClearVectorMemory<int > tempClear(m_getRandom_data);
    tempClear.clearVector();
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

/*
 * @breif 获取取背包中该道具ID的数目
 * @param inPropID 道具ID
 * @return prop count
 */
int CGamePlayer::getPropCountFromBag(int inPropId)
{
    int count = 0;
    map<int, int>::iterator i = m_vProps.find(inPropId);
    if (i!= m_vProps.end())
    {
        count = i->second;
    }
    return count;
}

/*
 * @breif :获取该道具能填充的背包的最大添充数
 * @param inPropId 道具ID
 * @return int
 */

int CGamePlayer::getPropMaxCountAddToBag(int inPropId)
{
    if (inPropId >= EXPID && inPropId < NOPROPIDEND)
    {
        return  0;
    }
    CPtProp* prop = NULL;
    int count = 0;
    int useGridCount = getUseGridCount();
    int restGridCount = getOpenGridCount()-useGridCount;
    map<int, int>::iterator i = m_vProps.find(inPropId);
    map<int, CPtProp*>::iterator propIterator = m_rAllProps.end();
    propIterator  =  m_rAllProps.find(inPropId);
    if (propIterator != m_rAllProps.end())
    {
        prop = propIterator->second;
        CCAssert(prop, "the prop is null");
        int limit = prop->getLimitNum();
        if (i != m_vProps.end())
        {
            if (prop->getIsOnlyNum()==1)
            {
                count = 0;
                return count;
            }
            
            int restNum = i->second%limit;
            count += restNum != 0 ? limit-restNum : 0;
        }
        if (restGridCount>=0)
        {
            count += restGridCount*limit;
        }
    }
    
    CCLog("the %d prop 's max count : %d ", i->first, count);
    return count;
}

/*
 *  @return  nRet--> -1 不存在， inNum 够， <inNum : 不够的数目
 */
int CGamePlayer::haveEnoughPropById(int inPropId, int inNum)
{
    int nRet = -1;
    map<int, int>::iterator it = m_vProps.find(inPropId);
    if (it != m_vProps.end())
    {
        nRet =it->second-inNum;
    }
    else
    {
        return nRet;
    }
    if (nRet >= 0 )
    {
        nRet = inNum;
    }else
    {
        nRet = -nRet;
    }
    return nRet;
}

bool CGamePlayer::addGridBySys()
{
    
    return getPlayerLevel() >= OPENGGRIDLEVLE ;
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
            
            CCLog("the userGridcount: %d", useGridCount);
        }
    }
    for (int i = 0; i < emailIds.size(); i++)
    {
        CCLog("the size: %d", emailIds.at(i));
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
    bool propEnable = false;
    CPtProp *tmp = NULL;
    for (map<int, int>::iterator i = inAddProps.begin(); i != inAddProps.end() && (inUserGridCount+count) <= m_nOpenGridCount; i++)
    {
        
        key = i->first;
        // filter exp and coin:
        if(i->first >= EXPID && i->first < NOPROPIDEND) //(key == EXPID || key == COINSID)
        {
            continue;
        }
        
        propEnable = true;
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
                addCount = (addCount/limitCount) + (addCount%limitCount == 0 ? 0 : 1);
                addCount2 = (addCount2/limitCount) + (addCount2%limitCount == 0 ? 0 : 1);
                count += (addCount-addCount2);
                
            }else
            {
                
                count += (addCount/limitCount) + (addCount%limitCount == 0 ? 0 : 1);
            }
        }
    }
    
    if((inUserGridCount+count) <= m_nOpenGridCount)
    {
        bRet = count;
        
    }
    if (propEnable == false)
    {
        bRet = 0;
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
        if(i->first >= EXPID && i->first < NOPROPIDEND) //(i->first == EXPID || i->first == COINSID)
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
                // is only replace:a
            }
            else
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

void CGamePlayer::setOpenGridCount(int inOpenGridCount)
{
    m_gGamePlayerData->m_nOpenGridCount = inOpenGridCount;
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
        if (keyId >= EXPID && keyId < NOPROPIDEND)
        {
            continue;
        }
        
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
            useGridCount += (itemCount/limitMax) + ((itemCount % limitMax) == 0 ? 0 : 1);
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
    if (inPropId >= EXPID && inPropId < NOPROPIDEND)
    {
        return;
    }
    map<int, int>::iterator it =  m_vProps.find(inPropId);
    if (it != m_vProps.end())
    {
        map<int, CPtProp*>::iterator pt= m_rAllProps.find(inPropId);
        if (pt != m_rAllProps.end() && pt->second->getIsOnlyNum() == 1)
        {
            return;
        }
        
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


vector<CFightCard *> & CGamePlayer::getCardBagVector()
{
    return m_vCardBag;
}

vector<vector<CFightCard *> > &CGamePlayer::getCardBattleArray()
{
    return m_vvBattleArray;
}

void CGamePlayer::setCurrentTaskId(int inTaskId)
{
    m_nCurrentTaskId = inTaskId;
    setChapterAndSectionByTask();
}

void CGamePlayer::setChapterAndSectionByTask()
{
    CPtTask* task = SingleTaskConfig::instance()->getTaskById(m_nCurrentTaskId);
    if(task)
    {
        m_nMaxChapterId = task->getChapterId();
        m_nMaxSectionId = task->getSectionId();
        if (m_pTaskLogic) {
            m_pTaskLogic->setInitDataByCPtTask(task);
        }
        m_nCurrentTaskId=task->getTaskId();
    }
}

void CGamePlayer::sendUpdateAp()
{
//      m_pUpdateAp->pause();
//    if (getApMax()>getPlayerAp())
//    {
        //这种做法的时候 必须是服务端如果是满值的情况下必须在服务端满值的情况下 传值必须》1
        m_pUpdateAp->stop();
        string connectData="sig=";
        connectData += m_strSig;
        ADDHTTPREQUESTPOSTDATA(STR_URL_UPDATEAPHP(194), "CALLBACK_CGamePlayer_decodeDataAp", "REQUEST_CGamePlayer_decodeDataAp",connectData.c_str(),callfuncO_selector(CGamePlayer::decodeDataAp));
//    }
//    else{
//        m_pUpdateAp->resume();
//    }
}

void CGamePlayer::sendUpdateGp()
{
//    m_pUpdateGp->pause();
//    if (getGpMax()>getPlayerGp())
//    {
         m_pUpdateGp->stop();
        string connectData="sig=";
        connectData += m_strSig;
        ADDHTTPREQUESTPOSTDATA(STR_URL_UPDATEAPHP(194), "CALLBACK_CGamePlayer_decodeDataGp", "REQUEST_CGamePlayer_decodeDataGp",connectData.c_str(),callfuncO_selector(CGamePlayer::decodeDataGp));
//    }
//    else{
//        m_pUpdateGp->resume();
//    }
}

void CGamePlayer::decodeDataAp(cocos2d::CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_decodeDataAp");
    if(!object)
    {
        CCMessageBox("fuck 服务端 传递AP的数据是NULL的", "error");
        m_pUpdateAp->startOldTimer();
        return;
    }
    char *strdata=(char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    if (GameTools::intForKey("code", dict)!=0) {
        CCMessageBox("fuck 服务端 传递AP的数据是NULL的", "error");
        return;
    }
    CCDictionary *result=(CCDictionary *)dict->objectForKey("result");
    if(result)
    {
        CCDictionary *ap=(CCDictionary*)result->objectForKey("ap");
        setPlayerAp(GameTools::intForKey("ap", ap));
        m_pUpdateAp->start(GameTools::intForKey("lever_time", ap), 0.0f);
    }
    
}

void CGamePlayer::decodeDataGp(cocos2d::CCObject *object)
{
     CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_decodeDataGp");
    if(!object)
    {
        CCMessageBox("fuck 服务端 传递AP的数据是NULL的", "error");
        m_pUpdateGp->startOldTimer();
        return;
    }
    char *strdata=(char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    CCDictionary *result=(CCDictionary *)dict->objectForKey("result");
    if(result)
    {
        CCDictionary *gp=(CCDictionary*)result->objectForKey("gp");
        setPlayerGp(GameTools::intForKey("gp", gp));
        m_pUpdateGp->start(GameTools::intForKey("lever_time", gp), 0.0f);
    }

    
}

void CGamePlayer::setReBackXiangLiang()
{
    if(m_gameShop)
{
    m_gameShop->setRebackMaxNumber();
}
    
}
void CGamePlayer::setShopItemByItemId(int itemID,int nValue)
{
    if(m_gameShop)
    {
        m_gameShop->logicShopItemNumber(itemID, nValue);
    }
}
CStructShopSellItem *CGamePlayer::getShopItemById(int itemID)
{
    if (itemID>m_gameShop->mapShopItem.size()) {
        return NULL;
    }
    return m_gameShop->mapShopItem[itemID];
}

int CGamePlayer::getShopItemCount()
{
    return m_gameShop->getShopItemCount();
}

int CGamePlayer::getShopType()
{
    return m_gameShop->getShopType();
}

string CGamePlayer::getShopName()
{
    return m_gameShop->getShopName();
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

void CGamePlayer::onGameBegin(const char* pchNickname)
{
    //xianbei modify
    gameInitStatus=0;
    int getEmailMax=G_GAMESINGEMAIL::instance()->getCurrentTotalEmail();
    char data[256];
    sprintf(data, "&info={\"max_id\":%d,\"name\":\"%s\"}",getEmailMax,pchNickname);
    string connectData="sig=";
    connectData += m_strSig;
    connectData+=data;
    ADDHTTPREQUESTPOSTDATA(STR_URL_GAMEINIT(194), "CALLBACK_CGamePlayer_GameBegin", "REQUEST_CGamePlayer_GameBegin",connectData.c_str(),callfuncO_selector(CGamePlayer::onGameBeginCallBack));
    
}

void CGamePlayer::onGameBeginCallBack(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_GameBegin");
    if (!dealWithCallbackObject(object))
    {
        gameInitStatus=ERROR_MSG_CONNECTSERVERERROR;
        return;
    }
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
        string teamStrType;
        if (dictresult->objectForKey("inbox_info"))
        {
            teamStrType=typeid(*dictresult->objectForKey("inbox_info")).name();
            if(teamStrType.find("CCDictionary")!=std::string::npos)
            {
                CCDictionary *dicinfobox_info=(CCDictionary *)dictresult->objectForKey("inbox_info");
                G_GAMESINGEMAIL::instance()->decodeEmap(dicinfobox_info);
            }
        }
        if (dictresult->objectForKey("task_info")) {
            teamStrType=typeid(*dictresult->objectForKey("task_info")).name();
            if(teamStrType.find("CCDictionary")!=std::string::npos)
            {
                CCDictionary *getTaskInfo=(CCDictionary *)dictresult->objectForKey("task_info");
                parseTaskInfo(getTaskInfo);
            }
        }
        
        // add prop buffer by phileas
        CCObject *propBuffer = dictresult->objectForKey("buff");
        if (propBuffer)
        {
            teamStrType = typeid(*propBuffer).name();
            if (teamStrType.find("CCDictionary")!=std::string::npos)
            {
                CPlayerBufferManager::getInstance()->resetPropBufferByDict((CCDictionary*) propBuffer);
                CGameTimerManager::getInstance()->startTimer();
            }
        }
        
        //获得GP
        teamStrType=typeid(*dictresult->objectForKey("gp")).name();
        if(teamStrType.find("CCDictionary")!=std::string::npos)
        {
            CCDictionary *getGp=(CCDictionary *)dictresult->objectForKey("gp");
            int value =GameTools::intForKey("lever_time", getGp);
            if (m_pUpdateGp)
            {
                m_pUpdateGp->start(value, 0.0);
            }
        }
        teamStrType=typeid(*dictresult->objectForKey("ap")).name();
        if(teamStrType.find("CCDictionary")!=std::string::npos)
        {
            CCDictionary *getAp=(CCDictionary *)dictresult->objectForKey("ap");
            int value =GameTools::intForKey("lever_time", getAp);
            if (m_pUpdateAp)
            {
                m_pUpdateAp->start(value, 0.0);
            }
        }
        
        //获得AP
        
        // 必须先加载user_info的信息，在加载阵容的信息
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

void CGamePlayer::parseTaskInfo(CCDictionary *dict)
{
    CCArray *array=dict->allKeys();;
    for (int i=0; i<array->count(); i++) {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CCDictionary *detail=(CCDictionary*)(dict->objectForKey(key->m_sString));
        CCLog("CURRENT TASK INFO IS %d",key->intValue());
        if (key->intValue()==0)
        {
            continue;
        }
        setCurrentTaskId(key->intValue());
        setInitCurrentTaskTargetNumber(GameTools::intForKey("num", detail));
        m_pTaskLogic->setPtTaskCurrentStatus(GameTools::intForKey("status", detail));
        break;
    }

}

void CGamePlayer::parseJsonUserInfo(CCDictionary *dict)
{
    if(m_gGamePlayerData)
    {
        m_gGamePlayerData->decodeDictnory(dict);
    }
    CCDictionary *cardinfo=(CCDictionary *)(dict->objectForKey("card_item"));
    if (cardinfo)
    {
            decodeCardDictAppendCardBag(cardinfo);
    }
    
    CCDictionary *bag_info=(CCDictionary *)(dict->objectForKey("bag_info"));
    if(bag_info)
    {
        parseProsInfoByDict(bag_info);
    }
    
}

/*
 * @breif: 获取阵容中卡片领导力的总和
 * @param :inType [0-2] 0- 1进攻 1- 2进攻 2- 防御
 */
int CGamePlayer::getAllRvcBattlerArray(const int& inType)
{
    if (inType < 0 || inType > 2)
    {
        return 0;
    }
    
    int allRvc = 0;
    vector<CFightCard*> &tmp =  m_vvBattleArray.at(inType);
    CFightCard *tmpCard = NULL;
    for (int i = 0; i < tmp.size(); i++ )
    {
        tmpCard = tmp.at(i);
        if (tmpCard)
        {
            allRvc += tmpCard->m_pCard->m_icard_leadership;
        }
    }
    return allRvc;
}


/*
 * @breif: 玩家exp增加， 当等级提升shi
 */
void CGamePlayer::updatePlayerDataWithExp()
{
    int exp = getPlayerExp();
    int expMax = m_gGamePlayerData->getExpMax();
    if (exp >= expMax)
    {
        // send request:
        sendLeveleUpRequest();
    }
}


// example: cube.games.com/api.php?m=GameBegin&a=checkLevelUp&uid=1000001&sig=2ac2b1e302c46976beaab20a68ef95
void CGamePlayer::sendLeveleUpRequest()
{
      char buff[200]={0};
      sprintf(buff,"sig=%s", getUserSig());
      ADDHTTPREQUESTPOSTDATA(STR_URL_LEVEL_UP(196), "CALLBACK_CGamePlayer_sendLeveleUpRequest", "REQUEST_CGamePlayer_sendLeveleUpRequest",buff,callfuncO_selector(CGamePlayer::receiveLevelUpRequestMsg));
}

void CGamePlayer::receiveLevelUpRequestMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CGamePlayer_sendLeveleUpRequest");
    char *buffer =(char*)pObject;
    if (buffer)
    {
        CCDictionary * dict = PtJsonUtility::JsonStringParse(buffer);
        parseLevelUpInfoByDict(dict);
        delete [] buffer;
    }
}

void CGamePlayer::parseLevelUpInfoByDict(CCDictionary *inDict)
{
    if (inDict)
    {
        int code = GameTools::intForKey("code", inDict);
        if (code == 0)
        {
            CCDictionary *result = (CCDictionary *)inDict->objectForKey("result");
            changePlayerInfoWithLevelUp(result);

        }else
        {
            CCLog("server error code : %d", code);
        }
    }
}

void CGamePlayer::changePlayerInfoWithLevelUp(CCDictionary *inDict)
{
    if (inDict)
    {
        // save Data:
        int level = GameTools::intForKey("level", inDict);
        int exp = GameTools::intForKey("exp", inDict);
        int gp = GameTools::intForKey("gp", inDict);
        int ap = GameTools::intForKey("ap", inDict);
        int grid = GameTools::intForKey("grid", inDict);
        
        levelUpData(level);
        setPlayerExp(exp);
        setPlayerAp(ap);
        setPlayerGp(gp);
        if (grid != 0)
        {
            setOpenGridCount(grid);
        }
       
        // notify
      //  CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFYTAG_LEVELUP,NULL);
        CCLog( "level up --> level : %d, exp: %d, gp: %d, ap : %d, grid : %d", level, exp, gp, ap, grid);
        char buffTip[300]= {0};
        sprintf(buffTip, "level up --> level : %d, exp: %d, gp: %d, ap : %d, grid : %d", level, exp, gp, ap, grid);
        Middle::showAlertView(buffTip);
        
    }
}

void CGamePlayer::initZhongZu()
{
    memset(zhongzuCount, 0, 8);
}

void CGamePlayer::levelUpData(int level)
{
    if (level <= MAXLEVEL)
    {
        m_gGamePlayerData->m_sLevelPlayer = m_gGamePlayerData->m_gvPlayerLevel.at(level);
        m_gGamePlayerData->m_irvc = m_gGamePlayerData->m_sLevelPlayer->m_iLeader_max;
        m_gGamePlayerData->m_ilevel = level;

    }
}
