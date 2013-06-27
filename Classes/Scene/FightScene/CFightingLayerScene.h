//
//  CFightingLayerScene.h
//  91.cube
//
//  Created by linminglu on 13-6-18.
//
//

#ifndef ___1_cube__CFightingLayerScene__
#define ___1_cube__CFightingLayerScene__
#include "cocos2d.h"
#include <iostream>
#include <vector>
class CCardSprite;
class SFightCardSprite;
using namespace  cocos2d;
using namespace  std;

class CFightingLayerScene :public CCLayer{
public:
    CFightingLayerScene();
    ~CFightingLayerScene();
    CREATE_FUNC(CFightingLayerScene);
    void  updateGetGameDataToGetServerRandow();
public:
    static CCScene *scene();
    bool init();
private:
    void initText();
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
    void showJuqi(CCNode*node);
    void hideJuqi(CCNode *node);
    void showFightAnimation(CCNode *pSend,void *tag);
    void removeSprite(CCNode *node,void *tag);
    void runSchedule();
public:
    void animationSwf(int skillIndex,vector<SFightCardSprite *>ownFightCard,vector<SFightCardSprite *>MonstFightCard,int oWnindex,int  MonsteIndex,class SAnimationFps * spfs);
    void basicAnimation(vector<SFightCardSprite *>ownFightCard,vector<SFightCardSprite *>MonstFightCard,int  oWnindex,int  MonsteIndex,class SAnimationFps * spfs);
    
public:
    void animationMoveToLast(class CCObject  *pSend);
    void dealWithFight(CCObject *object);
    void startAnimationSwf(float  data);
    void loseDialog();
    void winDialog();
    void fightLogic(int huihe);
public:
    void animationCardPanel(class CCardPanel *card,void *tag);
    void animationShouShang(class CCardPanel *card,void *tag);
    void animationMoveBack(class CCardPanel *card);
    void setVistablHit();
    void animationHpFight(class CCNode *card,void *tag);
    void animationHpMonster(class CCNode *card,void *tag);
    void setHideHpAnimation();
    void nextAnimation(CCNode *node);
    void updateSetHp();

private:
    void callBackOwnAnimationChangeFlag(CCObject *pSend);
    void callBackMonsterAnimationChangeFlag(CCObject *pSend);
    void checkOwnIsDeadAndMove();
    void checkMonsterIsDeadAndMove();
    void secheudelUpdateToDoDealWith(float time);
private:
    vector<SFightCardSprite *>m_vfightCardSprite;
    vector<SFightCardSprite *>m_vMonsterCardSprite;
    class   CCardLayer  *currOwnCardSprite;
    class   CCardLayer  *monsterCardSprite;
    int     m_iZhujiangHuihe;
    int     m_iMonsterZhujiangHuihe;
    int     m_vFightCardIndex;
    int     m_vMonsterCardIndex;
    int     gongjiHuiHe;
    int     jiaHp;
    int     JianHp;
    bool    isOwnActionEnd;
    bool    isMonsterEnd;
    class   SAnimationFps *m_sfps;
    
    
private:
    bool initBufferEnd;
    int  currentSwfIndex;
    bool isDoingAnimation;

};
#endif /* defined(___1_cube__CFightingLayerScene__) */
