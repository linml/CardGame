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
#include "PtJsonUtility.h"
#include "CReward.h"
#include "CPVPRankLayer.h"
#include "CSceneShowOtherUidTeam.h"
#include "gamePlayer.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "CPVPMonsterData.h"
#include "CPVPMonsterPlayerLayer.h"
#include "CPVPReportLayer.h"
#include "SceneManager.h"
#include "CPVPPublicStruct.h"

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
    m_pDataManager = PVPDataManager::getInstance();
    m_pBgContainer = NULL;
    m_bCanTouch = false;
    m_bCanGetPVPRankReward = false;
    memset(m_pBtns, 0, sizeof(m_pBtns));
    m_nProtect_time=0;
}
PVPSceneLayer::~PVPSceneLayer()
{
    PVPDataManager::releasManager();
    
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
    if (!m_bCanTouch)
    {
        return;
    }
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
                    case BackBtn:
                        onClickBack();
                        break;
                    case PVPRankRewardBtn:
                        onClickPVPRankReward();
                        break;
                    case PVPRankBtn:
                        onClickPVPRank();
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

void PVPSceneLayer::callBack()
{
    m_bCanTouch = true;
    
    PVPDataManager *dataManager = PVPDataManager::getInstance();
    int headCount ;
    PVPRankData * data = dataManager->getHeadRankInfo(headCount);
    createPVPRankInfo(data, headCount);
    createPlayerPVPInfo(dataManager->getUserRankInfo());
    PVPRankReward* rankReward = NULL;
    m_bCanGetPVPRankReward = dataManager->getRankReward(rankReward);
    if (m_bCanGetPVPRankReward)
    {
       
    }
    createPlayePVPRankReward(rankReward);
    createTiaoZhanCountInfo();
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
    
    PVPDataManager::getInstance()->getPVPMainUIInfo(this, callfunc_selector(PVPSceneLayer::callBack));

}

void PVPSceneLayer::createPlayerPVPInfo(PVPRankData *inUserRankInfo)
{
    CCSize size(30, 80);
    m_pPlayerPVPPanel = CCNode::create();
    m_pPlayerPVPPanel->setContentSize(size);
    CCPoint point(m_pBgContainer->getContentSize().width*0.07, m_pBgContainer->getContentSize().height*0.75);
    m_pPlayerPVPPanel->setAnchorPoint(ccp(0, 1));
    m_pPlayerPVPPanel->setPosition(point);
    m_pBgContainer->addChild(m_pPlayerPVPPanel);
    
    int credits = inUserRankInfo->credits; // 游戏积分
    int coin = SinglePlayer::instance()->getCoin();
    int rank = inUserRankInfo->rank;
    int level = inUserRankInfo->level;
    int fight = inUserRankInfo->fightpointer;
    
    
    // 积分
    char buffer[30] = {0};
    point.y = size.height;
    point.x = 0;
    snprintf(buffer, sizeof(char)*30, "积分: %d", credits);
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pPlayerPVPPanel->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "金币: %d", coin);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pPlayerPVPPanel->addChild(label);
    m_pCoinLabel = label;
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "排名: %d", rank);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pPlayerPVPPanel->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "等级: %d", level);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pPlayerPVPPanel->addChild(label);
    
    point.y -= 25;
    snprintf(buffer, sizeof(char)*30, "战力: %d", fight);
    label = CCLabelTTF::create(buffer, "Arail", 13);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(point);
    m_pPlayerPVPPanel->addChild(label);
}
void PVPSceneLayer::createPVPRankInfo(PVPRankData *inHeadRankInfo, int inCount)
{
    CCNode* node = NULL;
    CCSprite * rankBg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "rankbg.png"));
    m_pBgContainer->addChild(rankBg);
    CCPoint point(m_pBgContainer->getContentSize().width*0.5, m_pBgContainer->getContentSize().height*0.52);
    rankBg->setPosition(point);
    CCSize size = rankBg->getContentSize();
    
    float posfactor[3]={0.5, 0.2, 0.8};
    inCount = inCount >= 3? 3: inCount;
    for (int i = 0;  i< inCount; i++)
    {
        node = createRankNode((inHeadRankInfo+i));
        node ->setPosition(ccp(size.width*posfactor[i], size.height*0.4));
        rankBg->addChild(node);
    }
    m_pBtns[PVPRankBtn]=rankBg;
}
void PVPSceneLayer::createPlayePVPRankReward(PVPRankReward * inRewardData)
{
    CCSize bgSize = m_pBgContainer->getContentSize();
    CCPoint point(bgSize.width*0.84, bgSize.height*0.67);
    
    CCSize size(30, 80);
    m_pPVPRankRewardPanel = CCNode::create();
    m_pPVPRankRewardPanel->setContentSize(size);
    m_pBgContainer->addChild(m_pPVPRankRewardPanel);
    m_pPVPRankRewardPanel->ignoreAnchorPointForPosition(false);
    m_pPVPRankRewardPanel->setAnchorPoint(ccp(0, 1));
    m_pPVPRankRewardPanel->setPosition(point);
  
    point.x = 0;
    point.y = size.height;
    
    char buffer[30] = {0};
    sprintf(buffer, "此排名可获得:");
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 14);
    label->setAnchorPoint(ccp(0,0));
    m_pPVPRankRewardPanel->addChild(label);
    label->setPosition(point);
    m_pLabel[0] = label;
    
    if (inRewardData == NULL)
    {
        label->setString("该排名未进榜单");
        m_pLabel[2]=m_pLabel[1] = NULL;
        point.y -= 50;
       
    }
    else
    {
        point.y -= 25;
        sprintf(buffer, "金币*%d", inRewardData->coin);
        label = CCLabelTTF::create(buffer, "Arial", 14);
        label->setAnchorPoint(ccp(0,0));
        m_pPVPRankRewardPanel->addChild(label);
        label->setPosition(point);
        m_pLabel[1] = label;
        
        point.y -= 25;
        sprintf(buffer, "卡魂*%d", inRewardData->kahun);
        label = CCLabelTTF::create(buffer, "Arial", 14);
        label->setAnchorPoint(ccp(0,0));
        m_pPVPRankRewardPanel->addChild(label);
        label->setPosition(point);
        m_pLabel[2] = label;

    }
    const char * title = "可领取";
    int time = PVPDataManager::getInstance()->getOverTime();
    if(time != -1)
    {
        m_cOverTime.hour = time/3600;
        m_cOverTime.date = m_cOverTime.hour/24;
        m_cOverTime.hour %= 24;
        time %= 3600;
        m_cOverTime.min = time/60;
        time %= 60;
        m_cOverTime.sec = time;
        
    }
    if (!m_bCanGetPVPRankReward)
    {
     
        sprintf(buffer, "%02d:%02d:%02d:%02d",m_cOverTime.date,m_cOverTime.hour, m_cOverTime.min, m_cOverTime.sec);
        title = buffer;
    }
    
    point.y -= 40;
    CCSprite * baoxiang = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "baoxiang.png"));
    baoxiang->setPosition(point);
    baoxiang->setAnchorPoint(ccp(0,1));
    m_pPVPRankRewardPanel->addChild(baoxiang);
    m_pBtns[PVPRankRewardBtn] = baoxiang;
   
    CCLabelTTF *tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point.y -= 80;
    point.x += 10;
    tmpLabel->setPosition(point);
    tmpLabel->setAnchorPoint(ccp(0.125,1));
    m_pPVPRankRewardPanel->addChild(tmpLabel);
    m_pTimeLabel = tmpLabel;
    schedule(schedule_selector(PVPSceneLayer::updateTime),1);
    
}

void PVPSceneLayer::createTiaoZhanCountInfo()
{

    CCSize size=  m_pBgContainer->getContentSize();
    m_pAddTiaoZhanCountPanel = CPVPAddTiaoZhanCountLayer::create();
    m_pAddTiaoZhanCountPanel->setAnchorPoint(CCPointZero);
    m_pAddTiaoZhanCountPanel->setPosition(ccp(size.width*0.785, size.height*0.04));
    m_pBgContainer->addChild(m_pAddTiaoZhanCountPanel);
    m_pAddTiaoZhanCountPanel->setTouchPriority(PVPSCENETOUCH_PRIORITY -1);
    
}

void PVPSceneLayer::updatePVPRankReward()
{
    PVPDataManager *dataManager = PVPDataManager::getInstance();
    PVPRankReward* rankReward = NULL;
    m_bCanGetPVPRankReward = dataManager->getRankReward(rankReward);
    if (rankReward==NULL)
    {
       
        m_pLabel[0]->setString("该排名未进榜单");
        m_pLabel[1]->setString("");
        m_pLabel[2]->setString("");
        return;
    }
    int value[2]={rankReward->coin,rankReward->kahun};
    char buffer[30] = {0};
    const char *names[] ={"金币*", "卡魂*"};
    for (int i = 1; i < 3; i++)
    {
        snprintf(buffer, sizeof(buffer), "%s%d", names[i-1],value[i-1]);
        m_pLabel[i]->setString(buffer);
    }
    if (!m_bCanGetPVPRankReward)
    {
         updateTimeTip();
    }
    snprintf(buffer, sizeof(buffer), "金币: %d", SinglePlayer::instance()->getCoin());
    m_pCoinLabel->setString(buffer);
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
    snprintf(buffer, sizeof(char)*30, "战力: %d", inRankData->fightpointer);
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

void PVPSceneLayer::updateTime(float dt)
{
    if (m_pDataManager->getOverTime() < 0)
    {
        this->unschedule(schedule_selector(PVPSceneLayer::updateTime));
        reloadData();
        
    }
    else
    {
        m_pDataManager->subOverTime();
        if (!m_bCanGetPVPRankReward)
        {
            updateTimeTip();
        }
        
    }
}

void PVPSceneLayer::updateTimeTip()
{
    m_cOverTime.sec--;
    if (m_cOverTime.sec < 0)
    {
        m_cOverTime.sec += 60;
        m_cOverTime.min--;
        if (m_cOverTime.min < 0)
        {
            m_cOverTime.min += 60;
            m_cOverTime.hour--;
            if (m_cOverTime.hour < 0)
            {
                m_cOverTime.hour += 24;
                m_cOverTime.date--;
            }
        }
    }
    char buffer[30] = {0};
    sprintf(buffer, "%02d:%02d:%02d:%02d",m_cOverTime.date,m_cOverTime.hour, m_cOverTime.min, m_cOverTime.sec);
    m_pTimeLabel->setString(buffer);
    
    
}
void PVPSceneLayer::reloadData()
{
    m_pPVPRankRewardPanel->removeFromParentAndCleanup(true);
    m_pPlayerPVPPanel->removeFromParentAndCleanup(true);
    m_pAddTiaoZhanCountPanel->removeFromParentAndCleanup(true);
    m_pBtns[PVPRankBtn]->removeFromParentAndCleanup(true);
    PVPDataManager::getInstance()->getPVPMainUIInfo(this, callfunc_selector(PVPSceneLayer::callBack));
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

void PVPSceneLayer::onClickPVPRankReward()
{
    if (m_bCanGetPVPRankReward)
    {
        sendRequestGetPVPRankReward();
    }
    else
    {
        CCMessageBox("can get current pvp rank reward","tip");
    }
    
}
void PVPSceneLayer::onClickBack()
{
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_HALLSCENE);

}
void PVPSceneLayer::onClickPVPRank()
{

    CPVPRankLayer *layer = CPVPRankLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}

void PVPSceneLayer::sendRequestGetPVPRankReward()
{
    char buffer[100] = {0};
    snprintf(buffer, sizeof(buffer), "sig=%s", STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPRANKREWARD(196), "CALLBACK_PVPSceneLayer::sendRequestGetPVPRankReward", "REQUEST_PVPSceneLayer::sendRequestGetPVPRankReward", buffer, callfuncO_selector(PVPSceneLayer::receiveGetPVPRankRewardMsg));
}
void PVPSceneLayer::receiveGetPVPRankRewardMsg(CCObject* pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_PVPSceneLayer::sendRequestGetPVPRankReward");
    char *buffer = (char *)pObject;
    if (buffer)
    {
        CCLog("buffer: %s", buffer);
        CCDictionary *resultDict = (CCDictionary*) PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                resultDict = (CCDictionary*)resultDict->objectForKey("result");
                const char *names[]={"reward", "add", "charts_pvp"};
                for (int i = 0; i < 3; i++)
                {
                    resultDict = (CCDictionary*) resultDict->objectForKey(names[i]);
                    if (resultDict == NULL)
                    {
                        break;
                    }
                }
                if(resultDict)
                {
                    CReward *reward = CReward::create(resultDict);
                    reward->excuteReward(ADD);
                    updatePVPRankReward();
                }
            }
            else
            {
                CCMessageBox(CCString::createWithFormat("error code : %d")->getCString(), "error");
            }
        }
    }
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
    layer->setCustomerTouchProty(PVPSCENETOUCH_PRIORITY-1);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
}
