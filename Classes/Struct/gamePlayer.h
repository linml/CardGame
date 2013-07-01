//
//  gamePlayer.h
//  91.cube
//
//  Created by xianbei1987 on 13-5-24.
//
//

#ifndef _1_cube_gamePlayer_h
#define _1_cube_gamePlayer_h
#include "gameStruct.h"
#include "CSingleton.h"
#include "CardSprite.h"
#include "CCard.h"
#include <vector>
using namespace std;
struct SLevelPlayer {
    int m_iLevel;
    int m_iCard_max;
    int m_iLeadership;
    int m_iFriend_max;
    int m_iHP;
    int m_iMp;
    int m_iexp;
};
class CGamePlayer {
public:
    int m_nFaceTo;
    int m_nTurnTo;
    STC_HOUSEPOS m_stcCurrenPos;
    vector<CCardSprite *>m_vYongyouCardSprite;
    vector<CCardSprite *>m_vZhangdouCardSprite;
    void forTest();
    void forTestCard();
    void forTestMonsterCard();
    //
    void forTestMonster();
    void forTestDeleteMonster();
    vector<CCardSprite *>m_vMonsterCardSprite;
public:
    CGamePlayer();
    ~CGamePlayer();
    void clearAllCard();
    void initAllCard(const char *cardFileName);
//根据loading的界面配置了 游戏的  资料
public:
    map<int, CCard *>m_hashmapAllCard;
    vector<CCard *>m_hashmapFight;
    vector<CCard *>m_hashmapMonster;
    
private:
    int m_iCurrentExp;
    int m_iCurrentHp;
    int m_iCurrentMp;
   struct SLevelPlayer *m_sLevelPlayer;
public:
    bool isCanUpdateLevel(int nExp);
    bool UpdateLevel();
    int getTotoalHp();
    int getCurrentHp();
    int getTotoalMp();
    int getCurrentMp();
    int getTotalExp();
    int getCurrentExp();
    void setCurrentHp(int nHp);
    void setCurrentMp(int nMp);
    void setCurrentExp(int nExp);

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
