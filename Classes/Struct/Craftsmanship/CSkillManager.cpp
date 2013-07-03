//
//  GongjiPutongJineng.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "CSkillManager.h"
#include "gameConfig.h"
#include "cocos2d.h"
using namespace cocos2d;
/*
 class CCraftsmanship gstr_sskillList[]=
 {
 {EN_SKILL_BASICSKILL,1,"女神之枪"},
 {EN_SKILL_BLESSINGSKILL,2,"女神之拥"},
 {EN_SKILL_ADVOCACYSKILL,3,"英灵颂歌"},
 {EN_SKILL_BASICSKILL,4,"寒霜号角"},
 {EN_SKILL_BLESSINGSKILL,5,"水晶龙甲"},
 {EN_SKILL_ADVOCACYSKILL,6,"岩石技能"},
 {EN_SKILL_BASICSKILL,7,"藤编"},
 {EN_SKILL_BLESSINGSKILL,8,"冰雪森林"},
 {EN_SKILL_ADVOCACYSKILL,9,"回复术"},
 {EN_SKILL_BASICSKILL,10,"铁锤挥击"},
 {EN_SKILL_BLESSINGSKILL,11,"大地守护"},
 {EN_SKILL_ADVOCACYSKILL,12,"精锻"},
 {EN_SKILL_BASICSKILL,13,"霜爪击"},
 {EN_SKILL_BLESSINGSKILL,14,"狼魂"},
 {EN_SKILL_ADVOCACYSKILL,15,"撕咬"},
 };
 */

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}
inline  int  RangeLand(int min=1,int max=100)
{
    return  rand() % (max-min+1) + min;
}




CSkillManager::CSkillManager()
{
    
}

CSkillManager::~CSkillManager()
{
   // DELETE_POINT_VECTOR(m_skillList,vector<CCraftsmanship* >);
}

void CSkillManager::monsterDead(int  huihe)
{
   m_Animationlist.push_back(new SAnimationFps(0,999,0,0,0,0,huihe,0,0));
    cout<<"huihe:"<<huihe<<" "<<0<<" "<<0<<endl;
}
void CSkillManager::fightDead(int  huihe)
{
     m_Animationlist.push_back(new SAnimationFps(0,1000,0,0,0,0,huihe,0,0));
     cout<<"huihe:"<<huihe<<" "<<0<<" "<<0<<endl;
}
int CSkillManager::refactorpuTongGongJi(CFightCard **ownCardProperty,CFightCard **enemyCardpropert)
{
    int oldHp=(*ownCardProperty)->m_iCurrHp;
    //普通攻击    A攻击者攻击- B敌人的防御  =伤害  需要加上  防护的伤害
    (*enemyCardpropert)->m_iCurrHp -=(* ownCardProperty)->m_attack-(*enemyCardpropert)->m_defend;
    if((*enemyCardpropert)->m_iCurrHp<0)
    {
        (*enemyCardpropert)->m_iCurrHp=0;
    }
    int retHp=oldHp-(*enemyCardpropert)->m_iCurrHp;
    return retHp>=0?retHp:0;
}
int CSkillManager::checkShanghai(int OldCurrHp,int NewCurrHp)
{
    if(NewCurrHp==OldCurrHp)
    {
        return 0;
    }
    else
    {
        return OldCurrHp-NewCurrHp;
    }
}
void CSkillManager::basicGongji(int index,CFightCard **ownCard,CFightCard **monsterCard,int ownIndex,int monstexIndex,int Huihe)
{
    int OwncurrHp=(*ownCard)->m_iCurrHp;
    int MonsterHp=(*monsterCard)->m_iCurrHp;
     if(refactorpuTongGongJi(ownCard,monsterCard)!=-1)
    {
        OwncurrHp=checkShanghai(OwncurrHp,(*ownCard)->m_iCurrHp);
        MonsterHp=checkShanghai(MonsterHp,(*monsterCard)->m_iCurrHp);
        m_Animationlist.push_back(new SAnimationFps(ownIndex,0,(*ownCard)->m_iCurrHp,(*ownCard)->m_iHp,(*monsterCard)->m_iCurrHp,(*monsterCard)->m_iHp,Huihe,OwncurrHp,MonsterHp));
        cout<<"huihe:"<<Huihe<<" "<<ownIndex<<" "<<monstexIndex<<endl;
    }
    
}

void CSkillManager::refactorJisuan(int skillIndex,vector<CFightCard *>ownCardProperty,vector<CFightCard *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe)
{
    if(ownIndex > ownCardProperty.size() || enemyIndex >enemyCardpropert.size() )
    {
        PT_ERR_LOG("error");
        return ;
    }
    switch (skillIndex)
    {
        case 0:
        {
            basicGongji(skillIndex,&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex],ownIndex,enemyIndex,Huihe);
        }
      break;
     default:
      break;
    }
    if(enemyCardpropert[enemyIndex]->m_iCurrHp<=0)
    {
        //敌人移动
        enemyCardpropert[enemyIndex]->isDead=true;
    }
    //自己移动
    if(ownCardProperty[ownIndex]->m_iCurrHp<=0)
    {
        ownCardProperty[ownIndex]->isDead=true;
    }

}

void CSkillManager::clearAnimationList()
{
    
    DELETE_POINT_VECTOR(m_Animationlist, vector<SAnimationFps *>)
}

vector<SAnimationFps *> CSkillManager::getAnimationVector()
{
    return this->m_Animationlist;
}

