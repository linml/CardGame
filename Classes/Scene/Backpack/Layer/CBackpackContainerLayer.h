//
//  CBackpackContainerLayer.h
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#ifndef ___1_cube__CBackpackContainerLayer__
#define ___1_cube__CBackpackContainerLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CBattleArrayLayer.h"
#include "CBackpackPageLayer.h"
#include "LayoutLayer.h"

#include <vector>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class CBackpackContainerLayer : public CCLayer
{
public:
    static CBackpackContainerLayer* create(int inOpenNumber = 9);
    
public:
    CBackpackContainerLayer();
    virtual ~CBackpackContainerLayer();
public:
    virtual bool init(int inOpenNumber);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initCBackpackContainerLayer(int inOpenNumber);
    void handlerTouch();
    
    void adjustScrollView(float offset);
    void scrollToPage(int inPage);
    void updateIndicator(const int &srcIndex, const int &desrIndex);
    
    void setStateTab(int inTag, int inState);
    
    // update Data
    void updateCurrentPage();
    
    // update UI
    void updateIndicators(const int  & inCount);
    void updatePageContent(const int &inCount);
    void updateUI(const int &inCount);

    // action:
    
    void goAllTab();
    void goPropTab();
    void goMaterialTab();
    void goTaskTab();
    void closeBackPack();
    // about resource load & release:
    void loadResource();
    void releaseResource();
    
protected:
    bool m_bScrollEnable;
    bool m_bPageTouchEnable;
    bool m_bMoveEnable;
    
    int m_nTouchTag;
    int m_nCurPage;
    int m_nOpenGridCount;
    int m_nPageCount;
    int m_nMaxPageCount;
    int m_nCurrentTab;
    
    CCSize  m_cContainerSize;
    CCPoint m_cContainerOffset;
    CCPoint m_cTouchPoint;
    
    
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    CCScrollView *m_pContainer;
    CCArray *m_pItems;
    
    // currentPage:
    CBackpackPageLayer *m_pCurrentPage;
    
    //about indicate :
    CCRect m_cSelectedIndicateRect;
    CCRect m_cUnSelectedIndicateRect;
    CCArray *m_pIndicators;
    
    // about tab:
    CCRect m_cSelectedTabRect;
    CCRect m_cNoramlTabRect;
    CCRect m_cPressedTabRect;
    CCArray *m_pTabs;
    
    //test:
    void load();
    void callback(float dt);
    
};


#endif /* defined(___1_cube__CBackpackContainerLayer__) */
