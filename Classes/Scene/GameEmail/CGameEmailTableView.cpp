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


}
//bool CCTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    //让  父类 去计算  中间点。
//    bool flag= CCScrollView::ccTouchBegan(pTouch, pEvent);
//    m_oldPoint =m_tTouchPoint;
//    return flag;
//}

CEmrysTableView *CEmrysTableView::Create(cocos2d::extension::CCTableViewDataSource *dataSource, cocos2d::CCSize size)
{
        CEmrysTableView*tableView=new CEmrysTableView();
        tableView->initWithViewSize(size, NULL);
        tableView->autorelease();
        tableView->setDataSource(dataSource);
        tableView->_updateContentSize();
        return tableView;;
}

CEmrysTableView::~CEmrysTableView()
{
    
}


void CEmrysTableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (!this->isVisible()) {
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
            if (m_eVordering == kCCTableViewFillTopDown) {
                CCSize cellSize = m_pDataSource->cellSizeForTable(this);
                point.y -= cellSize.height;
            }
            index = this->_indexFromOffset(point);
            cell  = this->_cellWithIndex(index);
            
            if (cell) {
                m_pTouch=pTouch;
                m_pTableViewDelegate->tableCellTouched(this, cell);
            
            }
        }
        CCScrollView::ccTouchEnded(pTouch, pEvent);
}


CGameEmailTableView::CGameEmailTableView()
{
    
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
    
    
    tableView = CEmrysTableView::Create(this, s);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(p);
    tableView->setDelegate(this);
    tableView->setTouchPriority(-8);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    //this->addChild(tableView);
    this->addChild(tableView,2,77777);
    tableView->retain();
    tableView->reloadData();
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
    
   tableView =CEmrysTableView::Create(this, m_tableViewSize);
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
        tableView->reloadData();
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
                CGameButtonControl *gameLingqu=CGameButtonControl::createButton(TEXTMID, word.c_str(), "anniu2_Normal.png", "anniu2_Normal.png");
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
                    CCPoint point=Utility::getNodeByTag(cell, "1,0,5")->getPosition();
                    cell->addChild(label);
                    label->setPosition(ccp(point.x+2,point.y-8));
                    cell->reorderChild(label, Utility::getNodeByTag(cell, "1,0,10")->getZOrder());
                    label->setColor(g_custom_color[10]);
                    
                    CCLabelTTF *labelContext=CCLabelTTF::create(gameData->getGameEmailContent().c_str(),"Arial",15);
                    cell->addChild(labelContext);
                    labelContext->setDimensions(CCSizeMake(500, 200));
                    point=Utility::getNodeByTag(cell, "1,0,11")->getPosition();
                    labelContext->setPosition(ccp(point.x+8,point.y-3));
                    cell->reorderChild(labelContext, Utility::getNodeByTag(cell, "1,0,30")->getZOrder());
                    labelContext->setColor(g_custom_color[18]);
                    labelContext->setAnchorPoint(ccp(0,1));
                    labelContext->setHorizontalAlignment(kCCTextAlignmentLeft);
                    string str;
                    gameData->getEmailCreateTime(str);
                    CCLabelTTF *labelContextTime=CCLabelTTF::create(str, "Arial", 15);
                    
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
            sendPostHttpGetSingleItem(value);
        }
    }
    
    
}

void CGameEmailTableView::sendPostHttpGetSingleItem(int msg_id)
{
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
        ADDHTTPREQUESTPOSTDATA(STR_URL_EMAILGETITEMS(194), "MERLINEMAILSTATUS", "EMAILSTATUS",str.c_str(),callfuncO_selector(CGameEmailTableView::recvBockHttpCallBack));
    }
        
}
    
void CGameEmailTableView::recvBockHttpCallBack(CCObject *object)
{
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
            CCArray *array=(CCArray*)dictresult->objectForKey("mail_ids");
            vector<int >livetable;
            for (int i=0; i<array->count();i++) {
                CCString * cocosstr=(CCString *)array->objectAtIndex(i);
                livetable.push_back(atoi(cocosstr->m_sString.c_str()));
            }
        if (livetable.size()==0) {
            G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
            //numberOfCellsInTableView(tableView);
            //tableView->removeCellAtIndex(0);
            tableView->reloadData();
        }
        else{
            if(livetable.size()!=G_GAMESINGEMAIL::instance()->getCurrentTotalEmail())
            {
                G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
                //numberOfCellsInTableView(tableView);
                //tableView->removeCellAtIndex(0);
                tableView->reloadData();
            }
        }
       
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
