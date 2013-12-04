//
//  CGameRankLayer.cpp
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#include "CGameRankLayer.h"
#include "gameStruct.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"

#define TAG_TABBEGIN 1000
#define TAG_FRIENDCOUNT 12
#define TAG_BOTTOMALLBUTTON 14
#define TAG_QUITBUTTON 20
#define TAG_TABLEVIEW 2
#define TAG_BACKGROUDMAP 1
class CGameRankData
{
public:
    CGameRankData(string name,int value){this->gamePlayerName=name;this->value=value;};
    string gamePlayerName;
    int   value;
};

CGameRankLayer::CGameRankLayer()
{
    m_currentTabIndex=-1;
    m_vTableViewData.clear();
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    size=CCDirector::sharedDirector()->getWinSize();
    m_tabs = new CCArray();
    if (m_tabs && m_tabs->init())
    {
        m_tabs->retain();
    }
    else
    {
        CC_SAFE_DELETE(m_tabs);
    }
    tempSprite=CCSprite::create((g_mapImagesPath+"rankpagecell.png").c_str());
    tempSprite->retain();
    
    {
        char data[20];

        for (int i=100; i>=1; i--) {
            sprintf(data, "gameplayername%d",100-i);
            m_vTableViewData.push_back(new CGameRankData(data,i+10000));
        }
    }
}

CGameRankLayer::~CGameRankLayer()
{
    
    {
        for (int i=0; i<m_vTableViewData.size(); i++) {
            delete m_vTableViewData[i];
            m_vTableViewData[i]=NULL;
        }
    }
    
    
    CC_SAFE_RELEASE(tempSprite);
    CC_SAFE_RELEASE(m_tabs);
    CEmrysClearVectorMemory<CGameRankData *>m_vData(m_vTableViewData);
    m_vData.clearVector();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}

void CGameRankLayer::callBackFunc(void)//让对方去回调的数据;
{
    
}

bool CGameRankLayer::init()
{
    if (!CCLayer::init()) {
        return  false;
    }
    createBackGround();
    createRankTypeButton();
    createQuitButton();
    setGunDongTiaoPtr();
    setTouchEnabled(true);
    setTouchPriority(-9);
   if(switchToTab(0))
   {
       onClickZongZhandouTabItem();
   }
    return true;
}

void CGameRankLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_BACKGROUDMAP);

}



void CGameRankLayer::createRankTypeButton()
{
    
    const string str[3]={Utility::getWordWithFile("word.plist", "zongzhandoulipaihang"),Utility::getWordWithFile("word.plist", "dengjipaihang"),Utility::getWordWithFile("word.plist", "fubushipaihang")};
    for (int i=0; i<3; i++) {
        addTab(str[i].c_str(),i);
    }
    
}

CCPoint CGameRankLayer::getTabPositionByIndex(int index)
{
    return CCPointMake(size.width *0.5+index*130-280, size.height *0.5+210);
}

bool  CGameRankLayer::addTab(const char* label,int index)
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, label, "tabButton_normal.png", "tabButton_selected.png");
    addChild(gameButton,2,TAG_TABBEGIN+index);
    gameButton->setPosition(getTabPositionByIndex(index));
    m_tabs->addObject(gameButton);
    return true;
}
void CGameRankLayer::onClickZongZhandouTabItem()
{
    if(getChildByTag(TAG_TABLEVIEW))
    {
        CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
        tableView->reloadDataView();
    }
    else{
        createTableView(0);
    }
}

void CGameRankLayer::onClickDengJiTabItem()
{
    if(getChildByTag(TAG_TABLEVIEW))
    {
        CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
        tableView->reloadDataView();
    }
    else{
        createTableView(0);
    }

}

void CGameRankLayer::onClickFuhaoTabItem()
{
    if(getChildByTag(TAG_TABLEVIEW))
{
    CUtilityTableView *tableView=(CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW));
    tableView->reloadDataView();
}
else{
    createTableView(0);
}
    
}

bool CGameRankLayer::switchToTab(int index)
{
    
    if(m_currentTabIndex !=-1 && m_currentTabIndex == index)
    {
        return false;
    }
    if (m_currentTabIndex!=-1)
    {
        CGameButtonControl *pTempButton=((CGameButtonControl *)m_tabs->objectAtIndex(m_currentTabIndex));
        if(pTempButton)
        {
            pTempButton->unselected();
        }
    }
    CGameButtonControl *pTempButton=((CGameButtonControl *)m_tabs->objectAtIndex(index));
    if(pTempButton)
    {
        pTempButton->selected();
    }
    m_currentTabIndex=index;
    return true;
}

void CGameRankLayer::setGunDongTiaoPtr()
{
    m_pScrollViewGuanDongTiao=(CCSprite *)Utility::getNodeByTag(this, "1,1,7,0");
    m_fOldScrollBarPosiontYtop=m_pScrollViewGuanDongTiao->getPosition().y;
    m_foldscrollBarPosiontylow=Utility::getNodeByTag(this, "1,1,7,26")->getPosition().y;
}

void CGameRankLayer::scrollViewBar(CCTableView* table)
{
    CCNode* bar = Utility::getNodeByTag(this, "1,1,7,40");
    if(bar == NULL)
    {
        return;
    }
    
    CCSize tableSize = table->getContentSize();
    CCSize tableViewSize = table->getViewSize();
    
    CCPoint contOffsetPos = table->getContentOffset();
    float maxOff = tableViewSize.height - tableSize.height;
    //CCLog("tableViewSize.height:%f,tableSize.height:%f,table->getContentOffset():%f",tableViewSize.height,tableSize.height,table->getContentOffset().y);
    float curOff = contOffsetPos.y - maxOff;
    float percentage = fabs(curOff)/fabs(maxOff);
    printf("curOff:%f, maxOff:%f, per:%f\n", curOff, maxOff, percentage);
    if(percentage < 0.1f)
    {
        percentage = 0;
    }
    if(percentage > 1.0f)
    {
        percentage = 1.0f;
    }
    float barTopPosY = m_fOldScrollBarPosiontYtop;
    float barLowPosY = m_foldscrollBarPosiontylow;
    float h = barTopPosY - percentage*(barTopPosY- barLowPosY);
    bar->setPosition(ccp(bar->getPosition().x, h));
}

void CGameRankLayer::tableScrolBarView(CCTableView *view)
{
     scrollViewBar((CCTableView*)view);
}

void CGameRankLayer::tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch)
{
    //判断具体点击哪一个按钮
}

void CGameRankLayer::scrollViewDidScroll(CCScrollView* view)
{
      scrollViewBar((CCTableView*)view);
}

void CGameRankLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CGameRankLayer::createTableView(int)
{
    if (!getChildByTag(TAG_TABLEVIEW)) {
        CCPoint p=ccp(200,150);
        CCSize s=CCSizeMake(650, 400);
        m_pTableCellSize=CCSizeMake(650, 100);
        CUtilityTableView * tableView = CUtilityTableView::Create(this, s,this);
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(p);
        tableView->setDelegate(this);
        tableView->setTouchPriority(-10);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        this->addChild(tableView,2,TAG_TABLEVIEW);
    }
    else{
        ((CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW)))->reloadDataView();
    }
    
}

void CGameRankLayer::removeTableView(int)
{
    if (getChildByTag(TAG_TABLEVIEW)) {
        removeChildByTag(TAG_TABLEVIEW, true);
    }
    
}

void CGameRankLayer::reloadTableViewData(int index)
{
    if (getChildByTag(TAG_TABLEVIEW)) {
        ((CUtilityTableView *)(getChildByTag(TAG_TABLEVIEW)))->reloadDataView();
    }
}

unsigned int CGameRankLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vTableViewData.size();
}

void CGameRankLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    CCSprite *sprite=CCSprite::createWithTexture(tempSprite->getTexture());
    sprite->setPosition(CCPointMake(0, 0));
    sprite->setAnchorPoint(CCPointMake(0, 0));
    cell->addChild(sprite,0);
    string imgIcon=ConvertToString(idx%5+1)+".png";
    CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, imgIcon));
    cell->addChild(icon);
    icon->setPosition(CCPointMake(50, 50));
    string strName;
    switch (m_currentTabIndex) {
        case 0:
        {
           strName="玩家战斗力";
            
        }
            break;
        case 1:
            strName="玩家成就";
            break;
        case 2:
            strName="消费积分";
            break;
        default:
            break;
    }
    CCLabelTTF *labelTTF=CCLabelTTF::create(("玩家姓名:"+m_vTableViewData[idx]->gamePlayerName).c_str(), "Arial", 15);
    labelTTF->setPosition(CCPointMake(150, 40));
    labelTTF->setAnchorPoint(CCPointZero);
    cell->addChild(labelTTF);
    labelTTF=CCLabelTTF::create((strName +ConvertToString(m_vTableViewData[idx]->value)).c_str(), "Arial", 20);
    labelTTF->setAnchorPoint(CCPointZero);
    labelTTF->setPosition(CCPointMake(150, 10));
    cell->addChild(labelTTF);
}

CCTableViewCell* CGameRankLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        this->initCellItem(cell, idx);
    }
    else
    {
        scrollViewBar(table);
    }
    return cell;
}

CCSize CGameRankLayer::cellSizeForTable(CCTableView *table)
{
    return m_pTableCellSize;
}

void CGameRankLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

//CCSize CGameRankLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
//{
//    
//}



bool CGameRankLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CGameRankLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CGameRankLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));
    
}
int CGameRankLayer::checkTouchQuitButton(CCPoint point)
{
    if (getChildByTag(TAG_QUITBUTTON)) {
        CGameButtonControl *pGameButton=(CGameButtonControl *)getChildByTag(TAG_QUITBUTTON);
        if(pGameButton->boundingBox().containsPoint(point))
        {
            return TAG_QUITBUTTON;
        }
    }
    return -1;
}

int CGameRankLayer::checkTouchTableIndex(CCPoint point)
{
    CCObject *tempButton=NULL;
    CCARRAY_FOREACH(m_tabs, tempButton)
    {
        if (tempButton) {
            if(((CGameButtonControl *)tempButton)->boundingBox().containsPoint(point))
            {
                return ((CGameButtonControl *)tempButton)->getTag();
            }
        }
    }
    return -1;
}
void CGameRankLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point =pTouch->getLocation();
    int tag;
    if ((tag=checkTouchTableIndex(point))!=-1)
    {
        handleTagCallBack(tag);
    }
    else if(checkTouchQuitButton(point)!=-1)
    {
        removeFromParentAndCleanup(true);
    }
}

void CGameRankLayer::handleTagCallBack(int tag)
{
    switch (tag) {
        case TAG_TABBEGIN:
            if(switchToTab(0))
            {
                onClickZongZhandouTabItem();
            }
            break;
        case TAG_TABBEGIN+1:
            if(switchToTab(1))
            {
                onClickDengJiTabItem();
            }
            break;
        case TAG_TABBEGIN+2:
            if(switchToTab(2))
            {
                onClickFuhaoTabItem();
            }
            break;
        default:
            break;
    }
}

void CGameRankLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameRankLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
void CGameRankLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
