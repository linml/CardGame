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
#include "CStructGameActionData.h"
#include "Utility.h"
#include "CActionItemLayer.h"
#include "gameStruct.h"
#include "CPtTool.h"

#define GOLDPLACE_TOUCH_PRORITY -2


#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}

/*
 * @function: init data & preload texture resource
 */
CSceneActionGameLayer::CSceneActionGameLayer()
{
    m_fOffsetX = 0.0f;
    size=CCDirector::sharedDirector()->getWinSize();
    m_enStatus=EN_CSceneActionGameLayerStatus_NONE;
    m_pBackData=NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "huodongjiemian.plist"));
}

CSceneActionGameLayer::~CSceneActionGameLayer()
{
    CCLog("~CSceneActionGameLayer");
    DELETE_POINT_VECTOR(m_vActionList,vector<CStructGameActionData *>,CStructGameActionData);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "huodongjiemian.plist"));
    
}

bool CSceneActionGameLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    createBackGroud();
    m_enStatus=EN_CSceneActionGameLayerStatus_RUNNING;
    //延迟0.2秒 调用http 去刷新服务
    scheduleOnce(schedule_selector(CSceneActionGameLayer::sendHttpToGetAction), 0.2);
    setTouchEnabled(true);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-1);
    return true;
}




void CSceneActionGameLayer::createBackGroud()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
    this->addChild(layer,1,1);
    CCSprite *tableViewBackGroud=CCSprite::createWithSpriteFrameName("huodongdiban.png");
    addChild(tableViewBackGroud,2,1000);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    tableViewBackGroud->setPosition(ccp(size.width*0.5-200, size.height *0.5-30));
}



void CSceneActionGameLayer::createLoadingLayer()
{
    CCLayer *layer=CPanelLoadingLayer::create();
    addChild(layer,2,99);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    layer->setPosition(ccp(size.width*0.5,size.height*0.5));
}

void CSceneActionGameLayer::removeLoadingLayer()
{
    if (getChildByTag(99))
    {
        removeChildByTag(99, true);
    }
}

void CSceneActionGameLayer::createShowNoHaveActivity()
{
    if(!getChildByTag(97))
    {
        CCLabelTTF *labelTTF=CCLabelTTF::create("活动暂未开启", "Arail", 45);
        addChild(labelTTF,20,97);
        labelTTF->setPosition(ccp(size.width *0.5,size.height *0.5));
    }
    
}
void CSceneActionGameLayer::removeShowNoHaveActivity()
{
    if (getChildByTag(97)) {
        removeChildByTag(97, true);
    }
}
void CSceneActionGameLayer::sendHttpToGetAction(float t)
{
// 从服务端获取信息
#ifndef DDEBUG__
    removeShowNoHaveActivity();
    createLoadingLayer();
    m_enStatus=EN_CSceneActionGameLayerStatus_RUNNING;
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_ACTIVITY(194), "CALLBACK_CSceneActionGameLayer_sendHttpToGetAction", "REQUEST_CALLBACK_CSceneActionGameLayer_sendHttpToGetAction",postStrdata.c_str(),callfuncO_selector(CSceneActionGameLayer::decodeHttpToGetAction));
#else
// 从本地获取信息
    const  char *data=CPtTool::readFileName((resRootPath +"cc.txt").c_str()).c_str();
    char *tt=new char[strlen(data+1)];
    memcpy(tt, data, strlen(data));
    decodeHttpToGetAction((CCObject *)tt);
#endif
    
}

void CSceneActionGameLayer::decodeHttpToGetAction(cocos2d::CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneActionGameLayer_sendHttpToGetAction");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
        return ;
    }
    char * tempdata=(char *)object;
    //解析字符串。 赋值与vector中
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    CCLog("tempdata:%s",tempdata);
    delete [] tempdata;
    tempdata=NULL;
    
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    removeLoadingLayer();
    std::string typeName = typeid(*dict->objectForKey("result")).name();
    if (typeName.find("CCArray") != std::string::npos)
    {
        createShowNoHaveActivity();
        m_enStatus=EN_CSceneActionGameLayerStatus_END;
        return ;
    }
    m_vActionList.clear();
    CCDictionary *result=(CCDictionary *)dict->objectForKey("result");
    if (result) {
        CCDictElement *element = NULL;
        CCDictionary *tmpDict = NULL;
        CCDICT_FOREACH(result, element)
        {
            tmpDict =(CCDictionary*) element->getObject();
            if (tmpDict) {
                CStructGameActionData *pSGameAction=new CStructGameActionData;
                pSGameAction->setDataValue(tmpDict);
                m_vActionList.push_back(pSGameAction);
            }
            
        }
        if (m_vActionList.size()>0)
        {
            reloadTableView();
        }
        else{
            createShowNoHaveActivity();
        }
        m_enStatus=EN_CSceneActionGameLayerStatus_END;
    }
    
}

/*
 * @breif : 创建活动展示界面
 * @note : 使用tableview 时，当内容窗口比视窗小的使用，出现滑动问题
 */
#include "CPtListViewWidget.h"
void CSceneActionGameLayer::createTableView()
{
    CCLog("Creata Table View");
//    int count= 6;//m_vActionList.size()>6?6:m_vActionList.size();

    CCTableView *tableView = CCTableView::create(this, CCSizeMake(250,400));
//    TableView *tableView = TableView::create(this, CCSizeMake(250,400));
//    tableView->addBackground(CCLayerColor::create(ccc4(125,0, 0, 125)));


    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0.0));
    tableView->setPosition(ccp(190,145));
    tableView->setDelegate(this);
    tableView->setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
  
    this->addChild(tableView,2,999);
}

void CSceneActionGameLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    if (idx<m_vActionList.size())
    {
        if (m_vActionList[idx])
        {
            CCSprite *sprite=CCSprite::createWithSpriteFrameName("biaotiban.png");
            sprite->setAnchorPoint(CCPointZero);
            CCLabelTTF *labelTTF=CCLabelTTF::create(m_vActionList[idx]->getActionName().c_str(), "Arial", 15);
            float pointX=sprite->getContentSize().width*0.5+sprite->getPosition().x;
            float pointY=sprite->getContentSize().height*0.5+sprite->getPosition().y;
            labelTTF->setPosition(ccp(pointX,pointY));
            sprite->addChild(labelTTF);
            cell->addChild(sprite,1,idx+100);
           // sprite->setPosition(ccp(120,25));
        }
    }
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

// implement interface of CCTableViewDataSource,CCTableViewDelegate

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
    int value =cell->getIdx();
    CCLog("you touch cell at %d", value);
    createRightLayer(m_vActionList[value]);
    
}

void CSceneActionGameLayer::adjustScrollView(float distance)
{
    if (!getScrollView()||m_bTouchScrollView==false) {
        return ;
    }
    //该地方逻辑需要重新构建
    CCPoint adjustPoint;
    CCLOG("getScrollView()->getContentOffset().y:%f",getScrollView()->getContentOffset().y);
    if(getScrollView()->getContentOffset().y >=50.0f)
    {
       
        CCLayer *layer=(CCLayer *)(getScrollView()->getContainer());
        CCLOG("%f,%f,%f", getScrollView()->getContentOffset().y , distance ,layer->getContentSize().height);
        if(distance >0 &&(int)( getScrollView()->getContentOffset().y +  + getScrollView()->getViewSize().height) > (int)layer->getContentSize().height)
        {
            adjustPoint=ccp(m_fOffsetX,layer->getContentSize().height-getScrollView()->getViewSize().height*0.5);
            getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
        }
        else if(distance<0)
        {
            if(getScrollView()->getContentOffset().y-50<=0.0)
            {
                adjustPoint=ccp(m_fOffsetX,0);
            }
            else{
                adjustPoint=ccp(m_fOffsetX,getScrollView()->getContentOffset().y-50);
            }
            
            getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
        }
    }
    else{
        adjustPoint=ccp(m_fOffsetX,0);
        CCLOG("%f",adjustPoint.y);
        getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
    }
}

void CSceneActionGameLayer::createRightLayer(CStructGameActionData *data)
{
    if(m_pBackData==data)
    {
        return;
    }
    if (m_pBackData && m_pBackData!=data) {
        removeChildByTag(101, true);
    }
    CCScrollView *view=CCScrollView::create(CCSizeMake(400, 420),CCLayerColor::create(ccc4(130,0 , 0, 220)));
    //m_pScrollView= cocos2d::extension::CCScrollView::create(CCSizeMake(850, 670));
    CCLayer *layer=CActionItemLayer::Created(data);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(CCPointMake(0, 1.0));
    m_fOffsetX = -70;
   
    
    layer->setPositionX(m_fOffsetX);
    view->setContainer(layer);
    view->setTouchEnabled(false);
    view->setPosition(ccp(450,150));
    view->setTouchPriority(-8);
    view->setDirection(kCCScrollViewDirectionHorizontal);
    CCLog("m_pScrollView::%f,%f",view->getAnchorPoint().x,view->getAnchorPoint().y);
    addChild(view,2,101);
    
    
    //addChild(layer,2,101);
    //layer->setPosition(ccp(400, 200));
    m_pBackData=data;
}

void CSceneActionGameLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CSceneActionGameLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}
void CSceneActionGameLayer::onEnter()
{
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(),true);
    CCLayer::onEnter();
}
void CSceneActionGameLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
// default implements are used to call script callback if exist
bool CSceneActionGameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bTouchScrollView = false;
    m_bTouchEventEnable = false;
    //if (((CCScrollView *)getChildByTag(101)->boundingBox().containsPoint(pTouch->getLocation())) {
    m_touchPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    //
     CCScrollView* node =(CCScrollView*) getChildByTag(101);
    if (node)
    {
       
        CCRect rect;
        rect.size=((CCScrollView*)node)->getViewSize();
        rect.origin = node->boundingBox().origin;
        if(CPtTool::isInRect(node, rect, pTouch))
        {
            m_bTouchScrollView = true;
            m_touchOffset = node->getContentOffset();
            m_bTouchEventEnable = ((CCLayer*)node->getContainer())->ccTouchBegan(pTouch, pEvent);
        }
//          m_bTouchScrollView = ((CCScrollView*)(getChildByTag(101)))->ccTouchBegan(pTouch, pEvent);

    }
    return true;
}

CCScrollView *CSceneActionGameLayer::getScrollView()
{
    return (CCScrollView *)getChildByTag(101);
}

void CSceneActionGameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint movePoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    float distance = movePoint.y - m_touchPoint.y;
    // 设定当前偏移位置
    CCPoint adjustPoint = ccp(m_fOffsetX, m_touchOffset.y + distance);
    if(getScrollView()&&m_bTouchScrollView && m_bTouchEventEnable == false)
    {
        getScrollView()->setContentOffset(adjustPoint, false);
    }
    
//    CCNode *node = getChildByTag(101);
//    if (node&&m_bTouchScrollView)
//    {
//       
//        ((CCScrollView*)(getChildByTag(101)))->ccTouchMoved(pTouch, pEvent);
//        
//    }

    
}

void CSceneActionGameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_enStatus!=EN_CSceneActionGameLayerStatus_END) {
        return;
    }
    CCNode *node=Utility::getNodeByTag(this,"1,1,7,41");
    if (node &&node->boundingBox().containsPoint(pTouch->getLocation()))
    {
        removeFromParentAndCleanup(false);
    }
    
    CCScrollView* layer = (CCScrollView*)getChildByTag(101);
    if (layer && m_bTouchEventEnable)
    {
        ((CCLayer*)layer->getContainer())->ccTouchEnded(pTouch, pEvent);
        return;
    }
    
    CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    float distance = endPoint.x - m_touchPoint.x;
    float distanceY = endPoint.y - m_touchPoint.y;
    if (fabs(distance) < 10 && fabs(distanceY) < 10)
    {
        //检查触控
    }
    else
    {
     adjustScrollView(distance);
    }
//    node = getChildByTag(101);
//    if (node&&m_bTouchScrollView)
//    {
//        ((CCScrollView*)(getChildByTag(101)))->ccTouchEnded(pTouch, pEvent);
//        
//    }
    

    
}

void CSceneActionGameLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}