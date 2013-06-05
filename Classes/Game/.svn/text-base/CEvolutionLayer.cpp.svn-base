//
//  CEvolution.cpp
//  91.cube
//
//  Created by linminglu on 13-5-27.
//
//

#include "CEvolutionLayer.h"
#include "SceneManager.h"
#include "Utility.h"
#include "PtMapUtility.h"

CEvolutionLayer::CEvolutionLayer()
{
    
}

CEvolutionLayer::~CEvolutionLayer()
{
    
}

void CEvolutionLayer::dealWhithTouchEndSprite(cocos2d::CCSprite *touchSprite, int touch_tag)
{
    if(!SingleSceneManager::instance()->runSceneSelect(touch_tag))
    {
        
    }
}
bool CEvolutionLayer::initWithMapFile(const char *fileName)
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
		setTouchEnabled(true);
        bRet = true;
	} while (0);
    return bRet;
}