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

CGameStoryLayer::CGameStoryLayer()
{
    m_oldGameDialog=NULL;
    wndSize=CCDirector::sharedDirector()->getWinSize();
}

CGameStoryLayer::~CGameStoryLayer()
{
    
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
    node->addChild(this);
    schedule(schedule_selector(CGameStoryLayer::updateTimeToShow), 1.0f);
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

void CGameStoryLayer::endTalk()
{
    m_vDialogTalkIndex++;
    m_bIsEndDialog=true;
}

void CGameStoryLayer::loadDialogList()
{
    map<int , CGameTalkDialog *>m_mapDialogTalk;
    CCDictionary *directory = CCDictionary::createWithContentsOfFile((resRootPath+"storydialog.plist").c_str());
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
        gameDialogTalk->setGameTalkDialogPng(GameTools::valueForKey("png", storyDialogTalk));
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
//每个界面的对话框设置 是显示2 个对话框。 第一个的tag 是100， 第二个是200
void  CGameStoryLayer::displayDialog(CGameTalkDialog *gameDialog)
{
    
    if(m_oldGameDialog)
    {
        if(m_oldGameDialog->getGameTalkUiPlan()%2==0)
        {
            //如果是偶数 那么是右边的。 如果是技术 那么是左边的
            
            CCNode *node=getChildByTag(200);
            CCPoint point=node->getPosition();
            node->runAction(CCMoveTo::create(0.2f, ccp(point.x,600)));
            node=getChildByTag(100);
            if(node)
            {
                removeChild(node, true);
            }

        }
        else{
            CCNode *node=getChildByTag(100);
            CCPoint point=node->getPosition();
            node->runAction(CCMoveTo::create(0.2f, ccp(point.x,600)));
            node=getChildByTag(200);
            if(node)
            {
                removeChild(node, true);
            } 
        }
    }
    
    
    
 
    if(m_oldGameDialog!=gameDialog)
    {
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, gameDialog->getGameTalkDialogPng().c_str()));
        addChild(sprite,1,101); //该东西 不需要移动
        CCLayerColor *colordialog=CCLayerColor::create(ccc4(125, 125, 125, 125));
        addChild(colordialog,1,100);
        colordialog->setContentSize(CCSizeMake(800, 200));
        colordialog->setPosition(ccp(wndSize.width*0.5,200));
        CCLabelTTF *labelttf=CCLabelTTF::create(gameDialog->getGameTalkDialogWord().c_str(), "Arial", 100);
        colordialog->addChild(labelttf);
 
    }
    else{
        
    }
    m_oldGameDialog=gameDialog;
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
    endTalk();
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
