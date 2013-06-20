//
//  CraftsmanshipMul.h
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#ifndef __en_cube__SkillManager__
#define __en_cube__SkillManager__
#include <iostream>
#include <vector>
#include "Craftsmanship.h"
#include "CSingleton.h"
using namespace std;


//技能管理类
class CSkillManager :public cocos2d::CCObject {
public:
    CSkillManager();
    ~CSkillManager();
    CCraftsmanship *getSkillByIndex(int index);
    void dealWithSkillShanghai(int index,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianhp,cocos2d::CCLayer *layer);
    
    void animationDealWithSkillShanghai(vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianhp,cocos2d::CCLayer *layer);
    
    int puTongGongJi(SFightCardSprite **ownCardProperty,SFightCardSprite **enemyCardpropert);
    void removeSprite(cocos2d::CCNode *node,void *tag);
    
    void appendAnimationListBuffer(int ownIndex,int Huihe,SFightCardSprite *ownCardProperty,SFightCardSprite *enemyCardpropert);
    void zhujiangjiaxue(int uHp,vector<SFightCardSprite *>vecFight);
    /*
     
    */
    int checkShanghai(int OldCurrHp,int NewCurrHp);
    void dealWithSkillShanghaiList(int index,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe);
    void shangHaiZhiLogic(int index,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe);
    void putongGongji(int index,SFightCardSprite **ownSprite,SFightCardSprite **enemyCardpropert,int ownIndex,int enemyIndex,int Huihe);
    void monsterDead(int  huihe);
    void fightDead(int  huihe);
    
    void clearAnimationList();
    vector<SAnimationFps *> getAnimationVector();
    
private:
    vector<SAnimationFps *>m_Animationlist;
public:
    vector<CCraftsmanship* >m_skillList;
private:
    cocos2d::CCLayer *layer;

};
typedef Singleton<CSkillManager> G_SkillManager;


#endif /* defined(__en_cube__GongjiPutongJineng__) */
