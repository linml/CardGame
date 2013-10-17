//
//  CSceneGameShopLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#include "CSceneGameShopLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "cocos-ext.h"
#include "CCScrollView.h"
#include "gamePlayer.h"
#include "CStructShopSellItem.h"

#include "CGameRechargeLayer.h"
#include "CGameRechargeTip.h"
#include "CGameArrageBackpackTip.h"
#include "CReward.h"

#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "Utility.h"
#include "PtJsonUtility.h"

#include "CPtRecharge.h"
const int tagNodeBall = 1002;
const int tagScrollView=1003;

using namespace::cocos2d;
using namespace extension;


CScrollViewCellNode::CScrollViewCellNode() {
	m_iEventId = 0;
}

CScrollViewCellNode::~CScrollViewCellNode() {
}

bool CScrollViewCellNode::init() {
    
	return true;
}

void CScrollViewCellNode::setDisplay(CCSprite* dis) {
	// 设置之前先清除，没有也无所谓
    if (getChildByTag(TAG_DISPLAY)) {
        removeChildByTag(TAG_DISPLAY, true);
    }
	addChild(dis, 0, TAG_DISPLAY);
}

CCSprite* CScrollViewCellNode::getDisplay(){
    return dynamic_cast<CCSprite*>(this->getChildByTag(TAG_DISPLAY));
}

void CScrollViewCellNode::setEventId(int eventId) {
	m_iEventId = eventId;
}

int CScrollViewCellNode::getEventId() {
	return m_iEventId;
}

bool CScrollViewCellNode::selfCheck(CCTouch* ccTouch, CScrollViewCellTouchEvent* lsTe) {
	bool bRef = false;
	// 可点击项的检测，可扩展
	do {
		// 是否通过点击位置检测
		CC_BREAK_IF(!containsCCTouchPoint(ccTouch));
		// 是否正在运行，排除可能存在已经从界面移除，但是并没有释放的可能
		CC_BREAK_IF(!isRunning());
		// 判断是否隐藏
		CC_BREAK_IF(!isVisible());
		// 这里可能还需要判断内部显示项目是否隐藏
		///// 暂留
		bRef = true;
	} while (0);
	return bRef;
}
void CScrollViewCellNode::setUpdateDataXiangGou()
{
    CCNode* dis = getChildByTag(TAG_DISPLAY);
    ((CSceneGameShopItem*)dis)->updateXiangGou();
}


void CScrollViewCellNode::setTouch()
{
    CCNode* dis = getChildByTag(TAG_DISPLAY);
    ((CSceneGameShopItem*)dis)->setTouch();
}
void CScrollViewCellNode::setUnTouch()
{
    CCNode* dis = getChildByTag(TAG_DISPLAY);
    ((CSceneGameShopItem*)dis)->setUnTouch();
}
bool CScrollViewCellNode::containsCCTouchPoint(CCTouch* ccTouch) {
	// 获得显示内容
	CCNode* dis = getChildByTag(TAG_DISPLAY);
	CCSprite* sprite = dynamic_cast<CCSprite*>(dis);
	CCPoint point = sprite->convertTouchToNodeSpaceAR(ccTouch);
	CCRect rect = ((CSceneGameShopItem*)sprite)->getBuyItemRect();;
	return rect.containsPoint(point);
}

bool CScrollViewCellNode::isParentAllVisible(CScrollViewCellTouchEvent* lsTe) {
	bool bRef = true;
	// 向父类转型，以便获取地址比较对象，LsTouchEvent 的对象必须同时直接或者简介继承 CCNode
	CCNode* nLsTe = dynamic_cast<CCNode*>(lsTe);
    
	CCNode* parent = getParent();
	do {
		// 如果遍历完毕，说明 LsTouch 不再 LsTouchEvent 之内
		if (!parent) {
			bRef = false;
			break;
		}
		// 如果 LsTouch 在 LsTouchEvent 之内，返回 true
		// 注意：如果想让LsTouchEvent 处理 不在其 CCNode 结构之内的元素，则取消此处判断
		if (nLsTe == parent) {
			break;
		}
		if (!parent->isVisible()) {
			bRef = false;
			break;
		}
		parent = parent->getParent();
	} while (1);
	return bRef;
}

CScrollViewCellTouchEvent::CScrollViewCellTouchEvent() {
	m_pLsTouches = CCArray::create();
	m_pLsTouches->retain();
}

CScrollViewCellTouchEvent::~CScrollViewCellTouchEvent() {
	m_pLsTouches->release();
}

void CScrollViewCellTouchEvent::addLsTouch(CScrollViewCellNode* touch, int eventId) {
	touch->setEventId(eventId);
	m_pLsTouches->addObject(touch);
}

void CScrollViewCellTouchEvent::removeAllTouch()
{
    m_pLsTouches->removeAllObjects();
}

void CScrollViewCellTouchEvent::removeLsTouch(CScrollViewCellNode* touch) {
	m_pLsTouches->removeObject(touch, true);
}
CScrollViewCellNode *CScrollViewCellTouchEvent::getObjectTouchNode(int level)
{
    return (CScrollViewCellNode *)m_pLsTouches->objectAtIndex(level);
}
bool CScrollViewCellTouchEvent::sendTouchMessage(CCTouch* ccTouch, int type) {
	// 编写判断，集合中的哪个元素级别高，就触发哪一个
	CScrollViewCellNode* lsTouch = NULL;
	// 获得点击的点
	CCObject* pObj = NULL;
	CScrollViewCellNode* lt = NULL;
	CCARRAY_FOREACH(m_pLsTouches, pObj) {
		lt = dynamic_cast<CScrollViewCellNode*>(pObj);
		if (lt) {
			if (lt->selfCheck(ccTouch, this))
            {
				if (lsTouch == NULL)
					lsTouch = lt;
                //				else
                //					// 如果已存在符合条件元素，比较优先级
                //					lsTouch = getPriorityTouch(lsTouch, lt);
			}
		}
	}
    // 返回 可以为空
    touchEventAction(lsTouch, type);
	return true;
}

CScrollViewCellNode* CScrollViewCellTouchEvent::getPriorityTouch(CScrollViewCellNode* a, CScrollViewCellNode* b) {
	// 触摸优先级通过 CCNode 树判断，也既是显示层次级别等因素
	// 以当前元素为“根”向父类转型，以便获取地址比较对象，LsTouchEvent 的对象必须同时直接或者简介继承 CCNode
	CCNode* nLsTe = dynamic_cast<CCNode*>(this);
    
	// 共同的分枝
	CCNode* allParent = NULL;
	// 寻找 a 与 b 共同的分枝
	CCNode* nAParent = a;
	CCNode* nBParent = b;
	CCNode* nAChild = NULL;
	CCNode* nBChild = NULL;
	do {
		nAChild = nAParent;
		nAParent = nAParent->getParent();
		if (!nAParent)
			break;
        
		nBParent = b;
		do {
			nBChild = nBParent;
			nBParent = nBParent->getParent();
			if (!nBParent)
				break;
			if (nAParent == nBParent) {
				allParent = nAParent;
				break;
			}
			if (nBParent == nLsTe) {
				break;
			}
		} while (1);
		if (allParent)
			break;
		if (nAParent == nLsTe) {
			break;
		}
	} while (1);
    
	// 此处只需要判断 nAChild 和 nBChild 的优先级即可，默认返回 a
	if (!nAChild || !nBChild)
		return a;
	// 根据 ZOrder 判断，如果 ZOrder一样，根据索引位置判断
	if (nAChild->getZOrder() == nBChild->getZOrder())
		return allParent->getChildren()->indexOfObject(nAChild) > allParent->getChildren()->indexOfObject(nBChild)? a: b;
	else
		return nAChild->getZOrder() > nBChild->getZOrder()? a: b;
}
CSceneGameShopItem::CSceneGameShopItem()
{
    
}
CSceneGameShopItem::~CSceneGameShopItem()
{
    
}
CSceneGameShopItem * CSceneGameShopItem::Create(CStructShopSellItem *Item)
{
    CSceneGameShopItem *pShopItem=new CSceneGameShopItem();
    if(!pShopItem || !pShopItem->init(Item))
    {
        delete pShopItem;
        pShopItem=NULL;
        return NULL;
    }
    pShopItem->autorelease();
    return pShopItem;
    
}
CCRect CSceneGameShopItem::getBuyItemRect()
{
    return  getChildByTag(20)->boundingBox();
}

CCSize CSceneGameShopItem::getBuyItemSize()
{
    return getChildByTag(20)->getContentSize();
}
void CSceneGameShopItem::setTouch()
{
    if(getChildByTag(20))
    {
        ((CGameButtonControl *)getChildByTag(20))->selected();
    }
}



void CSceneGameShopItem::setUnTouch()
{
    if(getChildByTag(20))
    {
        ((CGameButtonControl *)getChildByTag(20))->unselected();
    }
}

bool CSceneGameShopItem::init(CStructShopSellItem *item)
{
    if (!super::init())
    {
        return false;
    }
    m_pItem=item;
    if (m_pItem) {
        CCSprite *sprite=CCSprite::createWithSpriteFrameName("shangchengzixiang.png");
        this->addChild(sprite,0,1);
    }
    CCLabelTTF *labelTTF=CCLabelTTF::create(m_pItem->getName().c_str(), "Arial", 15);
    addChild(labelTTF, 1, 10);
    labelTTF->setPosition(ccp(0,85));
    //    const CCSize& dimensions, CCTextAlignment hAlignment
    CCLabelTTF *labelTTFTip=CCLabelTTF::create(m_pItem->getTip().c_str(), "Arial", 15,CCSizeMake(130, 100),kCCTextAlignmentLeft);
    addChild(labelTTFTip, 1, 11);
    labelTTFTip->setAnchorPoint(ccp(0,0.5));
    labelTTFTip->setPosition(ccp(0,0));
    
    CCSprite *itemPng=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, m_pItem->getItemPng().c_str()));
    addChild(itemPng,1,12);
    itemPng->setPosition(ccp(-70,8));
    char priceData[40];
    if(m_pItem->isXiangLiangItem())
    {
        CCSprite *xianliang=CCSprite::createWithSpriteFrameName("xianggou.png");
        addChild(xianliang,1,13);
        xianliang->setPosition(ccp(-110, 60));
        //设置一个限量的比例
        int value=m_pItem->getItemSellMaxNum();
        sprintf(priceData, "现货供应量:%d",value);
        CCLabelTTF *xianglianggeshu=CCLabelTTF::create(priceData, "Arial", 15);
        addChild(xianglianggeshu,1,21);
        xianglianggeshu->setAnchorPoint(ccp(0, 0));
        xianglianggeshu->setPosition(ccp(-120, -90));
        xianglianggeshu->setColor(g_custom_color[17]);
    }
    if (m_pItem->isDazheItem()) {
        CCSprite *zuanshidazhe=CCSprite::createWithSpriteFrameName("zuanshi.png");
        addChild(zuanshidazhe,1,14);
        zuanshidazhe->setPosition(ccp(-70,-40));
        sprintf(priceData, "%d",m_pItem->getOldValue());
        CCLabelTTF *labelttf=CCLabelTTF::create(priceData, "Arial", 15);
        addChild(labelttf,1,15);
        labelttf->setPosition(ccp(-50,-40));
        float width=labelttf->getContentSize().width+20;
        CCLayerColor *laycolor=CCLayerColor::create(ccc4(255, 0, 0, 255), width, 5);
        addChild(laycolor, 1, 151);
        laycolor->ignoreAnchorPointForPosition(false);
        laycolor->setAnchorPoint(ccp(0.5,0.5));
        laycolor->setPosition(ccp(-60,-40));
        
    }
    CCSprite *zuanshidazhenow=CCSprite::createWithSpriteFrameName("zuanshi.png");
    addChild(zuanshidazhenow,1,16);
    zuanshidazhenow->setPosition(ccp(80,-40));
    sprintf(priceData, "%d",m_pItem->getValue());
    CCLabelTTF *labelttfnew=CCLabelTTF::create(priceData, "Arial", 15);
    addChild(labelttfnew,1,17);
    labelttfnew->setPosition(ccp(110, -40));
    labelttfnew->setColor(g_custom_color[13]);
    
    string word = Utility::getWordWithFile("word.plist", "goumai");
    CGameButtonControl *pGamebutton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "Use_Normal.png", "Use_Pressed.png");
    addChild(pGamebutton,1,20);
    pGamebutton->setFontColor(g_custom_color[17]);
    pGamebutton->setPosition(ccp(80,-80));
    if (m_pItem->getItemSellMaxNum()==0) {
        pGamebutton->setEnable(false);
    }
    return true;
}

void CSceneGameShopItem::updateXiangGou()
{
    if(getChildByTag(21))
    {
        char priceData[20];
        int value=m_pItem->getItemSellMaxNum();
        sprintf(priceData, "限量%d",value);
        ((CCLabelTTF *)getChildByTag(21))->setString(priceData);
    }
    
    if (m_pItem->getItemSellMaxNum()==0)
    {
        ((CGameButtonControl *)getChildByTag(20))->setEnable(false);
    }
}

CSceneGameShopLayer::CSceneGameShopLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"shangchenganniu.plist"));
    m_enHttpReuslt=EN_LOADING_HTTPRESUALT_NONE;
    m_nUpdateTime=INT_MAX;
    m_pPlayer = SinglePlayer::instance();
    initLayerData();
}
void CSceneGameShopLayer::initLayerData()
{
    m_nCurPage=0;
    m_backTouch=NULL;
    m_nUpdateTime=INT_MAX;
    touchIndexCell=-1;
    m_pPlayer->setReBackXiangLiang();

    
}
CSceneGameShopLayer::~CSceneGameShopLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"shangchenganniu.plist"));
}

void CSceneGameShopLayer::createQuNiuDanButton()
{
    string word = Utility::getWordWithFile("word.plist", "quniudan");
    
    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    
    CCNode *node=Utility::getNodeByTag(this, "1,1,2");
    CCPoint point;
    if(node)
    {
        sprite->setPosition(node->getPosition());
        sprite->setAnchorPoint(node->getAnchorPoint());
        addChild(sprite,node->getZOrder(),7);
        node->removeFromParentAndCleanup(true);
    }
    
}
void CSceneGameShopLayer::createQuChongZhi()
{
    string word = Utility::getWordWithFile("word.plist", "quchongzhi");
    
    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    CCNode *node=Utility::getNodeByTag(this, "1,1,3");
    if(node)
    {
        sprite->setPosition(node->getPosition());
        sprite->setAnchorPoint(node->getAnchorPoint());
        addChild(sprite,node->getZOrder(),8);
        node->removeFromParentAndCleanup(true);
        
    }
}
bool CSceneGameShopLayer::createShopItem()
{
    
    if(getChildByTag(777))
    {
        removeChildByTag(777, true);
    }
    // 创建一个 CCScrollView, 内容大小和当前的界面一样
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    m_pScrollView= cocos2d::extension::CCScrollView::create(CCSizeMake(850, 670));
    m_pScrollView->setContainer(getContainLayer());
    m_pScrollView->setTouchEnabled(false);
    m_pScrollView->setPosition(ccp(80,0));
    CCLog("m_pScrollView::%f,%f",m_pScrollView->getAnchorPoint().x,m_pScrollView->getAnchorPoint().y);
    
    addChild(m_pScrollView,2,tagScrollView);
    setCurPageBall();
    if (m_nUpdateTime!=-1)
    {
        if(m_nUpdateTime==0)
        {
            m_nUpdateTime=5;
        }
        schedule(schedule_selector(CSceneGameShopLayer::selectorUpdateTime), 1.0);
    }
    m_enHttpReuslt=EN_LOADING_HTTPRESUALT_END;
    return true;
    
}


void CSceneGameShopLayer::selectorUpdateTime(float t)
{
    //得到某个值 然后做判断
    if(getChildByTag(112))
    {
        if (m_nUpdateTime ==0 && m_enHttpReuslt==EN_LOADING_HTTPRESUALT_END)
        {
            m_enHttpReuslt=EN_LOADING_HTTPRESUALT_RUN;
            unschedule(schedule_selector(CSceneGameShopLayer::selectorUpdateTime));
            //发送更新请求
            m_pPlayer->setReBackXiangLiang();
            sendGetShopItemData();
        }
        else {
            if(m_nUpdateTime>0)
            {
                m_nUpdateTime--;
                CCLabelTTF *labelTTF=(CCLabelTTF *)(getChildByTag(112));
                labelTTF->setString(CPtTool::stringForObjectValue(m_nUpdateTime));
            }
        }
        
    }
    
}

bool  CSceneGameShopLayer::init()
{
    //his->m_bTimerStatus=EN_GLOBALTIMER_STOP;
    
    long value=clock();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "shangdianjiemian.plist"));
    
    createQuNiuDanButton();
    createQuChongZhi();
    m_enHttpReuslt=EN_LOADING_HTTPRESUALT_RUN;
    //应该调用一个读取刷新商店信息的接口
    createLoadLayer();
    createShopName();
    createUpdateTime();
    updateQianBi();
    schedule(schedule_selector(CSceneGameShopLayer::schudelUpdateQianbi), 2.0);
    scheduleOnce(schedule_selector(CSceneGameShopLayer::sendGetShopItemData), 1.0);
    CCLog("stop:%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
    setTouchEnabled(true);
    return true;
}

CCLayer* CSceneGameShopLayer::getContainLayer()
{
    // 48 个关卡
    int levelCount = SinglePlayer::instance()->getShopItemCount() ;
    int widthCount = 3;
    int heightCount = 3;
    m_nPageCount = (levelCount - 1) / (widthCount * heightCount) + 1;
    
    CCLog("总共：%d, 页数：%d", levelCount, m_nPageCount);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCLayer* layer = CCLayer::create();
    layer->setPosition(CCPointZero);
    
    // 添加关卡项
    for (int i = 0; i < levelCount; i ++) {
        CCNode* shopItem = getSpriteByLevel(i);
        shopItem->setAnchorPoint(ccp(0,0));
        if(!shopItem)
        {
            continue;
        }
        // 设置位置
        int curPageIndex = i / (widthCount * heightCount);
        float xOffset = i % widthCount * 282+150;
        float yOffset = winSize.height - ((i % (widthCount * heightCount)) / widthCount * 202+200);
        shopItem->setPosition(ccp(xOffset + winSize.width * curPageIndex, yOffset));
        layer->addChild(shopItem ,1001, i);
        CCLog("append item%d",i);
    }
    CCLog("m_nPageCount=%d",m_nPageCount);
    layer->setContentSize(CCSizeMake(winSize.width * m_nPageCount, winSize.height));
    return layer;
}

CCNode* CSceneGameShopLayer::getSpriteByLevel(int level)
{
    CGamePlayer *pGamePlayer=SinglePlayer::instance();
    if(pGamePlayer->getShopItemById(level))
    {
        CSceneGameShopItem *itemSprite=CSceneGameShopItem::Create(pGamePlayer->getShopItemById(level));
        // 触摸处理
        CScrollViewCellNode* touch = CScrollViewCellNode::create();
        touch->setDisplay(itemSprite);
        this->addLsTouch(touch, level);
        
        return touch;
    }
    return NULL;
}
/*
 * @breif :当前购买象金币是否足够
 * @param : tagIdVector ui 中项的index
 */
bool CSceneGameShopLayer::checkIsHaveEnoughCashForOneItem(int tagIdVector)
{
    CStructShopSellItem *pItem=m_pPlayer->getShopItemById(tagIdVector);
    return checkIsHaveEnoughCashForOneItem(pItem);
}

bool CSceneGameShopLayer::checkIsHaveEnoughCashForOneItem(CStructShopSellItem *inRechargeItem)
{
    bool bRet = false;
    if(inRechargeItem)
    {
        CCLog("物品单价:%d,玩家金额:%d", inRechargeItem->getValue(),m_pPlayer->getPlayerCash());
        if (inRechargeItem->getValue() <= m_pPlayer->getPlayerCash())
        {
            return true;
        }
    }
    
    return bRet;
}

/*
 * @breif : 背包中是否足够格子容纳购买的一组道具
 */
bool CSceneGameShopLayer::checkIshaveEounghBackpackGridWithItem(CStructShopSellItem *inRechargeItem)
{
    
    bool bRet = false;
    int count = m_pPlayer->getPropMaxCountAddToBag(inRechargeItem->getShopSellItemPropData()->getPropId())/inRechargeItem->getGroupNum();
    bRet = (count == 0) ? false : true;
    return bRet;
}


/*
 *
 */
int CSceneGameShopLayer::checkRechargeItemState(CStructShopSellItem * inRechargeItem)
{
    
    if (!checkIsHaveEnoughCashForOneItem(inRechargeItem))
    {
        return  NO_ENOUGH_CASH;
    }
    if(!checkIshaveEounghBackpackGridWithItem(inRechargeItem))
    {
        return NO_ENOUGH_GRID;
    }
    
    return ENOUGH_EVERYTHING;
}

void CSceneGameShopLayer::sendGetShopItemData()
{
    if (getChildByTag(tagNodeBall))
    {
        removeChildByTag(tagNodeBall, true);
    }
    removeScrollView();
    if (!getChildByTag(777))
    {
        createLoadLayer();
    }
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    postStrdata+="&shop_id=800000";
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETLIMITITEMS(194), "CALLBACK_CSceneGameShopLayer_getLimitItems", "REQUEST_CSceneGameShopLayer_getLimitItems",postStrdata.c_str(),callfuncO_selector(CSceneGameShopLayer::decodeShopItemData));
}

void CSceneGameShopLayer::subNowItem(int itemId,int nowHaveNumber)
{
    SinglePlayer::instance()->setShopItemByItemId(itemId, nowHaveNumber);
}
void CSceneGameShopLayer::createLoadLayer()
{
    if(!getChildByTag(777))
    {
        CCLabelTTF *label=CCLabelTTF::create("等待更新服务器商店数据", "Arial", 40);
        addChild(label,2,777);
        label->setColor(g_custom_color[14]);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        label->setPosition(ccp(size.width*0.5,size.height*0.5));
    }
}
void CSceneGameShopLayer::createShopName()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create(m_pPlayer->getShopName().c_str(), "Arial", 20);
    addChild(labelTTF,2,111);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    labelTTF->setPosition(ccp(150,700));//,
}
void CSceneGameShopLayer::schudelUpdateQianbi(float t)
{
    if (getChildByTag(114))
        {
            char data[20];
            sprintf(data, "%d",m_pPlayer->getPlayerCash());
            ((CCLabelTTF *)getChildByTag(114))->setString(data);
    }
    

}
void CSceneGameShopLayer::updateQianBi()
{
    if (!getChildByTag(114))
    {
        CCSprite *sprite=CCSprite::createWithSpriteFrameName("zuanshi.png");
        addChild(sprite,2,113);
        sprite->setPosition(ccp(480,700));
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 20);
        labelTTF->setColor(g_custom_color[3]);
        labelTTF->setPosition(ccp(540, 700));
        addChild(labelTTF,2,114);
    }
    char data[20];
    sprintf(data, "%d",m_pPlayer->getPlayerCash());
    ((CCLabelTTF *)getChildByTag(114))->setString(data);
}


void CSceneGameShopLayer::createUpdateTime()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 20);
    addChild(labelTTF,2,112);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    labelTTF->setPosition(ccp(400,700));
    labelTTF->setColor(ccGREEN);
}

void CSceneGameShopLayer::tishiGoumaiChenggong()
{
    if(!getChildByTag(9999))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("购买成功", "Arial", 45);
        addChild(labelttf,tagScrollView+1,9999);
    }
    CCLabelTTF *labelttf =(CCLabelTTF *)getChildByTag(9999);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    labelttf->stopAllActions();
    labelttf->setPosition(ccp(size.width *0.5, size.height*0.5));
    labelttf->setOpacity(255);
    labelttf->runAction(CCSpawn::create(CCMoveBy::create(1.0, ccp(0,100)),CCFadeOut::create(0.8),NULL));
}

void CSceneGameShopLayer::removeScrollView()
{
    initLayerData();
    touchIndexCell=-1;
    removeAllTouch();
    if(getChildByTag(tagScrollView))
    {
        CCLayer *layer=(CCLayer *)((CCScrollView *)getChildByTag(tagScrollView))->getContainer();
        if(layer)
        {
            removeChild(layer, true);
        }
        removeChildByTag(tagScrollView, true);
    }
}

void CSceneGameShopLayer::updateNumber(int itemId)
{
    //判断该物品在vector中的第几个然后去做删除数量就可以了。
    if(touchIndexCell!=-1)
    {
        CScrollViewCellNode *temp=getObjectTouchNode(touchIndexCell);
        if (temp)
        {
            temp->setUpdateDataXiangGou();
        }
    }
    touchIndexCell=-1;
}

void CSceneGameShopLayer::onClickPurchase(int inTagId)
{
    CStructShopSellItem *pItem=SinglePlayer::instance()->getShopItemById(inTagId);
    if (pItem)
    {
        if (pItem->getItemSellMaxNum()==0) {
            return;
        }
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        int state = checkRechargeItemState(pItem);
        CCLayer *layer = NULL;
        if (state == NO_ENOUGH_CASH)
        {
            layer = CGameRechargeTip::create();
        }
        else if(state == NO_ENOUGH_GRID)
        {
            layer = CGameArrageBackpackTip::create();
        }
        else if(state == ENOUGH_EVERYTHING)
        {
            CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CSceneGameShopLayer::sendSendBuyItemData), "CGameRechargeLayercallback_layer", NULL);
            touchIndexCell=inTagId;
            layer = CGameRechargeLayer::create(pItem);
        }else
        {
            CCMessageBox("校验错误", "购买错误提升");
            return;
        }
        
        addChild(layer, 10000);
        
        
    }else
    {
        CCMessageBox("点击Item获取为NULL", "购买错误提示");
    }
    
    
}

void CSceneGameShopLayer::sendSendBuyItemData(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CGameRechargeLayercallback_layer");
    int *pSellNumberData=(int *)object;
    int nSellNumberData=*pSellNumberData;
    delete pSellNumberData;
    pSellNumberData=NULL;
    if(nSellNumberData <=0)
    {
        return ;
    }
    else{
        if (touchIndexCell!=-1)
        {
            m_enHttpReuslt=EN_LOADING_HTTPRESUALT_RUN;
            //获取该item里面的信息。 然后得到其prop的信息
            string postStrdata="sig=";
            postStrdata+=SinglePlayer::instance()->getUserSig();
            postStrdata+="&shop_id=800000";
            CStructShopSellItem *pStructSellItem=SinglePlayer::instance()->getShopItemById(touchIndexCell);
            if (pStructSellItem==NULL) {
                m_enHttpReuslt=EN_LOADING_HTTPRESUALT_END;
                return ;
            }
            char buf[50];
            sprintf(buf, "&item_id=%d&num=%d",pStructSellItem->propID(),nSellNumberData);
            postStrdata += buf;
            ADDHTTPREQUESTPOSTDATA(STR_URL_GETSHOPBUY(194), "CALLBACK_CSceneGameShopLayer_decodeShopItemSell", "REQUEST_CSceneGameShopLayer_decodeShopItemSell",postStrdata.c_str(),callfuncO_selector(CSceneGameShopLayer::decodeShopItemSell));
        }
        
    }
}
// {"code":0,"result":{"buy_items":[],"reward":{"dec":{"shop":{"cash":450}},"add":{"shop":{"item":{"100004":5}}}}}}
void CSceneGameShopLayer::decodeShopItemSell(CCObject *object)
{
   CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneGameShopLayer_decodeShopItemSell");
    if(!object)
    {
        CCMessageBox("破服务端发送数据为空", "破服务端");
        return;
    }
    char*resultStr=(char *)object;
    CCLog("%s",resultStr);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(resultStr);
    if(resultStr)
    {
        delete [] resultStr;
        resultStr=NULL;
    }
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut ==0)
    {
        string teamStrType;
        CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
        if (resualt)
        {
            {
                //解析购买的物品要刷新的东西
                if (resualt->objectForKey("buy_items")) {
                    teamStrType=typeid(*(resualt->objectForKey("buy_items"))).name();
                    if(teamStrType.find("CCDictionary")!=std::string::npos)
                    {
                        CCDictionary *buyitems=(CCDictionary*)(resualt->objectForKey("buy_items"));
                        if(buyitems)
                        {
                            CCDictElement* pElement = NULL;
                            CCDICT_FOREACH(buyitems, pElement)
                            {
                                const char* propId = pElement->getStrKey();
                                int propNum=GameTools::intForKey(propId, buyitems);
                                subNowItem(atoi(propId),propNum);
                                updateNumber(touchIndexCell);
                            }
                        }
                    }
                }
                

            }
            
            {
            CCDictionary *rewardResult=(CCDictionary*)(resualt->objectForKey("reward"));
            if(rewardResult){
                CReward *reward =NULL;
                if (rewardResult->objectForKey("add") && ((CCDictionary*)rewardResult->objectForKey("add"))->objectForKey("shop") )
                {
                    reward = CReward::create((CCDictionary *)((CCDictionary*)rewardResult->objectForKey("add"))->objectForKey("shop"));
                    reward->excuteReward(ADD);
                    
                }
                if(rewardResult->objectForKey("dec") &&((CCDictionary*)rewardResult->objectForKey("dec"))->objectForKey("shop"))
                {
                    reward = CReward::create((CCDictionary *)((CCDictionary*)rewardResult->objectForKey("dec"))->objectForKey("shop"));
                    reward->excuteReward(DEC);
                }

                }
            }
            tishiGoumaiChenggong();
            m_enHttpReuslt=EN_LOADING_HTTPRESUALT_END;
            return ;
        }
    }
    CCMessageBox("服务端说有错误", "error");
    m_enHttpReuslt=EN_LOADING_HTTPRESUALT_END;
}

void CSceneGameShopLayer::decodeShopItemData(CCObject *object)
{
    //更新下数据读取商店里面限购物品的东西
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSceneGameShopLayer_getLimitItems");
    if(!object)
    {
        CCMessageBox("服务端发送数据为空", "服务端");
        return;
    }
    char *strdata=(char *)object;
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut ==0)
    {
        string teamStrType;
        CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
        if(resualt)
        {
            //读取时间
            m_nUpdateTime=GameTools::intForKey("over_time", resualt);
            teamStrType=typeid(*(resualt->objectForKey("buy_items"))).name();
            if(teamStrType.find("CCDictionary")!=std::string::npos)
            {
                CCDictionary *buyitems=(CCDictionary*)(resualt->objectForKey("buy_items"));
                if(buyitems)
                {
                    CCDictElement* pElement = NULL;
                    CCDICT_FOREACH(buyitems, pElement)
                    {
                        const char* propId = pElement->getStrKey();
                        int propNum=GameTools::intForKey(propId, buyitems);
                        subNowItem(atoi(propId),propNum);
                    }
                }
            }
            createShopItem();
            return;
            
        }
    }
    //修改本地的值得
    m_enHttpReuslt=EN_LOADING_HTTPRESUALT_END;
    CCLog("shop code %d",codeReslut);
    CCMessageBox("服务端传递的code不成功", "error");
    return;
}



void CSceneGameShopLayer::adjustScrollView(float offset)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 我们根据 offset 的实际情况来判断移动效果
    if (offset < 0)
        m_nCurPage ++;
    else if (offset > 0)
        m_nCurPage --;
    
    if (m_nCurPage < 0)
        m_nCurPage = 0;
    else if (m_nCurPage > m_nPageCount - 1)
        m_nCurPage = m_nPageCount - 1;
    
    CCPoint adjustPoint = ccp(-winSize.width * m_nCurPage , 0);
    getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
    
    CCLog("current page index: %d", m_nCurPage);
}

void CSceneGameShopLayer::setCurPageBall()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	float fs = 0.2f;
	float scale = 0.7f;
	// 根据分屏数量显示小圆球标示
	CCNode* layer = this->getChildByTag(tagNodeBall);
	if (layer)
	{
		CCLayer* lyBall = dynamic_cast<CCLayer*>(layer);
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCNode* n = lyBall->getChildByTag(i);
			if (n)
			{
				CCSprite* sprite = dynamic_cast<CCSprite*>(n);
				if (m_nCurPage == i)
				{
					sprite->runAction(
                                      CCScaleTo::create(fs,
                                                        1.0f));
				}
				else
				{
					sprite->runAction(
                                      CCScaleTo::create(0.5,
                                                        scale));
				}
			}
		}
	}
	else
	{
		CCLayer* lyBall = CCLayer::create();
		this->addChild(lyBall, 1002, tagNodeBall);
        
		int ball_width = 30;			// 圆球间距
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCSprite* sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"ball.png"));
			sprite->setPosition(ccp(ball_width / 2 + (i - 1) * ball_width, ball_width / 2));
			sprite->setScale(scale);
			lyBall->addChild(sprite,1, i);
			if (i == m_nCurPage)
			{
				sprite->runAction(CCScaleTo::create(fs, 1.0f));
			}
		}
        float xPoint= size.width / 2 - ball_width * (m_nPageCount - 1) / 2;
        float yPoint=50;
        if (this->m_pScrollView->boundingBox().containsPoint(CCPointMake(xPoint, yPoint))) {
            CCLog("AAAAAAAAAAAAAAAA=%f,%f,%f,%f",xPoint,yPoint,m_pScrollView->getAnchorPoint().x,m_pScrollView->getAnchorPoint().y);
        }
		lyBall->setPosition(xPoint,yPoint);
	}
}

void CSceneGameShopLayer::touchEventAction(CScrollViewCellNode *touch, int type){
    // type 事件类型，0：touchbegin 1：touchend 触发 2：touchend 未触发
    if (touch)
        CCLog("touch event action id: %d  type: %d", touch->getEventId(), type);
    //const int selectTag = 10001;
    if (type == 0 && touch)
    {
        touch->setTouch();
        m_backTouch=touch;
    }
    else
    {
        if(m_backTouch)
        {
            m_backTouch->setUnTouch();
            m_backTouch=NULL;
            touchIndexCell=-1;
        }
    }
    
    if (type == 1 && touch)
    {
        
        onClickPurchase(touch->getEventId());
    }
}
bool CSceneGameShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_enHttpReuslt==EN_LOADING_HTTPRESUALT_RUN)
    {
        return true;
    }
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    if(node && node->boundingBox().containsPoint(touchPoint))
    {
        m_tempTouchNode=node;
    }
    else if(getChildByTag(7) && getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(7))->selected();
        m_tempTouchNode=getChildByTag(7);
    }
    else if(getChildByTag(8) && getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(8))->selected();
        m_tempTouchNode=getChildByTag(8);
    }
    else{
        m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
        if (getScrollView()) {
            // 发送触摸消息，并会在 touEventAction 自动相应， 如果触摸到元素
            sendTouchMessage(pTouch, 0);
            m_touchOffset = getScrollView()->getContentOffset();
        }
        
        
    }
    return true;
}

void CSceneGameShopLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_enHttpReuslt==EN_LOADING_HTTPRESUALT_RUN)
    {
        return ;
    }
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(pTouch->getLocation()))
    {
        if(m_tempTouchNode!=node)
        {
            if(m_tempTouchNode==getChildByTag(7))
            {
                ((CGameButtonControl *)getChildByTag(7))->unselected();
            }
            m_tempTouchNode=node;
        }
    }
    else if(getChildByTag(7) && getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        if( m_tempTouchNode && m_tempTouchNode!=Utility::getNodeByTag(this, "1,1,1"))
        {
            if(m_tempTouchNode)
            {
                CGameButtonControl *tempdatabutton=dynamic_cast<CGameButtonControl *>(m_tempTouchNode);
              if(tempdatabutton)
              {
                  tempdatabutton->unselected();
              }
            }
        }
        ((CGameButtonControl *)getChildByTag(7))->selected();
        m_tempTouchNode=getChildByTag(7);
    } else if(getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        if( m_tempTouchNode && m_tempTouchNode!=Utility::getNodeByTag(this, "1,1,1"))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        ((CGameButtonControl *)getChildByTag(8))->selected();
        m_tempTouchNode=getChildByTag(8);
    }
    else
    {
        if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
            m_tempTouchNode=NULL;
        }
        CCPoint movePoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
        float distance = movePoint.x - m_touchPoint.x;
        // 设定当前偏移位置
        CCPoint adjustPoint = ccp(m_touchOffset.x + distance, 0);
        if(getScrollView())
        {
            getScrollView()->setContentOffset(adjustPoint, false);
        }
    }
}

void CSceneGameShopLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_enHttpReuslt==EN_LOADING_HTTPRESUALT_RUN)
    {
        return ;
    }
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    // m_newPoint=touchPoint;
    // CCLog("%f,%f,%f",m_oldPoint.x,m_newPoint.x,ccpDistance(m_oldPoint, m_newPoint));
    //  m_oldPoint=m_newPoint;
    if(node&&node->boundingBox().containsPoint(touchPoint))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if(m_tempTouchNode==node)
        {
            removeFromParentAndCleanup(true);
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint)|| getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (m_tempTouchNode!=node && m_tempTouchNode)
        {
            if (dynamic_cast<CGameButtonControl *>(m_tempTouchNode)) {
                ((CGameButtonControl *)m_tempTouchNode)->unselected();
            }
        }
        m_tempTouchNode=NULL;
        if (getChildByTag(8)->boundingBox().containsPoint(touchPoint)) {
            CPtRecharge * layer = CPtRecharge::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000000);
            //removeFromParentAndCleanup(true);
           // CCNotificationCenter::sharedNotificationCenter()->postNotification("CHONGZHIJIEMIAN");
            return;
        }
    }
    else if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
        ((CGameButtonControl *)m_tempTouchNode)->unselected();
        m_tempTouchNode=NULL;
    }
    else
    {
        if(getScrollView())
        {
            CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
            float distance = endPoint.x - m_touchPoint.x;
            float distanceY = endPoint.y - m_touchPoint.y;
            if (fabs(distance) < 10 && fabs(distanceY) < 10)
            {
                // 小于三，不做拖动操作，也排除了（抖动误操作）,第二个参数，事件类型 1： touch end， 由 touchEventAction 自动相应
                sendTouchMessage(pTouch, 1);
                
            }
            else if (fabs(distance) > 50)
            {
                // 大于 50，执行拖动效果
                adjustScrollView(distance);
                setCurPageBall();
            }else
            {
                // 回退为拖动之前的位置
                adjustScrollView(0);
            }
            sendTouchMessage(pTouch, 2);
        }
    }
    
}
void CSceneGameShopLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    adjustScrollView(0);
}
void CSceneGameShopLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-8, true);
}
void CSceneGameShopLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}