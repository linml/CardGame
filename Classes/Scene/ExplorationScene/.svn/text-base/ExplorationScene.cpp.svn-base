//
//  ExplorationScene.cpp
//  91.cube
//
//  Created by phileas on 13-6-9.
//
//

#include "ExplorationScene.h"
#include "gameConfig.h"
#include "HallScene.h"
#include "CConfirmLayer.h"
#include "SceneManager.h"
#include "Utility.h"
#include "PtJsonUtility.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "CGameStoryLayer.h"
#include "CPtTool.h"



int g_index = -1;

SECTION_DATA CExploration::s_SectionData ;

int CExploration::s_CurrentEventId = -1;

void CExploration::setCurrentEvnetId(int inEventId)
{
    s_CurrentEventId = inEventId;
}
int  CExploration::getCurrentEventId()
{
    return s_CurrentEventId;
}


int CExploration::getCurrentChapterId()
{
    return s_SectionData.sectionInfo->getChapterId();
}
int CExploration::getCurrentSectionId()
{
    return s_SectionData.sectionInfo->getSectionId();
}
int CExploration::getCurrentTaskId()
{
    return s_SectionData.sectionInfo->getTaskId();
}

void CExploration::setNextEventByDict(CCDictionary *inEventInfo)
{
    if (inEventInfo)
    {
        EVENTDATA eventData =  CBiforestLayer::dispatchEventWithType(inEventInfo);
        setEvents(eventData);
    }
    else
    {
        EVENTDATA eventData;
        setEvents(eventData);
    }

}
void CExploration::setRewardsByDict(CCDictionary *inReward)
{
    CCDictionary * addDict = NULL;
    CCDictionary * decDict = NULL;
    if (inReward)
    {
        addDict = (CCDictionary*) inReward->objectForKey("add");
        decDict = (CCDictionary*) inReward->objectForKey("dec");
        
        if (addDict)
        {
            // add
            int coin = GameTools::intForKey("coin", addDict);
            int hp = GameTools:: intForKey("hp", addDict);
            int energy = GameTools::intForKey("energy", addDict);
            int exp = GameTools::intForKey("exp", addDict);
        }
        if(decDict)
        {
            //dec
            int coin = GameTools::intForKey("coin", decDict);
            int hp = GameTools:: intForKey("hp", decDict);
            int energy = GameTools::intForKey("energy", decDict);
            int exp = GameTools::intForKey("exp", decDict);
            
        }
        
    }
    
}


void CExploration::resetStaticInfo()
{
    s_CurrentEventId = NULL;
    s_SectionData.currentStep = -1;
    s_SectionData.sectionInfo = NULL;
}

void CExploration::setSectionData(int inCurrentStep, const EVENTDATA &inEventData, CPtSection * inSectionData)
{
    s_SectionData.currentStep = inCurrentStep;
    s_SectionData.eventData = inEventData;
    if(inSectionData)
    {
        s_SectionData.sectionInfo = inSectionData;
    }
}

void CExploration::setExplorationInfo(CPtSection * inSectionData)
{
    s_SectionData.sectionInfo = inSectionData;
}

void CExploration::setEvents(const EVENTDATA& inEventData)
{
    s_SectionData.eventData = inEventData;
}

void CExploration::addCurrentStep()
{
    s_SectionData.currentStep++;
}
int CExploration::getCurrentStep()
{
    return s_SectionData.currentStep;
}
void CExploration::setCurrentStep( const int & inCurrentStep)
{
    if (inCurrentStep == 0)
    {
        setCurrentStep(s_SectionData.sectionInfo->getMaxStep()+1);
    }else
    {
        s_SectionData.currentStep = inCurrentStep;
    }

}

const EVENTDATA & CExploration::getEventData()
{
    return s_SectionData.eventData;
}
CPtSection * CExploration::getExplorationSectionInfo()
{
    return s_SectionData.sectionInfo;
}



CCScene* CExploration::scene()
{
    CCScene *scene = CCScene::create();
    CExploration *layer = CExploration::create();
    if (layer)
    {
        scene->addChild(layer);
    }
    return scene;
}

CExploration::CExploration()
{
 
}

CExploration::~CExploration()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    
    CC_SAFE_RELEASE(m_pTriggers);
}


bool CExploration::init()
{
    bool bRet = false;
    do {
        initData();
        CC_BREAK_IF(!CCLayer::init());
        // todo:
        CC_BREAK_IF(!initExploration());
        bRet = true;
    } while (0);
    return bRet;
}

void CExploration::onEnter()
{
    CCLayer::onEnter();
    handlerTrigger();
    
}
bool CExploration::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bLoadTaskInfo == false)
    {
        return false;
    }
    if (m_bCanTouch == false)
    {
        return false;
    }
    // todo:
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag =  TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if (m_nTouchTag == -1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (CPtTool::isInNode(m_pBtn[i], pTouch))
            {
                m_nTouchTag = 3001+i;
                m_pBtn[i]->setPress();
                return true;
            }
        }
       
    }
    else
    {
        return  true;
    }
    return false;

}

void CExploration::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CExploration::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CExploration::ccTouchEnded");
    CCPoint touchPoint = pTouch->getLocation();
    
    if (m_nTouchTag == TouchRect::SearchTouchTag(touchPoint, m_cTouches, &m_pTouchSprite))
    {
        
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
                break;
            default:
                Utility::handleBtnCallBack(m_pTouchSprite, this, NULL);
                break;
        }
        handlerTouch();
        
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            if (CPtTool::isInNode(m_pBtn[i], pTouch) && m_nTouchTag == 3001+i)
            {
                handlerTouch();
                break;
            }
        }
        
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
                m_pBtn[m_nTouchTag-3001]->setNormal();
                break;
            default:
                break;
        }

    }
    
    
}

void CExploration::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
   
}


// protectd method:

void CExploration::initData()
{
    m_bLoadTaskInfo = true;
    m_bCanTouch = true;
    for (int i = 0; i < 3; i++)
    {
        m_pBtn[i] = NULL;
        m_aEvents[i] = NULL;
    }
    m_pTouchSprite = NULL;
    m_pProgress = NULL;

    
    m_pPlayer = SinglePlayer::instance();
    m_pEventData = SingleEventDataConfig::instance();
    m_pEventBoxData = SingleEventBoxes::instance();
    m_pTriggers = CCArray::create();
    m_pTriggers->retain();
    getTriggers();
}

void CExploration::getTriggers()
{
    CPtTriggerConfigData * triggerData =  SingleTriggerConfig::instance();
    const vector<int>& triggerIds = s_SectionData.sectionInfo->getTriggers();
    CPtTrigger * trigger = NULL;
    for (int i = 0; i < triggerIds.size(); i++)
    {
        trigger =  triggerData->getTriggerById(triggerIds.at(i));
        m_pTriggers->addObject(trigger);
    }
}

bool CExploration::initExploration()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    bool bRet = false;
    do {
        // init data:
        m_nTouchTag = -1;
        
        // init map:
        m_cMaps = LayoutLayer::create();
        m_cMaps->retain();
        m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "exploration.plist"));
        
        CCNode *node = m_cMaps->getElementByTags("0");
        if (node) {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSprite *sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "ironforest.png"));
            sprite->setPosition(ccp(size.width/2, size.height/2));
            node->addChild(sprite, -1);
        }
        
        // add the bg,maybe it will be separate to a layer
        
        node = NULL;
        node  = m_cMaps->getElementByTags("0,0,143");
        if (node) {
            node->setVisible(false);
            
            
        }
        
        
        
        // add buttons:
        CCSprite *btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        btnActivity->setAnchorPoint(CCPointZero);
        btnActivity->setPosition(ccp(850, 680));
        this->addChild(btnActivity, 200, 2001);
        Utility::addTouchRect(2001, btnActivity, m_cTouches);
        
        string word = Utility::getWordWithFile("word.plist", "option");
        CCLog("word :%s",word.c_str());
		CCLabelTTF* pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setPosition(ccp(65,30));
        btnActivity->addChild(pLabel);
        
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        btnActivity->setAnchorPoint(CCPointZero);
        btnActivity->setPosition(ccp(720, 680));
        this->addChild(btnActivity, 200, 600);
        Utility::addTouchRect(2002, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "email");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setPosition(ccp(65,30));
        btnActivity->addChild(pLabel);
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        btnActivity->setAnchorPoint(CCPointZero);
        btnActivity->setPosition(ccp(590, 680));
        this->addChild(btnActivity, 200, 2003);
        Utility::addTouchRect(2003, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "activity");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setPosition(ccp(65,30));
        btnActivity->addChild(pLabel);
        
        // bottom buttons:
        
        char buffer[10] = {0};
        int maxStep = s_SectionData.sectionInfo->getMaxStep();
        CCLog("");
        int currentStep = getCurrentStep();
        currentStep = currentStep >= maxStep ? maxStep : currentStep;
        CCLog("%d, %d", currentStep, maxStep);
        sprintf(buffer, "%d/%d",currentStep, maxStep);
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "mission.png"));
        btnActivity->setPosition(ccp(size.width/2, btnActivity->getContentSize().height/2 +15.0f));
        pLabel = CCLabelTTF::create(buffer, "Scissor Cuts", 20);
        pLabel->setPosition(ccp(150,60));
        btnActivity->addChild(pLabel);
        this->addChild(btnActivity, 200, 2004);
        Utility::addTouchRect(2004, btnActivity, m_cTouches);
        
        m_pProgress = pLabel;
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        btnActivity->setPosition(ccp(size.width-230, btnActivity->getContentSize().height/2 +15.0f));
        this->addChild(btnActivity, 200, 2005);
        Utility::addTouchRect(2005, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "voltron");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setPosition(ccp(65,30));
        btnActivity->addChild(pLabel);
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        btnActivity->setPosition(ccp(size.width-100, btnActivity->getContentSize().height/2 +15.0f));
        this->addChild(btnActivity, 200, 2006);
        Utility::addTouchRect(2006, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "packsack");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setPosition(ccp(65,30));
        btnActivity->addChild(pLabel);
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button2.png"));
        
        btnActivity->setPosition(ccp(330, btnActivity->getContentSize().height/2+25.0f ));
        this->addChild(btnActivity, 200, 2007);
        Utility::addTouchRect(2007, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "indeterminate");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setDimensions(CCSizeMake(1, 100));
        pLabel->setPosition(ccp(25,18));
        btnActivity->addChild(pLabel);
        
        
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button2.png"));
        btnActivity->setPosition(ccp(695, btnActivity->getContentSize().height/2+25.0f ));
        this->addChild(btnActivity, 200, 2008);
        Utility::addTouchRect(2008, btnActivity, m_cTouches);
        
        word = Utility::getWordWithFile("word.plist", "back");
        CCLog("word :%s",word.c_str());
		pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
        pLabel->setDimensions(CCSizeMake(1, 100));
        pLabel->setPosition(ccp(25,18));
        btnActivity->addChild(pLabel);
        
        CCSprite * card;
        // add card:
        for (int i = 0; i < 5; i++)
        {
            card = NULL;
            card = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "d_card.png"));
            if (card)
            {
                card->setAnchorPoint(CCPointZero);
                card->setPosition(ccp(160-i*30, 20));
                card->setScale(0.35f);
                this->addChild(card, 200, 30+i);
            }
            
        }

        
        // add CPtButton:
        CCLayer * outLayer = CCLayer::create();
        addChild(outLayer, 10, 100);
        
        for (int i = 0; i < 3; i++)
        {
            m_pBtn[i] = CPtButtonWidget::create("");
            m_pBtn[i]->setAnchorPoint(CCPointZero);
            m_pBtn[i]->setPosition(ccp(120+290*i, 370));
            outLayer->addChild(m_pBtn[i], 3001+i, 200);
           // Utility::addTouchRect(3001+i, m_pBtn[i], m_cTouches);
        }
       
        
        
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(-1);

        
        // init arrows:
        
      //  randonArrows(s_SectionData.currentStep);
        updateEventData();
        initEvent();
        updateBtn();
        
        createOrUpdatePlayerData();
       // handlerTrigger();
        scheduleOnce(schedule_selector(CExploration::goSection),1.0f);
        bRet = true;
    } while (0);
    
    return bRet;
}


void CExploration:: initEvent()
{
    char buffer[50] = {};
    
    int wordInex = 0;
    srand(time(0));
    
    for (int i = 0; i < 3; i++)
    {
        m_pBtn[i]->setType(rand()%4);
        wordInex = (rand()%30)+1;
        sprintf(buffer, "%d", wordInex);
        m_pBtn[i]->setText(Utility::getWordWithFile("tips.plist", buffer).c_str());
    }
        
        
      

}

void CExploration::handlerTouch()
{
    
    if (m_nTouchTag > 3000) {
        m_pBtn[m_nTouchTag-3001]->setEnd();
    }
    switch (m_nTouchTag) {
        case LEFT_TOUCH_TAG:
            CCLog("CExploration:: left");
            m_nWhichEvent = 0;            // to do:
            m_pBtn[m_nTouchTag-3001]->setPress();
            handlerEvent();
            break;
        case CENTER_TOUCH_TAG:
            m_nWhichEvent = 1;
            CCLog("CExploration:: center");
            // to do:
            m_pBtn[m_nTouchTag-3001]->setPress();
            handlerEvent();
            break;
            
        case RIGHT_TOUCH_TAG:
            CCLog("CExploration:: right");
            m_pBtn[m_nTouchTag-3001]->setPress();
            m_nWhichEvent = 2;
            handlerEvent();
            // to do:
            break;
            
        case 2008:
            //CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CHallScene::scene()));
            onSaveSectionProgress();
            
            break;
        case 2001:
             SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_FIGHTSCENE);
            break;
        default:
            break;
    }
}

/*
 * @note: event type: 0-7:
 * 0:空事件 1:普通战斗 2: 精英战斗 3: Boss战斗 4: 商人 5: 拥兵 6:宝箱，陷阱 7: 神坛
 */

void CExploration::handlerEvent()
{
    if (m_nWhichEvent>= 0 || m_nWhichEvent < 3)
    {
        m_nEventType= m_aEvents[m_nWhichEvent]->getEventType(); // s_SectionData.eventData.eventId[m_nWhichEvent];
        setCurrentEvnetId(m_aEvents[m_nWhichEvent]->getEventId());
        CCLog("event type: %d", m_nEventType);

        onSendEventRequest();
    }
    
}

void CExploration::handlerSuccess()
{
    // add 
    s_SectionData.currentStep++;
//    s_SectionData.currentStep >= s_SectionData.sectionInfo->getMaxStep()? s_SectionData.sectionInfo->getMaxStep() : s_SectionData.currentStep;
    updateUI();
}

void CExploration::handlerTrigger()
{
    if (m_pTriggers)
    {
        CPtTrigger *trigger = NULL;
        for (int i = 0; i < m_pTriggers->count(); i++)
        {
            trigger = (CPtTrigger*)m_pTriggers->objectAtIndex(i);
            if (trigger && trigger->getTriggerStep() == s_SectionData.currentStep)
            {
                CGameStoryLayer::CreateStoryLayer(trigger->getStoryId(), this);// CCDirector::sharedDirector()->getRunningScene());
                break;
            }
        }
     
    }
}


void CExploration::createOrUpdatePlayerData()
{
    
    //HP，体力，金币，现金币，领导力，等级
    //username
    CCSize wndSize=CCDirector::sharedDirector()->getWinSize();
    if (!getChildByTag(1000001))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 20);
        addChild(labelttf,0,1000001);
        labelttf->setPosition(ccp(50, 200));
        labelttf->setString(CCUserDefault::sharedUserDefault()->getStringForKey("name").c_str());
    }
    char data[30];
    // usercoin 金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000002);
        if (!getChildByTag(1000002))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000002);
            labelttf->setPosition(ccp(200, 200));
        }
        sprintf(data, "金币:%d",SinglePlayer::instance()->getCoin());
        labelttf->setString(data);
       
    }
    //user exp;现金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000003);
        if (!getChildByTag(1000003))
        {
           labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000003);
            labelttf->setPosition(ccp(350, 200));
        }
        sprintf(data, "现金:%d",SinglePlayer::instance()->getPlayerPrice());
        labelttf->setString(data);
    }
    
    //user 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000004);
        if (!getChildByTag(1000004))
        {
           labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000004);
            labelttf->setPosition(ccp(500, 200));
        }
        sprintf(data, "体力:%d",SinglePlayer::instance()->getPlayerEnergy());
        labelttf->setString(data);
    }
    
    //user 领导力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000005);
        if (!getChildByTag(1000005))
        {
           labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000005);
            labelttf->setPosition(ccp(650, 200));
        }
        sprintf(data, "领导力:%d",SinglePlayer::instance()->getRVC());
        labelttf->setString(data);
    }
    // 等级
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000006);
        if (!getChildByTag(1000006))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000006);
            labelttf->setPosition(ccp(800, 200));
        }
        sprintf(data, "等级:%d",SinglePlayer::instance()->getPlayerLevel());
        labelttf->setString(data);
    }
    
    // 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000007);
        if (!getChildByTag(1000007))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,1,1000007);
            labelttf->setPosition(ccp(900, 200));
        }
        sprintf(data, "神力:%d",SinglePlayer::instance()->getPlayerHp());
        labelttf->setString(data);
    }
    
}


void CExploration::attachConfirm()
{
    
    CCNode *node = NULL;
    node = getChildByTag(100);
    
    if(node)
    {
        node->removeFromParentAndCleanup(true);
        CConfirmLayer *layer = CConfirmLayer::create();
        this->addChild(layer, 5, 10);
    }
    
}

void CExploration::updateBtn()
{
    for (int i = 0; i < 3; i++)
    {
        if (m_aEvents[i])
        {
            m_pBtn[i]->setVisible(true);
        }else
        {
            m_pBtn[i]->setVisible(false);
        }
    }
}

// ui:

void CExploration::updateUI()
{
    int maxStep = s_SectionData.sectionInfo->getMaxStep();
    CCLog("");
    int currentStep = getCurrentStep();
    currentStep = currentStep >= maxStep ? maxStep : currentStep;
    CCLog("%d, %d", currentStep, maxStep);
    char buffer[10] = {0};
    sprintf(buffer, "%d/%d",currentStep, maxStep);
    m_pProgress->setString(buffer);
    handlerTrigger();
}


// connect server:

// api.php?m=Part&a=recordPart&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)&item_id=1(道具ID)
void CExploration::onSaveSectionProgress()
{
    m_bCanTouch = false;
    int propId = 20001;
    char buffer[200]={0};
    CPtSection*  m_pSection = s_SectionData.sectionInfo;
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&item_id=%d",STR_USER_SIG, m_pSection->getChapterId(), m_pSection->getSectionId(), propId);
    
    ADDHTTPREQUESTPOSTDATA(STR_URL_SAVE_PROGRESS(196),"saveProgress", "saveProgress",buffer, callfuncO_selector(CExploration::onReceiveSaveMsg));

    
}
void CExploration::onReceiveSaveMsg(CCObject *pObject)
{
    m_bCanTouch = true;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "saveProgress");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCDictionary *tmp = PtJsonUtility::JsonStringParse(buffer);
        if (tmp)
        {
            onParseSaveMsgByDictionary(tmp);
        }else
        {
            backHall();
        }
        
    }else
    {
        CCLog("error: cann't connect server");
        backHall();
    }
    

    
}
void CExploration::onParseSaveMsgByDictionary(CCDictionary *pResultDict)
{
    int result = GameTools::intForKey("code", pResultDict);
    if (result == 0)
    {
        //success:
       
        
    }else
    {
        CCLog("error: code: %d", result);
    }
     backHall();
}


//cube.games.com/api.php?m=Part&a=finishEvent&uid=194&sig=2ac2b1e302c46976beaab20a68ef95&chapter_id=1&part_id=1&step=1&event_id=1&type=1&task_id=300001
void CExploration::onFishEventRequest(int inType)
{
    int taskId = s_SectionData.sectionInfo->getTaskId();
    m_bCanTouch = false;
    char buffer[200]={0};
    CPtSection*  m_pSection = s_SectionData.sectionInfo;
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&step=%d&event_id=%d&type=%d&task_id=%d",STR_USER_SIG, m_pSection->getChapterId(), m_pSection->getSectionId(), getCurrentStep()-1, getCurrentEventId(),inType,taskId);
    
    ADDHTTPREQUESTPOSTDATA(STR_URL_FINISH_EVENT(196),"finishEvent", "finishEvent",buffer, callfuncO_selector(CExploration::onReceiveFishEventRequestMsg));
    
}
void CExploration::onReceiveFishEventRequestMsg(CCObject * pObject)
{
    m_bCanTouch = true;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "finishEvent");
    char *buffer = (char *)pObject;
    if (buffer)
    {
        CCLog("CExploration::onReceiveFishEventRequestMsg--->%s", buffer);
        CCDictionary *tmpDict = PtJsonUtility::JsonStringParse(buffer);
        onParseFishEventRequestMsg(tmpDict);
        delete [] buffer;
    }else
    {
        CCLog("server: error!---");
    }
   
    
}
void CExploration::onParseFishEventRequestMsg(CCDictionary * pResultDict)
{
    if (pResultDict)
    {
        int code = GameTools::intForKey("code", pResultDict);
        if (code == 0)
        {
            CCLog("success");
            CCDictionary *tmp =(CCDictionary*) pResultDict->objectForKey("result");
            dispatchParaseFinishEvent(tmp, m_nEventType);
        }
        else
        {
            CCLog("result code: %d", code);
        }
    }
}

/*
 * @param: inType:
 */

void CExploration::dispatchParaseFinishEvent(CCDictionary *pResult, int inType)
{
    CCAssert(pResult, "result null");
    
    int nexstep = GameTools::intForKey("next_step", pResult);
    setCurrentStep(nexstep);

    
    
    
    CCDictionary *eventInfo =(CCDictionary*) pResult->objectForKey("event_info");
    if (eventInfo)
    {
         setNextEventByDict(eventInfo);
         updateEventData();
         updateBtn();
    }else
    {
        EVENTDATA eventData;
        setEvents(eventData);
    }
    if (inType == 6)
    {
        CEventBoxData * inEvenBoxData = m_pEventBoxData->getEventBoxDataById(getCurrentEventId());
        CCLog("the currenEventId %d", getCurrentEventId());
        if (inEvenBoxData&&inEvenBoxData->getBoxType() == 0)
        {
            if (m_nEventBoxSelectType == 0)
            {
                CEventBoxRewordLayer* layer = CEventBoxRewordLayer::create(inEvenBoxData, 0);
                layer->setHanlder(this, callfuncO_selector(CExploration::onCanfirmCallback));
                setInsiable();
                addChild(layer);
                return;
            }else
            {
                
            }
            
        }
        else
        {
            CEventBoxRewordLayer* layer = CEventBoxRewordLayer::create(inEvenBoxData, 1);
            layer->setHanlder(this, callfuncO_selector(CExploration::onCanfirmCallback));
            setInsiable();
            addChild(layer);
            return;
        }
       
    }
    
    getBiforest();
}

/*
 *  1 - 7:
 */
void CExploration::dispatchParaseEvent(CCDictionary *pEventInfo, int inType)
{
    char buffer[20] = {0};
    sprintf(buffer, "%d", getCurrentEventId());

    if (inType == 6)
    {
        //宝箱：
        int eventBoxId = GameTools::intForKey(buffer, pEventInfo);
        CCLog("the eventBoxId-- %d", eventBoxId);
        hanlderEventBox(eventBoxId);
        
    }else if(inType >= 1 && inType <= 3)
    {
        // fight
        CCDictionary * tmp  = (CCDictionary*) pEventInfo->objectForKey(buffer);
        m_pPlayer->onFightInterScene();
        m_pPlayer->parseNpcCard(tmp);
        handlerFightEvent(inType);

    }else if(inType == 0)
    {
        handlerEmptyEvent();
    }
    
}

// http://cube.games.com/api.php?m=Part&a=startEvent&uid=194&sig=2ac2b1e302c46976beaab20a68ef95&chapter_id=1&part_id=1&step=1&event_id=1 go event
void CExploration::onSendEventRequest()
{
    // constructor post data:
    CPtSection *m_pSection = s_SectionData.sectionInfo;
    
    char buffer[200]={0};
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&step=%d&event_id=%d",STR_USER_SIG, m_pSection->getChapterId(), m_pSection->getSectionId(),  s_SectionData.currentStep, getCurrentEventId());

    ADDHTTPREQUESTPOSTDATA(STR_URL_NEXT_EVENT(196),"nextEvent", "nextEvent",buffer, callfuncO_selector(CExploration::onReceiveEventRequetMsg));
    m_bCanTouch =false;
}
void CExploration::onReceiveEventRequetMsg(CCObject *pObject)
{
    m_bCanTouch = true;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "nextEvent");
    char *buffer = (char*) pObject;
    
    CCLog("buffer: %s", buffer);
    if (buffer)
    {
        CCDictionary *tmp = PtJsonUtility::JsonStringParse(buffer);
        if (tmp)
        {
            handlerSuccess();
            onParseEventRequestMsg(tmp);
        }
      
    }else
    {
        CCLog("error: cann't connect server");
    }
    
}
void CExploration::onParseEventRequestMsg(CCDictionary *pResultDict)
{
    int result = GameTools::intForKey("code", pResultDict);
    if (result == 0)
    {
        CCDictionary * tmp = (CCDictionary*) pResultDict->objectForKey("result");
        CCAssert(tmp, "result not null");
        tmp = (CCDictionary*) tmp->objectForKey("event_info");
        dispatchParaseEvent(tmp, m_nEventType);
    }else
    {
        CCLog("error: code: %d", result);
    }
}




void CExploration::backHall()
{
     SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
}

void CExploration::onReceiveTaskInfo(CCObject *pObject)
{
    m_bLoadTaskInfo = true;
    char *buff = (char*) pObject;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "getNextTaskInfo");
    if (buff)
    {
        CCDictionary * tmp = PtJsonUtility::JsonStringParse(buff);
        SinglePlayer::instance()->onParseTaskInfoByDictionary(tmp);
        delete [] buff;
    }
    SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE, 1);



    
}
void CExploration::hanlderLoadTaskInfo()
{
    m_bLoadTaskInfo = false;
    char buffer[200]={0}; 
    sprintf(buffer, "sig=%s",STR_USER_SIG);
    
    CCLog("%s", buffer);
    ADDHTTPREQUESTPOSTDATA(STR_URL_TASK(196),"getNextTaskInfo", "getNextTaskInfo",buffer, callfuncO_selector(CExploration::onReceiveTaskInfo));

}

void CExploration::callBack(float dt)
{
    hanlderLoadTaskInfo();
}


void CExploration::addForwordReword()
{
    
    
}

void CExploration::requestCallBack(float dt)
{
    onFishEventRequest(1);
}

void CExploration::goSection(float dt)
{
    getBiforest();
}
/*
 * @brief: 分发事件
 * @param inEventId:
 *
 */
void CExploration::dispatchEventByEventId(const int &inEventId)
{

}

void CExploration::updateEventData()
{
    for (int i = 0 ; i < 3 ; i++)
    {
        if (s_SectionData.eventData.eventId[i] == 0)
        {
            m_aEvents[i] = NULL;
        }else
        {
            m_aEvents[i] = m_pEventData->getEventById(s_SectionData.eventData.eventId[i]);
        }
    }
}

void CExploration::handlerEmptyEvent()
{
    onFishEventRequest(1);
}
/*
 * @param inType: 1 - 3:
 *
 */

void CExploration::handlerFightEvent(int inType)
{
    m_bCanTouch = false;
    attachConfirm();
    
}

/*
 * 
 */
void CExploration::hanlderEventBox(int inEventBoxId)
{
    CEventBoxData * eventBoxData = m_pEventBoxData->getEventBoxDataById(inEventBoxId);
    CCAssert(eventBoxData, "assert ...");
    createEventBoxDialogByType(eventBoxData ,eventBoxData->getBoxType());
    CCLog("the eventBox type : %d ", eventBoxData->getBoxType());
}

/*
 * @param inType 1-4
 */

void CExploration::createEventBoxDialogByType(int inEventBoxId, int inType)
{
    CCLayer *layer = NULL;
    if (inType == 1)
    {
        layer = CEventBoxLayer::create(inEventBoxId);
    }else
    {
        scheduleOnce(schedule_selector(CExploration::requestCallBack),0.0f);
    }
}

void CExploration::createEventBoxDialogByType(CEventBoxData *inEventBoxData, int inType)
{
  
    if (inType == 1)
    {
        CEventBoxLayer* layer = CEventBoxLayer::create(inEventBoxData);
        layer->setHanlder(this, callfuncO_selector(CExploration::onOpenCallBack), callfuncO_selector(CExploration::onCancleCallback));
        setInsiable();
        addChild(layer);
    }else
    {
       scheduleOnce(schedule_selector(CExploration::requestCallBack),0.0f);
    }
    
}

void CExploration::setVisiable()
{
    CCNode *node = getChildByTag(100);
    if (node)
    {
        node->setVisible(true);
    }
}
void CExploration::setInsiable()
{
    CCNode *node = getChildByTag(100);
    if (node)
    {
        node->setVisible(false);
    }
}

void CExploration:: onCancleCallback(CCObject *pObject)
{
    CCNode * node = (CCNode*) pObject;
    node->removeFromParentAndCleanup(true);
    setVisiable();
    m_nEventBoxSelectType = 0;
    onFishEventRequest(0);
    
}
void CExploration:: onCanfirmCallback(CCObject *pObject)
{
    CCNode * node = (CCNode*) pObject;
    int type = (int)node->getUserData();
    node->removeFromParentAndCleanup(true);
    setVisiable();
    getBiforest();
}

void CExploration::onOpenCallBack(CCObject* pObject)
{
    CCNode * node = (CCNode*) pObject;
    node->removeFromParentAndCleanup(true);
    setVisiable();
    m_nEventBoxSelectType = 1;
    onFishEventRequest(1);
    
}

void CExploration::getBiforest()
{
    if (s_SectionData.currentStep > s_SectionData.sectionInfo->getMaxStep())
    {
        scheduleOnce(schedule_selector(CExploration::callBack),0.0f);
        return;
    }
    
}