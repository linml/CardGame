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
#include "gameMiddle.h"
#include "CPlayerBufferManager.h"
#include "CGameEmailManager.h"
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
    CGamePlayer *m_player=SinglePlayer::instance();
    char buffer[300]={0};
    //sig=2ac2b1e302c46976beaab20a68ef95&info={"opponent_uid":195,"uid":195,"troops":1,"fight_info":"1,2,3"}& is_pvp = 1 & revenge_id = 1
    sprintf(buffer,"sig=%s&info={\"opponent_uid\":%d,\"uid\":%s,\"troops\":%d,\"fight_info\":\"1,2,3\"}&is_pvp=%d&revenge_id=%d",STR_USER_SIG, m_player->getFightUid(), m_player->getUserId(), 1,1,m_player->getFuChouId());
    ADDHTTPREQUESTPOSTDATA(STR_URL_FIGHT(196),"CALLBACK_FightResultConfirm_postHttpTeam", "REQUEST_FightResultConfirm_postHttpTeam",buffer, callfuncO_selector(FightResultConfirm::callBackDataTeam));
}
void FightResultConfirm::callBackDataTeam(cocos2d::CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_FightResultConfirm_postHttpTeam");
    
    CCLog("FightResultConfirm: %s",(char*)object);
    if(!object)
    {
        test_print("服务器信息错误");
        return ;
    }
    char *buffer=(char *)object;
    CCDictionary *pResult = PtJsonUtility::JsonStringParse(buffer);
    CCLog("buffer :%s",buffer);
    if (pResult->objectForKey("code")&&GameTools::intForKey("code", pResult)==0)
    {
        CCDictionary *resultDict=(CCDictionary *)pResult->objectForKey("result");
        if(((CCString *) resultDict->objectForKey("info"))->intValue()==1)
        {
            
            m_nResult->setFightResult(1);
            PtSoundTool::playSysSoundEffect("fight_win.mp3");
            test_print("胜利了");
        }
        else
        {
            m_nResult->setFightResult(0);
            PtSoundTool::playSysSoundEffect("fight_failed.mp3");
            test_print("失败");
        }
        CCDictionary  *inAllReward=(CCDictionary *)resultDict->objectForKey("reward");
        CReward *reward =NULL;
        if (inAllReward->objectForKey("add") &&((CCDictionary*)inAllReward->objectForKey("add"))->objectForKey("pvp_score") ) {
            reward = CReward::create((CCDictionary *)((CCDictionary*)inAllReward->objectForKey("add"))->objectForKey("pvp_score"));
            reward->excuteReward(ADD);
            
        }
        else if(inAllReward->objectForKey("dec") &&((CCDictionary*)inAllReward->objectForKey("dec"))->objectForKey("pvp_score"))
        {
            reward = CReward::create((CCDictionary *)((CCDictionary*)inAllReward->objectForKey("dec"))->objectForKey("pvp_score"));
            reward->excuteReward(DEC);
        }
    }
    else
    {
        CCMessageBox("pvp战斗信息错误", "pvp战斗信息错误");
    }
    delete [] buffer;
    buffer=NULL;

}
void FightResultConfirm::postHttpNpc()
{
   
    
    int taskId = SinglePlayer::instance()->getCurrentTaskId() == CExploration::getCurrentTaskId() && !SinglePlayer::instance()->getAllTaskCompleted() ? CExploration::getCurrentTaskId() : 0;
    int eventId=CExploration::getCurrentEventId(); //事件ID
    int chapterId=CExploration::getCurrentChapterId(); //章ID
    int sectionId=CExploration::getCurrentSectionId(); //节ID
    int stepNumber=CExploration::getCurrentStep();
    int typeSuccess=m_nResult->getFightResult();
    int troops = 1; // 默认 1阵容
    char buffer[200]={0};

    //    CPtSection*  m_pSection = s_SectionData.sectionInfo;
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d&step=%d&event_id=%d&type=%d&task_id=%d&troops=%d",STR_USER_SIG, chapterId, sectionId, stepNumber-1, eventId,typeSuccess,taskId,troops);
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
                
                // maybe product email
                SinglePlayer::instance()->addEmails((CCDictionary*)pResult->objectForKey("inbox_info"));
                
                CCDictionary *buffs =(CCDictionary*) pResult->objectForKey("buffs");
                CExploration::resetPropBufferByDict(buffs);
                
                CCDictionary *tmp = (CCDictionary*) pResult->objectForKey("event_info");
                CExploration::setNextEventByDict(tmp);
                resetAltarBuffer(pResult->objectForKey("chapter_buff"));
                tmp = (CCDictionary*) pResult->objectForKey("reward");
                CCDictionary* reward=NULL;
                if (tmp)
                {
                    reward = CExploration::setRewardsByDict(tmp);
                    handlerEventReward(reward);
   
                }else
                {
                   
                    handlerEventReward(NULL);
                }
                if(((CCString *) pResult->objectForKey("info"))->intValue()==1)
                {
                    m_nResult->setFightResult(1);
                    PtSoundTool::playSysSoundEffect("fight_win.mp3");
                }
                else
                {
                    //如果是失败的话 记录战斗的怪物信息;
//                    char dataMoster[200];
//                    int iLenStr=0;
//                    CGamePlayer *pGamePlayer=SinglePlayer::instance();
//                    for (int i=0; i<pGamePlayer->m_hashmapMonsterCard.size(); i++) {
//                        if (pGamePlayer->m_hashmapMonsterCard[i]) {
//                            iLenStr=sprintf(&dataMoster[iLenStr], " monter: %d",pGamePlayer->m_hashmapMonsterCard[i]->m_pCard->m_icard_id);
//                        }
//                    }
//                    appendFileLog(dataMoster);
                    //
                    m_nResult->setFightResult(0);
                    PtSoundTool::playSysSoundEffect("fight_failed.mp3");
                }
                int nextStep = GameTools::intForKey("next_step", pResult);
              
                CExploration::setCurrentStep(nextStep);
            }else
            {
                test_print(code);
            }
        }
        
    }else
    {
        test_print("server: no response");
    }
    delete [] buff;
    buff=NULL;
    
   
    
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
        
}

void FightResultConfirm::handlerTouch()
{
    if (m_nTouchTag == 2001)
    {
        if(m_nResult->getFightType()==0)
        {
            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
        }
        else{
            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_PVPSCENE);
        }
    }

}

void FightResultConfirm::handlerEventReward(CCDictionary * inAllReward)
{
    int flag  = 0;
    char tips[200] = {0};
    sprintf(tips, "没有奖励：");
    if (inAllReward)
    {
     
     
        CReward * addReward = (CReward*)inAllReward->objectForKey(EVENTREWARD_ADD);
        CReward * decReward = (CReward*) inAllReward->objectForKey(EVENTREWARD_DEC);
        if (addReward)
        {
            flag = addReward->excuteReward(ADD);
            sprintf(tips, "add: ap: %d, gp: %d \n     exp:%d, coin: %d, cash: %d\n card count: %d, prop count: %d\n extra coin: %d, extra exp : %d", addReward->getEnergy(), addReward->getHP(), addReward->getExp(),
                    addReward->getCoin(), addReward->getCash(), addReward->getCardCount(), addReward->getPropCount(), addReward->getExtarCoin(), addReward->getExtarExp());
           // addReward->getRewardContent(tips, 200);
           
        }
        if (decReward)
        {
            char buffer[60] = {0};
            decReward->excuteReward(DEC);
//            decReward->getRewardContent(buffer, 60);
            sprintf(buffer, "dec: ap: %d, gp: %d \n     exp:%d, coin: %d, cash: %d\n card count: %d, prop count: %d\n", decReward->getEnergy(), decReward->getHP(), decReward->getExp(),
                    decReward->getCoin(), decReward->getCash(), decReward->getCardCount(), decReward->getPropCount());

            strcat(tips, buffer);
        }
    
        // print:
        
    }
    test_print(tips);
    if(REWARD_IS_LEVEL_UP_SUCCESS(flag))
    {
        SinglePlayer::instance()->updatePlayerDataWithExp();
    }
}


void FightResultConfirm::resetAltarBuffer(CCObject* inBuffers)
{
    CPlayerBufferManager * bufferManager = CPlayerBufferManager::getInstance();
    bufferManager->clearAllAltarBufferes();

    std::string typeName = typeid(*inBuffers).name();
    if(typeName.find("CCDictionary") != std::string::npos)
    {
        CCDictionary *dict = (CCDictionary*) inBuffers;
        CCDictionary *tmpDict = NULL;
        CCDictElement *element = NULL;
        CCDICT_FOREACH(dict, element)

        {
            tmpDict = (CCDictionary*) element->getObject();
            if(tmpDict)
            {
                int effectId = GameTools::intForKey("object_id", tmpDict);
                ALTARBUFFERTYPE altarType = GameTools::intForKey("num_type", tmpDict) == 1 ? KEEPTIMES : KEEPTIME;
                int keeptime  = GameTools::intForKey("num", tmpDict);
                bufferManager->addAltarBufferById(effectId, altarType, keeptime);
            }
        }
    }
}
// test:

void FightResultConfirm::test_print(int code)
{
    char buffer[30] = {0};
    sprintf(buffer, "fight result error code : %d", code);    
    test_print(buffer);
}

void FightResultConfirm::test_print(const char * inMsg)
{
    CCNode * node = m_cMaps->getElementByTags("2,0,0");
    if(node && node->getChildByTag(912))
    {
        ((CCLabelTTF *)(node->getChildByTag(912)))->setString(inMsg);
    }
    setTouchEnabled(true);
}


