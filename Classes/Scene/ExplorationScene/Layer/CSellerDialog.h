//
//  CSellerDialog.h
//  91.cube
//
//  Created by phileas on 13-10-17.
//
//

#ifndef ___1_cube__CSellerDialog__
#define ___1_cube__CSellerDialog__

#include "cocos2d.h"
#include "gameStruct.h"
#include <string>
using namespace cocos2d;
using std::string;

#define CSELLERDIALOG_TOUCH_PRORITY -50000

class CSellerDialog : public CCLayerColor
{
public:
    static CSellerDialog * create(SELLER_DATA inSellData);
public:
    CSellerDialog();
    virtual  ~CSellerDialog();
    
    bool initCSellerDialog(SELLER_DATA inSellData);
    void setCloseHandler(CCObject *inTarget, SEL_CallFuncO inSelector);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

protected:
    void initSellerDialogUI();
    
    void handlerTouch(CCTouch *pTouch);
    
    void onClickGoSellerShop();
    void onClickCancel();
    
    void loadResource();
protected:
    SELLER_DATA m_sSellerData;
    int m_nTouchTag;
    int m_nSellerType;
    int m_nShopId;
    string m_sSellerName;
    
    CCSprite * m_pGoshop;
    CCSprite * m_pCancel;
    
    CCObject *m_pCloseHandler;
    SEL_CallFuncO m_pCloseSelector;

};
#endif /* defined(___1_cube__CSellerDialog__) */
