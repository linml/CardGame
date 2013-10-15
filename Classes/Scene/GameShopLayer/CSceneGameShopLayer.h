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

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

/**
 * 定义可触摸元素，用于统一管理
 */
class CScrollViewCellTouchEvent;

class CScrollViewCellNode: public CCNode {
public:
	CScrollViewCellNode();
	~CScrollViewCellNode();
	CREATE_FUNC(CScrollViewCellNode);
	virtual bool init()	;
    void setTouch();
    void setUnTouch();
	// 设置显示项
	void setDisplay(CCSprite* dis);
    CCSprite* getDisplay();
    
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
    
	bool sendTouchMessage(CCTouch* ccTouch, int type);
    
	// 返回优先级较高的可触摸对象
	CScrollViewCellNode* getPriorityTouch(CScrollViewCellNode* a, CScrollViewCellNode* b);
    
	virtual void touchEventAction(CScrollViewCellNode* touch, int type) = 0;
private:
	CCArray* m_pLsTouches;
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
private:
    CStructShopSellItem *m_pItem;
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
    CCNode* getSpriteByLevel(int level);
    void touchEventAction(CScrollViewCellNode* touch, int type);
    void setCurPageBall();
    void adjustScrollView(float offset);
    bool checkIsHaveEnoughCashForOneItem(int tagIdVector);
    bool checkIsHaveEnoughBagToSaveItem(int tagIdVector);
    void sendGetShopItemData();
    void decodeShopItemData(CCObject *object);
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
    CScrollViewCellNode *m_backTouch;
    CC_SYNTHESIZE(CCScrollView*, m_pScrollView, ScrollView);
};

#endif /* defined(___1_cube__CSceneGameShopLayer__) */
