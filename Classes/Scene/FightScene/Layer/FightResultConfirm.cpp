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
        bRet = true;
    } while (0);
    return bRet;
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
    m_nResult = (int)getUserData();
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-INT_MAX);
    
     m_cMaps->getElementByTags("2")->setVisible(true);
     m_cMaps->getElementByTags("2,1")->setVisible(false);
     m_cMaps->getElementByTags("2,2")->setVisible(false);
     m_cMaps->getElementByTags("3")->setVisible(false);
    
    string word ;
    if (m_nResult == 1)
    {
         word = Utility::getWordWithFile("word.plist", "win");
        Utility::getWordWithFile("word.plist", "option");
        
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
        CCLog("exist,%s", word.c_str());
        node->addChild(pLabel);
    }

    
}

void FightResultConfirm::handlerTouch()
{
    CCLog("FightResultConfirm:%d, %d",  m_nTouchTag, m_nResult);
    if (m_nTouchTag == 2001)
    {
        if (g_nLevle == 9 && m_nResult == 1)
        {
            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_HALLSCENE);
            g_nLevle = 0;

            
        }else
        {
            if (m_nResult == 1)
            {
                g_nLevle++;
            }
              SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
        }
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

    }
  }

