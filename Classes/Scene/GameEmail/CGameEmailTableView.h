//
//  CGameEmailTableView.h
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#ifndef ___1_cube__CGameEmailTableView__
#define ___1_cube__CGameEmailTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;
using namespace std;

class CEmrysTableViewDelegate {
public:
    virtual void tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch)=0;
    virtual void tableScrolBarView(CCTableView *)=0;
protected:
    CCNode *_cellNode;
};


class CEmrysTableView:public CCTableView
{
public:
    CEmrysTableView();
    ~CEmrysTableView();
    static CEmrysTableView *Create(CCTableViewDataSource *dataSource,CCSize size,CEmrysTableViewDelegate *_mydefineDeleagte);
    virtual  void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   // virtual void ccTouchMove(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    virtual    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void scrollViewDidScroll(CCScrollView* view);
    void  reloadDataView();
public:
    CCTouch *m_pTouch;
    CCPoint m_beginTouchPoint;
    CEmrysTableViewDelegate *_mydefineDeleagte;
};
enum EN_EMAILHTTPREQUEST {
    EN_EMAILHTTPREQUEST_NONE = 0,
    EN_EMAILHTTPREQUEST_CHANEGSTATUS =1,
    EN_EMAILHTTPREQUEST_GETSINGLEITEM,
    EN_EMAILHTTPREQUEST_GETALLEMAIL
};




class CGameEmailTableView : public cocos2d::CCLayer, public CCTableViewDataSource, public CCTableViewDelegate ,public CEmrysTableViewDelegate
{
public:
    CGameEmailTableView();
    ~CGameEmailTableView();
    virtual bool initView(CCPoint p , CCSize s ,int cellNum , CCSize cellSize , CCSize tableCellSize);
    virtual bool initView(CCPoint p , CCSize s ,int cellNum , CCSprite*cellImage , int cellgap);
    static CGameEmailTableView *CreateEmailLayer();
    void initCellItem(CCTableViewCell*cell, unsigned int idx);
    void tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch);
    CREATE_FUNC(CGameEmailTableView);
    void scrollBar(CCTableView* table);
    void selector_update(float _dt);
private:
    bool initCreate();
    bool loadPlistFile();
    void createRecvAllButton();
    void creaetEmailTableView();
    void getAllEmailItem();
    void sendPostHttpChangeEmailStatus();
    void sendPostHttpGetAllItem();
    void sendPostHttpGetSingleItem();
    bool decodeRecvBackStr(char *data); //true 邮件校验成功
    void decodeSingleRecvEmail(char *data);
    void showDialogBagFull(CCObject *obect);
    void setGunDongTiaoPtr();
    
    void dialogOkButtonSetFunc(CCObject *object);
    void dialogCancelButtonSetFunc(CCObject *object);
private:
    CCNode *m_tempTouchNode;
    EN_EMAILHTTPREQUEST m_enhttpStatus;
    vector<int>canereadList;
    CCSprite *m_pScrollViewGuanDongTiao;
    float m_fOldScrollBarPosiontYtop;
    float m_foldscrollBarPosiontylow;
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();


private:
    CCNode *node;
    bool  isSendPostGetData;
public:
    void tableScrolBarView(CCTableView *tableveiw);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void reloadData();
    void runDialogAction();
    void removeDialog();
    
public:
    unsigned int numberOfCellsInTableView(CCTableView *table);
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    CCSize cellSizeForTable(CCTableView *table);
    CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    void sendPostHttpGetSingleItem(int msgID);
    void recvBockHttpCallBack(CCObject *object);
public:
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    
public:
    CCSprite*m_huaTiao;
    int m_tableCount;
    CCSize m_cellSize;
    CCSize m_tableCellSize;
    
    CCSize m_tableViewSize;
    CCPoint m_tableViewPoint;
    
    CCSprite *m_pCell;
    CCTexture2D *m_pTexture;
    CCRect m_TextureRect;
    float m_cellScaleX ;
    float m_cellScaleY ;
    CEmrysTableView * tableView;
};


#endif /* defined(___1_cube__CGameEmailTableView__) */
