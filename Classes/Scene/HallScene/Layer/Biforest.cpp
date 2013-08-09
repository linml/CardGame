//
//  Biforest.cpp
//  91.cube
//
//  Created by phileas on 13-6-13.
//
//

#include "Biforest.h"
#include "AsgardLayer.h"
#include "gameConfig.h"
#include "ExplorationScene.h"
#include "SceneManager.h"

static CCPoint p;

CBiforestLayer::CBiforestLayer()
{
    
}

CBiforestLayer::~CBiforestLayer()
{
    m_cMaps->release();
}

bool CBiforestLayer::init()
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 125)));
        initBiforest();
        bRet = true;
    } while (0);
    return  bRet;
}

bool CBiforestLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if(m_nTouchTag != -1)
    {
        return true;
    }
    

    return  true;
}
void CBiforestLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CBiforestLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    if (m_nTouchTag ==  TouchRect::SearchTouchTag(touchPoint, m_cTouches))
    {
        handlerTouch();
    }

}
void CBiforestLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CBiforestLayer::initBiforest()
{
 
    // init data:
    m_nTouchTag  = -1;
    m_bSectionTouchEnable = false;
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "biforest.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-1);
    
    // init
    CCNode* node = m_cMaps->getElementByTags("2,0,3");
    if (node)
    {
        node->setVisible(false);
        p = m_cMaps->getElementByTags("2,0,2")->getPosition();
        m_cMaps->getElementByTags("2,0,2")->setPositionY(node->getPositionY());
        
    }
    
    // set invisiable:
    m_cMaps->getElementByTags("2,0,5")->setVisible(false);
    m_cMaps->getElementByTags("2,0,6")->setVisible(false);
    m_cMaps->getElementByTags("2,0,7")->setVisible(false);
    
    
}

void CBiforestLayer::handlerTouch()
{
    CCLog("CBiforestLayer: %d", m_nTouchTag);
    CCNode* node = NULL;
    switch (m_nTouchTag) {
        case 2001:
            
            node= m_cMaps->getElementByTags("2,0,3");
            if (node)
            {
                node->setVisible(true);
                m_bSectionTouchEnable = true;
                m_cMaps->getElementByTags("2,0,2")->setPosition(p);
            }

            break;
            
        case 2002:
            
            break;
        case 2003:
            if (m_bSectionTouchEnable)
            {
                m_cMaps->getElementByTags("2,0,5")->setVisible(true);
                m_cMaps->getElementByTags("2,0,6")->setVisible(true);
                m_cMaps->getElementByTags("2,0,7")->setVisible(true);

            }
            break;
        case 2004:
            removeFromParentAndCleanup(true);
            break;
        case 2005:
            //如果是场景切换 请调用scenemanage
            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
          //  CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CExploration::scene()));

            break;
            
        default:
            break;
    }
    
}


