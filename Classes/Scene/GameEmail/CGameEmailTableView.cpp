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
#include "CGameDialogLayer.h"


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
    if(_mydefineDeleagte)
    {
        _mydefineDeleagte->tableScrolBarView(this);
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
           
            
            if (cell)
            {
                CCLog("%f,%f",cell->getContentSize().width,cell->getContentSize().height);
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
   m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
    node=NULL;
    isSendPostGetData=false;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    m_tempTouchNode=NULL;
    m_pScrollViewGuanDongTiao=NULL;
}
CGameEmailTableView::~CGameEmailTableView()
{
     CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

void CGameEmailTableView::setGunDongTiaoPtr()
{
    m_pScrollViewGuanDongTiao=(CCSprite *)Utility::getNodeByTag(this, "1,7,0");
    m_fOldScrollBarPosiontYtop=m_pScrollViewGuanDongTiao->getPosition().y;
    m_foldscrollBarPosiontylow=Utility::getNodeByTag(this, "1,7,26")->getPosition().y;
}



CGameEmailTableView *CGameEmailTableView::CreateEmailLayer()
{
     CGameEmailTableView *gameEmailLayer=new CGameEmailTableView();
    if(!gameEmailLayer || !gameEmailLayer->initCreate())
    {
        delete gameEmailLayer;
        gameEmailLayer=NULL;
    }
    gameEmailLayer->autorelease();
    return gameEmailLayer;
}

bool CGameEmailTableView::loadPlistFile()
{
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
    return true;
}


void CGameEmailTableView::creaetEmailTableView()
{
    initView(ccp(200,150), CCSizeMake(800, 400),0, CCSizeMake(800, 100), CCSizeMake(600, 120));
    
}

void CGameEmailTableView::showDialogBagFull(cocos2d::CCObject *obect)
{
    string word = Utility::getWordWithFile("word.plist", "caonima");
    CPtDialog *ptDialog=CPtDialog::create(word.c_str() , this , callfuncO_selector(CGameEmailTableView::dialogOkButtonSetFunc), callfuncO_selector(CGameEmailTableView::dialogCancelButtonSetFunc), NULL, NULL);
    addChild(ptDialog,1000,10000);
}


void CGameEmailTableView::createRecvAllButton()
{
    string word = Utility::getWordWithFile("word.plist", "quanbulingqu");
    
    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(sprite,900,7);
    CCNode *node=Utility::getNodeByTag(this, "1,3,0");
    CCPoint point;
    if(node)
    {
        point=node->getPosition();
        sprite->setPosition(ccp(point.x,point.y-32));
        sprite->setAnchorPoint(ccp(0,0));
    }
}

bool CGameEmailTableView::initCreate()
{
    loadPlistFile();
    createRecvAllButton();
    creaetEmailTableView();
    sendPostHttpChangeEmailStatus();
    setGunDongTiaoPtr();
    setTouchEnabled(true);
    return true;
}

void CGameEmailTableView::sendPostHttpChangeEmailStatus()
{
    int value=G_GAMESINGEMAIL::instance()->getCurrentEmailUnreadCount();
    if(value>0)
    {
        m_enhttpStatus=EN_EMAILHTTPREQUEST_CHANEGSTATUS;
        ADDHTTPREQUESTPOSTDATA(STR_URL_EMAILREADSTATUS(194), "MERLINEMAILSTATUS", "EMAILSTATUS","sig=2ac2b1e302c46976beaab20a68ef95",callfuncO_selector(CGameEmailTableView::recvBockHttpCallBack));
    }
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
        tableView->reloadDataView();
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
    if(m_enhttpStatus!=EN_EMAILHTTPREQUEST_NONE)
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
        m_enhttpStatus=EN_EMAILHTTPREQUEST_GETSINGLEITEM;
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
void CGameEmailTableView::decodeSingleRecvEmail(char *object)
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

bool CGameEmailTableView::decodeRecvBackStr(char *strdata)
{
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut!=0)
    {
        return false;
    }
    else
    {
        CCDictionary *dictresult=(CCDictionary *)dict->objectForKey("result");
        if (m_enhttpStatus==EN_EMAILHTTPREQUEST_GETSINGLEITEM  ||
            m_enhttpStatus==EN_EMAILHTTPREQUEST_GETALLEMAIL )
        {
            //与 单个领取里面代码重复 得重构
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
            for (int i=0; i<array->count();i++) {
                CCString * cocosstr=(CCString *)array->objectAtIndex(i);
                livetable.push_back(atoi(cocosstr->m_sString.c_str()));
            }
            if(livetable.size()!=G_GAMESINGEMAIL::instance()->getMailCount())
            {
                G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
                reloadData();
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification("youjiangengxin");
            return true;
        }
        else
        {
            CCString *str=(CCString *)dictresult->objectForKey("info");
            CCLog("%s",str->m_sString.c_str());
            if(str->compare("1")==0)
            {
                G_GAMESINGEMAIL::instance()->changeEmailStatus();
                CCNotificationCenter::sharedNotificationCenter()->postNotification("youjiangengxin");
                return true;
            }
            else
            {
                return  false;
            }
        }
        return true;
    }
}

void CGameEmailTableView::recvBockHttpCallBack(CCObject *object)
{
    
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "MERLINEMAILSTATUS");
    if(!object)
    {
        m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
        return;
    }
    decodeRecvBackStr((char *)object);
    G_GAMESINGEMAIL::instance()->writeToFile();
    m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
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
void CGameEmailTableView::tableScrolBarView(CCTableView *tableveiw)
{
    scrollBar((CCTableView*)tableveiw);
}
void CGameEmailTableView::scrollViewDidScroll(CCScrollView* view)
{
    scrollBar((CCTableView*)view);
}

void CGameEmailTableView::scrollViewDidZoom(CCScrollView* view) {
    
}

void CGameEmailTableView::dialogCancelButtonSetFunc(cocos2d::CCObject *object)
{
        removeChildByTag(10000, true);
}

void CGameEmailTableView::dialogOkButtonSetFunc(cocos2d::CCObject *object)
{
    removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("CAONIMAXIANSHIBEIBAO");
}



void CGameEmailTableView::getAllEmailItem()
{
    //  ((CGameEmailTableView*)getChildByTag(8))->reloadData();
    // return;
    CCLog("getAllEmailItem");
    if(m_enhttpStatus!=EN_EMAILHTTPREQUEST_NONE)
    {
        return;
    }
    vector<EMAIL_DATA >vemaildatalist;
    G_GAMESINGEMAIL::instance()->copyDataTovectory(vemaildatalist);
    canereadList.clear();
    CGamePlayer *player=SinglePlayer::instance();
    canereadList=player->getCanAddToBackPackEmals(vemaildatalist);
    if(canereadList.size()>0)
    {
        // & info="[1,2,3]"
        m_enhttpStatus=EN_EMAILHTTPREQUEST_GETALLEMAIL;
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
    else  if(G_GAMESINGEMAIL::instance()->getMailCount()>0 && canereadList.size()==0)
    {
        showDialogBagFull(NULL);
    }
}

bool CGameEmailTableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(touchPoint))
    {
        CCLog("aaaaaaa1");
        m_tempTouchNode=node;
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(7))->selected();
        m_tempTouchNode=getChildByTag(7);
    }
    return true;
}

void CGameEmailTableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(pTouch->getLocation()))
    {
        CCLog("aaaaaaa2");
        if(m_tempTouchNode!=node)
        {
            if(m_tempTouchNode==getChildByTag(7))
            {
                ((CGameButtonControl *)getChildByTag(7))->unselected();
            }
            m_tempTouchNode=node;
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        if(m_tempTouchNode!=getChildByTag(7))
        {
            ((CGameButtonControl *)getChildByTag(7))->selected();
            m_tempTouchNode=getChildByTag(7);
        }
    } else if(m_tempTouchNode ==getChildByTag(7))
    {
        ((CGameButtonControl *)m_tempTouchNode)->unselected();
        m_tempTouchNode=NULL;
    }
}

void CGameEmailTableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(touchPoint))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        CCLog("aaaaaaa4");
        if(m_tempTouchNode==node)
        {
            removeFromParentAndCleanup(true);
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        CCLog("aaaaaaa3");
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (((CGameButtonControl *)m_tempTouchNode))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        
        if(m_tempTouchNode==getChildByTag(7))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
            m_tempTouchNode=NULL;
            getAllEmailItem();
        }
    }
    else
    {
        ((CGameButtonControl *)getChildByTag(7))->unselected();
    }
}
void CGameEmailTableView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
#define GOLDPLACE_TOUCH_PRORITY -2
void CGameEmailTableView::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,GOLDPLACE_TOUCH_PRORITY-1, true);
}

void CGameEmailTableView::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}


//add处理滚动:

void CGameEmailTableView::scrollBar(CCTableView* table)
{
    CCNode* bar = Utility::getNodeByTag(this, "1,7,40");
    if(bar == NULL)
    {
        return;
    }
    
    CCSize tableSize = table->getContentSize();
    CCSize tableViewSize = table->getViewSize();
    
    CCPoint contOffsetPos = table->getContentOffset();
    float maxOff = tableViewSize.height - tableSize.height;
    CCLog("tableViewSize.height:%f,tableSize.height:%f,table->getContentOffset():%f",tableViewSize.height,tableSize.height,table->getContentOffset().y);
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
    float barLowPosY =m_foldscrollBarPosiontylow;
    float h = barTopPosY - percentage*(barTopPosY- barLowPosY);
    CCLog("h:%f,barTopPosY %f");
    bar->setPosition(ccp(bar->getPosition().x, h));
}
