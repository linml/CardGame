//
//  CCardPanel.cpp
//  91.cube
//
//  Created by linminglu on 13-6-14.
//
//

#include "CCardPanel.h"
CCardPanel::CCardPanel()
{
    
}
CCardPanel::~CCardPanel()
{
    
}
CCardPanel  *CCardPanel::Create(const char *pngFileName, const char *name)
{
    CCardPanel *cardSprite=new CCardPanel();
    if(cardSprite ==NULL|| !cardSprite->initSprite(pngFileName, name))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;
    
}

bool CCardPanel::initSprite(const char *pngFileName,const char *name)
{
//    if(access((string("./")+pngFileName).c_str(), F_OK) ==-1)
//    {
//        CCLog("error:%s",pngFileName);
//    }
    this->initWithFile(pngFileName);
    CCLabelTTF *label=CCLabelTTF::create(name, "Arial", 15);
    addChild(label,1,11);
    label->setPosition(ccp(getContentSize().width/2, getContentSize().height+20));
    return true;
}

CCLabelTTF *CCardPanel::getLabel()
{
    return (CCLabelTTF *)getChildByTag(11);
}
