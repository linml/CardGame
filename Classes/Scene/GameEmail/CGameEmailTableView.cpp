//
//  CGameEmailTableView.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CGameEmailTableView.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "Utility.h"
#include "CGameEmailManager.h"
#include "CGameEmailData.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"


CEmrysTableView::CEmrysTableView():CCTableView()
{
    _mydefineDeleagte=NULL;
    m_beginTouchPoint=CCPointZero;
    
}

bool CEmrysTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //让  父类 去计算  中间点。
    bool flag=CCTableView::ccTouchBegan(pTouch, pEvent);
    if(flag)
    {
        CCPoint  point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        m_beginTouchPoint=pTouch->getLocation();
        int index = this->_indexFromOffset(point);
        CCTableViewCell   *cell;
        cell=this->_cellWithIndex(index);
        if(_mydefineDeleagte &&cell)
        {
            _mydefineDeleagte->tablecellTouchNode(cell, pTouch);
        }
    }
    return flag;
}

void CEmrysTableView::reloadDataView()
{
    CCTableView::reloadData();
    CCLog("CCScrollView1--- %f, %f",m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(),
          m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
    CCLog("CCScrollView2--- %f, %f",m_tViewSize.width ,
          m_tViewSize.height );
    float y = (m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY()) ;
    if (y >= 0)
    {
        setContentOffset(ccp(0,y));
    }else
    {
        setContentOffset(ccp(0,y));
    }
}

void CEmrysTableView::scrollViewDidScroll(CCScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    CCPoint offset = ccpMult(this->getContentOffset(), -1);
    maxIdx = MAX(uCountOfItems-1, 0);
    const CCSize cellSize = m_pDataSource->cellSizeForTable(this);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY() - cellSize.height;
    }
    startIdx = this->_indexFromOffset(offset);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y -= m_tViewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += m_tViewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += m_tViewSize.width/this->getContainer()->getScaleX();
    //  CCLog("offset=%f,offset=%f",offset.x,offset.y);
    endIdx   = this->_indexFromOffset(offset);
    
#if 0 // For Testing.
    CCObject* pObj;
    int i = 0;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(m_pCellsFreed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell* cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
        
        idx = cell->getIdx();
        while(idx <startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell *cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
                idx = cell->getIdx();
                
            }
            else
            {
                break;
            }
        }
    }
    
    for (unsigned int i=startIdx; i <= endIdx; i++)
    {
        //if ([m_pIndices containsIndex:i])
        if (m_pIndices->find(i) != m_pIndices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}

void CEmrysTableView::ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)
{
    if(abs((int)(pTouch->getLocation().y-m_beginTouchPoint.y))<20.0)
    {
        return;
    }
    CCTableView::ccTouchMoved(pTouch, pEvent);
    if (_mydefineDeleagte) {
        CCPoint  point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        if (m_eVordering == kCCTableViewFillTopDown) {
            CCSize cellSize = m_pDataSource->cellSizeForTable(this);
            point.y -= cellSize.height;
        }
        int index = this->_indexFromOffset(point);
        CCTableViewCell   *cell;
        cell=this->_cellWithIndex(index);
        if(cell)
        {
            _mydefineDeleagte->tablecellTouchNode(cell, pTouch);
        }
    }
}


CEmrysTableView *CEmrysTableView::Create(cocos2d::extension::CCTableViewDataSource *dataSource, cocos2d::CCSize size,CEmrysTableViewDelegate *mydefineDeleagte)
{
    CEmrysTableView*tableView=new CEmrysTableView();
    tableView->initWithViewSize(size, NULL);
    tableView->autorelease();
    tableView->setDataSource(dataSource);
    tableView->_updateContentSize();
    tableView->_mydefineDeleagte=mydefineDeleagte;
    return tableView;;
}

CEmrysTableView::~CEmrysTableView()
{
    CCLog("remove=============>");
}


void CEmrysTableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (!this->isVisible())
    {
        return;
    }
    CCPoint   newPoint= this->convertTouchToNodeSpace(pTouch);
    
    
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    if (m_pTouches->count() == 1 &&  (abs((int)(newPoint.x-m_oldPoint.x))<=20 && abs((int)(newPoint.y-m_oldPoint.y))<=20))
    {
        CCLOG("adfasdf");
#else
        if (m_pTouches->count() == 1 && !this->isTouchMoved())
        {
#endif
            unsigned int        index;
            CCTableViewCell   *cell;
            CCPoint           point;
            
            point = this->getContainer()->convertTouchToNodeSpace(pTouch);
            if (m_eVordering == kCCTableViewFillTopDown)
            {
                CCSize cellSize = m_pDataSource->cellSizeForTable(this);
                point.y -= cellSize.height;
            }
            
            index = this->_indexFromOffset(point);
            CCLog("index=%d",index);
            cell  = this->_cellWithIndex(index);
            CCLog("%f,%f",cell->getContentSize().width,cell->getContentSize().height);
            
            if (cell)
            {
                m_pTouch=pTouch;
                m_pTableViewDelegate->tableCellTouched(this, cell);
                
            }
        }
        CCScrollView::ccTouchEnded(pTouch, pEvent);
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    }
#endif
}


CGameEmailTableView::CGameEmailTableView()
{
    node=NULL;
    isSendPostGetData=false;
}
CGameEmailTableView::~CGameEmailTableView()
{
    
}

CGameEmailTableView*CGameEmailTableView::creat(CCPoint p , CCSize s ,int cellNum , CCSize cellSize , CCSize tableCellSize)
{
    CGameEmailTableView* layerView = new CGameEmailTableView;
    if (layerView) {
        layerView->initView( p ,  s , cellNum ,  cellSize ,  tableCellSize);
        layerView->autorelease();
        return layerView;
    }
    CC_SAFE_DELETE(layerView);
    return NULL;
}

CGameEmailTableView *CGameEmailTableView::creat(CCPoint p , CCSize s ,int cellNum , CCSprite*cellImage , int cellgap){
    CGameEmailTableView* layerView = new CGameEmailTableView;
    if (layerView) {
        layerView->initView(  p ,  s , cellNum , cellImage ,  cellgap);
        layerView->autorelease();
        return layerView;
    }
    CC_SAFE_DELETE(layerView);
    return NULL;
}
bool CGameEmailTableView::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    return true;
}

bool CGameEmailTableView::initView(CCPoint p , CCSize s ,int cellNum , CCSize cellSize , CCSize tableCellSize){
    if ( !CCLayer::init() )
    {
        return false;
    }
    m_tableCount = cellNum;
    m_cellSize = cellSize;
    m_tableCellSize = tableCellSize;
    m_tableViewSize = s;
    m_tableViewPoint = p;
    
    
    tableView = CEmrysTableView::Create(this, s,this);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(p);
    tableView->setDelegate(this);
    tableView->setTouchPriority(-8);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,77777);
    return true;
}
bool CGameEmailTableView::initView(CCPoint p , CCSize s ,int cellNum , CCSprite*cellImage , int cellgap){
    if ( !CCLayer::init() )
    {
        return false;
    }
    m_pCell = cellImage;
    m_tableCount = cellNum;
    m_cellSize = cellImage->getContentSize();
    m_cellScaleX = cellImage->getScaleX();
    m_cellScaleY = cellImage->getScaleY();
    m_cellSize = CCSize(m_cellSize.width*m_cellScaleX, m_cellSize.height*m_cellScaleY);
    
    m_tableCellSize = CCSize(s.width , m_cellSize.height+cellgap*2);
    m_tableViewSize = s;
    m_tableViewPoint = p;
    
    m_pTexture = cellImage->getTexture();
    m_TextureRect = cellImage->getTextureRect();
    
    tableView =CEmrysTableView::Create(this, m_tableViewSize,this);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(p);
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,77777);
    
    return true;
}

void CGameEmailTableView::reloadData()
{
    tableView=(CEmrysTableView *)getChildByTag(77777);
    if(tableView)
    {
        CCLog("reloadddata");
        tableView->reloadDataView();
        //        tableView->setContentOffset(tableView->minContainerOffset());
    }
}
#pragma mark - CCTableViewDataSource

unsigned int CGameEmailTableView::numberOfCellsInTableView(CCTableView *table)
{
    return G_GAMESINGEMAIL::instance()->getMailCount();
}



void CGameEmailTableView::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    if(cell)
    {
        LayoutLayer *tempLayerout=LayoutLayer::create();
        tempLayerout->initWithFile(cell, CSTR_FILEPTAH(plistPath, "mail_cell.plist"));
        CCSprite *lingquButton=(CCSprite *)Utility::getNodeByTag(cell, "1,0,32");
        if(lingquButton)
        {
            string word = Utility::getWordWithFile("word.plist", "lingqujiangpin");
            CGameButtonControl *gameLingqu=CGameButtonControl::createButton(TEXTMID, word.c_str(), "anniu2_Normal.png", "anniu2_Pressed.png");
            gameLingqu->setPosition(lingquButton->getPosition());
            //gameLingqu->setTag(lingquButton->getTag());
            gameLingqu->setAnchorPoint(lingquButton->getAnchorPoint());
            cell->addChild(gameLingqu,lingquButton->getZOrder(),2000);
            //cell ->reorderChild(gameLingqu,lingquButton->getZOrder());
            cell->removeChild(lingquButton, true);
            CCLog("load index::%d",idx);
            CGameEmailData *gameData=G_GAMESINGEMAIL::instance()->getGameDataByIndex(idx);
            if(gameData)
            {
                CCLog("getGameEmailTitle:::%s",gameData->getGameEmailTitle().c_str());
                CCLabelTTF *label=CCLabelTTF::create(gameData->getGameEmailTitle().c_str(),"Arial",15);
                CCPoint point=Utility::getNodeByTag(cell, "1,0,1")->getPosition();
                cell->addChild(label);
                label->setPosition(ccp(point.x+8,point.y-21));
                cell->reorderChild(label, Utility::getNodeByTag(cell, "1,0,10")->getZOrder());
                label->setColor(g_custom_color[10]);
                label->setAnchorPoint(ccp(0,0));
                label->setHorizontalAlignment(kCCTextAlignmentLeft);
                
                CCLabelTTF *labelContext=CCLabelTTF::create(gameData->getGameEmailContent().c_str(),"Arial",15);
                cell->addChild(labelContext);
                labelContext->setDimensions(CCSizeMake(480, 200));
                point=Utility::getNodeByTag(cell, "1,0,11")->getPosition();
                labelContext->setPosition(ccp(point.x+8,point.y-5));
                cell->reorderChild(labelContext, Utility::getNodeByTag(cell, "1,0,30")->getZOrder());
                labelContext->setColor(g_custom_color[18]);
                labelContext->setAnchorPoint(ccp(0,1));
                labelContext->setHorizontalAlignment(kCCTextAlignmentLeft);
                
                string str;
                gameData->getEmailCreateTime(str);
                CCLabelTTF *labelContextTime=CCLabelTTF::create(str.c_str(), "Arial", 15);
                cell->addChild(labelContextTime);
                point=Utility::getNodeByTag(cell, "1,0,31")->getPosition();
                labelContextTime->setPosition(ccp(point.x-50,point.y-30));
                labelContextTime->setColor(g_custom_color[17]);
                labelContextTime->setAnchorPoint(ccp(0,1));
                labelContextTime->setHorizontalAlignment(kCCTextAlignmentRight);
                cell->reorderChild(labelContextTime, Utility::getNodeByTag(cell, "1,0,31")->getZOrder()+1);
                string strPng=g_propImagesPath+gameData->getFirstItemsPng();
                CCSprite  *sprite = CCSprite::create(strPng.c_str());
                CCSize rect=Utility::getNodeByTag(cell, "1,0,0")->boundingBox().size;
                int valueZorder=Utility::getNodeByTag(cell, "1,0,0")->getZOrder()+1;
                Utility::getNodeByTag(cell, "1,0,0")->addChild(sprite,valueZorder);
                sprite->setPosition(ccp(rect.width/2,rect.height/2));
                
                
            }
        }
    }
}

CCTableViewCell* CGameEmailTableView::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        this->initCellItem(cell, idx);
    }
    else
    {
        scrollBar(table);
    }
    return cell;
}


CCSize CGameEmailTableView::cellSizeForTable(CCTableView *table) {
    return m_cellSize;
}

CCSize CGameEmailTableView::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
    return m_tableCellSize;
}

#pragma mark - CCTableViewDelegate
void CGameEmailTableView::tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch)
{
    if(!(cell->getChildByTag(2000)))
    {
        return;
    }
    CCLog("cell->getIdx() :%d", cell->getIdx());
    CCRect rect=cell->getChildByTag(2000)->boundingBox();
    CCPoint point=pTouch->getLocation();
    point=cell->convertTouchToNodeSpace(pTouch);
    if(cell->getChildByTag(2000)->boundingBox().containsPoint(point))
    {
        CGameButtonControl *gamebutton=(CGameButtonControl*)cell->getChildByTag(2000);
        if(node!=gamebutton)
        {
            if(node)
            {
                ((CGameButtonControl*)node)->unselected();
            }
            node=gamebutton;
        }
        gamebutton->selected();
    }
    else {
        if(node)
        {
            ((CGameButtonControl*)node)->unselected();
        }
        node=NULL;
    }
    
}

void CGameEmailTableView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    int value =cell->getIdx();
    CCLog("you touch cell at %d", value);
    if(((CEmrysTableView *)table)->m_pTouch)
    {
        CCTouch *tempTouch=((CEmrysTableView *)table)->m_pTouch;
        CCRect rect=cell->getChildByTag(2000)->boundingBox();
        CCPoint point=tempTouch->getLocation();
        point=cell->convertTouchToNodeSpace(tempTouch);
        if(cell->getChildByTag(2000)->boundingBox().containsPoint(point))
        {
            if(node)
            {
                ((CGameButtonControl *)node)->unselected();
                node=NULL;
            }
            ((CGameButtonControl *)cell->getChildByTag(2000))->unselected();
            sendPostHttpGetSingleItem(value);
        }
    }
    
    
}

void CGameEmailTableView::sendPostHttpGetSingleItem(int msg_id)
{
    
    CCLog("this=%x",this);
    if(isSendPostGetData)
    {
        return ;
    }
    isSendPostGetData=true;
    PtSoundTool::playSysSoundEffect("UI_click.wav");
    vector<EMAIL_DATA >vemaildatalist;
    G_GAMESINGEMAIL::instance()->copyDataTovectory(vemaildatalist,msg_id);
    vector<int>canereadList;//.clear();
    CGamePlayer *player=SinglePlayer::instance();
    canereadList=player->getCanAddToBackPackEmals(vemaildatalist);
    
    if(canereadList.size()>0)
    {
        // & info="[1,2,3]"
        //        m_enhttpStatus=EN_EMAILHTTPREQUEST_GETALLEMAIL;
        string str="&info=[";
        char data[12];
        for (int i=0; i<canereadList.size()-1; i++) {
            sprintf(data, "%d,",canereadList[i]);
            str +=data;
        }
        sprintf(data, "%d]",canereadList[canereadList.size()-1]);
        str+=data;
        CCLog("post get data:%s",str.c_str());
        str +="&sig=2ac2b1e302c46976beaab20a68ef95";
        ADDHTTPREQUESTPOSTDATA(STR_URL_EMAILGETITEMS(194), "MERLINEMAILSTATUS123", "EMAILSTATUS",str.c_str(),callfuncO_selector(CGameEmailTableView::recvBockHttpCallBack));
    }
    else{
        if (G_GAMESINGEMAIL::instance()->getMailCount()>0) {
            if(this->getParent())
            {
                isSendPostGetData=false;
                CCNotificationCenter::sharedNotificationCenter()->postNotification("BEIBAOMANLEXIANSHIDUIHUAKUAN");
            }
        }
    }
    
}

void CGameEmailTableView::recvBockHttpCallBack(CCObject *object)
{
    CCLog("%x",object);
    isSendPostGetData=false;
    CCLog("recvBockHttpCallBack");
    if(!object)
    {
        return;
    }
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "MERLINEMAILSTATUS123");
    char *strdata=(char *)object;
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut!=0)
    {
        return ;
    }
    else
    {
        CCDictionary *dictresult=(CCDictionary *)dict->objectForKey("result");
        //获取 coin
        int  coin=((CCString *)dictresult->objectForKey("coin"))->intValue();
        //获取items
        //获取 经验
        int  exp=((CCString *)dictresult->objectForKey("exp"))->intValue();
        map<int , int>mapitems;
        CCDictionary *emailItemDirector=(CCDictionary *)(dictresult->objectForKey("item"));
        if(emailItemDirector)
        {
            CCDictElement* pElement = NULL;
            CCDICT_FOREACH(emailItemDirector, pElement)
            {
                const char* pchKey = pElement->getStrKey();
                mapitems[atoi(pchKey)]=GameTools::intForKey(pchKey, emailItemDirector);
            }
        }
        SinglePlayer::instance()->receiveEmail(mapitems,exp, coin);
        
        CCArray *array=(CCArray*)dictresult->objectForKey("mail_ids");
        vector<int >livetable;
        for (int i=0; i<array->count();i++)
        {
            CCString * cocosstr=(CCString *)array->objectAtIndex(i);
            livetable.push_back(atoi(cocosstr->m_sString.c_str()));
        }
        if (livetable.size()==0)
        {
            G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
            tableView->reloadDataView();
        }
        else
        {
            if(livetable.size()!=G_GAMESINGEMAIL::instance()->getCurrentTotalEmail())
            {
                G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
                tableView->reloadDataView();
            }
        }
        runDialogAction();
        CCNotificationCenter::sharedNotificationCenter()->postNotification("youjiangengxin");
        G_GAMESINGEMAIL::instance()->writeToFile();
    }
    
}

void CGameEmailTableView::runDialogAction()
{
    CCLabelTTF *labelttf=NULL;
    if (getChildByTag(255555))
    {
        labelttf=(CCLabelTTF *)getChildByTag(255555);
        labelttf->stopAllActions();
    }
    else
    {
        labelttf=CCLabelTTF::create("领取成功", "Arial", 45);
        addChild(labelttf,900,255555);
    }
    labelttf->setColor(ccc3(255, 0, 0));
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    labelttf->setPosition(ccp(size.width*0.5,size.height*0.5));
    labelttf->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(1.0, ccp(0,100)),CCFadeOut::create(1.0),NULL),CCCallFunc::create(this, callfunc_selector(CGameEmailTableView::removeDialog)),NULL));
                        
    
}
void CGameEmailTableView::removeDialog()
{
    if (getChildByTag(255555))
    {
        removeChildByTag(255555, true);
    }
}

void CGameEmailTableView::scrollViewDidScroll(CCScrollView* view)
{
    scrollBar((CCTableView*)view);
}

void CGameEmailTableView::scrollViewDidZoom(CCScrollView* view) {
    
}




//add处理滚动:

void CGameEmailTableView::scrollBar(CCTableView* table)
{
    CCSize conSize = table->getContentSize();
    CCSize viewSize = table->getViewSize();
    
    if (conSize.height < viewSize.height) {
        return;
    }
    
    //    if (!m_huaTiao) {
    //        m_huaTiao = CCSprite::create("scrollBar.png");
    //        if(m_huaTiao != NULL)
    //        {
    //            CCSize sizeBar = m_huaTiao->getContentSize();
    //            this->addChild(m_huaTiao, 1);
    //            m_huaTiao->setScaleY(viewSize.height/sizeBar.height);
    //            float scaley = 0;
    //            scaley = m_huaTiao->getScaleY();
    //            float per = viewSize.height/conSize.height;
    //            scaley = scaley*per;
    //            m_huaTiao->setScaleY(scaley);
    //
    //            m_huaTiao->setAnchorPoint(ccp(0,0.5));
    //        }
    //    }
    //	if(m_huaTiao == NULL)
    //	{
    //		return;
    //	}
    //    m_huaTiao->setOpacity(255);
    //
    //    CCPoint scrollViewPoint = table->getPosition();
    //
    //    CCSize sizeBar = m_huaTiao->getContentSize();
    //	// tableSize.height == cell个数*cell的height
    //	CCSize tableSize = table->getContentSize();
    //	// CCTableView
    //	CCSize tableViewSize = table->getViewSize();
    //    //    tableViewSize = CCSize(tableViewSize.width, tableViewSize.height-sizeBar.height);
    //	// 每次拖动的偏移量？(负值)
    //	CCPoint contOffsetPos = table->getContentOffset();
    //
    //	// 总的偏移量
    //	float maxOff = tableViewSize.height - tableSize.height;
    //	// 拖动的偏移量
    //	float curOff = contOffsetPos.y - maxOff;
    //	// 计算百分百
    //	float percentage = fabs(curOff)/fabs(maxOff);
    //
    //    //	printf("contOffsetPos:%f ,curOff:%f, maxOff:%f, per:%f\n", contOffsetPos.y, curOff, maxOff, percentage);
    //
    //	// 拖拉到最顶端或最低端后继续拖动(弹回)会出现percentage值小于0.1和大于1.0的情况，我们分别把percentage重置为0和1.0f
    //	if(curOff < 0)
    //	{
    //		percentage = 0;
    //	}
    //	if(percentage > 1.0f)
    //	{
    //		percentage = 1.0f;
    //	}
    //
    //	// bar移动到最顶端的position.y
    //	float barTopPosY = scrollViewPoint.y+tableViewSize.height -(sizeBar.height*m_huaTiao->getScaleY())/2 ;
    //	// bar移动到最低端的position.y
    //	float barLowPosY = scrollViewPoint.y+(sizeBar.height*m_huaTiao->getScaleY())/2;
    //	// ....
    //	float h = barTopPosY - percentage*(barTopPosY- barLowPosY);;
    //
    //	m_huaTiao->setPosition(ccp(tableViewSize.width*1.05 , h));
}
