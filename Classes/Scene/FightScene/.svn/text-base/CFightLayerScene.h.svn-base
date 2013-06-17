//
//  CFightLayer.h
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#ifndef ___1_cube__CFightLayerScene__
#define ___1_cube__CFightLayerScene__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

struct SATKLIST {
    int gongjiZheindex;
    int gongjiShanghai;
    int gongjiJiNeng;
    int gongjiZijijiaxue;
    int gongji;
};
class CCardSprite;
class SFightCardSprite;

class CFightLayerScene :public CCLayer{
public:
    CFightLayerScene();
    ~CFightLayerScene();
    CREATE_FUNC(CFightLayerScene);
    void  updateGetGameDataToGetServerRandow();
public:
    static CCScene *scene();
    bool init();
private:
    void createOwnFightCardPosition();
    void setHp(CCardSprite *pCardSprite,CCLabelTTF *labelttf);
    void createMonster();
    void updateHp(CCardSprite *pCardSprite,CCardSprite *pMonsterCardSprite);
    void setText(const char *data);
    int  getWinStats();
    void initAssignHuihe();
    void initOwnHuihe();
    void initMonsterHuihe();

public:
        void animationMoveToLast(class CCObject  *pSend);
    void dealWithFight(CCObject *object);
    void loseDialog();
    void winDialog();
    void fightLogic(int huihe);
public:
    
    void animationCardPanel(class CCardPanel *card,void *tag);
    void animationShouShang(class CCardPanel *card,void *tag);
    void animationMoveBack(class CCardPanel *card);
    void setVistablHit();
private:
    void callBackOwnAnimationChangeFlag(CCObject *pSend);
    void callBackMonsterAnimationChangeFlag(CCObject *pSend);
    void checkOwnIsDeadAndMove();
    void checkMonsterIsDeadAndMove();
    void secheudelUpdateToDoDealWith(float time);
    
private:
    vector<SFightCardSprite *>m_vfightCardSprite;
    vector<SFightCardSprite *>m_vMonsterCardSprite;
    class CCardLayer  *currOwnCardSprite;
    class CCardLayer  *monsterCardSprite;
    int  m_iZhujiangHuihe;
    int  m_iMonsterZhujiangHuihe;
    int  m_vFightCardIndex;
    int  m_vMonsterCardIndex;
    int  gongjiHuiHe;
    int jiaHp;
    int JianHp;
    bool  isOwnActionEnd;
    bool  isMonsterEnd;
};

#endif /* defined(___1_cube__CFightLayer__) */
