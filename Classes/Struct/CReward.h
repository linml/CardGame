//
//  CReward.h
//  91.cube
//
//  Created by phileas on 13-9-22.
//
//

#ifndef ___1_cube__CReward__
#define ___1_cube__CReward__

#include "cocos2d.h"
#include "gamePlayer.h"

#define MAX_DROP_COUNT 8
using namespace cocos2d;


#define  EVENTREWARD_ADD    1
#define  CHAPTERREWARD_ADD  2
#define  TASKREWARD_ADD     3
#define  SECTIONREWARD_ADD  4
#define  FORWARDREWARD_ADD  5
#define  UNLOCKREWARD_ADD   6
#define  CARDITEMREWARD_ADD 7
#define  ITEMREWARD_ADD     8
#define  INBOXREWARD_ADD    9
#define  RECHARGEREWARD_ADD 10
#define  SHOPREWARD_ADD     11
    
#define  EVENTREWARD_DEC    21
#define  CHAPTERREWARD_DEC  22
#define  TASKREWARD_DEC     23
#define  SECTIONREWARD_DEC  24
#define  FORWARDREWARD_DEC  25
#define  UNLOCKREWARD_DEC   26
#define  CARDITEMREWARD_DEC 27
#define  ITEMREWARD_DEC     28
#define  INBOXREWARD_DEC    29
#define  RECHARGEREWARD_DEC 30
#define  SHOPREWARD_DEC     31


    
enum OPEARTORTYPE
{
    ADD = 0,
    DEC = 1
};

#define REWARD_RESULTE_MASK     0x0001
#define REWARD_LEVEL_UP_MASK    0x0002
#define REWARD_FAIL 0x0000

#define REWARD_IS_RESULTE_SUCCESS(flag) (((flag)&REWARD_RESULTE_MASK) != REWARD_FAIL)
#define REWARD_IS_LEVEL_UP_SUCCESS(flag) (((flag)&REWARD_LEVEL_UP_MASK) != REWARD_FAIL)

#define REWARD_SET_RESULTE_SUCCESS(flag)  (flag|=REWARD_RESULTE_MASK)
#define REWARD_SET_LEVEL_UP_SUCCESS(flag) (flag|=REWARD_LEVEL_UP_MASK)


class CReward : public CCObject
{
public:
    // tool methods:
    static CCDictionary *getAllRewardByDict(CCDictionary *inRewardDict);
    
    static CReward* create(CCDictionary * inReward);
    static CReward* create(CReward * inReward1, CReward *inReward2);
    
public:
    CReward();
    virtual ~CReward();
    virtual bool initWithDictionary(CCDictionary * inReward);
    int excuteReward(OPEARTORTYPE inType);
    
    bool merger(CReward * inReward1, CReward *inReward2);
    
    int getExtarExp();
    int getExtarCoin();
    int getCoin();
    int getHP();
    int getEnergy();
    int getExp();
    int getCash();
    int getFriendly();
    int getKaHun();
    int getCardCount();
    int getPropCount();
 
   
    CCDictionary * getCards();
    CCDictionary * getProps();
    
protected:
    void initData();
    void copyWithCReward(CReward * inReward);
    void addCReward(CReward *inAddReward);
    void parseDict(CCDictionary * inReward);
    void addCards();
    void subCards();
    void addProps();
    void subProps();
protected:
    bool m_bExecute;
    int m_nCoin;
    int m_nHp;
    int m_nEnergy; //神力
    int m_nExp;
    int m_nCash;
    int m_nFriendly;
    
    int m_nExtarExp; // 道具buff
    int m_nExtarCoin; // 道具buff
    
    int m_nKaHun;
    CCDictionary *m_pCards; // key-> card_item_id, value-> card_id+suit
    CCDictionary *m_pProps;
    
    CGamePlayer *m_pPlayer;
// test:
public:

    void printContent();
    void getRewardContent(char * outString, const int leng);
};

#endif /* defined(___1_cube__CReward__) */
