//
//  CReward.cpp
//  91.cube
//
//  Created by phileas on 13-9-22.
//
//

#include "CReward.h"
#include "gameTools.h"


CReward* CReward::create(cocos2d::CCDictionary *inReward)
{
    
    CReward * reward = new CReward();
    if (reward->initWithDictionary(inReward))
    {
        reward->autorelease();
    }else
    {
        delete reward;
        reward = NULL;
    }
    
    
    return reward;
}

CReward* CReward::create(CReward * inReward1,CReward *inReward2)
{
    CReward * reward = new CReward();
    if (reward->merger(inReward1, inReward2))
    {
        reward->autorelease();
    }else
    {
        delete reward;
        reward = NULL;
    }
    return reward;
}

CReward::CReward()
{
    initData();
}

CReward::~CReward()
{
    CC_SAFE_RELEASE_NULL(m_pCards);
    CC_SAFE_RELEASE_NULL(m_pProps);
}

bool CReward::initWithDictionary(CCDictionary * inReward)
{
    bool bRet = false;
    do
    {
        parseDict(inReward);
        bRet = true;
    } while (0);
    return bRet;
}

bool CReward::merger(CReward * inReward1, CReward *inReward2)
{
    if (inReward1 || inReward2)
    {
       
        if (inReward1 && inReward2)
        {
            copyWithCReward(inReward1);
            addCReward(inReward2);
            
        }else
        {
            CReward * tmpReward = inReward1 == NULL ? inReward2 : inReward1;
            copyWithCReward(tmpReward);
           
        }
        
        return true;
    }
    return false;
}

/*
 * @param inType: [0-1]---> 0:add , other:dec
 */
bool CReward::excuteReward(OPEARTORTYPE inType)
{
    if (m_bExecute)
    {
        return false;
    }else
    {
        if (inType == ADD)
        {
            m_pPlayer= SinglePlayer::instance();
            m_pPlayer->addCoin(m_nCoin);
            m_pPlayer->addPalyerExp(m_nExp);
            m_pPlayer->addPlayerCash(m_nCash);
            m_pPlayer->addPlayerAp(m_nEnergy);
            m_pPlayer->addPlayerGp(m_nHp);

            addCards();
            addProps();
            
        }else if(inType == DEC)
        {
            m_pPlayer->subCoin(m_nCoin);
            m_pPlayer->subPlayerExp(m_nExp);
            m_pPlayer->subPlayerCash(m_nCash);
            m_pPlayer->subPlayerAp(m_nEnergy);
            m_pPlayer->subPlayerGp(m_nHp);
            subCards();
            subProps();
            
        }else
        {
            return false ;
        }
        
        m_bExecute = true;
        return m_bExecute;
    }
}

// getter and setter:

int CReward::getCash()
{
    return m_nCash;
}
int CReward::getCoin()
{
    return m_nCoin;
}
int CReward::getHP()
{
    return m_nHp;
}
int CReward::getEnergy()
{
    return m_nEnergy;
}
int CReward::getExp()
{
    return m_nExp;
}
int CReward::getCardCount()
{
    return m_pCards == NULL ? 0 : m_pCards->count(); 
}
int CReward::getPropCount()
{
    return m_pProps == NULL ? 0 : m_pProps->count();
}

CCDictionary * CReward::getCards()
{
    return m_pCards;
}
CCDictionary * CReward::getProps()
{
    return m_pProps;
}

// protected method:
void CReward::initData()
{
    m_nCoin = 0;
    m_nHp = 0;
    m_nEnergy = 0;
    m_nExp = 0;
    m_nCash = 0;
    m_pCards = NULL;
    m_pProps = NULL;
    m_bExecute = false;
    m_pPlayer = SinglePlayer::instance();
}

void CReward::copyWithCReward(CReward * inReward)
{
    if (inReward == NULL)
    {
        return;
    }
    m_nCash = inReward->getCash();
    m_nCoin = inReward->getCoin();
    m_nHp  = inReward->getHP();
    m_nExp = inReward->getExp();
    m_nEnergy = inReward->getEnergy();
    
    m_pCards = inReward->getCards();
    m_pProps = inReward->getProps();
    
    CC_SAFE_RETAIN(m_pCards);
    CC_SAFE_RETAIN(m_pProps);
}

void CReward::addCReward(CReward *inAddReward)
{
    m_nCoin += inAddReward->getCoin();
    m_nCash += inAddReward->getCash();
    m_nHp  += inAddReward->getHP();
    m_nExp += inAddReward->getExp();
    m_nEnergy += inAddReward->getEnergy();
    
    if (m_pCards == NULL)
    {
        m_pCards = inAddReward->getCards();
        CC_SAFE_RETAIN(m_pCards);
    }
    if (m_pProps == NULL)
    {
        m_pProps = inAddReward->getProps();
        CC_SAFE_RETAIN(m_pProps);
    }
}

void CReward::parseDict(cocos2d::CCDictionary *inReward)
{
    if (inReward)
    {
        m_nCoin = GameTools::intForKey("coin", inReward);
        m_nHp = GameTools::intForKey("gp", inReward);
        m_nEnergy = GameTools::intForKey("ap", inReward);
        m_nExp = GameTools::intForKey("exp", inReward);
        m_nCash = GameTools::intForKey("cash", inReward);
        
        m_pCards = (CCDictionary*)inReward->objectForKey("card");
        m_pProps = (CCDictionary*)inReward->objectForKey("item");
        
        CC_SAFE_RETAIN(m_pProps);
        CC_SAFE_RETAIN(m_pCards);
    }
    else
    {
        CCLog("the reward dictionary is null");
    }
   
}

void CReward::addCards()
{
    if (m_pCards)
    {
        m_pPlayer->decodeCardDictAppendCardBag(m_pCards);
    }
}

void CReward::subCards()
{
    if (m_pCards)
    {
        CCLog("add cards: ");
        CCArray* tmp = m_pCards->allKeys();
        CCString *tmpKey = NULL;
        if (tmp)
        {
            for (int i = 0; i < tmp->count(); i++)
            {
                tmpKey = (CCString*) tmp->objectAtIndex(i);
                m_pPlayer->subCardByUserId(tmpKey->intValue());
            }
        }
    }
    
}

void CReward::addProps()
{
    if (m_pProps)
    {
        int propId;
        int propNum;
        CCString * tmpNum = NULL;
        CCDictElement *elemnt = NULL;
        CCDICT_FOREACH(m_pProps, elemnt)
        {
            propId = atoi(elemnt->getStrKey());
            tmpNum = (CCString*) elemnt->getObject();
            propNum = tmpNum->intValue();
            m_pPlayer->addProp(propId, propNum);
        }
    }
}

void CReward::subProps()
{
    if (m_pProps)
    {
        int propId;
        int propNum;
        CCString * tmpNum = NULL;
        CCDictElement *elemnt = NULL;
        CCDICT_FOREACH(m_pProps, elemnt)
        {
            propId = atoi(elemnt->getStrKey());
            tmpNum = (CCString*) elemnt->getObject();
            propNum = tmpNum->intValue();
            m_pPlayer->subProp(propId, propNum);
        }
    }
    
}

// test method:

void CReward::printContent()
{
    CCLog("Coin: %d --- HP: %d --- Energy: %d --- EXP: %d", m_nCoin, m_nHp, m_nEnergy, m_nExp);
    if (m_pProps)
    {
        CCLog("the prop count: %d", m_pProps->count());
    }else
    {
         CCLog("the prop null");
    }
    if (m_pCards)
    {
        CCLog("the cards count: %d", m_pCards->count());
    }else
    {
         CCLog("the card null");
    }
}

void CReward::getRewardContent(char * outString, const int leng)
{
    memset(outString, 0, sizeof(char)*leng);
    sprintf(outString,"Coin: %d --- HP: %d --- Energy: %d --- EXP: %d, card count: %d, prop count: %d", m_nCoin, m_nHp, m_nEnergy, m_nExp
            
            ,m_pCards == NULL ? 0 : m_pCards->count(), m_pProps == NULL ? 0 : m_pProps->count());
}
