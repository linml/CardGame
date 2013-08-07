//
//  TaskLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-18.
//
//

#include "TaskLayer.h"
#include "gameConfig.h"
#include "AsgardLayer.h"
#include "Biforest.h"

CTaskLayer::CTaskLayer()
{
    
}

CTaskLayer::~CTaskLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CTaskLayer::init()
{
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 125)));
        initTask();
        bRet = true;
    } while (0);
    return bRet;
}

bool CTaskLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    return true;
}

void CTaskLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CTaskLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag =  TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if (m_nTouchTag != -1)
    {
        handlerTouch();
    }

}

void CTaskLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CTaskLayer::initTask()
{
    CCLog(" CTaskLayer::initTask");
    
    // init data:
    m_nTouchTag = -1;
    m_bGoEnable = false;
    m_cMaps = NULL;
    
    // init maps:
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "demo_task.plist"));
    m_cMaps->getTouchRects(m_cTouches);

    // set invisiable:
    CCNode * node = NULL;
    char buffer[10] = {0};
    for (int i = 4; i <6 ; i++)
    {
        memset(buffer, 0, sizeof(char)*10);
        sprintf(buffer, "1,%d",i);
        node = m_cMaps->getElementByTags(buffer);
        if (node)
        {
            node->setVisible(false);
        }
    }
    
    node = m_cMaps->getElementByTags("1,6,1");
    if (node)
    {
        ((CCSprite *)node)->setColor(ccc3(125, 125, 125));
    }
    
    sprintf(buffer, "1,1,%d", 1);
    node = m_cMaps->getElementByTags(buffer);
    ((CCSprite *)node)->setOpacity(125);
    
    // set touch

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-1);
    
}

void CTaskLayer::handlerTouch()
{
    CCLog("CTaskLayer::handlerTouch: %d", m_nTouchTag);
    
    if (m_nTouchTag == 7003 || m_nTouchTag == 7004 || m_nTouchTag == 7005)
    {
        
        
        CCNode * node = NULL;
    
        char buffer[10] = {0};
        
        sprintf(buffer, "1,%d", m_nTouchTag-7000);
        
        node = m_cMaps->getElementByTags(buffer);
        if (node)
        {
            if (node->isVisible())
            {
              
                return;
            }else
            {
              
                node->setVisible(true);
                sprintf(buffer, "1,1,%d", m_nTouchTag-7002);
                node = m_cMaps->getElementByTags(buffer);
                if(node)
                {
                    
                    ((CCSprite*) node)->setOpacity(125);
                }
                
                    m_bGoEnable = false;
                    node = NULL;
                    node = m_cMaps->getElementByTags("1,6,0");
                    if (node)
                    {
                        ((CCSprite *)node)->setColor(ccc3(255, 255, 255));
                    }
                
                    node = m_cMaps->getElementByTags("1,6,1");
                    if (node)
                    {
                        ((CCSprite *)node)->setColor(ccc3(125, 125, 125));
                    }

                
            
                for (int i = 3; i < 6; i++)
                {
                    if (m_nTouchTag == (7000+i))
                    {
                        continue;
                    }
                    sprintf(buffer, "1,%d", i);
                    node = m_cMaps->getElementByTags(buffer);
                    if (node)
                    {
                        node->setVisible(false);
                        
                        sprintf(buffer, "1,1,%d", i-2);
                        node = m_cMaps->getElementByTags(buffer);
                        if (node)
                        {
                             ((CCSprite*) node)->setOpacity(255);
                            
                        }
                        
                        
                    }
                    
                    
                }
        
                
            }
            
            
        }

        
        
        node = m_cMaps->getElementByTags("1,2");
        if (node)
        {
            node->setVisible(true);
        }
        
        node = m_cMaps->getElementByTags("1,6");
        if (node)
        {
            node->setVisible(true);
        }
        

        
    }
    
    if (m_nTouchTag == 7001 || m_nTouchTag == 7002)
    {
        if (m_bGoEnable && m_nTouchTag == 7002)
        {
            // go:
            CCLayer *layer = CBiforestLayer::create();
            this->getParent()->addChild(layer, 30000, 6001);
            removeFromParentAndCleanup(true);
            
        }else if(m_nTouchTag == 7001 && m_bGoEnable == false)
        {
            CCNode *node = NULL;
            node = m_cMaps->getElementByTags("1,6,1");
            if (node)
            {
                ((CCSprite *)node)->setColor(ccc3(255, 255, 255));
            }
            node = m_cMaps->getElementByTags("1,6,0");
            if (node)
            {
                ((CCSprite *)node)->setColor(ccc3(125, 125, 125));
            }

            
            m_bGoEnable = true;
        }
            
    
    }
    
    if (m_nTouchTag == 7006)
    {
        removeFromParentAndCleanup(true);
    }
}