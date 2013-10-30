//
//  CDrawCardLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-18.
//
//

#ifndef ___1_cube__CDrawCardLayer__
#define ___1_cube__CDrawCardLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;
using namespace std;

/*******
 *定义一个抽取卡牌的界面
 *
 */
enum EN_NIUDANTYPE {
    EN_NIUDANTYPE_FRIEND = 1,
    EN_NIUDANTYPE_CASH = 2
    };
enum EN_NIUDANLAYER
{
    EN_NIUDANLAYER_CHOOSETYPE,
    
    EN_NIUDANLAYER_ENDGETCARD,
};

class CGamePlayer;
class CGameButtonControl;
class CFightCard;

struct ScanDrawValue {
    ScanDrawValue(){m_nCount=0;m_nNextTimer=5;}
    int m_nCount;
    int m_nNextTimer;
};



enum EN_NIUDANHTTPSTATUS {
    EN_NIUDANHTTPSTATUS_NONE = 0,
    EN_NIUDANHTTPSTATUS_INIT = 1,
    EN_NIUDANHTTPSTATUS_RUNNINT,
    EN_NIUDANHTTPSTATUS_END
    };

class CDrawCardLayer: public cocos2d::CCLayer,public cocos2d::extension::CCTableViewDataSource,public cocos2d::extension::CCTableViewDelegate
{
public:
    CDrawCardLayer();
    ~CDrawCardLayer();
    CREATE_FUNC(CDrawCardLayer);
    bool init();
private:
    void updateFriendTimer(float t);
    void updateDiamondTimer(float t);
    void initSendGetData(float t);
    void decodeInitEnterDrawCardLayer(CCObject *object);
    void decodeParseFriendDraw(CCObject *object);
    void decodeParseCashDraw(CCObject *object);
    void decodeGetCardRandom(CCObject *object);
    void sendHttpToFriendGetValue(float t);
    void sendHttpToCashGetValue(float t);
    void createGetCardLayer(EN_NIUDANTYPE  tempEntype);
    bool checkIsCanDoDrawCard(EN_NIUDANTYPE entype);
    void sendGetCardRandom(EN_NIUDANTYPE entype,int number,bool isFree);
    void rebackFirstLayer();

    bool getScanData(const char *value,CCDictionary *dict,ScanDrawValue &rebackValue);
    bool touchNumberAndType(int number, EN_NIUDANTYPE typeTouch,bool isFree);
    void moveOutEyeSize();
    void createTableView();
    void createNiudanButton(EN_NIUDANTYPE entype);
    void createTexiao();
protected:
    CCSize cellSizeForTable(CCTableView *table) ;
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void  initCellItem(CCTableViewCell*cell, unsigned int idx);

private:
    void createJiantou();
    void removeJiantouAndClearVector();
    void createBackGround();
    void createBackYingLing();
    void createLoadingLayer();
    void removeLoadingLayer();
    void createTopButton();
    void decodeGetData(CCObject *object,EN_NIUDANTYPE type);  //type 1 友情点数  ,type 2 消耗钻石扭蛋
    bool sendGetDataHttp(EN_NIUDANTYPE type);
    void startUpdateCashAndFriendly(CCDictionary *resultDict);
    void updateButton();
    void createLabelFriendAndUpdate();
    void createLabelCashAndUpdate();
    void hideLable();
    void showCardData(CCDictionary *dict);
    void clearSelectFreeItemLayer();
    void createGoldAndCash();
    void updateFriendButton();
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
    void dealWithTag(int tag);
    CGameButtonControl *getTouchButton(CCPoint point);
private:
    ScanDrawValue m_stempScanData[2];
    CGamePlayer *m_tempGamePlayer;
    vector<CGameButtonControl *>touchVector;
    EN_NIUDANHTTPSTATUS m_httpStatus;
    CGameButtonControl *m_pTouchBackUpd;
    CCSize size;
    EN_NIUDANTYPE m_enSaveSendValue;
    std::vector<CFightCard*>m_vectorGetCardList;
    int m_nBackValue;
    EN_NIUDANLAYER m_enLayerStatus;
    float m_fPostionX;
};

#endif /* defined(___1_cube__CDrawCardLayer__) */
