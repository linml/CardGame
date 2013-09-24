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


#define  EVENTREWARD_ADD  1
#define  CHAPTERREWARD_ADD 2
#define  TASKREWARD_ADD 3
#define  SECTIONREWARD_ADD 4
#define  FORWARDREWARD_ADD 5
    
#define  EVENTREWARD_DEC  11
#define  CHAPTERREWARD_DEC 12
#define  TASKREWARD_DEC 13
#define  SECTIONREWARD_DEC 14
#define  FORWARDREWARD_DEC 15
    
enum OPEARTORTYPE
{
    ADD = 0,
    DEC = 1
};

class CReward : public CCObject
{
public:
    static CReward* create(CCDictionary * inReward);
    static CReward* create(CReward * inReward1, CReward *inReward2);
public:
    CReward();
    virtual ~CReward();
    virtual bool initWithDictionary(CCDictionary * inReward);
    bool excuteReward(OPEARTORTYPE inType);
    
    bool merger(CReward * inReward1, CReward *inReward2);
    int getCoin();
    int getHP();
    int getEnergy();
    int getExp();
    int getCash();
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
    CCDictionary *m_pCards; // key-> card_item_id, value-> card_id+suit
    CCDictionary *m_pProps;
    
    CGamePlayer *m_pPlayer;
// test:
public:

    void printContent();
    void getRewardContent(char * outString, const int leng);
};

#endif /* defined(___1_cube__CReward__) */
