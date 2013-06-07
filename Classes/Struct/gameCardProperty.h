//
//  gameCardProperty.h
//  91.cube
//
//  Created by linminglu on 13-5-27.
//
//

#ifndef ___1_cube__gameCardProperty__
#define ___1_cube__gameCardProperty__

#include <iostream>
#include <vector>
using namespace std;
enum EN_CARDKIND
{
    EN_CARDKIND_GODCLAN=0,     //神族
    EN_CARDKIND_DROGANCLAN=1,  //龙族
    EN_CARDKIND_MACHINECLAN=2, //机械
    EN_CARDKIND_PNEUMACLAN=3  // 战魂
};

enum EN_CARDCATEGORY
{
    EN_EN_CARDCATEGORY_NONE =0,
    EN_EN_CARDCATEGORY_CLUB = 1,
    EN_EN_CARDCATEGORY_DIAMOND = 2,
    EN_EN_CARDCATEGORY_HEART = 3,
    EN_EN_CARDCATEGORY_SPADE = 4
};

class CCraftsmanship;   //技能
class CGameCardProperty //卡牌
{
public:
    CGameCardProperty()
    {
        m_pBasicSkill=m_pDeadSkill=m_pIncreaseSkill=m_pAdvocacy=NULL;
    }
    ~CGameCardProperty();
    int m_nPlayerCardIndex;
    string m_sPlayerCardName;         //卡牌名字
    unsigned int m_unCardLevel;        // 卡佩的星级
    EN_CARDKIND  m_eCardKind;          //组别类型
    EN_CARDCATEGORY m_eCardCategory;   //卡佩总类
    int m_nCardValue;               //花色 值
    unsigned int m_unPlayerCardAttack; //攻击力
    unsigned int m_unPlayerCardDefence;//防御力：
    unsigned int m_unHp ;              //血量
    unsigned int m_unCurrentHp;
    int  m_nPojia;
    int  m_nGeDang;
    CCraftsmanship *m_pBasicSkill;  //基础技能
    CCraftsmanship *m_pDeadSkill;   //战魂加持
    CCraftsmanship *m_pIncreaseSkill;//增幅技能
    CCraftsmanship *m_pAdvocacy;     //  拥护技能
};


#endif /* defined(___1_cube__gameCardProperty__) */
