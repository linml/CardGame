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
    
   
    void initLeftPanel(int inMaxChapterId);
    void initRightPanel(int inMaxChapterId);
    
    void updatePanel(int inCurrentId, bool inChapterEnable);
    
    void updateRightPanel(int inCurrentId, bool inChapterEnable);
    void updateRightPanelUI();
    
    void updateLeftPanel(int inCurrentId, bool inChapterEnable);
    void updateLeftPanelUI();
    
    
    void getChapters(int inMaxChapterId);
    void getSections(int inSelectedChapterId, int inMaxSectionId = -1);
    
    
    void backChapter();
    CCArray *getChapterItem();
    CCArray *getSectionItem();
    CCNode *createItemView(const char* const inTitle);
    
    void setSelect(CCNode *node);
    
    
    // test connect server:
    bool canGoSection();
    void onClickGoSection();
    void onReceiveGoSectionMsg(CCObject *pObject);
    void onParseGoSectionMsgByDictionary(CCDictionary * inDataDictionary);
    

    
    // load image resource:
    void loadResource();
    
protected:
    
    int m_nMaxChapterId;
    int m_nMaxSectionId;
    
    int m_nCurrentChapterId;
    int m_nCurrentSectionId;
    
    int m_nSelectIndex;
    
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
