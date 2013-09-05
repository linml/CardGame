#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "gameConfig.h"
#include "CDeletePropLayer.h"
#include "CCardSettingScene.h"
#include "CGameDialogLayer.h"
#include "PtActionUtility.h"
#include "Utility.h"
#include "HBActionScript.h"
#include "HBActionAni.h" 
#include "CGameCardBuffer.h"
#include "CBackpackContainerLayer.h"
#include "CGameStoryLayer.h"


using namespace cocos2d;
using namespace CocosDenshion;

void testBufferPng( CCLayer *layer)
{
    CGameCardBuffer *gameCardBuffer=CGameCardBuffer::CreateBuffer("resource_cn/img/fighting/suo.png", 2);
    layer->addChild(gameCardBuffer,2);
    CCSize  size=CCDirector::sharedDirector()->getWinSize();
    gameCardBuffer->setPosition(ccp(size.width*0.5 ,size.height*0.5));
    
    
}

static void test(CCNode *pSender);
static bool compareScores(const void* p1, const void* p2)
{
    //HBActionScript* score1 = (HBActionScript*)p1;
   // HBActionScript* score2 = (HBActionScript*)p2;
    //bool result = score1->getd < score2->raceTime();
    return true;
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create(g_sysSoundPath.c_str(), "Scissor Cuts", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

  //  test(this);
    // add "HelloWorld" splash screen"

 //   CCSprite* pSprite = CCSprite::create("resource_cn/img/HelloWorld.png");

  //  CCSprite* pSprite = CCSprite::create("resource_en/img/HelloWorld.png");

    // position the sprite on the center of the screen
    //pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
   // this->addChild(pSprite, 0);
    
 //   CCSprite *sprite=CCSprite::create("resource_cn/action/xuanfeng/1.png");
  //  addChild(sprite,2,100);
//    CCAnimation* animation = CCAnimation::create();
//    //共有14帧，这里用for循环将对应的序列图加入到动画中。
//    for( int i=1;i<=30;i++)
//    {
//        char szName[100] = {0};
//        sprintf(szName, "resource_cn/action/xuanfeng/%d.png", i);
//        animation->addSpriteFrameWithFileName(szName);
//    }
   // sprite->setPosition(ccp(size.width/2, size.height/2) );
    //设置每两帧间时间间隔为1秒。
//    animation->setDelayPerUnit(0.1f);
//    //设置动画结束后仍保留动画帧信息。
//    animation->setRestoreOriginalFrame(true);
//    //由这个动画信息创建一个序列帧动画。
//    CCAnimate* action = CCAnimate::create(animation);
//    //让演员演示这个动画。
//     Utility::runPtActionScript(sprite,"xuanfeng/xuanfeng.act",100);
//    sprite->runAction(CCRepeatForever::create(action));
//    removeChildByTag(<#int tag#>, <#bool cleanup#>)
//    testBufferPng(this);
//    this->getCamera()->setEyeXYZ(-60, 0,100);
    
    CGameStoryLayer *gamelayer=CGameStoryLayer::CreateStoryLayer(30005,this);
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
   // CCJumpBy::create(0.2, {0,100}, 100, 1);
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

PropItem item;
void test(CCNode *pSender)
{
    
   // CBattleArrayLayer *layer = CBattleArrayLayer::create();
   // CCardSettingScene *layer = CCardSettingScene::create();
    //CBackpackContainerLayer *layer = CBackpackContainerLayer::create();
   // pSender->addChild(layer);
  // CPtDialog * dialog = CPtDialog::create();
    
    item.propId = 100001;
    item.propCount = 20;
    CDeletePropLayer * layer = CDeletePropLayer::create(&item);
    pSender->addChild(layer);

}


