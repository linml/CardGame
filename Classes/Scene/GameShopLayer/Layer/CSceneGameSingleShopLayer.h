//
//  CSceneGameSingleShopLayer.h
//  91.cube
//
//  Created by linminglu on 13-12-19.
//
//

#ifndef ___1_cube__CSceneGameSingleShopLayer__
#define ___1_cube__CSceneGameSingleShopLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CGamePlayer;
class CStructShopSellItem;



enum EN_LOADING_HTTPRESUALT {
    EN_LOADING_HTTPRESUALT_NONE ,
    EN_LOADING_HTTPRESUALT_RUN,
    EN_LOADING_HTTPRESUALT_END,
    EN_LOADING_HTTPRESUALT_SHOWLAYER,
};


//注册一个类定义成 每个子项目的类
class CSceneGameShopItem :public CCSprite
{
    typedef  CCSprite super;
public:
    CSceneGameShopItem();
    ~CSceneGameShopItem();
    static CSceneGameShopItem * Create(CStructShopSellItem *Item);
    bool init(CStructShopSellItem *item);
    CCSize getBuyItemSize();
    CCRect getBuyItemRect();
    void setTouch();
    void setUnTouch();
    void updateXiangGou();
    void setUpdateTimeText(const char *str);
private:
    CStructShopSellItem *m_pItem;
};
/**
 * 定义可触摸元素，用于统一管理
 */
class CScrollViewCellTouchEvent;

class CScrollViewCellNode: public CCNode {
public:
	CScrollViewCellNode();
	virtual ~CScrollViewCellNode();
	CREATE_FUNC(CScrollViewCellNode);
	virtual bool init()	;
    void setTouch();
    void setUnTouch();
	// 设置显示项
	void setDisplay(CCSprite* dis);
    CCSprite* getDisplay();
    void setUpdateDataXiangGou();
	void setEventId(int eventId);
	int getEventId();
    void setTimerText(const char *str);
    
	/// 常规判断
	bool selfCheck(CCTouch* ccTouch, CScrollViewCellTouchEvent* lsTe);
    
private:
	// 判断当前的元素是否被点击
	bool containsCCTouchPoint(CCTouch* ccTouch);
	bool isParentAllVisible(CScrollViewCellTouchEvent* lsTe);
    
	// 用户保存显示精灵的 tag
	static const int TAG_DISPLAY = 100;
	int m_iEventId;
    
};

class CScrollViewCellTouchEvent {
public:
	CScrollViewCellTouchEvent();
	~CScrollViewCellTouchEvent();
    
	void addLsTouch(CScrollViewCellNode* touch, int eventId);
    
	void removeLsTouch(CScrollViewCellNode* touch);
    void removeAllTouch();
    
	bool sendTouchMessage(CCTouch* ccTouch, int type);
    void setUpdateTimer(const char *str);
    CScrollViewCellNode *getObjectTouchNode(int level);
	// 返回优先级较高的可触摸对象
	CScrollViewCellNode* getPriorityTouch(CScrollViewCellNode* a, CScrollViewCellNode* b);
    
	virtual void touchEventAction(CScrollViewCellNode* touch, int type) = 0;
private:
	CCArray* m_pLsTouches;
};
class CStructShopInfo;
class CSceneGameSingleShopLayer :public CCLayer,CScrollViewCellTouchEvent
{
public:
    CSceneGameSingleShopLayer();
    ~CSceneGameSingleShopLayer();
    bool  init(CStructShopInfo *shopInfo);
    static CSceneGameSingleShopLayer* Create(CStructShopInfo *shopInfo);
    
protected:
    
    void createQuNiuDanButton();
    void createQuChongZhi();
    CCLayer* getContainLayer();
    bool createShopItem();
    CCNode* getSpriteByLevel(int level);
    void touchEventAction(CScrollViewCellNode* touch, int type);
    void setCurPageBall();
    void adjustScrollView(float offset);
    bool checkIsHaveEnoughCashForOneItem(int tagIdVector);
    // change by phileas:
    bool checkIsHaveEnoughCashForOneItem(CStructShopSellItem *inRechargeItem);
    bool checkIshaveEounghBackpackGridWithItem(CStructShopSellItem *inRechargeItem);
    int checkCanExtendCardBag(CStructShopSellItem *inRechargeItem);
    int checkRechargeItemState(CStructShopSellItem * inRechargeItem);
    int getPlayerMoneyByType();
    
    void onClickPurchase(int inTagId);
    // change end:
    
    void sendGetShopItemData();
    
    void sendSendBuyItemData(CCObject *object);
    void decodeShopItemSell(CCObject *object);
    void decodeShopItemData(CCObject *object);
    void subNowItem(int itemId,int nowHaveNumber);
    
    void selectorUpdateTime(float t);
    void updateNumber(int nItemId);
    void removeScrollView();
    void createLoadLayer();
    void createShopName();
    void createUpdateTime();
    void tishiGoumaiChenggong();
    void updateQianBi();
    void schudelUpdateQianbi(float t);
    void initLayerData();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
private:
    CCNode *m_tempTouchNode;
    CCPoint m_touchPoint;
    CCPoint m_touchOffset;
    int m_nPageCount;
    int m_nCurPage;
    int touchIndexCell;
    EN_LOADING_HTTPRESUALT m_enHttpReuslt;
    CScrollViewCellNode *m_backTouch;
    int m_nUpdateTime;
    
    CC_SYNTHESIZE(CCScrollView*, m_pScrollView, ScrollView);
    CGamePlayer *m_pPlayer; // change by phileas
    CStructShopInfo *m_pShopInfo;
};

#endif /* defined(___1_cube__CSceneGameSingleShopLayer__) */
