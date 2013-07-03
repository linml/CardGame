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
#include "CSingleton.h"
#include "CCard.h"
#include "AnimationLogic.h"
using namespace std;


//技能管理类
class CSkillManager :public cocos2d::CCObject {
public:
    CSkillManager();
    ~CSkillManager();
    
    int checkShanghai(int OldCurrHp,int NewCurrHp);
    void refactorJisuan(int index,vector<CFightCard *>ownCardProperty,vector<CFightCard *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe);
    void basicGongji(int index,CFightCard **ownCard,CFightCard **monsterCard,int ownIndex,int monstexIndex,int Huihe);
    int refactorpuTongGongJi(CFightCard **ownCardProperty,CFightCard **enemyCardpropert);
   
    
    void monsterDead(int  huihe);
    void fightDead(int  huihe);
    
    void clearAnimationList();
    vector<SAnimationFps *> getAnimationVector();
    
private:
    vector<SAnimationFps *>m_Animationlist;

private:
    cocos2d::CCLayer *layer;

};
typedef Singleton<CSkillManager> G_SkillManager;


#endif /* defined(__en_cube__GongjiPutongJineng__) */
