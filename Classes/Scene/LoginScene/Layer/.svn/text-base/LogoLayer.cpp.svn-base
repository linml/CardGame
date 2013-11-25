//
//  LogoLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-6.
//
//

#include "LogoLayer.h"
#include "LoginScene.h"

// static method:
LogoLayer* LogoLayer::create()
{
    LogoLayer *layer = new LogoLayer();
    layer->initWithColor(ccc4(250, 250, 250, 255));
    layer->initLogo();
    layer->autorelease();
    return layer;
}

// public method:
LogoLayer::LogoLayer()
{
    
}

LogoLayer::~LogoLayer()
{
    
}




// protected method:

void LogoLayer::initLogo()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *logo = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cmplogo.png"));
    logo->setOpacity(0);
    logo->setPosition(ccp(winSize.width/2, winSize.height/2));
    CCFiniteTimeAction *logoFade = CCSequence::create(
                CCFadeIn::create(2.0f),
                CCDelayTime::create(1.0f),
                CCFadeOut::create(2.0f),
                CCCallFunc::create(this,
                callfunc_selector(LogoLayer::removeCallBack)),
                                                      NULL
    );
       
   
    this->addChild(logo);
    logo->runAction(logoFade);
}

// callback method:
void LogoLayer::removeCallBack()
{
    ((CLoginScene*)this->getParent())->setLogoOverCallBack();
    removeFromParentAndCleanup(true);
  
}

