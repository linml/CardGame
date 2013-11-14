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
#include "CActivityEncounterManager.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;


#define TYPE_CHAPTER 1
#define TYPE_SECTION 2
enum EXPLORATIONTYPE
{
    NORMALEXPLORATION = 1,
    ACTIVITYEXPLORATION
};

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
    static CBiforestLayer *create(EXPLORATIONTYPE inType = NORMALEXPLORATION, int inSelectSectionId = -1);
    static  EVENTDATA dispatchEventWithType(CCDictionary *inDict);
public:
    CBiforestLayer(EXPLORATIONTYPE inType = NORMALEXPLORATION);
    virtual ~CBiforestLayer();
    void setSelectSectionId(int inSectionId){m_nCurrentSectionId = inSectionId;};
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
    
    void initPanel();
    void initActivityPanel();
    void initNormalPanel();

    void initBiforest();
    CCArray* getDispLayerData();
    
    void handlerTouch();
    void initRightPanel(int inCurrentChapterIndex);
    void updateRightPanelUI();
    void updatePanel(int inChapterId, int inSectionId);
    
        
    // test connect server:
    bool canGoSection();
    void onClickGoSection();
    void onReceiveGoSectionMsg(CCObject *pObject);
    void onParseGoSectionMsgByDictionary(CCDictionary * inDataDictionary);



    
    // load image resource:
    void loadResource();
    
    CCNode *createItemView(const char* const inTitle);
    CCNode* createItemViewByType(const char* inTitle, ITEM_TYPE inType = PARENT_TYPE);
    
    void setNormal(CCObject *pObject);
    void setSelected(CCObject *pObject);
    void getChildren(CCObject* pObject);
   
    void switchExplorationType(EXPLORATIONTYPE inType);
    
    CCArray *getChaptersByType(EXPLORATIONTYPE inType, int inMaxChapterId=0);
    CCArray *getNormalChapters(int inMaxChapterId);
    CCArray *getActivityChapters();

    CCArray *getSections(int inSelectedChapterIndex, int inMaxSectionId=-1);
    CCArray *getNormalSections(int inSelectedChapterIndex, int inMaxSectionId = -1);
    CCArray *getActivitySections();
    
    CCArray *getChapterItem();
    CCArray *getSectionItem(CCArray *inSectionInfo);
    
    CPtChapter *getChapterByIndex(int inIndex);
    
    
    void goToAcitivityEncounter(float dt);
    // getActivityInfo from sever:
    void onSendRequestGetActivity();
    void onReceiveMsgGetActivity(CCObject* pObject);
    
protected:
    
    EXPLORATIONTYPE m_eExploartionType;
    
    int m_nMaxChapterId; //最大章
    int m_nMaxSectionId; // 最大节
    
    int m_nCurrentChapterId; //当前章的ID
    int m_nCurrentSectionId; //当前节的ID
    
    int m_nCurrentSectionIndex; // 当前节的Index
    int m_nCurrentChaptetIndex; // 当前章的Index
    
    int m_nTouchTag;
    bool m_bSectionTouchEnable;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
    CCSprite * m_pTouchSprite;
    
    CCArray *m_pNormalChapters; // normal chapters;
    CCArray *m_pActivityChapters;// activityChapter;
    CCArray *m_pNormalSections;
    CCArray *m_pActivitySections;
    
    CPtExpandableListView *m_pListView;
    
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
    bool m_bSendRequest;
    bool m_bStartActivity;
    
    CCSprite * m_pSwtichBtn;
    
    CActivityEncounterManager *m_pActivityEncounterManager;
    CCArray *m_pActivityEncounterInfo;

    
    

};

#endif /* defined(___1_cube__Biforest__) */
