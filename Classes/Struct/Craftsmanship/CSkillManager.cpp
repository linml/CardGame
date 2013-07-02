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
#include "CCardPanel.h"
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

void CSkillManager::zhujiangjiaxue(int uHp,vector<SFightCardSprite *>vecFight)
{
    for (int i=0 ; i<vecFight.size(); i++) {
        if(!vecFight[i]->isDead)
        {
            vecFight[i]->cardsprite->m_cardData.setAddCurrentHp(uHp);
            return;
        }
    }
}

void CSkillManager::appendAnimationListBuffer(int ownIndex,int Huihe,SFightCardSprite *ownCardProperty, SFightCardSprite *enemyCardpropert)
{
    if (ownCardProperty->m_ibingdong)
    {
        m_Animationlist.push_back(new SAnimationFps(ownIndex,-2,ownCardProperty->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty->cardsprite->m_cardData.m_unHp,enemyCardpropert->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert->cardsprite->m_cardData.m_unHp,Huihe));
    }
    else if(ownCardProperty->m_iXuanYun)
    {
        m_Animationlist.push_back(new SAnimationFps(ownIndex,-3,ownCardProperty->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty->cardsprite->m_cardData.m_unHp,enemyCardpropert->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert->cardsprite->m_cardData.m_unHp,Huihe));
    }
    else if(ownCardProperty->m_iShuiMian)
    {
        m_Animationlist.push_back(new SAnimationFps(ownIndex,-4,ownCardProperty->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty->cardsprite->m_cardData.m_unHp,enemyCardpropert->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert->cardsprite->m_cardData.m_unHp,Huihe));
    }
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
void CSkillManager::removeSprite(cocos2d::CCNode *node,void *tag)
{
    CCSprite *sprite=(CCSprite *)(tag);
    if(layer)
    {
        layer->removeChild(sprite, true);
    }
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
void CSkillManager::monsterDead(int  huihe)
{
   m_Animationlist.push_back(new SAnimationFps(0,999,0,0,0,0,huihe,0,0));
}

void CSkillManager::fightDead(int  huihe)
{
     m_Animationlist.push_back(new SAnimationFps(0,1000,0,0,0,0,huihe,0,0));
}
void CSkillManager::putongGongji(int index,SFightCardSprite **ownSprite,SFightCardSprite **enemySprite,int ownIndex,int enemyIndex,int Huihe)
{
    int OwncurrHp=(*ownSprite)->cardsprite->m_cardData.m_unCurrentHp;
    int MonsterHp=(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp;
    int iShangHai =puTongGongJi(ownSprite,enemySprite);
    if(iShangHai!=-1)
    {
        OwncurrHp=checkShanghai(OwncurrHp,(*ownSprite)->cardsprite->m_cardData.m_unCurrentHp);
        MonsterHp=checkShanghai(MonsterHp,(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp);
        m_Animationlist.push_back(new SAnimationFps(ownIndex,0,(*ownSprite)->cardsprite->m_cardData.m_unCurrentHp,(*ownSprite)->cardsprite->m_cardData.m_unHp,(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp,(*enemySprite)->cardsprite->m_cardData.m_unHp,Huihe,OwncurrHp,MonsterHp));
    }
    else
    {
        appendAnimationListBuffer(ownIndex,Huihe,*ownSprite,*enemySprite);
    }
    
}
void CSkillManager::shangHaiZhiLogic(int index,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe)
{
    int OwncurrHp=ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp;
    cout<<"OwncurrHp"<<OwncurrHp<<endl;
    int MonsterHp=enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
    switch (index) {
        case 1:
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack*2-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence;
            break;
        case 2:
            break;
        case 7:
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= 3;
            break;
        case 8:
            break;
        case 9:
            zhujiangjiaxue(5, ownCardProperty);
        case 12:
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= (ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence)*1.2;
        case 13:
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= (ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence)*1.5;
        case 15:
            zhujiangjiaxue(1, ownCardProperty);
        case 16:
             enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= (ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack*1.2-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence);
        case 17:
            
        default:
            break;
    }
    
    enemyCardpropert[enemyIndex]->dealLastHp();
    enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
    enemyCardpropert[enemyIndex]->initShangHai();
    OwncurrHp=checkShanghai(OwncurrHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp);
    MonsterHp=checkShanghai(MonsterHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp);
    cout<<"enemyCardpropert:"<<enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp<<endl;
    cout<<"ownCardProperty:"<<ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp<<endl;
    m_Animationlist.push_back(new SAnimationFps(ownIndex,index,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,OwncurrHp,MonsterHp));
}
void CSkillManager::dealWithSkillShanghaiList(int skillIndex,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int Huihe)
{
    if(ownIndex>ownCardProperty.size() || enemyIndex >enemyCardpropert.size() )
    {
        PT_ERR_LOG("error");
        return ;
    }
    cout<<"skillIndex===>"<<skillIndex<<endl;
    switch (skillIndex)
    {
        case 0:
        {
            putongGongji(skillIndex,&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex],ownIndex,enemyIndex,Huihe);
        }
            break;
        case 1:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                appendAnimationListBuffer(ownIndex,Huihe,ownCardProperty[ownIndex],enemyCardpropert[enemyIndex]);
                break;
            }
            shangHaiZhiLogic(skillIndex,ownCardProperty,enemyCardpropert,ownIndex,enemyIndex,Huihe);
        }
            break;
        case 2:
        {
            m_Animationlist.push_back(new SAnimationFps(ownIndex,2,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            //战魂加持技能
            for (int i=0; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack+=5;
                    ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(5)); //增加攻击力
                }
            }            
        }
            break;
        case 3:
        {
            //拥护攻击
            if(RangeLand()>=50)
            {
                ownCardProperty[ownIndex]->cardsprite->m_cardData.setAddCurrentHp(2);
                enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -=2;
                m_Animationlist.push_back(new SAnimationFps(ownIndex,3,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,-2,2));
            }
            else
            {
                putongGongji(skillIndex,&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex],ownIndex,enemyIndex,Huihe);
            }
        }
            break;
        case 4:
            enemyCardpropert[enemyIndex]->m_ibingdong+=1; //回合 发动冰冻技能
            m_Animationlist.push_back(new SAnimationFps(ownIndex,4,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            break;
        case 5:
            //战魂加持技能
            for (int i=0; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardDefence+=3;
                    ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(4));//增加防御力
                }
            }
            m_Animationlist.push_back(new SAnimationFps(ownIndex,5,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            break;
        case 6:
            //用户技能
            if(RangeLand()<=50)
            {
                putongGongji(0, &ownCardProperty[ownIndex], &enemyCardpropert[enemyIndex], ownIndex, enemyIndex, Huihe);
            }
            else
            {
                for (int i=0; i<=3; i++)
                {
                    if(!ownCardProperty[i]->isDead)
                    {
                        ownCardProperty[i]->m_iHuduanshanghai+=2;
                        ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(3));//魂断伤害
                    }
                }
                m_Animationlist.push_back(new SAnimationFps(ownIndex,6,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            }
            break;
        case 7:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                appendAnimationListBuffer(ownIndex,Huihe,ownCardProperty[ownIndex],enemyCardpropert[enemyIndex]);
                break;
            }
            else
            {
                shangHaiZhiLogic(7, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            }
        }
            break;
        case 8:
        {
              m_Animationlist.push_back(new SAnimationFps(ownIndex,8,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.setAddCurrentHp(3);
                    ownCardProperty[i]->m_vbuffSkill.push_back(new  SkillBuff(4));
                }
            }
          
        }
            break;
        case 9 :
        {
            if(RangeLand()<=50)
            {
                putongGongji(9, &ownCardProperty[ownIndex],& enemyCardpropert[enemyIndex], ownIndex, enemyIndex, Huihe);
            }
            else
            {
                shangHaiZhiLogic(skillIndex, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            }
        }
            break;
        case 10:
            //有问题
            
            ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardDefence+=3;
            m_Animationlist.push_back(new SAnimationFps(ownIndex,10,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            break;
        case 11:
            m_Animationlist.push_back(new SAnimationFps(ownIndex,11,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack  += 1;
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardDefence += 1;
                    ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(2));
                }
            }
            break;
        case 12:
        {
            if(RangeLand()<50)
            {
                putongGongji(0, &ownCardProperty[ownIndex],& enemyCardpropert[enemyIndex], ownIndex, enemyIndex, Huihe);
            }
            else{
                shangHaiZhiLogic(12, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            }
        }
            break;
        case 13:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                appendAnimationListBuffer(ownIndex,Huihe,ownCardProperty[ownIndex],enemyCardpropert[enemyIndex]);
            }
            else
            {
                shangHaiZhiLogic(skillIndex, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            }
        }
        case 14:
             m_Animationlist.push_back(new SAnimationFps(ownIndex,skillIndex,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack +=2;
                    ownCardProperty[i]->cardsprite->m_cardData.m_unCurrentHp+=1;
                    ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(6));
                }
            }
            break;
        case 15:
            shangHaiZhiLogic(skillIndex, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            break;
        case 16:
            shangHaiZhiLogic(skillIndex, ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, Huihe);
            break;
        case 17:
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack +=2;
                    ownCardProperty[i]->cardsprite->m_cardData.m_unCurrentHp+=1;
                    ownCardProperty[i]->m_vbuffSkill.push_back(new SkillBuff(1));
                }
            }
             m_Animationlist.push_back(new SAnimationFps(ownIndex,skillIndex,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp,ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp,enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unHp,Huihe,0,0));
            break;
        default:
            break;
    }
    if(enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp<=0)
    {
        //敌人移动
        enemyCardpropert[enemyIndex]->isDead=true;
    }
            //自己移动
    if(ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp<=0)
    {
        ownCardProperty[ownIndex]->isDead=true;
    }
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
void CSkillManager::basicGongji(int index,CFightCard **ownCard,CFightCard **monsterCard,int ownIndex,int monstexIndex,int Huihe)
{
    int OwncurrHp=(*ownCard)->m_iCurrHp;
    int MonsterHp=(*monsterCard)->m_iCurrHp;
    
    if(refactorpuTongGongJi(ownCard,monsterCard)!=-1)
    {
        OwncurrHp=checkShanghai(OwncurrHp,(*ownCard)->m_iCurrHp);
        MonsterHp=checkShanghai(MonsterHp,(*monsterCard)->m_iCurrHp);
        m_Animationlist.push_back(new SAnimationFps(ownIndex,0,(*ownCard)->m_iCurrHp,(*ownCard)->m_iHp,(*monsterCard)->m_iCurrHp,(*monsterCard)->m_iHp,Huihe,OwncurrHp,MonsterHp));
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

