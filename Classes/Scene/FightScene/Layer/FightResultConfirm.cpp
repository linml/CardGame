//
//  FightResultConfirm.cpp
//  91.cube
//
//  Created by phileas on 13-6-17.
//
//

#include "FightResultConfirm.h"
#include "ExplorationScene.h"
#include "gameConfig.h"
#include "HallScene.h"
#include "ExplorationScene.h"
#include "SceneManager.h"
#include "gameMiddle.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"

FightResultConfirm::FightResultConfirm()
{

}

FightResultConfirm::~FightResultConfirm()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}


bool FightResultConfirm::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 125)));
        initFightResultConfirm();
        //添加一个战斗回放的效果
        CCSprite *sprite=CCSprite::create("Icon-Small@2x.png");
        if(getChildByTag(2))
        {
            getChildByTag(2)->addChild(sprite,1000,911);
        }
        sprite->setPosition(ccp(500,650));
        //添加一个战斗回放的效果
        bRet = true;
        int  value=(int)getUserData();
        if(value==1)
        {
             PtSoundTool::playSysSoundEffect("fight_win.mp3");
        }
        else{
             PtSoundTool::playSysSoundEffect("fight_failed.mp3");
        }
        
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(-INT_MAX);
        if(m_nResult->getHuiFang())
        {
            
            string word;
            if(m_nResult->getFightResult()==1)
            {
                word = Utility::getWordWithFile("word.plist", "win");
            }
            else
            {
                word = Utility::getWordWithFile("word.plist", "lose");
            }
            CCLabelTTF* pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(240,140));
            CCNode * node = m_cMaps->getElementByTags("2,0,0");
            if(node)
            {
                node->addChild(pLabel);
            }
            setTouchEnabled(true);
        }
        else
        {
            //setTouchEnabled(false);
            CCLabelTTF* pLabel = CCLabelTTF::create("loading...", "Scissor Cuts", 20);
            pLabel->setPosition(ccp(240,140));
            CCNode * node = m_cMaps->getElementByTags("2,0,0");
            if(node)
            {
                node->addChild(pLabel,1,912);
            }
          
            if (m_nResult->getFightType()==0)
            {
                postHttpNpc();
            }
            else{
                postHttpTeam();
            }
        }

    } while (0);
    return bRet;
}
void FightResultConfirm::postHttpTeam()
{
    callBackData(NULL);
}

void FightResultConfirm::postHttpNpc()
{
   
    
    int taskId =CExploration::getCurrentTaskId();
    int eventId=CExploration::getCurrentEventId(); //事件ID
    int chapterId=CExploration::getCurrentChapterId(); //章ID
    int sectionId=CExploration::getCurrentSectionId(); //节ID
    int stepNumber=CExploration::getCurrentStep();
    int typeSuccess=m_nResult->getFightResult();
    char buffer[200]={0};
    //    CPtSection*  m_pSection = s_SectionData.sectionInfo;
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&step=%d&event_id=%d&type=%d&task_id=%d",STR_USER_SIG, chapterId, sectionId, stepNumber-1, eventId,typeSuccess,taskId);
    ADDHTTPREQUESTPOSTDATA(STR_URL_FINISH_EVENT(196),"CALLBACK_FightResultConfirm_finshEvent", "REQUEST_FightResultConfirm_finshEvent",buffer, callfuncO_selector(FightResultConfirm::callBackData));
}

void FightResultConfirm::callBackData(cocos2d::CCObject *object)
{
    
    CCLog("FightResultConfirm: %s",(char*)object);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_FightResultConfirm_finshEvent");
    
    int code = -1;
    char *buff = (char*) object;
    if (buff)
    {
        CCDictionary *pResult = PtJsonUtility::JsonStringParse(buff);
        if (pResult)
        {
            code = GameTools::intForKey("code", pResult);
            if (code == 0)
            {
                pResult = (CCDictionary*) pResult->objectForKey("result");
                CCAssert(pResult, "result null");
                CCDictionary *tmp = (CCDictionary*) pResult->objectForKey("event_info");
                CExploration::setNextEventByDict(tmp);
                int nextStep = GameTools::intForKey("next_step", pResult);
                CExploration::setCurrentStep(nextStep);
            }
        }
        
    }
    
    CCNode * node = m_cMaps->getElementByTags("2,0,0");
    if(node && node->getChildByTag(912))
    {
        ((CCLabelTTF *)(node->getChildByTag(912)))->setString("测试数据0");
    }    
    setTouchEnabled(true);
    
}

bool FightResultConfirm::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return  true;
}
void FightResultConfirm::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void FightResultConfirm::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (getChildByTag(2)&&getChildByTag(2)->getChildByTag(911)&&((CCSprite *)(getChildByTag(2)->getChildByTag(911)))->boundingBox().containsPoint(pTouch->getPreviousLocation()))
    {
        m_nResult->setHuiFang(true);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("CONGTOUBOFANG");
        removeFromParentAndCleanup(true);
        return;
    }
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    handlerTouch();
}

void FightResultConfirm::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void FightResultConfirm::initFightResultConfirm()
{
    // init data:
    m_nTouchTag = -1;
    m_nResult = (SFightResultData *)getUserData();
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    CCLog("m_cMaps:%d", m_cMaps->retainCount());
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
  
    
     m_cMaps->getElementByTags("2")->setVisible(true);
     m_cMaps->getElementByTags("2,1")->setVisible(false);
     m_cMaps->getElementByTags("2,2")->setVisible(false);
     m_cMaps->getElementByTags("3")->setVisible(false);
    
//    string word ;
//    if (m_nResult == 1)
//    {
//         word = Utility::getWordWithFile("word.plist", "win");
//        Utility::getWordWithFile("word.plist", "option");
//        
//    }
//    else
//    {
//        word = Utility::getWordWithFile("word.plist", "lose");
//    }
 
   
//    CCLabelTTF* pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
//    pLabel->setPosition(ccp(240,140));
//   
//    CCNode * node = m_cMaps->getElementByTags("2,0,0");
//    if(node)
//    {
//        CCLog("exist,%s", word.c_str());
//        node->addChild(pLabel);
//    }
    
}

void FightResultConfirm::handlerTouch()
{
//    CCLog("FightResultConfirm:%d, %d",  m_nTouchTag, m_nResult);
//    if (m_nTouchTag == 2001)
//    {
//        if (g_nLevle == 9 && m_nResult == 1)
//        {
//            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_HALLSCENE);
//            g_nLevle = 0;
//
//            
//        }else
//        {
//            if (m_nResult == 1)
//            {
//                g_nLevle++;
//            }
              SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
//        }
//        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//    }
  }

