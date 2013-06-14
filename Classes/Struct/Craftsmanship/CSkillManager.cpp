//
//  GongjiPutongJineng.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "CSkillManager.h"
#include "gameConfig.h"
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
    m_skillList.push_back(new BasicCCraftsmanship(1,"女神之枪",g_str_skillPathPlist+"0-sanmang.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(2,"女神之拥",g_str_skillPathPlist+"0-xingxing.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(3,"英灵颂歌",g_str_skillPathPlist+"7-zhongji.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(4,"寒霜号角",g_str_skillPathPlist+"10-chouxue.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(5,"水晶龙甲",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(6,"岩石技能",g_str_skillPathPlist+"18-xingxing.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(7,"藤编",g_str_skillPathPlist+"26-xingxing.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(8,"冰雪森林",g_str_skillPathPlist+"32-xuanyunbuff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(9,"回复术",g_str_skillPathPlist+"35-xingxing.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(10,"铁锤挥击",g_str_skillPathPlist+"32-xuanyunbuff.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(11,"大地守护",g_str_skillPathPlist+"penxue.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(12,"精锻",g_str_skillPathPlist+"home-jiesuo.plist",0,1));//,
    m_skillList.push_back(new BasicCCraftsmanship(13,"霜爪击",g_str_skillPathPlist+"changjing-yan.plist",2,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(14,"狼魂",g_str_skillPathPlist+"32-xuanyunbuff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(15,"撕咬",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new BasicCCraftsmanship(16,"爪击",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(17,"寒月守护",g_str_skillPathPlist+"14-buff.plist",0,1));//,
}

CSkillManager::~CSkillManager()
{
    DELETE_POINT_VECTOR(m_skillList,vector<CCraftsmanship* >);
}


CCraftsmanship *CSkillManager::getSkillByIndex(int index)
{
    if (index>=1&&index<m_skillList.size())
    {
        return m_skillList[index-1];
    }
    return NULL;
}

int  CSkillManager::puTongGongJi(SFightCardSprite ** ownSprite, SFightCardSprite **enemySprite)
{
    if((*ownSprite)->isCannotATK())
    {
        return -1;
    }
    else
    {
        int oldHp=(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp;
        //普通攻击    A攻击者攻击- B敌人的防御  =伤害  需要加上  防护的伤害
        (*enemySprite)->cardsprite->m_cardData.m_unCurrentHp -=(* ownSprite)->cardsprite->m_cardData.m_unPlayerCardAttack-(*enemySprite)->cardsprite->m_cardData.m_unPlayerCardDefence;
        if((*enemySprite)->cardsprite->m_cardData.m_unCurrentHp<0)
        {
            (*enemySprite)->cardsprite->m_cardData.m_unCurrentHp=0;
        }
        (*enemySprite)->dealLastHp();
        (*enemySprite)->fantanShanghai(ownSprite);
        (*enemySprite)->initShangHai();
        int retHp=oldHp-(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp ;
        return retHp>=0?retHp:0;
    } 
}



void  CSkillManager::dealWithSkillShanghai(int skillIndex,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianHp)
{
    if(ownIndex>ownCardProperty.size() ||enemyIndex >enemyCardpropert.size() )
    {
        PT_ERR_LOG("error");
        return ;
    }
    switch (skillIndex) {
        case 0:
        {
            jianHp=puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
            if(jianHp<=0)
            {
                jianHp=0;
            }
            
        }
            break;
        case 1:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                return ;
            }
            //回合攻击
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp=ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack*2-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence;
            enemyCardpropert[enemyIndex]->dealLastHp();
            enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
            enemyCardpropert[enemyIndex]->initShangHai();
        }
            break;
        case 2:
        {
            //死亡加成
            for (int i=0; i<ownCardProperty.size(); i++) {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack+=5;
                }
            }
            
        }
            break;
        case 3:
        {
            //拥护攻击
            if(RangeLand()>=50)
            {
                ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp   +=2;
                if(ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp>
                   ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp )
                {
                    ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp = ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp ;
                }
                enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -=2;
            }
            else
            {
                puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
            }
        }
            break;
        case 4:
        {
            ownCardProperty[ownIndex]->m_ibingdong+=1;
        }
            break;
        case 5:
        {
            for (int i=0; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardDefence+=3;
                }
            }
        }
            break;
        case 6:
        {
            if(RangeLand()<=50)
            {
                puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
            }
            else
            {
                ownCardProperty[ownIndex]->m_iHuduanshanghai+=2;
            }
        }
            break;
        case 7:
        {
            
        }
            break;
        case 8:
        {
            
            
        }
            break;
        case 9:
        {
            
        }
            break;
        case 10:
        {
            
        }
            break;
        case 11:
        {
            
        }
            break;
        case 12:
        {
            
        }
            break;
        case 13:
        {
            
            
        }
            break;
        case 14:
        {
            
            
        }
            break;
        case 15:
        {
            
        }
            break;
        default:
            break;
    }
}

