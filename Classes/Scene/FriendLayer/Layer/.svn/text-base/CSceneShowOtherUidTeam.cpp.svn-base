//
//  CSceneShowOtherUidTeam.cpp
//  91.cube
//
//  Created by linminglu on 11/18/13.
//
//

#include "CSceneShowOtherUidTeam.h"
#include "CGameButtonControl.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "gameStruct.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CCard.h"
#include "CGamesCard.h"

#define TAG_NUEJIYOU  10
#define TAG_QUITBUTTON 12

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


CSceneShowOtherUidTeam *CSceneShowOtherUidTeam::CreateLayer(int Uid)
{
    CSceneShowOtherUidTeam *pLayer=new CSceneShowOtherUidTeam;
    if (!pLayer||!pLayer->initCreate(Uid)) {
        CC_SAFE_DELETE(pLayer);
        return NULL;
    }
    return pLayer;
}
void CSceneShowOtherUidTeam::clearVectorList()
{
    CEmrysClearVectorMemory<CGameButtonControl *>tempClear(m_vTouchVector);
    tempClear.clearVector();
    for (int i=0; i<3; i++)
    {
        DELETE_POINT_VECTOR(m_vFightCardList[i],vector<CFightCard *>,CFightCard);
    }
}

CSceneShowOtherUidTeam::~CSceneShowOtherUidTeam()
{
    clearVectorList();
}

CSceneShowOtherUidTeam::CSceneShowOtherUidTeam()
{
    clearVectorList();
    m_tempGamePlayer=SinglePlayer::instance();
}
//触控问题
bool CSceneShowOtherUidTeam::initCreate(int Uid)
{
    if (!CCLayer::init()) {
        return false;
    }
    m_nUid=Uid;
    createBackGroud();
    //createFightButton();
    createQuitButton();
    createFightText();
    createTeamText();
    sendGetTeamInfo();
    setTouchPriority(-9);
    setTouchEnabled(true);
    return true;
}

void CSceneShowOtherUidTeam::createBackGroud()
{
    CCLayer  *layer=CCLayer::create();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(layer, CSTR_FILEPTAH(plistPath, "tujianjiemian.plist"));
    addChild(layer,1,1);
}

void CSceneShowOtherUidTeam::createFightCardContext()
{
    for (int i=0; i<3; i++) {
        for (int vIndex=0; vIndex<m_vFightCardList[i].size(); vIndex++) {
            if (m_vFightCardList[i][vIndex]) {
                CGamesCard *gamecard=CGamesCard::Create(m_vFightCardList[i][vIndex]);
                addChild(gamecard,2);
                gamecard->setPosition(getPostionCard(i, vIndex));
            }
        }
    }
    for (int i=0;i<m_vFightCardList[2].size();i++)
    {
     for (int vIndex=0; vIndex<m_vFightCardList[i].size(); vIndex++)
     {
        if (m_vFightCardList[2][vIndex])
        {
            createFightButton();
            return;
        }
     }
    }
}

void CSceneShowOtherUidTeam::createFightButton()
{
    CGameButtonControl *pGameButton=CGameButtonControl::createButton(TEXTMID, "虐下基友", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    pGameButton->setPosition(ccp(size.width*0.5, 100));
    addChild(pGameButton,2,TAG_NUEJIYOU);
    m_vTouchVector.push_back(pGameButton);
    
}

void CSceneShowOtherUidTeam::createQuitButton()
{
    CGameButtonControl *pGameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    pGameButton->setPosition(ccp(size.width*0.5+300, 700));
    addChild(pGameButton,2,TAG_QUITBUTTON);
    m_vTouchVector.push_back(pGameButton);
}

void CSceneShowOtherUidTeam::createOtherUidText()
{
    //
}

bool CSceneShowOtherUidTeam::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CSceneShowOtherUidTeam::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSceneShowOtherUidTeam::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    this->convertToNodeSpace(pTouch->getLocation());
    for (int i=0; i<m_vTouchVector.size(); i++) {
        if (m_vTouchVector[i]->boundingBox().containsPoint(pTouch->getLocation()))
        {
            handTouchTag(m_vTouchVector[i]->getTag());
        }
    }
}

void CSceneShowOtherUidTeam::handTouchTag(int tag)
{
    switch (tag) {
        case TAG_QUITBUTTON:
            removeFromParentAndCleanup(true);
            break;
        case TAG_NUEJIYOU:
            //设置打战了。
            break;
        default:
            break;
    }
}

void CSceneShowOtherUidTeam::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSceneShowOtherUidTeam::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -9, true);
}

void CSceneShowOtherUidTeam::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CSceneShowOtherUidTeam::createFightText()
{
    string value="基友ID:";
    value +=ConvertToString(m_nUid);
    CCLabelTTF *labelTTF=CCLabelTTF::create(value.c_str(), "Arial", 25);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    labelTTF->setPosition(ccp(size.width*0.5, size.height-100));
    addChild(labelTTF,2);
}

CCPoint CSceneShowOtherUidTeam::getPostionCard(int row ,int cloumn)
{
       CCPoint topPoint=CCPointMake(300, 560);
        return CCPointMake(topPoint.x+cloumn*100,topPoint.y-row*220);
}

void CSceneShowOtherUidTeam::createTeamText()
{
    char data[20];
    for (int i=0; i<3; i++)
    {
        sprintf(data, "Team%d:",i+1);
        CCLabelTTF *pLableTTF=CCLabelTTF::create(data, "Arial", 20);
        pLableTTF->setColor(ccc3(255, 0, 0));
        pLableTTF->setPosition(ccp(120, 660-220*i));
        addChild(pLableTTF,2);
    }
    
}

void CSceneShowOtherUidTeam::sendGetTeamInfo()
{
    //获取对方阵容的信息。
   /* 
    api.php?m=Card&a=getCardTeam&uid=194(用户ID)
    参数 ：
	&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&friend_uid=1100(获取指定uid的阵容信息)
    */
    
    string  str="&sig=";
    str+=m_tempGamePlayer->getUserSig();
    str+="&friend_uid=";
    str+=ConvertToString(m_nUid);
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_GET_TEAM(194), "CALLBACK_CSceneShowOtherUidTeam_sendGetTeamInfo", "REQUEST_CSceneShowOtherUidTeam_sendGetTeamInfo",str.c_str(),callfuncO_selector(CSceneShowOtherUidTeam::decodeGetTeamInfo))
}

void CSceneShowOtherUidTeam::decodeGetTeamInfo(CCObject *object)
{
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneShowOtherUidTeam_sendGetTeamInfo");
    if (!object) {
        CCMessageBox("服务端传输数据为NULL", "INFO");
        return ;
    }
    char *datat=(char *)object;
    CCLog("%s",datat);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(datat);
    delete [] datat;
    datat=NULL;
    if (GameTools::intForKey("code", dict)!=0) {
        CCMessageBox("有数据错误", "senddata");
        return;
    }
  CCDictionary *resutlDict=(CCDictionary *)(dict->objectForKey("result"));
    if (!resutlDict) {
        return ;
    }
    CCDictionary *carditems=(CCDictionary *)(resutlDict->objectForKey("card_item"));
    //到时候 队列里面找到这个数干。。。
    //解析team
    CCDictionary *cardTeams=(CCDictionary *)(resutlDict->objectForKey("card_team"));
    CCDictElement *element = NULL;
    CCDictionary *onlyTeam=NULL;
    CCDICT_FOREACH(cardTeams, element)
    {
        CCObject * object = element->getObject();
        
        if (object && (onlyTeam = (CCDictionary *)(object)))
        {
            int keyValue=atoi(element->getStrKey());
            initCardTeam(keyValue-1, onlyTeam, carditems);
        }
    }
    createFightCardContext();
 }
void CSceneShowOtherUidTeam::initCardTeam(int index,CCDictionary *cardTeam,CCDictionary *cardItem)
{
    CCDictionary *teamVectry=(CCDictionary *)cardTeam->objectForKey("team");
    m_vFightCardList[index].clear();
    m_vFightCardList[index].resize(5);
    CCDictElement *element = NULL;
    CCDictionary *onlyCard=NULL;
    CCDICT_FOREACH(teamVectry, element)
    {
        CCObject * object = element->getObject();
        
        if (object && (onlyCard = (CCDictionary *)(object)))
        {
            int card_user_id=GameTools::intForKey("card_item_id", onlyCard);
            int postion=GameTools::intForKey("position", onlyCard);
            CCDictionary *dictCard=(CCDictionary *)cardItem->objectForKey(ConvertToString(card_user_id).c_str());
            if (dictCard) {
                int cardid=GameTools::intForKey("card_id", dictCard);
                int level=GameTools::intForKey("level", dictCard);
                CCard *card=m_tempGamePlayer->getCardByCardId(cardid);
                if(card)
                {
                    CFightCard *pFightCard=new CFightCard(card,level);
                    m_vFightCardList[index][postion-1]=pFightCard;
                }
            }
        }
    }
}