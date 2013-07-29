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
    
    CC_SYNTHESIZE(CPtDisPlayCard*, m_pCardPBagPointer, InCardBagPointer);
    CPtDisPlayCard *getCopy();
};

// define class of CPtBattleArray

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
	bool addCard(CCNode *inCard, const int & inCardType, const bool &inAppendEnable = false);
	bool removeCard(const int& inCardType);
    bool replaceCard(CCNode *inCard, const int & inCardType);
    bool isAssistantCard(CCNode *inCard , bool inRemove = false);
    bool isAssistantCard(CCTouch *pTouch);
    int getReplaceCard(CCNode *inCard);
	int getFightPower();
	int getLeaderShip();
	int getGrowthValue();
	CardSuitKind getCardSuitKind();
    void insertMoveCard(CCNode *inCard);
	CCArray *getCardArray();
    void save();
    int  resortFightCard();
    int  getInsertIndex();
    void updateBattleArray();
    
    void callBack(CCObject *pSender);
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
protected:
    
    
	CCNode* m_pCardArray[CARDCOUNT];
    int m_nCardCount;
	int m_nGrowthValue;
	BattleArrayType m_cType;
	CardSuitKind   m_SuitKind;
    cocos2d::CCPoint m_cPositions[CARDCOUNT];
    LayoutLayer *m_cMaps;
    CCSprite * m_pSuitLogo[CARDCOUNT];
    CCSprite * m_pSaveBtn;
    int m_aSuitArray[CARDCOUNT];
    int m_aSequenceArray[CARDCOUNT];
    
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
    
    
};

class  CPtBattleArrayPanel : public CCScrollView
{

public:
    CREATE_FUNC(CPtBattleArrayPanel);
    static CPtBattleArrayPanel* create(CCSize size, CCNode* container = NULL);
public:
    CPtBattleArrayPanel();
    virtual ~CPtBattleArrayPanel();
    virtual bool init();
    
    CPtBattleArray *getBattleArray(CCTouch *pTouch);
    CPtBattleArray *getCurrentArray(){return  m_pCurrentBatterArray;};
public:

    
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
};

#endif
