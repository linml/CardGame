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
    m_nCard_star=1;
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
    isShiFouYongyouguo=false;
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
    m_iEngryMax=card->m_icard_energyMax;
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
            //CCLog("local: %d, %d, %d", m_iHp, m_attack, m_defend);
            m_nCurrentPrice = levelConfig->getConin();
            m_nNeedExp = levelConfig->getExp();
            m_nLeadShip = card->m_icard_leadership;
        }
        //CCLog("local: %d, %d, %d", m_iHp, m_attack, m_defend);
        
        m_iMaxExp = SingleLevleConfigData::instance()->getLevelExp(level+1);
        m_iCurrLevel = level;
  
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
    DELETE_POINT_VECTOR(m_vlistBuffer, list<CCardBufferStatusRefactor*>);
}

bool CFightCard::isCanFight()
{
    if(m_vlistBuffer.size()==0)
    {
        return true;
    }

    for (list<CCardBufferStatusRefactor *>::iterator it=m_vlistBuffer.begin() ;it!=m_vlistBuffer.end();it++)
    {
        if(!(*it)->m_bIsCanfight)
        {
            return false;
        }
    }
    return true;
}

int CFightCard::getFightPoint()
{
    float star_param = SingleLevleConfigData::instance()->getStarParamter(m_pCard->m_nCard_star, m_iCurrLevel);
    int result = CPtTool::getFightPoint(m_attack, m_defend, m_iHp, m_iCurrLevel, star_param);
    return result;
}

bool CFightCard::isHaveParamid10(int nParamidValue)
{
    if(m_vlistBuffer.size()==0)
    {
        return false;
    }
    for (list<CCardBufferStatusRefactor *>::iterator it=m_vlistBuffer.begin() ;it!=m_vlistBuffer.end();it++) {
        if((*it)->m_iEffect_paramid10==nParamidValue)
        {
            return true;
        }
    }
    return false;
}

bool CFightCard::isHaveBufferRefactor(int prameid)
{
    if(m_vlistBuffer.size()==0)
    {
        return false;
    }
    for (list<CCardBufferStatusRefactor *>::iterator it=m_vlistBuffer.begin() ;it!=m_vlistBuffer.end();it++) {
        if((*it)->m_iEffectid==prameid)
        {
            return true;
        }
    }
    return false;
}

bool CFightCard::hasAssistantSkill()
{
    return m_pCard->m_iskillHelp == 0 ? false: true;
    
}

void CFightCard::setNegativeToZero()
{
    if(m_iCurrHp<0)
    {
        m_iCurrHp=0;
    }
}

void CFightCard::subAtk(int value)
{
    m_attack+=value;
    if(m_attack<0)
    {
        m_attack=0;
    }
}
void CFightCard::subDef(int value)
{
    m_defend+=value;
    if(m_defend<0)
    {
        m_defend=0;
    }
}
void CFightCard::appendEngry(int iEngry)
{
    this->m_iCurrEngry+=iEngry;
    if(this->m_iCurrEngry<0)
    {
        m_iCurrEngry=0;
    }else if(this->m_iCurrEngry >= this->m_iEngryMax)
    {
        this->m_iCurrEngry=this->m_iEngryMax;
    }
    CCLog("this->m_iCurrEngry:%d",this->m_iCurrEngry);
}

void CFightCard::appendHp(int iAddHp)
{
    this->m_iCurrHp+=iAddHp;
    if(m_iCurrHp<0)
    {
        m_iCurrHp=0;
    }
    else if (m_iCurrHp>=m_iHp)
    {
        m_iCurrHp=m_iHp;
    }
}

void CFightCard::updateCard(int level)
{
//    if (m_iCurrLevel >= level)
//    {
//        return;
//    }
    
    if(level > 1)
    {
        CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
        levelConfig->update(level);
        m_iHp =  CPtTool::calulate(m_pCard->m_icardhp,levelConfig->getHp(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                    levelConfig->getCorrectTow(), levelConfig->getCorrect());/// m_pCard->m_icardhp;
       
        m_attack =CPtTool::calulate(m_pCard->m_icard_attack, levelConfig->getAttack(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                   levelConfig->getCorrectTow(), levelConfig->getCorrect());
        m_defend =CPtTool::calulate(m_pCard->m_icard_defend, levelConfig->getDefine(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                   levelConfig->getCorrectTow(), levelConfig->getCorrect());
        
        m_nCurrentPrice = levelConfig->getConin();
        m_nNeedExp = levelConfig->getExp();
        m_iMaxExp = levelConfig->getLevelExp(level+1);
    }
    m_iCurrHp=m_iHp;

}


CFightCard::~CFightCard()
{
    CCLog("CLEAR BUFFER PTR");
    DELETE_POINT_VECTOR(m_vlistBuffer, list<CCardBufferStatusRefactor*>);
}


/*
 * @param type: 1: attak, 2: defense, 3: hp
 */
int CFightCard:: getAddValue(int level, int type)
{
    if (level == 1)
    {        
        switch (type)
        {
            case 1:
                return  m_pCard->m_icard_attack;
            case 2:
                return  m_pCard->m_icard_defend;
            case 3:
                return  m_pCard->m_icardhp;
            default:
               return 0;
        }
    }
    
    CPtLevelConfigData * levelConfig = SingleLevleConfigData::instance();
    levelConfig->update(level);
    int result = 0;
   
     // int calulate(int card_base_value, int config_value, int star, float correct_value_1, float star_parameter, float correct_value_2,float correct_parameter)
    switch (type)
    {
        case 3:
            result = CPtTool::calulate(m_pCard->m_icardhp, levelConfig->getHp(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                     levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
            
        case 1:
            result =  CPtTool::calulate(m_pCard->m_icard_attack, levelConfig->getAttack(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                      levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
        case 2:
            result = CPtTool::calulate(m_pCard->m_icard_defend, levelConfig->getDefine(), m_pCard->m_nCard_star, levelConfig->getCorrectOne(), levelConfig->getStarParamter(m_pCard->m_nCard_star) ,
                                       levelConfig->getCorrectTow(), levelConfig->getCorrect());
            break;
        default:
            break;
    }
   
    return   result;
}

/*
 * @brief : 以该卡作为材料卡所需要消耗的金币, 向下取整
 */
int  CFightCard::getCostCoin()
{
    int result = 0;
    int value = SingleLevleConfigData::instance()->getValeWithLevel(m_iCurrLevel+1, 1);
    int star_param = SingleLevleConfigData::instance()->getStarParamter(m_pCard->m_nCard_star, m_iCurrLevel+1);
    result = (int)(value*star_param);
    return result;
}

/*
 * @breif: 该卡作为材料卡提供的经验值
 * @param:  1: exp
 */
int CFightCard::getSupportValue(int type)
{
    
    int result = 0;
    int value =  SingleLevleConfigData::instance()->getValeWithLevel(m_iCurrLevel, 2);
    float star_param = SingleLevleConfigData::instance()->getStarParamter(m_pCard->m_nCard_star, m_iCurrLevel);
    switch (type)
    {
        case 1:
            result = CPtTool::calSupportValue(m_pCard->m_icard_exp,value, star_param,  0.2);
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


bool CFightCard::appendBufferData(CCardBufferStatusRefactor *pBuffer)
{
    if(m_vlistBuffer.size()==0)
    {
        m_vlistBuffer.push_back(pBuffer);
        return true;
    }
    bool isDeleItetor=false;
    for (list<CCardBufferStatusRefactor *>::iterator it=m_vlistBuffer.begin(); it!=m_vlistBuffer.end();)
    {
        if(pBuffer->m_iEffectid==(*it)->m_iEffectid || (pBuffer->m_iMutex == (*it)->m_iMutex && pBuffer->m_iMutexLevel>=(*it)->m_iMutexLevel)) //其实 只需要后面的那个就可以
        {
            //ID相等的时候
            if((*it)->m_iNeedAddBack)
            {
                appendHp(-(*it)->m_iHp);
                m_attack +=-(*it)->m_iAtk;
                m_defend +=(*it)->m_iDef;
                appendEngry(-(*it)->m_iEngry);
            }
            isDeleItetor=true;
            CCardBufferStatusRefactor *tempData=(*it);
            it=m_vlistBuffer.erase(it);
            CC_SAFE_DELETE(tempData);
            m_vlistBuffer.push_back(pBuffer);
            return true;
        } else  if(pBuffer->m_iMutex == (*it)->m_iMutex && pBuffer->m_iMutexLevel <= (*it)->m_iMutexLevel){
            CC_SAFE_DELETE(pBuffer);
            return false;
        }
        if(!isDeleItetor)
        {
            it++;
        }
    }
    m_vlistBuffer.push_back(pBuffer);
    return true;
}

