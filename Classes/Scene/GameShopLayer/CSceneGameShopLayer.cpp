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

void CScrollViewCellTouchEvent::removeLsTouch(CScrollViewCellNode* touch) {
	m_pLsTouches->removeObject(touch, true);
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
    itemPng->setPosition(ccp(-70,0));
    
    if(m_pItem->isXiangLiangItem())
    {
        CCSprite *xianliang=CCSprite::createWithSpriteFrameName("xianggou.png");
        addChild(xianliang,1,13);
        xianliang->setPosition(ccp(-110, 60));
    }
    char priceData[20];
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
    labelttfnew->setPosition(ccp(100, -40));
    
    string word = Utility::getWordWithFile("word.plist", "goumai");
    CGameButtonControl *pGamebutton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "Use_Normal.png", "Use_Pressed.png");
    addChild(pGamebutton,1,20);
    pGamebutton->setFontColor(g_custom_color[17]);
    pGamebutton->setPosition(ccp(80,-80));

    return true;
}

CSceneGameShopLayer::CSceneGameShopLayer()
{
     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"shangchenganniu.plist"));
     m_nCurPage=0;
     m_backTouch=NULL;
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

bool  CSceneGameShopLayer::init()
{
    //his->m_bTimerStatus=EN_GLOBALTIMER_STOP;
  
    long value=clock();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "shangdianjiemian.plist"));
 
    createQuNiuDanButton();
    createQuChongZhi();
    
    
    // 创建一个 CCScrollView, 内容大小和当前的界面一样
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    m_pScrollView= cocos2d::extension::CCScrollView::create(CCSizeMake(1024, 680));
    m_pScrollView->setContainer(getContainLayer());
    m_pScrollView->setTouchEnabled(false);
    addChild(m_pScrollView,1000);
    setTouchEnabled(true);
    setCurPageBall();
    CCLog("stop:%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
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
        float xOffset = i % widthCount * 282 +220;
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
bool CSceneGameShopLayer::checkIsHaveEnoughCashForOneItem(int tagIdVector)
{
    CGamePlayer *gamePlayer=SinglePlayer::instance();
    CStructShopSellItem *pItem=gamePlayer->getShopItemById(tagIdVector);
    if(pItem)
    {
        CCLog("物品单价:%d,玩家金额:%d", pItem->getValue(),gamePlayer->getPlayerCash());
        if ( pItem->getValue() <= gamePlayer->getPlayerCash())
        {
                return true;
        }
    }
    return false;
}

bool CSceneGameShopLayer::checkIsHaveEnoughBagToSaveItem(int tagIdVector)
{
    
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
    const int tagNodeBall = 1002;
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
        }
    }
    if (type == 1 && touch){
        // 收到 type 为 1 表示触发关卡选择
        if (checkIsHaveEnoughCashForOneItem(touch->getEventId()))
        {
            CCString* str = CCString::createWithFormat("您选择的第%d个  .", touch->getEventId() + 1);
            CCMessageBox("提示", str->getCString());
        }
        else
        {
            CCMessageBox("警告","土豪你钱不够哦");
        }
    }
}
bool CSceneGameShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
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
        m_touchOffset = getScrollView()->getContentOffset();
        // 发送触摸消息，并会在 touEventAction 自动相应， 如果触摸到元素
        sendTouchMessage(pTouch, 0);
    }
    return true;
}

void CSceneGameShopLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
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
                ((CGameButtonControl *)m_tempTouchNode)->unselected();
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
        getScrollView()->setContentOffset(adjustPoint, false);
    }
}

void CSceneGameShopLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
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
        if (m_tempTouchNode!=node &&m_tempTouchNode) {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        m_tempTouchNode=NULL;
    }
    else if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
        ((CGameButtonControl *)m_tempTouchNode)->unselected();
        m_tempTouchNode=NULL;
    }
    else
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