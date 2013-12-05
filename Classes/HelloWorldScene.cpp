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
#include "CDrawCardLayer.h"
#include "CSceneActionGameLayer.h"
#include "CScreenHandBookLayer.h"
#include "CSceneFriendMainLayer.h"
#include "CGameRankLayer.h"
#include "CAnnouncementLayer.h"


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
   // HelloWorld *layer = HelloWorld::create();
 
    //CScreenHandBookLayer *layer=CScreenHandBookLayer::create();
   // CSceneFriendMainLayer *layer=CSceneFriendMainLayer::create();
    
    CAnnouncementLayer * gamelayer=CAnnouncementLayer::create();
    //addChild(gamelayer,1,1);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    gamelayer->setPosition(ccp(size.width*0.5,size.height*0.5));
    // add layer as a child to scene
    scene->addChild(gamelayer);


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
    
//    CDrawCardLayer *layer=CDrawCardLayer::create();
//    addChild(layer,0);
    //layer->setPosition(ccp(size.width*0.5,size.height*0.5));

    //test(this);
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
//    CCSprite *sprite=CCSprite::create("resource_cn/img/hall.png");
//    addChild(sprite,0,200);
//    sprite->setPosition(ccp(size.width*0.5,size.height*0.5));
//   CGameStoryLayer *gamelayer=CGameStoryLayer::CreateStoryLayer(30005,this);
    
 
    
    
    
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
#include "Biforest.h"
#include "CEventBoxDialog.h"
#include "CEventBoxReward.h"
#include "CGameRechargeTip.h"
#include "CGameArrageBackpackTip.h"
#include "CSceneGameShopLayer.h"
#include "CGameRechargeLayer.h"
#include "CSellerDialog.h"
#include "gameStruct.h"
#include "CAltarDialog.h"
#include "CGameBufferTip.h"
void testInit(SELLER_DATA &rSellData)
{
    rSellData.sellerId = 800103;
    rSellData.sellerShops[0].propId = 200002;
    rSellData.sellerShops[0].teamNum = 1;
    rSellData.sellerShops[0].limitNum = 5;
    rSellData.sellerShops[0].price = 20;

    
    rSellData.sellerShops[1].propId = 200006;
    rSellData.sellerShops[1].teamNum = 1;
    rSellData.sellerShops[1].limitNum = 0;
    rSellData.sellerShops[1].price = 20;
    
    rSellData.sellerShops[2].propId = 200005;
    rSellData.sellerShops[2].teamNum = 1;
    rSellData.sellerShops[2].limitNum = 0;
    rSellData.sellerShops[2].price = 20;

    
    rSellData.priceType = 1;
    
}

#include <vector>
#include <algorithm>

using namespace std;
struct Element
{
    int a;
    int b;
};
int camparent(const void * inElementOne, const void *inElementTwo)
{
    Element * temp1 = (Element*) inElementOne;
    Element * temp2 = (Element*) inElementTwo;
    int nRet = temp1->a - temp2->a;
    if (nRet == 0)
    {
        nRet = temp1->b - temp2->b;
    }
    return nRet;
}
int lcomparent(const void * inElementOne, const void *inElementTwo)
{
    Element * temp1 = (Element*) inElementOne;
    Element * temp2 = (Element*) inElementTwo;
    int nRet = temp1->a - temp2->a;
    if (nRet == 0)
    {
        nRet = temp1->b - temp2->b;
    }
    return -nRet;
    
}
void printArray(Element* array, int lenth)
{
    for(int i = 0; i  < lenth; i++)
    {
        printf(" the index : %d a: %d  b:%d \n", i, array[i].a , array[i].b);
    }
}
void testSort()
{
    int length = 1000;
    
    Element * array = new Element[length];
    srand(time(0));
    for (int i = 0;  i  < length; i++)
    {
        int temp = (rand() % 5) +1;
        int temp2 = (rand() % 100) +1;
        array[i].b = temp2;
        array[i].a = temp;
    }
    printArray(array, length);
    
    long t = clock();
    qsort(array, length, sizeof(Element), camparent);
    t = clock()-t;
    printf("===============the cost clock: %ld\n", t);
    printArray(array, length);
    t = clock();
    qsort(array, length, sizeof(Element), camparent);
    t = clock()-t;
    printf("===============the cost clock: %ld\n", t);
    printArray(array, length);
}

#include "CCardBagFullDialog.h"
#include "CGameFriedItem.h"
#include "CFriendEventDialog.h"
#include "CGameUserPropDialogLayer.h"
void test(CCNode *pSender)
{
   
    // CBattleArrayLayer *layer = CBattleArrayLayer::create();
    // CCardSettingScene *layer = CCardSettingScene::create();
    //CBackpackContainerLayer *layer = CBackpackContainerLayer::create();
    // pSender->addChild(layer);
    //   CPtDialog * layer = CPtDialog::create();
    //  CGameRechargeTip * layer = CGameRechargeTip::create();
    //  CGameArrageBackpackTip *layer = CGameArrageBackpackTip::create();
    //    item.propId = 100001;
    //    item.propCount = 20;
    //    CDeletePropLayer * layer = CDeletePropLayer::create(&item);
    //    CBiforestLayer *layer = CBiforestLayer::create();
    // CEventBoxLayer *layer = CEventBoxLayer::create(1);
    //   CEventBoxRewordLayer *layer = CEventBoxRewordLayer::create(NULL, 3);
    // CGameRechargeLayer * layer = CGameRechargeLayer::create(NULL);
    //  CSceneGameShopLayer * layer = CSceneGameShopLayer::create();
    //    SELLER_DATA inSellData;
    //    testInit(inSellData);
    //    CSellerDialog * layer = CSellerDialog::create(inSellData);
    // CAltarDialogLayer *layer = CAltarDialogLayer::create(0);
    //    CGameBufferTipLayer *layer = CGameBufferTipLayer::create(0);
//    CCardBagFullDialog *layer = CCardBagFullDialog::create();
//    pSender->addChild(layer);

//    CFriend *f = new CFriend( 123, 5,  99, "胸毛", true );
//    CGameFriedItem *item = CGameFriedItem::create(f, 5);
//    item->setAnchorPoint(CCPointZero);
//    pSender->addChild(item);
    
    CCNode *node = NULL;
    node = CGameUserPropDialogLayer::create(901080) ;//CFriendEventDialog::create(Friend(133,"doumeige"));
    //testSort();
    pSender->addChild(node);
}



