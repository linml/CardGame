//
//  CScreenHandBookLayer.cpp
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#include "CScreenHandBookLayer.h"
#include "CGamesCard.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "Utility.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include "LayoutLayer.h"
#include "CHandBookTableView.h"
#include "CCardInfoLayer.h"
#include "CGameButtonControl.h"
#include <map>
const int handbookheight=500;   //最上面的一点
const int handbookwidth=100;   //起点
const int cardWidth=250;
const int cardHeight=200;
const int layerCellCount=9;
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

#define DELETE_POINT_LIST(LISTARRAY,LISTITETYPE,__TYPECLASSNAME__) \
{\
for (LISTARRAY::iterator it=LISTITETYPE.begin(); it!= LISTITETYPE.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
LISTITETYPE.erase(LISTITETYPE.begin(),LISTITETYPE.end()); \
}


CHandBookLayerCell::CHandBookLayerCell()
{
    
}

CHandBookLayerCell::~CHandBookLayerCell()
{
    
}

CCPoint CHandBookLayerCell::getCardPosition(int nIndex)
{
        int  nRow=nIndex/3;
        int  nColomn=nIndex%3;
        return CCPointMake(handbookwidth+nColomn *(cardWidth+50),handbookheight-(cardHeight+10)*nRow);
}

CHandBookLayerCell *CHandBookLayerCell::Create(vector<CFightCard *>CardList,int begin,int end)
{
    CHandBookLayerCell *handcell=new CHandBookLayerCell;
    if (!handcell|| !handcell->initCreate(CardList, begin, end))
    {
        CC_SAFE_DELETE(handcell);
        return NULL;
    }
    handcell->autorelease();
    return handcell;
    
    
}
bool CHandBookLayerCell::initCreate(vector<CFightCard *>CardList,int begin,int end)
{
    for (int i=begin; i<CardList.size() && i<end; i++) {
        CGamesCard *card =CGamesCard::Create(CardList[i]);
        addChild(card, 1,i);
        card->setPosition(getCardPosition(i-begin));
        if(!CardList[i]->isShiFouYongyouguo)
        {
            card->addChild(CCLayerColor::create(ccc4(0, 0, 0, 150), card->getContentSize().width, card->getContentSize().height),5,77);
        }
        
    }
    setContentSize(CCSizeMake(800, 600));
    return true;
}

int  CHandBookLayerCell::checkTouchCell(CCTouch *pTouch)
{
    // 遍历子节点 判断是哪个 item被点击并获得该ID 然后new出一个layer 去显示
    CCPoint   newPoint= this->convertTouchToNodeSpace(pTouch);
    CCArray *arrayNode=getChildren();
    for (int i=0; i<arrayNode->count(); i++) {
        CCRect rect=((CCNode *)arrayNode->objectAtIndex(i))->boundingBox();
        if (((CCNode *)arrayNode->objectAtIndex(i))->boundingBox().containsPoint(newPoint)){
            return ((CCNode *)arrayNode->objectAtIndex(i))->getTag();
        }
    }
    return -1;
}
CScreenHandBookLayer::CScreenHandBookLayer()
{
    m_tempPlayer=SinglePlayer::instance();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"niudanchouka.plist"));
    isLoading=true;
}

CScreenHandBookLayer::~CScreenHandBookLayer()
{
    DELETE_POINT_VECTOR(m_vCardList,vector<CFightCard *>,   CFightCard );
      CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"niudanchouka.plist"));
}

bool CScreenHandBookLayer::init(){
    if (CCLayer::init())
    {
        setTouchEnabled(true);
        setTouchPriority(GOLDPLACE_TOUCH_PRORITY-1);
        createBackGroud();
        createQuitButton();
        this->runAction(CCSequence::create(CCDelayTime::create(0.2),CCCallFunc::create(this, callfunc_selector(CScreenHandBookLayer::sendGetTuJian)),NULL));
        
    }
    return true;
}

void CScreenHandBookLayer::createQuitButton()
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    float scenemidPointx=size.width*0.5;
    float scenemidPointy=size.height*0.5+310;
    CGameButtonControl *gameQuitButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
     gameQuitButton->setPosition(ccp(scenemidPointx+400, scenemidPointy));
    addChild(gameQuitButton,2,2);
   
}


void CScreenHandBookLayer::createBackGroud()
{
    CCLayer  *layer=CCLayer::create();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(layer, CSTR_FILEPTAH(plistPath, "tujianjiemian.plist"));
    addChild(layer,1,1);
}

void CScreenHandBookLayer::createTableView()
{
    
    CHandBookTableView *tableView = CHandBookTableView::create(this, CCSizeMake(800,600),NULL);
    tableView->setDirection(kCCScrollViewDirectionHorizontal);
    tableView->setAnchorPoint(ccp(0, 0.0));
    tableView->setPosition(ccp(100,70));
    tableView->setDelegate(this);
    tableView->setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,2,999);


}

CCSize CScreenHandBookLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(800 , 600);
}

void CScreenHandBookLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    CCLog("idx ====%d ",idx);
    int beginValue=idx*layerCellCount;
    if (beginValue<m_vCardList.size())
    {
        CCLayer *node  =CHandBookLayerCell::Create(m_vCardList,beginValue ,beginValue+layerCellCount);
        cell->addChild(node,1,idx+10000);
    }
}

CCTableViewCell* CScreenHandBookLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int CScreenHandBookLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vCardList.size()/layerCellCount+1;
}

void CScreenHandBookLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //触控的位置。
    int value =cell->getIdx();
    CCLog("you touch cell at %d", value);
    //createRightLayer(m_vActionList[value]);
    CCTouch *pTouch=((CHandBookTableView *)table)->getTouchSet();
    
    CHandBookLayerCell *cellLayer=(CHandBookLayerCell *)(cell->getChildByTag(10000+value));
    if(cellLayer)
    {
       int touchTag=cellLayer->checkTouchCell(pTouch);
        if(touchTag!=-1 && touchTag <m_vCardList.size())
        {
            CCardInfoLayer *layer=CCardInfoLayer::create(m_vCardList[touchTag]);
            addChild(layer, 5);
        }
    }
    
    
}
void CScreenHandBookLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CScreenHandBookLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

struct StructTuJian
{
    int  m_nTujianID;
    int  m_nCardId;
    bool operator < (const StructTuJian  &pTujian) const
    {
        return m_nTujianID < pTujian.m_nTujianID;
    }
    
    bool operator > (const StructTuJian  & pTujian) const
    {
        return m_nTujianID > pTujian.m_nTujianID;
    }

};

bool comp_sort(const StructTuJian *node1, const StructTuJian *node2)
{
    return node1->m_nTujianID < node2->m_nTujianID;
}

void CScreenHandBookLayer::loadCardVector()
{
    list<StructTuJian >tempList;
    CCDictionary *directory = CCDictionary::createWithContentsOfFile((resRootPath+"handbook.plist").c_str());
    CCArray *vKeyArray=directory->allKeys();
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *tujianDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        StructTuJian pTujian;//=new  StructTuJian;
        pTujian.m_nTujianID =GameTools::intForKey("handbook_id", tujianDirector);
        pTujian.m_nCardId= GameTools::intForKey("card_id", tujianDirector);
        CCLog("handbook_id:%d m_nCardId:%d",pTujian.m_nTujianID,pTujian.m_nCardId);
        tempList.push_back(pTujian);
    }
    directory->release();
    tempList.sort();
//    {
//    list<StructTuJian >::iterator it = tempList.begin();
//    while(it != tempList.end())
//    {
//        printf("value  %d \n", (*it).m_nTujianID);
//        it++;
//    }
//    }

    //先读取一个list
    CCLog("m_tempPlayer->getCardMap():size:%d",m_tempPlayer->getCardMap()->size());
    for (list<StructTuJian >::iterator it=tempList.begin(); it!=tempList.end();it++)
    {
        CCLog("(*it)->m_nCardId %d",(*it).m_nCardId);
        CCard *card=m_tempPlayer->getCardByCardId((*it).m_nCardId);
        if(card)
        {
            this->m_vCardList.push_back(new CFightCard(card,1));
        }
        else {
            CCLog("存在无效卡");
        }
    }
    //DELETE_POINT_LIST(list<StructTuJian *>, tempList, StructTuJian)
    tempList.erase(tempList.begin(),tempList.end());

}

void CScreenHandBookLayer::sendGetTuJian()
{
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_ATLAS(194), "CALLBACK_CScreenHandBookLayer_sendGetTuJian", "REQUEST_CScreenHandBookLayer_sendGetTuJian",postStrdata.c_str(),callfuncO_selector(CScreenHandBookLayer::decodeGetData));
}

void CScreenHandBookLayer::decodeGetData(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CScreenHandBookLayer_sendGetTuJian");
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
    //解析result 数据
    loadCardVector();
    
    CCArray *exsitArray= (CCArray*) (((CCDictionary *)(dict->objectForKey("result")))->objectForKey("atlas"));
    for (int i=0; i<exsitArray->count(); i++) {
        int cardid= ((CCString* )exsitArray->objectAtIndex(i))->intValue();
        for (int j=0; j<m_vCardList.size(); j++) {
            if (cardid==m_vCardList[j]->m_pCard->m_icard_id) {
                m_vCardList[j]->isShiFouYongyouguo=true;
            }
        }
    }
    if(m_vCardList.size()>0)
    {
        createTableView();

    }
    else{
        setTouchEnabled(true);
        setTouchPriority(-3);
    }
    isLoading=false;
}

bool CScreenHandBookLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CScreenHandBookLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CScreenHandBookLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isLoading)
    {
        if (getChildByTag(2)) {
            if(((CGameButtonControl *)(getChildByTag(2)))->boundingBox().containsPoint(pTouch->getLocation()))
            {
                removeFromParentAndCleanup(true);
            }
        }
    }
    
    
}
void CScreenHandBookLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CScreenHandBookLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
/*
void CScreenHandBookLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}
 */
void CScreenHandBookLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);//, getTouchPriority());
    CCLayer::onExit();
}