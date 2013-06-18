//
//  cn_cubeAppDelegate.cpp
//  cn.cube
//
//  Created by xianbei1987 on 13-5-17.
//  Copyright __MyCompanyName__ 2013骞? All rights reserved.
//
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "SceneManager.h"
#include "MemoryGameCollabLayer.h"


USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1280, 800,kResolutionShowAll);


	// turn on display FPS
	//pDirector->setDisplayStats(true);
    
    pDirector->setProjection(kCCDirectorProjection2D);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);
    
   /*
    CCScene *scene=CCScene::create();
    MemoryGameCollabLayer *layer=MemoryGameCollabLayer::Create<MemoryGameCollabLayer>("123");
    scene->addChild(layer);
    CCSize size =pDirector->getWinSize();
    layer->setPosition(ccp(size.width/2,size.height/2));
    scene->autorelease();
    pDirector->runWithScene(scene);
    */

  // SingleSceneManager::instance()->runMapScene();
    SingleSceneManager::instance()->runTest();
   // pDirector->runWithScene(HelloWorld::scene());

    
  // SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_FIGHTSCENE);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->pause();

	// if you use SimpleAudioEngine, it must be paused
	// SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->resume();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
