//
//  CSceneGameShopLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#ifndef ___1_cube__CSceneGameShopLayer__
#define ___1_cube__CSceneGameShopLayer__

#include <iostream>
#include "cocos2d.h"
#include "CCScrollView.h"
#include "gamePlayer.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;



#define NO_ENOUGH_CASH 1
#define NO_ENOUGH_GRID 2
#define ENOUGH_EVERYTHING 3


enum EN_LOADING_HTTPRESUALT {
    EN_LOADING_HTTPRESUALT_NONE ,
    EN_LOADING_HTTPRESUALT_RUN,
    EN_LOADING_HTTPRESUALT_END,
    EN_LOADING_HTTPRESUALT_SHOWLAYER,
    };

class CStructShopSellItem;
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
    
    CScrollViewCellNode *getObjectTouchNode(int level);
	// 返回优先级较高的可触摸对象
	CScrollViewCellNode* getPriorityTouch(CScrollViewCellNode* a, CScrollViewCellNode* b);
    
	virtual void touchEventAction(CScrollViewCellNode* touch, int type) = 0;
private:
	CCArray* m_pLsTouches;
};



class CSceneGameShopLayer : public cocos2d::CCLayer,CScrollViewCellTouchEvent
{
    typedef  cocos2d::CCLayer Super;
public:
    CSceneGameShopLayer();
    ~CSceneGameShopLayer();
    bool  init();
    CREATE_FUNC(CSceneGameShopLayer);
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
    int checkRechargeItemState(CStructShopSellItem * inRechargeItem);
    
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
};

#endif /* defined(___1_cube__CSceneGameShopLayer__) */
