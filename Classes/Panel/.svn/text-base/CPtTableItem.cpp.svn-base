//
//  CPtTableItem.cpp
//  91.cube
//
//  Created by phileas on 13-6-26.
//
//

#include "CPtTableItem.h"

CPtTableItem::CPtTableItem()
{
    m_pDisplayView = NULL;
    m_pTouchDelegate = NULL;
    touchNode = NULL;
}
CPtTableItem::~CPtTableItem()
{
    if (m_pDisplayView)
    {
        m_pDisplayView->release();
    }
}


bool CPtTableItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
        m_bMove = false;

        bool bRet = false;
   
        bRet = isSelect(touchNode, pTouch);
        if (bRet)
        {
            if (m_pTouchDelegate)
            {
                return  m_pTouchDelegate->ccTouchBegan(pTouch, pEvent);
            }
            
        }
    
    
    
    return bRet;
    
}

void CPtTableItem::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
     m_bMove = true;
   
    if (m_pTouchDelegate)
    {
        m_pTouchDelegate->ccTouchMoved(pTouch, pEvent);
        return;
    }
    
 
       
    

    
}
void CPtTableItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pTouchDelegate)
    {
        m_pTouchDelegate->ccTouchEnded(pTouch, pEvent);
        return;
    }
   
       
   

}
void CPtTableItem::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
      m_bMove = false;
    if (m_pTouchDelegate)
    {
        m_pTouchDelegate->ccTouchCancelled(pTouch, pEvent);
    }
    
}


bool CPtTableItem::isSelect(cocos2d::CCNode *node, cocos2d::CCTouch *pTouch)
{
    CCPoint location = pTouch->getLocation();
    return isSelect(node, location);
}


bool CPtTableItem::isSelect(CCNode * node, CCPoint location)
{
    bool bRet = false;
    if (node)
    {
        CCNode *parent = node->getParent();
        if (parent)
        {
            
            location = parent->convertToNodeSpace(location);
            if (node->boundingBox().containsPoint(location))
            {
                bRet = true;
            }
        }
    }
    
    return bRet;
    
}

void CPtTableItem::setLayerParent(cocos2d::CCNode *inLayer)
{
    parent = inLayer;
}

void CPtTableItem::setTouchNode(CCNode *node)
{
    touchNode = node;
}

void CPtTableItem::setDisplayView(cocos2d::CCNode *inDisplayview)
{
    if (inDisplayview)
    {
        if (m_pDisplayView)
        {
            m_pDisplayView->release();
        }
        inDisplayview->retain();
        m_pDisplayView = inDisplayview;
    }
}



