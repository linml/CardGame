//
//  Biforest.h
//  91.cube
//
//  Created by phileas on 13-6-13.
//
//

#ifndef ___1_cube__Biforest__
#define ___1_cube__Biforest__

#include <vector>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LayoutLayer.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
#include "gameStruct.h"

#include "CPtExpandableListView.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;


#define TYPE_CHAPTER 1
#define TYPE_SECTION 2

class CPtBookItem : public CPtTableItem
{
public:
    static CPtBookItem*create(const char *inName, int inType);
public:
    void setSelectItem();
    void setNormalItem();
protected:
    int m_nType;
    CCSprite *m_pBg;
    CCLabelTTF *m_pName;
};


class CBiforestLayer : public CCLayerColor , public CCTableViewDelegate
{
    
    
public:
    CREATE_FUNC(CBiforestLayer);
    static  EVENTDATA dispatchEventWithType(CCDictionary *inDict);
public:
    CBiforestLayer();
    virtual ~CBiforestLayer();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    

protected:
    void initBiforest();
    void handlerTouch();
    
    void initRightPanel(int inMaxCurrentIndex);
    void updateRightPanelUI();
    void updatePanel(int inChapterId, int inSectionId);
    
    void getChapters(int inMaxChapterId);
    void getSections(int inSelectedChapterIndex, int inMaxSectionId = -1);
    
    
    CCArray *getChapterItem();
    CCArray *getSectionItem();
    CCNode *createItemView(const char* const inTitle);
        
    // test connect server:
    bool canGoSection();
    void onClickGoSection();
    void onReceiveGoSectionMsg(CCObject *pObject);
    void onParseGoSectionMsgByDictionary(CCDictionary * inDataDictionary);
    


    
    // load image resource:
    void loadResource();
    
    // test with expandableListView:
    CPtExpandableListView *m_pListView;
    void initPanel();
    
    CCNode* createItemViewByType(const char* inTitle, ITEM_TYPE inType = PARENT_TYPE);
    void setNormal(CCObject *pObject);
    void setSelected(CCObject *pObject);
    void getChildren(CCObject* pObject);
   
    
    
protected:
    
    int m_nMaxChapterId;
    int m_nMaxSectionId;
    
    int m_nCurrentChapterId;
    int m_nCurrentSectionId;
    
    int m_nCurrentSectionIndex;
    int m_nCurrentChaptetIndex;
    
    int m_nTouchTag;
    bool m_bSectionTouchEnable;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
    CCSprite * m_pTouchSprite;
    CCArray *m_pChapters;
    CCArray *m_pSections;
    
    CPtListViewWidget *m_pLeftPanel;
    
    // back button:
    CCSprite *m_pBackBtn;
    
    // right panel conent:
    CCSprite * m_pBg;
    CCLabelTTF *m_pTips;
    CCSprite * m_pGoBtn;
    
    // data:
    std::string m_sPicName;
    std::string m_sTips;
    bool m_bGoneVisiable;
    
    //mode:
    bool m_bChapterMode;
    
    CCSprite * selectNode ;
    

    
    

};

#endif /* defined(___1_cube__Biforest__) */
