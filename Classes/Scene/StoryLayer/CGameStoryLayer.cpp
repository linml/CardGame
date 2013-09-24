//
//  CGameStoryLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-9-4.
//
//

#include "CGameStoryLayer.h"
#include "CGameStory.h"
#include "gameConfig.h"
#include "CGameTalkDialog.h"
#include "CGameRoleAnimation.h"

CGameStoryLayer::CGameStoryLayer()
{
    m_oldGameDialog=NULL;
    wndSize=CCDirector::sharedDirector()->getWinSize();
    m_pRoleAnimation=new  CGameRoleAnimation;
    isKuaiJingZhuangTai=false;
    setTouchPriority(-40001);
    CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png"));
    CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Pressed.png"));
    
    
}

CGameStoryLayer::~CGameStoryLayer()
{
    CCTextureCache::sharedTextureCache()->removeTextureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png"));
    CCTextureCache::sharedTextureCache()->removeTextureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Pressed.png"));
    if(m_pRoleAnimation)
    {
        delete m_pRoleAnimation;
        m_pRoleAnimation=NULL;
    }
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
    
}

CGameStoryLayer *CGameStoryLayer::CreateStoryLayer(int storyId,CCNode *node,int zorder)
{
    CGameStoryLayer *gameStory=new CGameStoryLayer();
    if(!gameStory || !gameStory->initCreateStory(storyId, node, zorder))
    {
        delete gameStory;
        gameStory=NULL;
        return NULL;
    }
    gameStory->autorelease();
    return gameStory;
    
}


bool CGameStoryLayer::initCreateStory(int storyId, cocos2d::CCNode *node, int zorder)
{
    string str=getStoryTalkList(storyId);
    m_vDialogTalk= transToIntVector(splitString(str.c_str(), ","));
    m_vDialogTalkIndex=0;
    m_bIsEndDialog=true;
    createColorStoryColorLayer();
    createKuaiJing();
    loadDialogList();
    createDialogLayer();

    if(node)
    {
        node->addChild(this, zorder);
    }
    else{
        CCDirector::sharedDirector()->getRunningScene()->addChild(this,zorder);
    }
    isCaneTouch=false;
    schedule(schedule_selector(CGameStoryLayer::updateTimeToShow));
    setTouchEnabled(true);
    setTouchPriority(-40001);
    return true;
}

void CGameStoryLayer::createColorStoryColorLayer()
{
    CCLayerColor *colorLayer=CCLayerColor::create(ccc4(0, 0, 0, 200));//
    addChild(colorLayer,0,1);//, <#GLfloat width#>, <#GLfloat height#>
    
}

void CGameStoryLayer::updateTimeToShow(float t)
{
    if(m_bIsEndDialog && m_vDialogTalkIndex< m_vGameTalkDialog.size())
    {
        //执行动画
        m_bIsEndDialog=false;
        CGameTalkDialog *gameDialogTalk=m_vGameTalkDialog[m_vDialogTalkIndex];
        displayDialog(gameDialogTalk);
    }
    else  if(m_vDialogTalkIndex >=m_vGameTalkDialog.size())
    {
        unschedule(schedule_selector(CGameStoryLayer::updateTimeToShow));
        CCNotificationCenter::sharedNotificationCenter()->postNotification("GUANBIDUIHUAKUANG");
        removeFromParentAndCleanup(true);
    }

    
}

void CGameStoryLayer::setCaneTouch()
{
    isCaneTouch=true;
}

void CGameStoryLayer::endTalk()
{
    if(isCaneTouch)
    {
        m_vDialogTalkIndex++;
        m_bIsEndDialog=true;
    }
}

void CGameStoryLayer::loadDialogList()
{
    map<int , CGameTalkDialog *>m_mapDialogTalk;
    CCDictionary *directory = CCDictionary::createWithContentsOfFile((resRootPath+"dialoguelist.plist").c_str());
    CCArray *vKeyArray=directory->allKeys();
    if (!vKeyArray)
    {
        return;
    }
   ;
    //读取字典表格
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *storyDialogTalk=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CGameTalkDialog *gameDialogTalk=new CGameTalkDialog ;
        gameDialogTalk->setGameTalkID(GameTools::intForKey("id", storyDialogTalk));
        gameDialogTalk->setGameTalkUiPlan(GameTools::intForKey("plan", storyDialogTalk));
        gameDialogTalk->setGameTalkDicntionaryID(GameTools::valueForKey("word", storyDialogTalk));
        gameDialogTalk->setGameTalkDialogWord( Utility::getWordWithFile("dictionary.plist", gameDialogTalk->getGameTalkDicntionaryID().c_str()));
        gameDialogTalk->setGameTalkDialogPng(GameTools::valueForKey("pic", storyDialogTalk));
        gameDialogTalk->setGameTalkSoundEffects(GameTools::valueForKey("soundeffects", storyDialogTalk));
        gameDialogTalk->setGameTalkEffects(GameTools::valueForKey("effects", storyDialogTalk));
        m_mapDialogTalk[gameDialogTalk->getGameTalkID()]=gameDialogTalk;
    }
    //取得向量里面的id 把map 里面的 指针得到出来 然后赋值过去； 最后删除map指针
    
    for (int i=0 ; i<m_vDialogTalk.size(); i++) {
        if(m_mapDialogTalk[m_vDialogTalk[i]])
        {
            m_vGameTalkDialog.push_back(new CGameTalkDialog(*m_mapDialogTalk[m_vDialogTalk[i]]));
        }
    }
    
    //CLEAR MAP
    for (map<int , CGameTalkDialog *>::iterator it=m_mapDialogTalk.begin(); it!=m_mapDialogTalk.end(); it++) {
        if(it->second)
        {
            CGameTalkDialog *data=it->second;
            delete data;
            data=NULL;
        }
    }
    m_mapDialogTalk.erase(m_mapDialogTalk.begin(),m_mapDialogTalk.end());
}

void CGameStoryLayer::createDialogLayer()
{
    CCPoint  labelPoint;
    CCSprite *lSprite=CCSprite::create("Icon.png");
    addChild(lSprite,1,100);
    lSprite->setVisible(false);
    
    
    CCSprite *storyLayer=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"storyduihuakuang.png"));
    
    float x=(wndSize.width-storyLayer->getContentSize().width)*0.5-20;
    storyLayer->setPosition(ccp(x,180));
    storyLayer->setVisible(false);
    //storyLayer->setAnchorPoint(ccp(0.5,0.5));
    addChild(storyLayer,2,101);
    labelPoint.x=storyLayer->getContentSize().width*0.5;
    labelPoint.y=storyLayer->getContentSize().height*0.5;
    
    
    CCLabelTTF *llabelTTF=CCLabelTTF::create("123", "Arial", 15);
    storyLayer->addChild(llabelTTF,1,102);
    llabelTTF->setPosition(labelPoint);
    llabelTTF->setDimensions(CCSizeMake(400, 120));
    llabelTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
    CCSprite *spriteshouzhi=CCSprite::create("resource_cn/img/storygif-1.png");
    storyLayer->addChild(spriteshouzhi,2,103);
    CCAnimation* animation = CCAnimation::create();
    for( int i=1;i<=2;i++)
    {
            char szName[100] = {0};
            sprintf(szName, "resource_cn/img/storygif-%d.png", i);
            animation->addSpriteFrameWithFileName(szName);
    }
    labelPoint.x=storyLayer->getContentSize().width-50;
    labelPoint.y=20;
    spriteshouzhi->setPosition(labelPoint);
    //设置每两帧间时间间隔为1秒。
       animation->setDelayPerUnit(0.2f);
    //设置动画结束后仍保留动画帧信息。
    animation->setRestoreOriginalFrame(true);
    //由这个动画信息创建一个序列帧动画。
    CCAnimate* action = CCAnimate::create(animation);
    spriteshouzhi->runAction(CCRepeatForever::create(action));
        
}


//每个界面的对话框设置 是显示2 个对话框。 第一个的tag 是100， 第二个是200
void  CGameStoryLayer::displayDialog(CGameTalkDialog *gameDialog)
{
    isCaneTouch=false;
    if(m_pRoleAnimation)
    {
        m_pRoleAnimation->runActionAnimation(this, gameDialog, m_oldGameDialog);
    }
    m_oldGameDialog=gameDialog;
}

void CGameStoryLayer::updateEveryAnimationPlayEnd(float t)
{
        if(isCaneTouch)
        {
           // isCaneTouch=false;
            endTalk();
            isCaneTouch=false;
        }
}

bool CGameStoryLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(getChildByTag(1000)->boundingBox().containsPoint(pTouch->getLocation()))
    {
            ((CCSprite *)getChildByTag(1000))->setTexture( CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Pressed.png")));
    }
    else{
        ((CCSprite *)getChildByTag(1000))->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png")));
    }
    return  true;
}

void CGameStoryLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(getChildByTag(1000)->boundingBox().containsPoint(pTouch->getLocation()))
    {
       ((CCSprite *)getChildByTag(1000))->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Pressed.png")));
    }
    else{
        ((CCSprite *)getChildByTag(1000))->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png")));
    }
}
void CGameStoryLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(getChildByTag(1000)->boundingBox().containsPoint(pTouch->getLocation()))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        ((CCSprite *)getChildByTag(1000))->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png")));
        if(!isKuaiJingZhuangTai)
        {
            
            isKuaiJingZhuangTai=true;
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(2.0f);
            schedule(schedule_selector(CGameStoryLayer::updateEveryAnimationPlayEnd));
            CCLog("快进 ing========");
        }
        else
        {
            isKuaiJingZhuangTai=false;
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
            unschedule(schedule_selector(CGameStoryLayer::updateEveryAnimationPlayEnd));
            CCLog("111快进 ing========");
        }

    }
    else
    {
        
        if(!isKuaiJingZhuangTai)
        {
            PtSoundTool::playSysSoundEffect("UI_click.wav");
            endTalk();
        }
    }
}
void CGameStoryLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameStoryLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
void CGameStoryLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
string CGameStoryLayer::getStoryTalkList(int storyID)
{
    CGameStory *data=new CGameStory;
    data->initGameStoryData();
    string dialogTalkId=data->getDialogTalkId(storyID);
    delete data;
    data=NULL;
    return dialogTalkId;
}

void CGameStoryLayer::createKuaiJing()
{
    ///Users/linminglu/Documents/CubeCOCO/proj.android/cn.cube/assets/resource_cn/img/Skip_Normal.png
    CCTexture2D *texture=CCTextureCache::sharedTextureCache()->textureForKey(CSTR_FILEPTAH(g_mapImagesPath,"Skip_Normal.png"));
    CCSprite *sprite=CCSprite::createWithTexture(texture);
    addChild(sprite,900,1000);
    sprite->setPosition(ccp(1000,200));
    
}
