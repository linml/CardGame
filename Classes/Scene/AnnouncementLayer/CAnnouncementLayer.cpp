//
//  CAnnouncementLayer.cpp
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#include "CAnnouncementLayer.h"
CAnnouncementLayer::CAnnouncementLayer()
{
    this->plabelTTF=NULL;
}
CAnnouncementLayer::~CAnnouncementLayer()
{
    
}
bool CAnnouncementLayer::init()
{
    createColorLayer();
    return true;
}

void CAnnouncementLayer::createColorLayer()
{
    CCLayerColor *pColorLayer=CCLayerColor::create(ccc4(255, 0, 0, 120), 500, 100);//用红色来表示下
    addChild(pColorLayer,1,1);
}

void CAnnouncementLayer::showLayer()
{
    if (getChildByTag(1)) {
        getChildByTag(1)->setVisible(true);
    }
}
void CAnnouncementLayer::hideColorLayer()
{
    
    if (getChildByTag(1)) {
        getChildByTag(1)->setVisible(false);
    }
}