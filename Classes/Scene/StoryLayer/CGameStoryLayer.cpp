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
}

CGameStoryLayer::~CGameStoryLayer()
{
    if(m_pRoleAnimation)
    {
        delete m_pRoleAnimation;
        m_pRoleAnimation=NULL;
    }
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
    
}

CGameStoryLayer *CGameStoryLayer::CreateStoryLayer(int storyId,CCNode *node)
{
    CGameStoryLayer *gameStory=new CGameStoryLayer();
    if(!gameStory || !gameStory->initCreateStory(storyId, node))
    {
        delete gameStory;
        gameStory=NULL;
        return NULL;
    }
    gameStory->autorelease();
    return gameStory;
    
}


bool CGameStoryLayer::initCreateStory(int storyId, cocos2d::CCNode *node)
{
    string str=getStoryTalkList(storyId);
    m_vDialogTalk= transToIntVector(splitString(str.c_str(), ","));
    m_vDialogTalkIndex=0;
    m_bIsEndDialog=true;
    createKuaiJing();
    loadDialogList();
    createDialogLayer();
    if(node)
    {
        node->addChild(this);
    }
    else{
        CCDirector::sharedDirector()->getRunningScene()->addChild(this);
    }
    isCaneTouch=false;
    schedule(schedule_selector(CGameStoryLayer::updateTimeToShow), 1.0f);
    setTouchEnabled(true);
    return true;
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
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *storyDialogTalk=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CGameTalkDialog *gameDialogTalk=new CGameTalkDialog ;
        gameDialogTalk->setGameTalkID(GameTools::intForKey("id", storyDialogTalk));
        gameDialogTalk->setGameTalkUiPlan(GameTools::intForKey("plan", storyDialogTalk));
        gameDialogTalk->setGameTalkDialogWord(GameTools::valueForKey("word", storyDialogTalk));
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
    CCSprite *lSprite=CCSprite::create("resource_cn/img/fighting/card_res_002_000.png");
    addChild(lSprite,1,100);
    lSprite->setVisible(false);
    CCLayerColor *lcolor=CCLayerColor::create(ccc4(125, 125, 125, 125), 600, 150);
    float x=(wndSize.width-lcolor->getContentSize().width)*0.5-20;
    lcolor->setPosition(ccp(x,180));
    lcolor->setVisible(false);
    lcolor->setAnchorPoint(ccp(0.5,0.5));
    addChild(lcolor,1,101);
    labelPoint.x=lcolor->getContentSize().width*0.5;
    labelPoint.y=lcolor->getContentSize().height*0.5;
    CCLabelTTF *llabelTTF=CCLabelTTF::create("123", "Arial", 25);
    lcolor->addChild(llabelTTF,1,102);
    llabelTTF->setPosition(labelPoint);
    llabelTTF->setDimensions(CCSizeMake(600, 150));
    llabelTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
    
        
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
    return  true;
}

void CGameStoryLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CGameStoryLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(getChildByTag(1000)->boundingBox().containsPoint(pTouch->getLocation()))
    {
        if(!isKuaiJingZhuangTai)
        {
            isKuaiJingZhuangTai=true;
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(2.0f);
            schedule(schedule_selector(CGameStoryLayer::updateEveryAnimationPlayEnd), 0.5f);
            CCLog("快进 ing========");
        }
        else
        {
            isKuaiJingZhuangTai=false;
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
            unschedule(schedule_selector(CGameStoryLayer::updateEveryAnimationPlayEnd));
        }

    }
    else
    {
        if(!isKuaiJingZhuangTai)
        {
            endTalk();
        }
    }
}
void CGameStoryLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameStoryLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -9, true);
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
    CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "kuaijin.png"));
    addChild(sprite,900,1000);
    sprite->setPosition(ccp(1000,200));
    
}
