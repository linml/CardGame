//
//  PVPSceneLayer.cpp
//  91.cube
//
//  Created by phileas on 14-1-7.
//
//

#include "PVPSceneLayer.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "CPVPKaHunLayer.h"
#include "CPVPRegulationLayer.h"
#include "CPVPStructMissionTaskRewordLayer.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "PtHttpClient.h"
#include "CPVPMonsterData.h"
#include "CPVPMonsterPlayerLayer.h"
#include "CPVPReportLayer.h"
#include "SceneManager.h"

int PVPSceneLayer::m_nProtect_time=0;


cocos2d::CCScene* PVPSceneLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    PVPSceneLayer *layer = PVPSceneLayer::create();
    
    scene->addChild(layer);
    
    return scene;

}
// public static method:
PVPSceneLayer* PVPSceneLayer::create()
{
    PVPSceneLayer *layer = new PVPSceneLayer();
    if (layer)
    {
        if (layer->init())
        {
            layer->autorelease();
        }
        else
        {
            delete layer;
            layer = NULL;
        }
    }
    return layer;
}

PVPSceneLayer::PVPSceneLayer()
{
    m_pBgContainer = NULL;
    m_nProtect_time=0;
}
PVPSceneLayer::~PVPSceneLayer()
{
    
}
bool PVPSceneLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        m_pPlayer = SinglePlayer::instance();
        // init 
        initPVPSceneLayerUI();
        // set touch enable:
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(PVPSCENETOUCH_PRIORITY);
        bRet = true;
    } while (0);
    return bRet;
}

bool PVPSceneLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    for (int i = 0; i < PVPSCENEBTNCOUNT; i++ )
    {
        if (CPtTool::isInNode(m_pBtns[i], pTouch))
        {
            m_nTouchTag = i;
            break;
        }
    }
    return true;
}
void PVPSceneLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void PVPSceneLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    for (int i = 0; i < PVPSCENEBTNCOUNT; i++ )
    {
        if (CPtTool::isInNode(m_pBtns[i], pTouch))
        {
            if(m_nTouchTag == i)
            {
                switch (m_nTouchTag)
                {
                    case RuleBtn:
                        onClickRule();
                        break;
                    case BattleLogBtn:
                        onClickBattleLog();
                        break;
                    case ChallengeRewardBtn:
                        onClickChallengeReward();
                        break;
                    case HuDianBtn:
                        onClickHuDian();
                        break;
                    case SearchOpponentBtn:
                        onClickSearchOpponent();
                        break;
                    case AddPVPCountBtn:
                        onClickAddPVPCount();
                        break;
                    case BackBtn:
                        onClickBack();
                        break;
                    case PVPRankRewardBtn:
                        onClickPVPRankReward();
                        break;
                    default:
                        break;
                }
            }
            break;
        }
    }

}
void PVPSceneLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



// protected:

void PVPSceneLayer::initPVPSceneLayerUI()
{
    //create bg:
    CCSize size(CCDirector::sharedDirector()->getWinSize());
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "niudanbeijing.png"));
    bg->setPosition(ccp(size.width*0.5, size.height*0.5));
    m_pBgContainer = bg;
    size = bg->getContentSize();
    addChild(bg);

    // create buttons:
    // create 规则按钮 0
    const char * title = "规则";
    CCSprite *tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    CCLabelTTF *tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    CCPoint point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.1, size.height*0.2));
    bg->addChild(tmpSprite);
    m_pBtns[RuleBtn] = tmpSprite;
    // create 战报按钮 
    
    title = "战报";
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.1, size.height*0.1));
    bg->addChild(tmpSprite);
    m_pBtns[BattleLogBtn] = tmpSprite;
    // create 挑战奖励按钮 
    
    title = "挑战奖励";
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.3, size.height*0.15));
    bg->addChild(tmpSprite);
    m_pBtns[ChallengeRewardBtn] = tmpSprite;
    // create 魂殿按钮 
    
    title = "魂殿";
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.5, size.height*0.15));
    bg->addChild(tmpSprite);
    m_pBtns[HuDianBtn] = tmpSprite;
    // create 搜索对手按钮 
    
    title = "搜索对手";
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.72, size.height*0.15));
    bg->addChild(tmpSprite);
    m_pBtns[SearchOpponentBtn] = tmpSprite;
    // create 返回按钮 7
    title = "返回";
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point = ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    tmpLabel->setPosition(point);
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(ccp(size.width*0.9, size.height*0.9));
    bg->addChild(tmpSprite);
    m_pBtns[BackBtn] = tmpSprite;
    
    PVPRankReward reward ;
    reward.coin = 999999;
    reward.kahun = 999;
    
    createPVPRankInfo();
    createPlayerPVPInfo();
    createPlayePVPRankReward(&reward);
    createTiaoZhanCountInfo();
}

void PVPSceneLayer::createPlayerPVPInfo()
{
    int credits = 9999; // 游戏积分
    int coin = 9999;
    int rank = 9999;
    int level = 9999;
    int fight = 9999;
    CCPoint point(m_pBgContainer->getContentSize().width*0.07, m_pBgContainer->getContentSize().height*0.75);
    
    // 积分
    char buffer[30] = {0};
    snprintf(buffer, sizeof(char)*30, "积分: %d", credits);
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pBgContainer->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "金币: %d", coin);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pBgContainer->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "排名: %d", rank);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pBgContainer->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "等级: %d", level);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pBgContainer->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "战力: %d", fight);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pBgContainer->addChild(label);
}
void PVPSceneLayer::createPVPRankInfo()
{
    // test data:
    PVPRankData data;
    data.name = "杀很大";
    data.fightpointer = 9999;
    data.credits = 999;
    data.level = 99;
    data.rank = 1;
    data.uid = 239;
    CCNode* node = NULL;
    CCSprite * rankBg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "rankbg.png"));
    m_pBgContainer->addChild(rankBg);
    CCPoint point(m_pBgContainer->getContentSize().width*0.5, m_pBgContainer->getContentSize().height*0.52);
    rankBg->setPosition(point);
    CCSize size = rankBg->getContentSize();
    
    data.rank = 2;
    node = createRankNode(&data);
    node ->setPosition(ccp(size.width*0.2, size.height*0.4));
    rankBg->addChild(node);
    
    data.rank = 1;
    node =  createRankNode(&data);
    node ->setPosition(ccp(size.width*0.5, size.height*0.4));
    rankBg->addChild(node);
    
    data.rank = 3;
    node =  createRankNode(&data);
    node ->setPosition(ccp(size.width*0.8, size.height*0.4));
    rankBg->addChild(node);
    m_pBtns[PVPRankBtn]=node;
}
void PVPSceneLayer::createPlayePVPRankReward(PVPRankReward * inRewardData, int inTime /*= -1*/)
{
    CCSize size = m_pBgContainer->getContentSize();
    CCPoint point(size.width*0.84, size.height*0.67);
    char buffer[20] = {0};
    sprintf(buffer, "此排名可获得:");
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 14);
    label->setAnchorPoint(ccp(0,0));
    m_pBgContainer->addChild(label);
    label->setPosition(point);
    
    point.y -= 25;
    sprintf(buffer, "金币*%d", inRewardData->coin);
    label = CCLabelTTF::create(buffer, "Arial", 14);
    label->setAnchorPoint(ccp(0,0));
    m_pBgContainer->addChild(label);
    label->setPosition(point);
    
    point.y -= 25;
    sprintf(buffer, "卡魂*%d", inRewardData->kahun);
    label = CCLabelTTF::create(buffer, "Arial", 14);
    label->setAnchorPoint(ccp(0,0));
    m_pBgContainer->addChild(label);
    label->setPosition(point);
    
    point.y -= 40;
    CCSprite * baoxiang = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "baoxiang.png"));
    baoxiang->setPosition(point);
    baoxiang->setAnchorPoint(ccp(0,1));
    m_pBgContainer->addChild(baoxiang);
    
    const char * title = "可领取";
    CCSprite *tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    CCLabelTTF *tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point.y -= 80;
    tmpLabel->setPosition(ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5));
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(point);
    tmpSprite->setAnchorPoint(ccp(0.125,1));
    m_pBgContainer->addChild(tmpSprite);
    m_pBtns[PVPRankRewardBtn] = tmpSprite;
    
}

void PVPSceneLayer::createTiaoZhanCountInfo()
{

    CCSize size=  m_pBgContainer->getContentSize();
    int currentCount = m_pPlayer->getPlayerPVPCount();
    
    char buffer[10] = {0};
    sprintf(buffer, "%d/%d",currentCount,5);
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 13);
    CCPoint point(size.width*0.78, size.height*0.10);
    label->setPosition(point);
    label->setAnchorPoint(ccp(0,1));
    m_pBgContainer->addChild(label);
    
    const char * title = "增加";
    CCSprite *tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    CCLabelTTF *tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point.x += 30;
    tmpLabel->setPosition(ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5));
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(point);
    tmpSprite->setAnchorPoint(ccp(0,0.78));
    m_pBgContainer->addChild(tmpSprite);
    m_pBtns[AddPVPCountBtn]=tmpSprite;
}
CCNode* PVPSceneLayer::createRankNode(PVPRankData * inRankData)
{
    CCNode *node = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath+"/fighting/", "bg1.png"));
    int uid = inRankData->uid;
    char buffer[30]={0};
    sprintf(buffer, "第%d名",inRankData->rank);
    CCPoint point(node->getContentSize().width*0.5, node->getContentSize().height*0.85);
    CCLabelTTF*label = CCLabelTTF::create(buffer, "Arail", 13);
    
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(ccp(0.5,0));
    label->setPosition(point);
    node->addChild(label);
    
    sprintf(buffer, "%d.png", (uid%5)+1);
    CCSprite * image = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, buffer));
    point.y *= 0.825;
    image->setPosition(point);
    node->addChild(image);
    
    point.x *= 0.4;
    point.y -= 50;
    snprintf(buffer, sizeof(char)*30, "昵称: %s", inRankData->name.c_str());
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    node->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "排名: %d", inRankData->fightpointer);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    node->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "积分: %d", inRankData->credits);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    node->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "等级: %d", inRankData->level);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    node->addChild(label);
    return node;
}

void PVPSceneLayer::onClickRule()
{
    CPVPRegulationLayer *layer = CPVPRegulationLayer::create();
    layer->setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}
void PVPSceneLayer::onClickBattleLog()
{
    CPVPReportLayer *layer = CPVPReportLayer::create();
    layer->setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
    
}
void PVPSceneLayer::onClickChallengeReward()
{
    CPVPStructMissionTaskRewordLayer *layer = CPVPStructMissionTaskRewordLayer::create();
    layer->setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}
void PVPSceneLayer::onClickHuDian()
{
    CPVPKaHunLayer *layer = CPVPKaHunLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}

void PVPSceneLayer::onClickSearchOpponent()
{
    //发送请求搜索对手
    string str;
    str+="&sig=";
    str+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPSEARCHDUISHOU(194), "CALLBACK_PVPSceneLayer_onClickSearchOpponent", "REQUEST_PVPSceneLayer_onClickSearchOpponent",str.c_str(),callfuncO_selector(PVPSceneLayer::callBackValue))
}

void PVPSceneLayer::onClickAddPVPCount()
{
    
}
void PVPSceneLayer::onClickPVPRankReward()
{
    
}
void PVPSceneLayer::onClickBack()
{
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_HALLSCENE);
}
void PVPSceneLayer::onClickPVPRank()
{
    
}

void PVPSceneLayer::callBackValue(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_PVPSceneLayer_onClickSearchOpponent");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
        return ;
    }
    char * tempdata=(char *)object;
    //解析字符串。 赋值与vector中
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    CCLog("tempdata:%s",tempdata);
    delete [] tempdata;
    tempdata=NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict)
    {
        CPVPMonsterData *pMonsterData=new CPVPMonsterData;
        pMonsterData->autorelease();
        pMonsterData->retain();
        PVPRankData *pp=new PVPRankData();
        CCDictionary *peoInfo=(CCDictionary *)resultDict->objectForKey("pk_user");
        if(peoInfo)
        {
        pp->name=GameTools::valueForKey("username", peoInfo);
        pp->rank=GameTools::intForKey("pvp_num", peoInfo);
        pp->uid=GameTools::intForKey("uid", peoInfo);
        pp->level=GameTools::intForKey("level", peoInfo);
        pp->fightpointer=GameTools::intForKey("power", peoInfo);
        pp->credits =GameTools::intForKey("score", peoInfo);
        m_nProtect_time=GameTools::intForKey("protect_time", peoInfo);
        pMonsterData->setPVPUserData(pp);
        if (CPtTool::isDictionary(resultDict->objectForKey("team_info"))) {
            CCDictionary *temp_info=(CCDictionary *)resultDict->objectForKey("team_info");
            CCDictionary *team=(CCDictionary *)temp_info->objectForKey("team");
            pMonsterData->m_vCardList.clear();
            pMonsterData->m_vCardList.resize(5);
            CCDictElement *element = NULL;
            CCDictionary *onlyCard=NULL;
            CCDICT_FOREACH(team, element)
            {
                CCObject * object = element->getObject();
                
                if (object && (onlyCard = (CCDictionary *)(object)))
                {
                    int postion=GameTools::intForKey("position", onlyCard);
                    int cardid=GameTools::intForKey("card_id", onlyCard);
                    int level=GameTools::intForKey("level", onlyCard);
                    CCard *card=m_pPlayer->getCardByCardId(cardid);
                    if(card)
                    {
                        CFightCard *pFightCard=new CFightCard(card,level);
                        pMonsterData->m_vCardList[postion]=pFightCard;
                    }
                }
            }

        }
        else{
            pMonsterData->m_vCardList.clear();
            pMonsterData->m_vCardList.resize(5);
        }
                    addPvpSearchLayer(pMonsterData);
        }
        else{
            CCMessageBox("您太厉害了 没人是你对手", "恭喜你");
        }

    }

}
void PVPSceneLayer::addPvpSearchLayer(CPVPMonsterData *data)
{
    CPVPMonsterPlayerLayer *layer=CPVPMonsterPlayerLayer::CreateByUserID(data, true);
    layer->setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}