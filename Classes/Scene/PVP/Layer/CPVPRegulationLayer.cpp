//
//  CPVPRegulationLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-1-6.
//
//

#include "CPVPRegulationLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"

#define TAG_PVP_BACKGROUDMAP 2
#define TAG_PVP_QUITBUTTON 3

CPVPRegulationLayer::CPVPRegulationLayer()
{
    size=CCDirector::sharedDirector()->getWinSize();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}

CPVPRegulationLayer::~CPVPRegulationLayer()
{
    if(scroll->getContainer())
    {
        scroll->getContainer()->removeFromParentAndCleanup(true);
    }
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
}

bool CPVPRegulationLayer::init()
{
    createBackGround();
    createQuitButton();
    createScrollView();
    setTouchEnabled(true);
    //addChild(createContianLayer(),3);

    setTouchPriority(-9);
    return  true;
}

CCLayer *CPVPRegulationLayer::createContianLayer()
{
    
    CCLayer *pContextlayer=CCLayer::create();
    const string noticeValue[8]={"319911","319912","319913","319914","319915","319916","319917","319918"};
    string word;
    float totalHeight=0.0f;
    for (int i=7; i>=0; i--) {
        word=Utility::getWordWithFile("dictionary.plist", noticeValue[i].c_str());
        CCLOG("::::::%s",word.c_str());
        CCLayer *layer=GameTools::createDiffColorWord(word, "Arial", 20,ccp(0,0.2),600);
        pContextlayer->addChild(layer, 2);
        float height=layer->getContentSize().height;
        layer->setPosition(ccp(0,height+totalHeight));
        totalHeight+=height;
    }
    pContextlayer->setContentSize(CCSizeMake(600, totalHeight));
    CCLOG("totalHeight:%f",totalHeight);
    return pContextlayer;
    
    
}

void CPVPRegulationLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CPVPRegulationLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool CPVPRegulationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CPVPRegulationLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPVPRegulationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    if (getChildByTag(TAG_PVP_QUITBUTTON)->boundingBox().containsPoint(point))
    {
        removeFromParentAndCleanup(true);
    }
    
}

void CPVPRegulationLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



void CPVPRegulationLayer::createScrollView()
{
    //设置scrollView的大小,为显示的view的尺寸
    scroll = CCScrollView::create(CCSizeMake(960, 640));
    
    CCLayer *layer=createContianLayer();
    int PostionY=(400-layer->getContentSize().height);
    PostionY = (PostionY>0?PostionY:0);
    CCLOG("PostionY:::::%d,%d",PostionY,layer->getContentSize().height);
    layer->setPosition(ccp(0,PostionY));

    scroll->setContainer(layer);
    //是开启弹性效果，关闭的话就不用使用这个控件
    //scroll->setBounceable(false);
    bool flag = scroll->isBounceable();
    CCLog("flag: %d",flag);
    scroll->setDirection(kCCScrollViewDirectionVertical);
    scroll->setContentSize(CCSizeMake(650,400));
        //触摸有效
    this->setTouchEnabled(true);
    CCSize scrollSize = scroll->getContentSize();
    CCLog("scrollSize: %f %f",scrollSize.width,scrollSize.height);
    scroll->setPosition(ccp(200,160));
    //设置代理为自身
    scroll->setDelegate(this);
    this->addChild(scroll,2);
    //黑边防御坐标
    xOffSet = size.width - scrollSize.width;
    yOffSet = size.height - scrollSize.height;
}

void CPVPRegulationLayer::scrollViewDidScroll(CCScrollView *view)
{
    static int flag = 0;
    CCLog("Scroll %d",flag++);
    
    CCPoint offSet = this->scroll->getContentOffset();
    CCLog("offSet : %f %f",offSet.x,offSet.y);
    if (offSet.x < this->xOffSet || offSet.y < this->yOffSet) {
        
        CCLog("scrollView 已经出现黑边问题了！");
        
        if (offSet.x < this->xOffSet ) {
            CCLog("scrollView X轴 出现黑边问题了！");
            this->scroll->setContentOffset(CCPoint(this->xOffSet, offSet.y));
        }else{
            CCLog("scrollView Y轴 已经出现黑边问题了！");
            this->scroll->setContentOffset(CCPoint(offSet.x, this->yOffSet));
        }
    }
    
    if (offSet.x > 0 || offSet.y > 0) {
        CCLog("scrollView 已经出现黑边问题了！");
        
        if (offSet.x > 0 ) {
            CCLog("scrollView X轴 出现黑边问题了！");
            this->scroll->setContentOffset(CCPoint(0, offSet.y));
        }else{
            CCLog("scrollView Y轴 已经出现黑边问题了！");
            this->scroll->setContentOffset(CCPoint(offSet.x, 0));
        }
        
    }
}


void CPVPRegulationLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CPVPRegulationLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_PVP_BACKGROUDMAP);
    
}

void CPVPRegulationLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_PVP_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));

}
