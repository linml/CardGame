//
//  CGameUserPropDialogLayer.cpp
//  91.cube
//
//  Created by phileas on 13-10-30.
//
//

#include "CGameUserPropDialogLayer.h"

CGameUserPropDialogLayer * CGameUserPropDialogLayer::create(int inPropId)
{
    CGameUserPropDialogLayer *layer = new CGameUserPropDialogLayer();
    if (layer)
    {
        layer->setPropId(inPropId);
        if (layer->initCGameUserPropDialogLayer())
        {
            layer->retain();
        }else
        {
            CC_SAFE_DELETE(layer);
            layer = NULL;
        }
        
    }
    return layer;
}

CGameUserPropDialogLayer::CGameUserPropDialogLayer()
{
    
}
CGameUserPropDialogLayer::~CGameUserPropDialogLayer()
{
    
}

bool CGameUserPropDialogLayer::initCGameUserPropDialogLayer()
{
    bool bRet = false;
    do
    {
        bRet = true;
    } while (0);
    return bRet;
}

bool CGameUserPropDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = true;
    return bRet;
    
}
void CGameUserPropDialogLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameUserPropDialogLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameUserPropDialogLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



