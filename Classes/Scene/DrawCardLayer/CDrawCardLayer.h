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
using namespace std;
/*******
 *定义一个抽取卡牌的界面
 *
 */
enum EN_NIUDANTYPE {
    EN_NIUDANTYPE_FRIEND = 1,
    EN_NIUDANTYPE_CASH = 2
    };
struct ScanDrawValue {
    ScanDrawValue(){m_nCount=0;m_nNextTimer=5;}
    int m_nCount;
    int m_nNextTimer;
};
using namespace cocos2d;

class CDrawCardLayer: public cocos2d::CCLayer
{
public:
    CDrawCardLayer();
    ~CDrawCardLayer();
    void updateFriendTimer(float t);
    void updateDiamondTimer(float t);
    void initSendGetData(float t);
    void decodeInitEnterDrawCardLayer(CCObject *object);
    void decodeParseFriendDraw(CCObject *object);
    void decodeParseCashDraw(CCObject *object);
    void sendHttpToFriendGetValue(float t);
    void sendHttpToCashGetValue(float t);
    CREATE_FUNC(CDrawCardLayer);
    bool init();
    bool getScanData(const char *value,CCDictionary *dict,ScanDrawValue &rebackValue);
private:
        void decodeGetData(CCObject *object,EN_NIUDANTYPE type);  //type 1 友情点数  ,type 2 消耗钻石扭蛋
        bool sendGetDataHttp(EN_NIUDANTYPE type);
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
   // virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
   // virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
   // virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
    
    
private:
    int m_nFriendTimer;
    int m_nDiamondTimer;
    ScanDrawValue m_stempScanData[2];
    
};

#endif /* defined(___1_cube__CDrawCardLayer__) */
