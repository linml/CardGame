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
public:
    map<int, CCard *>m_hashmapAllCard;
    vector<CCard *>m_hashmapFight;
    vector<CCard *>m_hashmapMonster;
};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
