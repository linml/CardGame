//
//  CCard.cpp
//  91.cube
//
//  Created by linminglu on 13-6-26.
//
//

#include "CCard.h"
#include "CCardBufferStatus.h"
#include "gameConfig.h"
#include "CPtTool.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CCard::CCard()
{
    m_icard_id=0;
    m_scard_name.clear();
    m_ccard_next=0;;
    m_sicard_star=1;
    m_icard_stirps=0;
    m_icard_suit=0;
    m_icard_leadership=0;
    m_icard_exp=0;    //吃掉该卡牌的 经验
    m_icard_price=0;  //卖掉该卡佩的 价格
    m_ileve_max=0;   //等级最高级别
    m_icard_attack=0; //攻击力
    m_icard_defend=0; //防御力
    m_icardhp=0;    //卡牌的总的HP
    m_iskillLine=0;
    m_iskillHelp=0;
    m_iskillDead=0;
    m_iskillBuff=0;
    m_scard_tips.clear();
    m_scard_resources.clear();
    m_scard_head.clear();
    m_scard_ground.clear();
    
    
}

CFightCard::CFightCard()
{
    init();
}
void CFightCard::init()
{
    m_pCard=NULL;
    m_iCurrEngry=0;
   // m_bInBattleArray = false;
    m_iCurrHp = 0;  //当前HP
    m_iHp = 0;      //当前总的HP
    m_attack = 0;   //当前按的攻击力量
    m_defend = 0;   //当前的防御力
    tag = -1;        //当前卡牌的tag
    m_iCurrEngry = 0; //当前卡牌的怒气值
    m_iEngryMax = 0 ; //当前卡牌的最大怒气值得
    m_iMaxExp = 0;    //卡牌该等级的max exp
    m_User_Card_ID = 0; //卡牌在卡牌背包里面的id
    m_iCurrLevel = 1; //当前卡牌的等级
    m_iCurrExp = 0;   //当前卡牌拥有的exp
    isDead = false;     //卡牌是否已经死亡
    m_iSuit = 0;
    
    m_nCurrentPrice =0; // 从1升级到该级别所需要的金币
    m_nNeedExp = 0;     // 从1级升到该级别所需要的经验值
    m_nLeadShip = 0;
    m_nWhichBattleArray = 0; // 不在阵容中
    m_bConsume = false; // 用于强化，进化， 出售的flag
}


CFightCard::CFightCard(CCard *card,int level)
{
    tag=-1;
    
    init();
    updateFight(card, level);
    isDead=false;
    initFighting();
}

/*
 * @breif : use in evlution: card_user_id not change: change card_id;
 */
void CFightCard::updateFight(CCard *card, int level /*=1*/)
{
    if(card)
    {
        m_pCard=card;
        
        this->m_iCurrHp=m_pCard->m_icardhp;
        m_iHp=m_iCurrHp;
        m_attack=card->m_icard_attack;
        m_defend=card->m_icard_defend;
        
        
        if (level == 1)
        {
            // read card_config.plist data:
            m_iCurrHp=   m_pCard->m_icardhp;
            m_iHp=m_iCurrHp;
            m_attack=card->m_icard_attack;
            m_defend=card->m_icard_defend;
            m_nCurrentPrice =0;
            m_nNeedExp = 0;
            m_nLeadShip = card->m_icard_leadership;
        }
        else
        {
            // 累加的
            CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
            levelConfig->update(level);
            
           
            updateCard(level);
            CCLog("local: %d, %d, %d", m_iHp, m_attack, m_defend);
            m_nCurrentPrice = levelConfig->getConin();
            m_nNeedExp = levelConfig->getExp();
            // test:
            m_nLeadShip = card->m_icard_leadership;
            
        }
        CCLog("local: %d, %d, %d", m_iHp, m_attack, m_defend);
        
        m_iMaxExp = SingleLevleConfigData::instance()->getLevelExp(level+1);
      
        
    
        
    }
    else
    {
        m_pCard=NULL;
    }

}

void CFightCard::initFighting()
{
    isSendZengfu=false;
    isDead=false;
    m_iCurrEngry=0;
    DELETE_POINT_VECTOR(m_vBuffer,list<CCardBufferStatus*>);
}
bool CFightCard::isHaveBuffer(int prameid)
{
    if(m_vBuffer.size()==0)
    {
        return false;
    }
    for (list<CCardBufferStatus *>::iterator it=m_vBuffer.begin() ;it!=m_vBuffer.end();it++) {
        if((*it)->m_ieffectid==prameid)
        {
            return true;
        }
    }
    return false;
}


void CFightCard::updateCard(int level)
{
    if (m_iCurrLevel >= level)
    {
        return;
    }
    
    if(level > 1)
    {
        CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
        levelConfig->update(level);
        m_iHp =  CPtTool::calulate(m_pCard->m_icardhp,levelConfig->getHp(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                    levelConfig->getCorrectTow(), levelConfig->getCorrect());/// m_pCard->m_icardhp;
       
        m_attack =CPtTool::calulate(m_pCard->m_icard_attack, levelConfig->getAttack(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                   levelConfig->getCorrectTow(), levelConfig->getCorrect());
        m_defend =CPtTool::calulate(m_pCard->m_icard_defend, levelConfig->getDefine(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                   levelConfig->getCorrectTow(), levelConfig->getCorrect());
        
        m_nCurrentPrice = levelConfig->getConin();
        m_nNeedExp = levelConfig->getExp();
        m_iMaxExp = levelConfig->getLevelExp(level+1);
    }
    m_iCurrHp=m_iHp;

}


CFightCard::~CFightCard()
{
    
}


/*
 * @param type: 1: attak, 2: defense, 3: hp
 */
int CFightCard:: getAddValue(int level, int type)
{
    if (level == 1)
    {
        //return 0;
        
        switch (type)
        {
            case 1:
                return  m_pCard->m_icard_attack;
                break;
            case 2:
                break;
                return  m_pCard->m_icard_defend;
            case 3:
                return  m_pCard->m_icardhp;
                break;
            default:
                break;
        }
        
        return 0;
    }
    
    CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
    levelConfig->update(level);
    int result = 0;
   
     // int calulate(int card_base_value, int config_value, int star, float correct_value_1, float star_parameter, float correct_value_2,float correct_parameter)
    switch (type)
    {
        case 3:
            result = CPtTool::calulate(m_pCard->m_icardhp, levelConfig->getHp(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                     levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
            
        case 1:
            result =  CPtTool::calulate(m_pCard->m_icard_attack, levelConfig->getAttack(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                      levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
        case 2:
            result = CPtTool::calulate(m_pCard->m_icard_defend, levelConfig->getDefine(), m_pCard->m_sicard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_sicard_star) ,
                                       levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
        default:
            break;
    }
   
    return   result;
}

/*
 * @brief : 以该卡作为材料卡所需要消耗的金币
 */
int  CFightCard::getCostConin()
{
    int result = 0;
    int value = SingleLevleConfigData::instance()->getValeWithLevel(m_iCurrLevel+1, 1);
    int star_param = SingleLevleConfigData::instance()->getStarParamter(m_pCard->m_sicard_star, m_iCurrLevel+1);
    result = (int)(value*star_param+1.5)-1;
    return result;
}

/*
 * @param:  1: exp
 */
int CFightCard::getSupportValue(int type)
{
    
   int result = 0;
   int value =    SingleLevleConfigData::instance()->getValeWithLevel(m_iCurrLevel, 2);
    switch (type)
    {
        case 1:
            result = CPtTool::calSupportValue(m_pCard->m_icard_exp,value, 0.2);
            break;
        default:
            break;
    }
    return result;
}

/*
 * @param type: 1: money, 2: exp
 */
int CFightCard::getNeedValue(int level, int type)
{
    int result = 0;
    if (level > m_iCurrLevel);
    {
        CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
        levelConfig->update(level);
        if (type == 1)
        {
            result =  levelConfig->getConin()-m_nCurrentPrice;
        }else if(type == 2)
        {
            result = levelConfig->getExp()-m_nNeedExp;
        }

    }
  
    
  
    return result;
    
}

/*
 * @brief : 设置该卡牌在那个整容中
 * @param inType: 0: 不在阵容中 1: 在进攻阵容1中 2: 在进攻阵容2中 3 在防御阵容中:
 */

void CFightCard::setInBattleArray(const int &inType)
{
    m_nWhichBattleArray = inType;
    
}
int CFightCard::getInWhichBattleArray()
{
    return m_nWhichBattleArray;
}


void CFightCard::needRebackAtkAndBuf(CCardBufferStatus *buffer)
{
    
    switch (buffer->m_enBuffer_Field) {
        case  EN_BUFF_FIELD_TYPE_ATTACK:   //影响攻击力
            this->m_attack+=-buffer->m_iValue;
            break;
        case  EN_BUFF_FIELD_TYPE_DEFEND:   //影响防御力:
            this->m_defend +=-buffer->m_iValue;
            break;
        default:
            break;
    }
}

bool CFightCard::appendBuffer(CCardBufferStatus *buffer)
{
    CCLog("CURR m_vBuffer.size():%d",m_vBuffer.size());
    if(m_vBuffer.size()==0)
    {
        m_vBuffer.push_back(buffer);
        return true;
    }
    else{
        list<CCardBufferStatus *>::iterator it;
        for (it=m_vBuffer.begin(); it!=m_vBuffer.end(); it++)
        {
            //如果mutex的需要 一样的话， 并且字段的类型都是一样的话。
            if((*it)->m_mutex==buffer->m_mutex && (*it)->m_enBuffer_Field==buffer->m_enBuffer_Field)
            {
                if ((*it)->m_mutexlevel > buffer->m_mutexlevel)
                {
                    if(buffer)
                    {
                        delete buffer;
                        buffer=NULL;
                    }
                }
                else
                {
                    needRebackAtkAndBuf(*it);
                    m_vBuffer.erase(it);
                    delete *it;
                    *it=NULL;
                    m_vBuffer.push_back(buffer);
                    return true;
                }
            }
        }
    }
    return false;
}

