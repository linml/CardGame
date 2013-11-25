//
//  CPtBattleArray.h
//  91.cube
//
//  Created by phileas on 13-7-14.
//
//

#ifndef ___1_cube__CPtBattleArray__
#define ___1_cube__CPtBattleArray__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LayoutLayer.h"
#include "CCard.h"
#include "CGamesCard.h"
#include "gamePlayer.h"

#include <map>
using namespace cocos2d;
using namespace cocos2d::extension;


#define CARDCOUNT  5
#define BATTERARRAYCOUNT 3
enum BattleArrayType 
{
	kAttackFirst = 0,
	kAttackScecond ,
	kDefense 
};

enum CardSuitKind 
{
	kTongHua = 1
};


// define class of CPtDisPlayCard
class CPtDisPlayCard : public CGamesCard
{
public:
    static CPtDisPlayCard  *Create(CFightCard *card);
public:
    CPtDisPlayCard(){m_nIndex = -1,m_pMagnifier = NULL;};
    CC_SYNTHESIZE(int , m_nIndex, Index);
    CC_SYNTHESIZE(CPtDisPlayCard*, m_pCardPBagPointer, InCardBagPointer);
    CPtDisPlayCard *getCopy();
    void displayManifier();
    void hideManifier();
    bool isClickManifier(CCTouch *pTouch);
    void setManifierNormal();
    void setManifierPress();
    void createManifier();
    bool isDisplay();
protected:
    CCSprite * m_pMagnifier;
};

// define class of CPtBattleArray

class CPtBattleArrayPanel;
class CPtBattleArray : public CCLayer
{

public: 
	static CPtBattleArray* create(vector<CFightCard *> &fightArray, const cocos2d::CCSize &size, const cocos2d::CCPoint &point,int Tag = 1 );
public:
	CPtBattleArray();
	virtual	~CPtBattleArray();

public:
    void initSize(const CCSize& size, const CCPoint & point);
    void initBattleArrayFromServer(vector<CFightCard *> &fightArray);
	bool addCard(CPtDisPlayCard *inCard, const int & inCardType, const bool &inAppendEnable = false);
	bool removeCard(const int& inCardType);
    bool replaceCard(CPtDisPlayCard *inCard, const int & inCardType);
    bool isAssistantCard(CPtDisPlayCard *inCard , bool inRemove = false, bool inSamePanel = false);
    bool isAssistantCard(CCTouch *pTouch);
    int getReplaceCard(CPtDisPlayCard *inCard, bool inSamePanel = false);
    int getReplaceCard(CPtDisPlayCard *inCard, const int &inSelfIndex , bool inSamePanel = false);
	int getFightPower();
	int getLeaderShip();
	int getGrowthValue();
	CardSuitKind getCardSuitKind();
    void insertMoveCard(CCNode *inCard);
	CCArray *getCardArray();
    void save();
    void saveOnClick();
    int  resortFightCard();
    int  getInsertIndex();
    void updateBattleArray();
    void callBack(CCObject *pSender);
    void resetBattleArray();
    void getSuitArray(int array[], int &len);
    CPtDisPlayCard** getBattleArray(){return m_pCardArray;};
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void)
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), false);
    };
protected:
    void initMap();
	void initData();
    void updateData();
	void save(vector<CFightCard *> & infightArray);
    void updateCardSuitKind();
    void updateLabel();
    bool hasInterveneCard(){ return m_pCardArray[4] == NULL ? false: true;};
    void createSuitLogo(const int& inSuit, const int &inSequence, const int &inPositionIndex);
    bool isOverRVC();
    bool hasMainAttacker(){return m_pCardArray[0]==NULL? false: true;};
    bool isAssistantCard();
    void swapCard(const int & inSrc, const int &inDes);
    
    void removeAction(CCNode *pNode);
    void deletNode();
    
protected:
    
    
	CPtDisPlayCard* m_pCardArray[CARDCOUNT];
    int m_nCardCount; // 主将卡牌的个数
	int m_nGrowthValue;
	BattleArrayType m_cType;
	CardSuitKind   m_SuitKind;
    cocos2d::CCPoint m_cPositions[CARDCOUNT];
    LayoutLayer *m_cMaps;
    CCSprite * m_pSuitLogo[CARDCOUNT];
    CCSprite * m_pSaveBtn;
    int m_aSuitArray[CARDCOUNT];
    int m_aSequenceArray[CARDCOUNT];
    
    CPtDisPlayCard * selectNode;
    int selectIndex;
    bool selectAssistant;
    
    bool m_bActionEnable;
//test:
public:
    int inTag ;
    bool m_bTouchEnable;
    bool m_bOnClick;
    
// getter:
    
    CC_SYNTHESIZE_READONLY(int, m_nLeadership, Rvc);
    CC_SYNTHESIZE_READONLY(int, m_nFightPower, Atk);
    CC_SYNTHESIZE_READONLY(int, m_nDefense, Def);
    CC_SYNTHESIZE_READONLY(int, m_nHP, Hp);
    CC_SYNTHESIZE_READONLY(int, m_nSuitAtk, SuitAtk);
    CC_SYNTHESIZE_READONLY(int, m_nSuitDef, SuitDef);
    CC_SYNTHESIZE(CPtBattleArrayPanel* , m_pPanelCntainer, PanelContainer)
    CC_SYNTHESIZE(CCLayer*, m_pMoveLayer, MoveLayer);
    
    
};

class  CPtBattleArrayPanel : public CCScrollView
{

public:
    CREATE_FUNC(CPtBattleArrayPanel);
    static CPtBattleArrayPanel* create(CCSize size, CCNode* container = NULL, CCLayer* moveLayer = NULL);
public:
    CPtBattleArrayPanel();
    virtual ~CPtBattleArrayPanel();
    virtual bool init();
    
    CPtBattleArray *getBattleArray(CCTouch *pTouch);
    CPtBattleArray *getCurrentArray(){return  m_pCurrentBatterArray;};
    void resetBattleArrays();
public:
    CGamePlayer * getGamePlayer(){return  gamePlayer;};
    
public:
    void registerWithTouchDispatcher()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    }

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
	void initData();
	void updatePanel();
    void adjustScrollView(const CCPoint& begin, const CCPoint &end);
    void scrollToPage(int nPage );
   

protected:
	int m_nCurrentPage;
	int m_nPlayerFightPower;
	int m_nPlayerLeadership;

    float m_fAdjustSpeed;
    CCPoint m_cBeginOffset;
    CCSize m_CellSize;
    
	CPtBattleArray *m_pBatterArrays[BATTERARRAYCOUNT];
	CPtBattleArray * m_pCurrentBatterArray;
    
    
    CGamePlayer *gamePlayer;
    
    CC_SYNTHESIZE_READONLY(bool, m_bMove, PanelMove);
    CC_SYNTHESIZE(CCLayer *, m_pMoveLayer, MoveLayer);
};

#endif
