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
#include "gameMiddle.h"
#include "CBackpackContainerLayer.h"


int g_index = -1;

SECTION_DATA CExploration::s_SectionData ;

int CExploration::s_CurrentEventId = -1;

CCDictionary * CExploration::s_Rewards = NULL;

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

/*
 * @param inReward: --- key reward
 * @return allRewards: it create retain is 1 and add to autorelease:
 *
 */

CCDictionary* CExploration::setRewardsByDict(CCDictionary *inReward)
{
    CCDictionary* array = CCDictionary::create();
    CCDictionary * addDict = NULL;
    CCDictionary * decDict = NULL;
    CReward * reward = NULL;
    
    CReward * taskReward =  NULL;
    CReward * sectionReward = NULL;
    if (inReward)
    {
        addDict = (CCDictionary*) inReward->objectForKey("add");
        decDict = (CCDictionary*) inReward->objectForKey("dec");
        
        if (addDict)
        {
            // add
            CCDictionary * tmp = (CCDictionary*) addDict->objectForKey("event");
            if (tmp)
            {
                // event reward:
                reward = CReward::create(tmp);
                array->setObject(reward, EVENTREWARD_ADD);

            }
            tmp =  (CCDictionary*) addDict->objectForKey("chapter");
            if (tmp)
            {
                // chpater reward:
                reward = CReward::create(tmp);
                array->setObject(reward, CHAPTERREWARD_ADD);
                setChapterReward(reward);
            }
            
            tmp = (CCDictionary*) addDict->objectForKey("part");
            if (tmp)
            {
                // part reward:
                reward = CReward::create(tmp);
                array->setObject(reward, SECTIONREWARD_ADD);
                sectionReward = reward;
            }
            
            tmp = (CCDictionary*) addDict->objectForKey("task");
            
            if (tmp)
            {
                // task reward:
                reward = CReward::create(tmp);
                array->setObject(reward, TASKREWARD_ADD);
                taskReward = reward;
    
            }

            
            tmp = (CCDictionary*) addDict->objectForKey("step");
            if (tmp)
            {
                //step reward:
                reward = CReward::create(tmp);
                array->setObject(reward, FORWARDREWARD_ADD);
                
            }
            
            if (sectionReward || taskReward)
            {
                CReward * task = CReward::create(sectionReward, taskReward);
                setTaskAndSectionReward(task);
            }

            
            
        }
        if(decDict)
        {
            //dec

            
            CCDictionary * tmp = (CCDictionary*) decDict->objectForKey("event");
            if (tmp)
            {
                // event reward:
                reward = CReward::create(tmp);
                array->setObject(reward, EVENTREWARD_DEC);
            }
            
            tmp =  (CCDictionary*) decDict->objectForKey("chapter");
            if (tmp)
            {
                // chpater reward:
                reward = CReward::create(tmp);
                array->setObject(reward, CHAPTERREWARD_DEC);
            }
            
            tmp = (CCDictionary*) decDict->objectForKey("part");
            if (tmp)
            {
                // part reward:
                reward = CReward::create(tmp);
                array->setObject(reward, SECTIONREWARD_DEC);
            }
            
            tmp = (CCDictionary*) decDict->objectForKey("task");
            if (tmp)
            {
                // task reward:
                reward = CReward::create(tmp);
                array->setObject(reward, TASKREWARD_DEC);
                
            }
            
            
            tmp = (CCDictionary*) decDict->objectForKey("step");
            if (tmp)
            {
                //step reward:
                reward = CReward::create(tmp);
                array->setObject(reward, FORWARDREWARD_DEC);
                
            }
            
           
            
        }
        
    }
    
    return array;
    
}


/*
 * @return null 没有章奖励
 */
CReward * CExploration::getChapterReward()
{
    CReward * reward  = NULL;
    if (s_Rewards)
    {
        reward = (CReward*)(s_Rewards->objectForKey(CHAPTERREWARD_ADD));
        // delete chapter reward from reward deque:
        if (reward)
        {
            s_Rewards->removeObjectForKey(CHAPTERREWARD_ADD);
        }
    }
    return reward;
    
}

/*
 * @return null 没有节和任务奖励
 */
CReward *CExploration::getTaskAndSectionReward()
{
    CReward * reward  = NULL;
    if (s_Rewards)
    {
        reward = (CReward*)(s_Rewards->objectForKey(SECTIONREWARD_ADD));
        // delete section and task rewards from reward deque:
        if (reward)
        {
            s_Rewards->removeObjectForKey(SECTIONREWARD_ADD);
        }
    }
    return reward;
}


void CExploration::setChapterReward(CReward * inChapterReward)
{
    if (s_Rewards == NULL)
    {
        s_Rewards = CCDictionary::create();
        s_Rewards->retain();
    }
    
    s_Rewards->setObject(inChapterReward, CHAPTERREWARD_ADD);
    
}
void CExploration::setTaskAndSectionReward(CReward *inTaskAndSectionReward)
{
    if (s_Rewards == NULL)
    {
        s_Rewards = CCDictionary::create();
        s_Rewards->retain();
    }
    s_Rewards->setObject(inTaskAndSectionReward, SECTIONREWARD_ADD);
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
        currentStep--;
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
        }
       
        
        
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(-1);

        
        // init arrows:
        
        updateEventData();
        initEvent();
        updateBtn();
        
        createOrUpdatePlayerData();
        addTaskAndSectionReward();
        getBiforest();
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
            
        case 2005:
            // card setting:
            showCardSetting();
            break;
        case 2006:
            // show backpack;
            CCLog("1--------");
            showBackPack();
            break;
        case 2008:
            onSaveSectionProgress();
            
            break;
        case 2001:
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
    updateUI();
}
void CExploration::handlerTrigger()
{
    if (getCurrentTaskId()== m_pPlayer->getCurrentTaskId() && s_SectionData.eventData.type == 1 && s_SectionData.eventData.storyId > 0)
    {
        CGameStoryLayer::CreateStoryLayer(s_SectionData.eventData.storyId, this);
    }

}


void CExploration::createOrUpdatePlayerData()
{
    
    //HP，体力，金币，现金币，领导力，等级
    //username 180
    CCSize wndSize=CCDirector::sharedDirector()->getWinSize();
    if (!getChildByTag(1000001))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 20);
        addChild(labelttf,0,1000001);
        labelttf->setPosition(ccp(350, 140));
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
            labelttf->setPosition(ccp(450, 140));
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
            labelttf->setPosition(ccp(350, 180));
        }
        sprintf(data, "现金:%d",SinglePlayer::instance()->getPlayerCash());
        labelttf->setString(data);
    }
    
    //user 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000004);
        if (!getChildByTag(1000004))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000004);
            labelttf->setPosition(ccp(500, 180));
        }
        sprintf(data, "体力:%d",SinglePlayer::instance()->getPlayerAp());
        labelttf->setString(data);
    }
    
    //user 领导力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000005);
        if (!getChildByTag(1000005))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000005);
            labelttf->setPosition(ccp(650, 180));
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
            labelttf->setPosition(ccp(550, 140));
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
            addChild(labelttf,0,1000007);
            labelttf->setPosition(ccp(650, 140));
        }
        sprintf(data, "神力:%d",SinglePlayer::instance()->getPlayerGp());
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
    currentStep--;
    currentStep = currentStep >= maxStep ? maxStep : currentStep;
    CCLog("%d, %d", currentStep, maxStep);
    char buffer[10] = {0};
    sprintf(buffer, "%d/%d",currentStep, maxStep);
    m_pProgress->setString(buffer);
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
        test_print(result);
    }
     backHall();
}


//cube.games.com/api.php?m=Part&a=finishEvent&uid=194&sig=2ac2b1e302c46976beaab20a68ef95&chapter_id=1&part_id=1&step=1&event_id=1&type=1&task_id=300001
void CExploration::onFishEventRequest(int inType)
{
    int taskId = m_pPlayer->getCurrentTaskId() == s_SectionData.sectionInfo->getTaskId() ? s_SectionData
    .sectionInfo->getTaskId() : 0;
    int evenId = getCurrentEventId();
    if (evenId == -1)
    {
        test_print("event id error");
        return;
    }
    m_bCanTouch = false;
    char buffer[200]={0};
    CPtSection*  m_pSection = s_SectionData.sectionInfo;
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&step=%d&event_id=%d&type=%d&task_id=%d",STR_USER_SIG, m_pSection->getChapterId(), m_pSection->getSectionId(), getCurrentStep()-1, evenId,inType,taskId);
    
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
            test_print(code);
        }
    }
}

/*
 * @param: inType: event type
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
        if (nexstep != 0)
        {
            test_print("finish event: eventinfo null");
        }
        EVENTDATA eventData;
        setEvents(eventData);
    }
    if (inType == 6)
    {
    
        CCDictionary * inRewards = (CCDictionary*) pResult->objectForKey("reward");
        if (inRewards)
        {
            inRewards =  setRewardsByDict(inRewards);
            addEventReward(inRewards);
        }
        else
        {
            CCLog("event reward : null");
        }
        CEventBoxData * inEvenBoxData = m_pEventBoxData->getEventBoxDataById(m_nCurrentEventBoxId);
        CCLog("the currenEventId %d", m_nCurrentEventBoxId);
        if (inEvenBoxData&&inEvenBoxData->getBoxType() == 1)
        {
            if (m_nEventBoxSelectType == EVENT_SUCCESS)
            {
                CEventBoxRewordLayer* layer = CEventBoxRewordLayer::create(inEvenBoxData, GET_BOX);
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
            if (inEvenBoxData == NULL)
            {
                test_print("eventbox id error:");
                return;
            }
            CEventBoxRewordLayer* layer = CEventBoxRewordLayer::create(inEvenBoxData);
            layer->setHanlder(this, callfuncO_selector(CExploration::onCanfirmCallback));
            setInsiable();
            addChild(layer);
            return;
        }
       
    }else if(inType == 0)
    {
        // create empty:
        CEventBoxRewordLayer* layer = CEventBoxRewordLayer::create(NULL, EMPTY_EVENT);
        layer->setHanlder(this, callfuncO_selector(CExploration::onCanfirmCallback));
        setInsiable();
        addChild(layer);
    }
   
    addTaskAndSectionReward();
    getBiforest();
}

/*
 *  1 - 7:
 */
void CExploration::dispatchParaseEvent(CCDictionary *pEventInfo, int inType)
{
    if(pEventInfo == NULL)
    {
        test_print("event_info null");
        return;
    }


    char buffer[20] = {0};
    sprintf(buffer, "%d", getCurrentEventId());

    if (inType == 6)
    {
        //宝箱：
        int eventBoxId = GameTools::intForKey(buffer, pEventInfo);
        m_nCurrentEventBoxId = eventBoxId;
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
    if (m_pPlayer->getPlayerAp() < m_pSection->getAP() || m_pPlayer->getPlayerGp() <= 0)
    {
        CCLog("体力or神力不够");
        Middle::showAlertView("体力or神力不够");
        return;
    }
    
    //
    int eventId = getCurrentEventId();
    if (eventId == -1)
    {
        test_print("event Id  == fail");
    }
    
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
        handlerSuccess();
        CCDictionary * result = (CCDictionary*) pResultDict->objectForKey("result");
        CCAssert(result, "result not null");
        CCDictionary* tmp = (CCDictionary*) result->objectForKey("reward");
        CCDictionary *reward = setRewardsByDict(tmp);
        addForwordReword(reward);
        tmp = (CCDictionary*) result->objectForKey("event_info");
        dispatchParaseEvent(tmp, m_nEventType);
    }else
    {
        CCLog("error: code: %d", result);
        test_print(result);
    }
}




void CExploration::backHall()
{
     SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
}




void CExploration::addForwordReword(CCDictionary * inAllRewards)
{
    if (inAllRewards)
    {
        CReward * reward = (CReward*) inAllRewards->objectForKey(FORWARDREWARD_ADD);
        if(reward)
        {
            reward->excuteReward(ADD);
        }
        reward = (CReward*) inAllRewards->objectForKey(FORWARDREWARD_DEC);
        if (reward)
        {
            reward->excuteReward(DEC);
        }
        
    }
    createOrUpdatePlayerData();
    
}

void CExploration::addEventReward(CCDictionary *inAllRewards)
{
    if (inAllRewards)
    {
        CReward * reward = (CReward*) inAllRewards->objectForKey(EVENTREWARD_ADD);
        if(reward)
        {
            reward->excuteReward(ADD);
        }
        reward = (CReward*) inAllRewards->objectForKey(EVENTREWARD_DEC);
        if (reward)
        {
            reward->excuteReward(DEC);
        }
        
    }
    createOrUpdatePlayerData();
}

void CExploration::addTaskAndSectionReward()
{
   CReward * taskAndSection =  getTaskAndSectionReward();
   if (taskAndSection)
    {
        char tips[200] = {0};
        taskAndSection->excuteReward(ADD);
        sprintf(tips, "task or section reward add: ap: %d, gp: %d \n     exp:%d, coin: %d, cash: %d\n card count: %d, prop count: %d\n", taskAndSection->getEnergy(), taskAndSection->getHP(), taskAndSection->getExp(),
                taskAndSection->getCoin(), taskAndSection->getCash(), taskAndSection->getCardCount(), taskAndSection->getPropCount());
        
        Middle::showAlertView(tips);
    }

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
    handlerTrigger();
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
    handlerTrigger();
}

/*
 * @param inType 1-4
 */

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
       //scheduleOnce(schedule_selector(CExploration::requestCallBack),0.0f);
       onFishEventRequest(EVENT_SUCCESS);
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
    m_nEventBoxSelectType = EVENT_FAILURE;
    onFishEventRequest(EVENT_FAILURE);
    
}
void CExploration:: onCanfirmCallback(CCObject *pObject)
{
    CCNode * node = (CCNode*) pObject;
    node->removeFromParentAndCleanup(true);
    setVisiable();
    addTaskAndSectionReward();
    getBiforest();
}

void CExploration::onOpenCallBack(CCObject* pObject)
{
    CEventBoxLayer * node = (CEventBoxLayer*) pObject;
    if (node)
    {
        if (node->getGPEnough())
        {
            m_nEventBoxSelectType = EVENT_SUCCESS;

        }else
        {
            m_nEventBoxSelectType = EVENT_FAILURE;
        }
       
        
    }
    node->removeFromParentAndCleanup(true);
    setVisiable();
    onFishEventRequest(m_nEventBoxSelectType);
    
}

void CExploration::getBiforest()
{
    if (s_SectionData.currentStep > s_SectionData.sectionInfo->getMaxStep())
    {
       // scheduleOnce(schedule_selector(CExploration::callBack),0.0f);
        if (getCurrentTaskId() == m_pPlayer->getCurrentTaskId())
        {
            CPtTask* task = SingleTaskConfig::instance()->getTaskById(getCurrentTaskId());
            CCAssert(task, "task is null");
            // type 1: 通关， 2: 杀怪 3: 寻物
            if(task->getTaskType() == 1)
            {
                m_pPlayer->addTaskOperator(1, NULL);
                if (m_pPlayer->isSuccessFinishTask())
                {
                    m_pPlayer->postCompleteTask(getCurrentTaskId(), this, callfuncO_selector(CExploration::taskCompleteCallback), "CALLBACK_CEXPLORATION_COMPLATETASK");
                    return;
                }
                
            }
           
        }
        SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE, 1);
        return;
    }
    
}

void CExploration::showBackPack()
{
    CCLayer * layer = CBackpackContainerLayer::create();
    addChild(layer, 1000);
    CCLog("backpack...");
}

void CExploration::showCardSetting()
{
    SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_CARDSETTINGSCENE, 1);
}

void CExploration::test_print(int code)
{
    char buffer[120] = {0};
    sprintf(buffer, "the error : code : %d", code);
    Middle::showAlertView(buffer);
    
}
void CExploration::test_print(const char * inMsg)
{
    Middle::showAlertView(inMsg);
}


void CExploration::taskCompleteCallback(CCObject* pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CEXPLORATION_COMPLATETASK");
    if (!pObject)
    {
        CCMessageBox("服务端传输NULL数据Complatetask", "ERROR");
    }
    //判断code 是否等于0  如果等于0 代表校验成功， 之后执行doin
    int codeValue=0;
    char *buff=(char *)pObject;
    CCDictionary *tmp= PtJsonUtility::JsonStringParse(buff);
    delete [] buff;
    
    if (tmp)
    {
        codeValue  = GameTools::intForKey("code", tmp);
    }
    if(!codeValue)
    {
        //获得info的答案。
        if (GameTools::intForKey("info", tmp)==0)
        {
            addTask();
            return;
        }else
        {
            CCLog("任务完成验证失败");
        }
    }else
    {
        CCLog("error code: %d", codeValue);
    }
  
    
}
void CExploration::taskAddCallback(CCObject* pObject)
{
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CEXPLORATION_ADDTASK");
    if (!pObject)
    {
        CCMessageBox("服务端传输NULL数据Complatetask", "ERROR");
    }
    int codeValue=0;
    char *buff=(char *)pObject;
    CCDictionary *tmp= PtJsonUtility::JsonStringParse(buff);
    delete [] buff;
    buff=NULL;
    if (tmp)
    {
        codeValue  = GameTools::intForKey("code", tmp);
    }
    
    if (!codeValue) {
        //获得info的答案。
        if (GameTools::intForKey("info", tmp)==0)
        {
            goSection();
            return;
        }
    }
    char messageData[100];
    sprintf(messageData, "服务端说不能添加本地的当前任务 出错CODE::%d",codeValue);
    CCMessageBox(messageData,"ALTER");
}

void CExploration::addTask()
{
    int value=m_pPlayer->getCurrentTaskId();
    CPtTask *pttask=SingleTaskConfig::instance()->getNextByPreTask(value);
    if (pttask)
    {
        m_pPlayer->setCurrentTaskId(pttask->getTaskId());
        m_pPlayer->postAddTask(m_pPlayer->getCurrentTaskId(), this, callfuncO_selector(CExploration::taskAddCallback), "CALLBACK_CEXPLORATION_ADDTASK");
    }
    else
    {
        goSection();
    }
   
}


void CExploration::goSection()
{
    scheduleOnce(schedule_selector(CExploration::callback), 1.0f);
}
void CExploration::callback(float dt)
{
    SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE, 1);
}