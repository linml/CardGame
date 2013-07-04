
//
//  ExplorationLayer.cpp
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#include "ExplorationLayer.h"
#include "PtMapUtility.h"
#include "SceneManager.h"
#include "gamePlayer.h"
#include "gameLogic.h"
CExplorationLayer::CExplorationLayer()
{
    
}

CExplorationLayer::~CExplorationLayer()
{
    
}

void CExplorationLayer::dealWhithTouchEndSprite(cocos2d::CCSprite *touchSprite, int touch_tag)
{
    if(!SingleSceneManager::instance()->runSceneSelect(touch_tag))
    {
        CCLOG("%d",touch_tag);
        if(!dealGotoRoom(touchSprite,touch_tag))
        {
            CCLOG("touch_tag not to do");
        }
    }
}
bool CExplorationLayer::dealGotoRoom(CCSprite * touchSprite,int touch_tag)
{
//    switch (touch_tag) {
//        case 3001:
//            SinglePlayer::instance()->m_nTurnTo=(int)EN_TURNTO_IN;
//            break;
//        case 3002:
//              SinglePlayer::instance()->m_nTurnTo=(int)EN_TURNTO_UP;
//            break;
//        case 3003:
//             SinglePlayer::instance()->m_nTurnTo=(int)EN_TURNTO_DOWN;
//            break;
//        case 3004:
//             SinglePlayer::instance()->m_nTurnTo=(int)EN_TURNTO_RIGHT;
//            break;
//        case 3005:
//             SinglePlayer::instance()->m_nTurnTo=(int)EN_TURNTO_LEFT;
//            break;
//            
//        default:
//            return false;
//    }
//    PtGameLogic::getGoPos(SinglePlayer::instance()->m_stcCurrenPos,SinglePlayer::instance()->m_nFaceTo,SinglePlayer::instance()->m_nTurnTo);
//    char data[40];
//    sprintf(data, "dangqian:{%d,%d,%d}",SinglePlayer::instance()->m_stcCurrenPos.x,SinglePlayer::instance()->m_stcCurrenPos.y,SinglePlayer::instance()->m_stcCurrenPos.z);
//    if(getChildByTag(12222))
//    {
//        CCLabelTTF *label=(CCLabelTTF *)(getChildByTag(12222));
//        label->setString(data);
//    }
    return true;
}


CCScene *CExplorationLayer::scene()
{
    CCScene *pScene=CCScene::create();
    CExplorationLayer *taskscene=CExplorationLayer::Create<CExplorationLayer>(scene_exploration.c_str());
    pScene->addChild(taskscene);
    return  pScene;
}



bool CExplorationLayer::initWithMapFile(const char *fileName)
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCLog("CardFactoryLayer::initWithMapFile");
	bool bRet = false;
	do
	{
        setUserData((void *)fileName);
		char *tmp = NULL;
		CC_BREAK_IF(!CCLayer::init());
		tmp = (char *)this->getUserData();
        mapFileName=tmp;
		PtMapUtility::addChildFromScript(this,mapFileName.c_str());
		PtMapUtility::addTouchRectFromScript(mapFileName.c_str(), this, &vTouchMapRect);
        char data[40];
//        sprintf(data, "dangqian:{%d,%d,%d}",SinglePlayer::instance()->m_stcCurrenPos.x,SinglePlayer::instance()->m_stcCurrenPos.y,SinglePlayer::instance()->m_stcCurrenPos.z);
        CCLabelTTF *labelroom=CCLabelTTF::create(data,"Arial",25);
        labelroom->setPosition(ccp(100,600));
        labelroom->setColor(ccc3(255, 0, 0));
        this->addChild(labelroom,1000,12222);
        
		setTouchEnabled(true);
        bRet = true;
	} while (0);
    return bRet;
}