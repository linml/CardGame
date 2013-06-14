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
    void dealWithSkillShanghai(int index,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianhp);
    int puTongGongJi(SFightCardSprite **ownCardProperty,SFightCardSprite **enemyCardpropert);
    /*
     
     */
 
public:
    vector<CCraftsmanship* >m_skillList;

};
typedef Singleton<CSkillManager> G_SkillManager;


#endif /* defined(__en_cube__GongjiPutongJineng__) */
