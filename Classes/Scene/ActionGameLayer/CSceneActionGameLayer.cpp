//
//  CSceneActionGameLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#include "CSceneActionGameLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "CPanelLoadingLayer.h"
#include "PtJsonUtility.h"
#include "gameTools.h"

CSceneActionGameLayer::CSceneActionGameLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "huodongjiemian.plist"));
}

CSceneActionGameLayer::~CSceneActionGameLayer()
{
   CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_plistPath, "huodongjiemian.plist"));
    
}

bool CSceneActionGameLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    createBackGroud();
    createLoadingLayer();
    //延迟0.2秒 调用http 去刷新服务
    scheduleOnce(schedule_selector(CSceneActionGameLayer::sendHttpToGetAction), 0.2);
    return true;
}




void CSceneActionGameLayer::createBackGroud()
{
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
}



void CSceneActionGameLayer::createLoadingLayer()
{
    CCLayer *layer=CPanelLoadingLayer::create();
    addChild(layer,1,99);
    
}

void CSceneActionGameLayer::removeLoadingLayer()
{
    if (getChildByTag(99))
    {
        removeChildByTag(99, true);
    }
}

void CSceneActionGameLayer::sendHttpToGetAction(float t)
{
#ifdef DDEBUG__
   // char *data=new char[20];
  //  decodeHttpToGetAction((CCObject *)data);
#else
    const  char *data=CPtTool::readFileName((resRootPath +"cardrandomtxt.txt").c_str()).c_str();
    char *tt=new char[strlen(data+1)];
    memcpy(tt, data, strlen(data));
    decodeHttpToGetAction((CCObject *)data);
#endif
    
}

void CSceneActionGameLayer::decodeHttpToGetAction(cocos2d::CCObject *object)
{
    if (!object ) {
        CCMessageBox("服务端传输的是", "error");
        return ;
    }
    //解析字符串。 赋值与vector中
    delete [] (char *)object;
    CCDictionary *dict=PtJsonUtility::JsonStringParse((char *)object);
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *result=(CCDictionary *)dict->objectForKey("result");
    if (result) {
        
    }
    reloadTableView();
}

void CSceneActionGameLayer::createTableView()
{
    CCTableView *tableView = CCTableView::create(this, CCSizeMake(200, 500));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(CCPointMake(20, 100));
    tableView->setDelegate(this);
    tableView->setTouchPriority(-125);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,999);
    
}

void CSceneActionGameLayer::removeTableView()
{
    if (getChildByTag(999))
    {
        removeChildByTag(999, true);
    }
    
}

void CSceneActionGameLayer::reloadTableView()
{
    if (!getChildByTag(999)) {
        createTableView();
    }
    ((CCTableView *)getChildByTag(999))->reloadData();
    
}

CCSize CSceneActionGameLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(200, 50);
}

CCTableViewCell* CSceneActionGameLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        initCellItem(cell, idx);
    }
    else
    {
        //scrollBar(table);
    }
    return cell;
}

unsigned int CSceneActionGameLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vActionList.size();
}

void CSceneActionGameLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

void CSceneActionGameLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CSceneActionGameLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

void CSceneActionGameLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
//    if (idx<m_vActionList.size()) {
//        if (m_vActionList[idx]) {
//            CCSprite *sprite=CCSprite::
//        }
//    }
}