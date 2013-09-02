//
//  CBackpackContainerLayer.h
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#ifndef ___1_cube__CBackpackContainerLayer__
#define ___1_cube__CBackpackContainerLayer__

#include <multimap.h>

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
    
    void reLoadPage(int inFromPage);
    void appendPage();
public:
    virtual bool init(int inOpenNumber);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initCBackpackContainerLayer(int inOpenNumber);
    
    // init grid data struct:
    void initGrids();
    
    void handlerTouch();
    
    void adjustScrollView(float offset);
    void scrollToPage(int inPage);
    void updateIndicator(const int &srcIndex, const int &desrIndex);
    
    void setStateTab(int inTag, int inState);
    
    // update Data
    void updateCurrentPage();
    
    // update UI
    void updateIndicators(const int  & inCount);
    void updatePageContent(const int &inCount, bool inFirstPage = true);
    void updateUI(const int &inCount ,bool inFirstPage = true);
    void updateTabContent(vector<multimap<int, int>::iterator> & inVector, int inFromPage = 0,bool inFirstPage = true);
    void updateToFirstPage();
    // action:
    
    void goAllTab();
    void goPropTab();
    void goMaterialTab();
    void goTaskTab();
    void closeBackPack();
    // about resource load & release:
    void loadResource();
    void releaseResource();
    
    
    vector<multimap<int, int>::iterator> getFilterPointer(int inType);
   
    
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
    
    CCSprite *m_pCloseButton;
    
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
    
    // grid data struct:
    std::multimap<int, int> m_cNumInGrid;
    
    
    
};


#endif /* defined(___1_cube__CBackpackContainerLayer__) */
