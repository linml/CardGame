//
//  CPVPRankLayer.cpp
//  91.cube
//
//  Created by phileas on 14-1-14.
//
//

#include "PVPSceneLayer.h"
#include "CPVPRankLayer.h"
#include "gameConfig.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
#include "CSceneShowOtherUidTeam.h"

CPVPRankLayer::CPVPRankLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));

    m_pAllRankData = NULL;
    m_nRankCount = 0;
}
CPVPRankLayer::~CPVPRankLayer()
{
    PVPDataManager::getInstance()->releasAllRankInfo();
}
bool CPVPRankLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCPVPRankLayer();
        bRet =true;
    } while (0);
    return bRet;
}
bool CPVPRankLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    if (CPtTool::isInNode(m_pQuitBtn, pTouch))
    {
        m_nTouchTag = PVPRANKQUITBTN_TAG;
    }
    return true;
}
void CPVPRankLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPRankLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pQuitBtn, pTouch)&& m_nTouchTag == PVPRANKQUITBTN_TAG)
    {
        onClickQuit();
    }
}
void CPVPRankLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPRankLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int index = cell->getIdx();
    if (m_pAllRankData && index < m_nRankCount)
    {
       int uid = m_pAllRankData[index].uid;
        CSceneShowOtherUidTeam *layer = CSceneShowOtherUidTeam::CreateLayer(uid);
        layer->setTouchPriority(PVPSCENETOUCH_PRIORITY - 3);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
    }
}


void CPVPRankLayer::initCPVPRankLayer()
{
    //create bg:
    CCSize size(CCDirector::sharedDirector()->getWinSize());
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "niudanbeijing.png"));
    bg->setPosition(ccp(size.width*0.5, size.height*0.5));
    size = bg->getContentSize();
    addChild(bg);
    CCSprite* quitBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "QuitButton_Normal.png"));
    quitBtn->setAnchorPoint(ccp(1,1));
    quitBtn->setPosition(ccp(size.width-20, size.height-20));
    bg->addChild(quitBtn);
    m_pQuitBtn = quitBtn;
    m_pBg = bg;
    
    PVPDataManager *dataManager = PVPDataManager::getInstance();
    dataManager->getPVPRankInfo(this, callfunc_selector(CPVPRankLayer::callBack));
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    
}

CCNode *CPVPRankLayer::createRankItem(const PVPRankData * inData)
{
    CCSize size = CCSizeMake(400, 100);
    CCNode *node = CCLayerColor::create(ccc4(123, 123, 0, 123), size.width,size.height);  //CCNode::create();
   
    node->setContentSize(size);
    if (inData)
    {
      
        char buffer[50]={0};
        CCPoint point(size.width*0.05, size.height*0.5);
        int uid = inData->uid;
        if (inData->rank == NOINRANKTAG)
        {
            snprintf(buffer, sizeof(buffer), "-");
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%d", inData->rank);
        }
        
        CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial-BLOD", 30);
        //排名
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(ccp(0,0.5));
        label->setPosition(point);
        node->addChild(label);
        
        //头像
        sprintf(buffer, "%d.png", (uid%5)+1);
        CCSprite * image = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, buffer));
        point.x = size.width * 0.215;
        image->setPosition(point);
        node->addChild(image);
        
        // info:
        point.x = size.width *0.325;
        point.y = size.height*0.65;
        
        snprintf(buffer, sizeof(char)*30, "等级: %d", inData->level);
        label = CCLabelTTF::create(buffer, "Arail", 13);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(CCPointZero);
        label->setPosition(point);
        node->addChild(label);
        
        point.x = size.width *0.525;
        snprintf(buffer, sizeof(char)*30, "昵称: %s", inData->name.c_str());
        label = CCLabelTTF::create(buffer, "Arail", 13);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(CCPointZero);
        label->setPosition(point);
        node->addChild(label);
        
        point.y = size.height*0.35;
        snprintf(buffer, sizeof(char)*30, "战力: %d", inData->fightpointer);
        label = CCLabelTTF::create(buffer, "Arail", 13);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(CCPointZero);
        label->setPosition(point);
        node->addChild(label);
        
        point.x = size.width *0.325;
        snprintf(buffer, sizeof(char)*30, "积分: %d", inData->credits);
        label = CCLabelTTF::create(buffer, "Arail", 13);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(CCPointZero);
        label->setPosition(point);
        node->addChild(label);
        
        int rank = inData->rank > 3 ? 4: inData->rank;
        sprintf(buffer, "medal_1_%d.png", rank);
        image = CCSprite::create(CSTR_FILEPTAH(g_rankIconPath, buffer));
        point.x = size.width * 0.875;
        point.y = size.height*0.5;
        image->setPosition(point);
        node->addChild(image);
        
        return node;

    }
    return node;
}

CCNode * CPVPRankLayer::createPVPRankList(const PVPRankData *inData, int inSize)
{
    CCArray *array = CCArray::create();
    if (inData)
    {
        CPtTableItem *item = NULL;
        CCNode *display = NULL;
        for (int i = 0; i < inSize; i++)
        {
            display = createRankItem(inData+i);
            item = CPtTableItem::create();
            item->setDisplayView(display);
            array->addObject(item);
        }
    }
    CPtListViewWidget *listView = CPtListViewWidget::create(array, CCSizeMake(420, 340));
    listView->getTableView()->setDelegate(this);
    listView->setBackGround(CCLayerColor::create(ccc4(123, 0, 0, 125)));
    listView->getTableView()->setTouchPriority(PVPSCENETOUCH_PRIORITY - 2);
    return listView;
}

void CPVPRankLayer::callBack()
{
    PVPDataManager *dataManager = PVPDataManager::getInstance();
   
    int allRankDataCount = 0;
    PVPRankData *allRankData = dataManager->getAllRankInfo(allRankDataCount);
    m_nRankCount = allRankDataCount;
    CCSize size = m_pBg->getContentSize();
    PVPRankData *data = dataManager->getPVPUserRankInfo();
    
    CCNode * node = createPVPRankList(allRankData, allRankDataCount); //createRankItem(&data);
    node->setAnchorPoint(ccp(0,1));
    node->ignoreAnchorPointForPosition(false);
    node->setPosition(ccp(180, size.height-80));
    m_pBg->addChild(node);
    node = createRankItem(data);
    node->setPosition(ccp(180, 20));
    m_pBg->addChild(node);
    m_pAllRankData = allRankData;
}

void CPVPRankLayer::onClickQuit()
{
    removeFromParentAndCleanup(true);
}
