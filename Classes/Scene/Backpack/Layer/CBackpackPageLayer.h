//
//  CBackpackPageLayer.h
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#ifndef ___1_cube__CBackpackPageLayer__
#define ___1_cube__CBackpackPageLayer__

#include "cocos2d.h"
#include "CBattleArrayLayer.h"
#include "CPtPropConfigData.h"
#include "LayoutLayer.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class CBackpackContainerLayer;

class CBackpackPageLayer : public CCLayer
{
public:
    static CBackpackPageLayer *create();

    static CBackpackPageLayer *create(multimap<int, int> *inGridData, multimap<int, int>::iterator inBegin, multimap<int, int>::iterator inEnd, int inOpenGridCount);
    
public:
    CBackpackPageLayer();
    virtual ~CBackpackPageLayer ();
public:
    // touch event:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool getTouchLock(){return m_bTouchLock;};
    
    // change page content:
    void updatePageContent(multimap<int, int>::iterator inBegin, multimap<int, int>::iterator inEnd, int inOpenGridCount);
    void updatePageContent(vector<multimap<int, int>::iterator> inDataIterators);
    
    void setGridData(multimap<int, int> * inGridData){m_pGridData = inGridData;};
    vector<multimap<int, int>::iterator> getCurrentGridDataIterators(){return m_cGridDataIterator;};
    void initGridPage();
protected:
    void initCBackpackPageLayer();
    void handlerTouch();
    
    void addItems(int inUseNum);
    void addMask(int inCloseNum);
    
    void clearItem(CCNode *node);
    void setItem(CCNode *node, CPtProp *inData);
    
    void initPanel(bool inResetTexture=false);
    void resetPanel();
    
    bool canOpenGrid();
    void openGrid();
    void openGridUI();
    
    void onClickOpenGrid();
    void onReceiveOpenGridMsg(CCObject *pOject);
    
    void updatePageContentUI(bool inAllProps = true);
    
    void onClickUseProp(CCObject *object);
    void onClickUseProp(int inPropId, int inPropNum);
    
    void onClickDeleteProp(CCObject *object);
    void onClickDeleteProp(int inProp, int inPropNum);
    
    void onReceiveUsePropMsg(CCObject *pOject);
    void onReceiveDeletProp(CCObject *pObject);
    
    void reloadPage(multimap<int, int>::iterator inZeroInterator, vector<multimap<int, int>::iterator>::iterator inVectorIterator);
protected:
    
    bool m_bLockRequest;
    bool m_bTouchLock;
    
    int m_nTouchTag;
    int m_nUserGridNumber;
    int m_nOpenGridNumber;
    
    CCArray * m_pItemNums;
    CCArray * m_pMasks;
    
    CCSprite *m_pDeleteButtons[9];
    CCSprite *m_pUseButton[9];
    
    // button rect:[0-2] 0: normal, 1: pressed: 2: disabled
    CCRect m_aButtonRect[3];
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    
//    CBackpackContainerLayer * m_pContainerHandler;
    multimap<int, int> * m_pGridData;
    vector< multimap<int, int>::iterator > m_cGridDataIterator;
    
    PropItem m_cPropItem;
    
    static const int AllGridInPageNumber = 9;
    static const int DeleteTag = 2000;
    static const int UseTag = 3000;
    
    CC_SYNTHESIZE(int , m_nCurrentPageTag, CurrentPageTag);
    CC_SYNTHESIZE(CBackpackContainerLayer*, m_pContainerHandler, ContainerHandler);


};



#endif /* defined(___1_cube__CBackpackPageLayer__) */
