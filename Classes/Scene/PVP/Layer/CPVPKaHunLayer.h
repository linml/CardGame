//
//  CPVPKaHunLayer.h
//  91.cube
//
//  Created by phileas on 14-1-8.
//
//

#ifndef ___1_cube__CPVPKaHunLayer__
#define ___1_cube__CPVPKaHunLayer__

#include "cocos2d.h"
#include "CCardEnchantConfig.h"
#include "CPtTableItem.h"
#include "CGamesCard.h"
#include "CPtListViewWidget.h"
//卡魂界面
const std::string UPDATEKAHUNNOTICE_TAG ="updatekahunui";
class EnchantCardItem : public CPtTableItem
{
public:
    static EnchantCardItem* create(EnchantCards *inData);
    EnchantCardItem();
    virtual ~EnchantCardItem();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    bool initEnchantCardItem(EnchantCards *inData);
    
protected:
    void onClickCompose();
    void onClickDecompose();
    
    int getDecomposeCardUserId();
    
    void onReceiveComposeMsg(CCObject *pObject);
    void onReceiveDecomposeMsg(CCObject *pObject);
    

    void updateUI();

protected:
    EnchantCards *m_pData;
    CCSprite *m_pComposeBtn;   //合成按钮
    CCSprite *m_pDecomposeBtn; //分解按钮
    int m_nTouchTag;
    int m_nSelectIndex;
    CCRect m_cTouchRect[2];
};

using namespace cocos2d;

class CPVPKaHunLayer : public CCLayer
{

public:
    CREATE_FUNC(CPVPKaHunLayer);
    
    CPVPKaHunLayer();
    virtual ~CPVPKaHunLayer();
    virtual bool init();

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

protected:
    void initCPVPKaHunLayer();
    void updateKaHunLabel(CCObject* pObject);
    void updateKanHunData(int inSelectIndex);
   
    void onClickBack();
protected:
    CCLabelTTF *m_pKaHunLabel;
    EnchantCardsContaner *m_pAllData;
    CPtListViewWidget *m_pHunDianContainer;
    
    int m_nTouchTag;
    int m_nSelectBtnIndex; // -1--> 不选 0-－>3星 ; 1-->4星 ; 2-->5星
    CCSprite *m_pStarBtns[3];
    DataExtent m_cDataExtent[3];
    CCSprite *m_pBackBtn;

};

#endif /* defined(___1_cube__CPVPKaHunLayer__) */
