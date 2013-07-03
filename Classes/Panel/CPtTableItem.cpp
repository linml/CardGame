//
//  CPtTableItem.cpp
//  91.cube
//
//  Created by phileas on 13-6-26.
//
//

#include "CPtTableItem.h"


CPtTableItem * CPtTableItem::create(const char *pszFileName)
{
    CPtTableItem *pSprite = new CPtTableItem();
    if (pSprite && pSprite->initWithFile(pszFileName))
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

CPtTableItem * CPtTableItem::create(const char *pszFileName, const cocos2d::CCRect &rect)
{
    
    CPtTableItem *pobSprite = new CPtTableItem();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}


bool CPtTableItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

    bool bRet = false;
   
    

        bRet = isSelect(touchNode, pTouch);
        if (bRet)
        {
        
        }
    
    
    
    return bRet;
    
}

void CPtTableItem::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
 
        CCLog("CCardItem::ccTouchMoved");

    

    
}
void CPtTableItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
   
        CCLog("CCardItem::ccTouchEnded");
   

}
void CPtTableItem::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
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
