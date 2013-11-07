//
//  CReward.cpp
//  91.cube
//
//  Created by phileas on 13-9-22.
//
//

#include "CReward.h"
#include "gameTools.h"

/*
 * @breif : 获取 card_item, item, inbox, recharge, shop reward
 */

CCDictionary * CReward::getAllRewardByDict(CCDictionary *inRewardDict)
{
    CCDictionary * resultDict = CCDictionary::create();
    
    CCDictionary * addDict = NULL;
    CCDictionary * decDict = NULL;
    CReward * reward = NULL;
    
   
    if (inRewardDict)
    {
        addDict = (CCDictionary*) inRewardDict->objectForKey("add");
        decDict = (CCDictionary*) inRewardDict->objectForKey("dec");
        
        if (addDict)
        {
            // add
            CCDictionary * tmp = (CCDictionary*) addDict->objectForKey("card_item");
            if (tmp)
            {
                // event reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, CARDITEMREWARD_ADD);
                
            }
            tmp =  (CCDictionary*) addDict->objectForKey("item");
            
            if (tmp)
            {
                // chpater reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, ITEMREWARD_ADD);

            }
            
            tmp = (CCDictionary*) addDict->objectForKey("inbox");
            if (tmp)
            {
                // part reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, INBOXREWARD_ADD);
            }
            
            tmp = (CCDictionary*) addDict->objectForKey("recharge");
            
            if (tmp)
            {
                // task reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, RECHARGEREWARD_ADD);
                
            }
            
            
            tmp = (CCDictionary*) addDict->objectForKey("shop");
            if (tmp)
            {
                //step reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, SHOPREWARD_ADD);
                
            }
                      
            
        }
        if(decDict)
        {
            //dec
            
            
            CCDictionary * tmp = (CCDictionary*) decDict->objectForKey("card_item");
            if (tmp)
            {
                // event reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, CARDITEMREWARD_DEC);
            }
            
            tmp =  (CCDictionary*) decDict->objectForKey("item");
            if (tmp)
            {
                // chpater reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, ITEMREWARD_DEC);
            }
            
            tmp = (CCDictionary*) decDict->objectForKey("inbox");
            if (tmp)
            {
                // part reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, INBOXREWARD_DEC);
            }
            
            tmp = (CCDictionary*) decDict->objectForKey("recharge");
            if (tmp)
            {
                // task reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, RECHARGEREWARD_DEC);
                
            }
            
            
            tmp = (CCDictionary*) decDict->objectForKey("shop");
            if (tmp)
            {
                //step reward:
                reward = CReward::create(tmp);
                resultDict->setObject(reward, SHOPREWARD_DEC);
                
            }
            
        }
        
    }
    

    
    return resultDict;
}

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
 * @
 */
int CReward::excuteReward(OPEARTORTYPE inType)
{
    int flag = REWARD_FAIL;
    if (m_bExecute)
    {
        return flag;
    }else
    {
        REWARD_SET_RESULTE_SUCCESS(flag);
        if (inType == ADD)
        {
            m_pPlayer= SinglePlayer::instance();
            m_pPlayer->addCoin(m_nCoin);
            bool leveUP = m_pPlayer->addPalyerExp(m_nExp);
            m_pPlayer->addPlayerCash(m_nCash);
            m_pPlayer->addPlayerAp(m_nEnergy);
            m_pPlayer->addPlayerGp(m_nHp);
            m_pPlayer->addCoin(m_nExtarCoin);
            m_pPlayer->addPalyerExp(m_nExtarExp);
            addCards();
            addProps();
            
            if (leveUP)
            {
                REWARD_SET_LEVEL_UP_SUCCESS(flag);
            }
            
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
            return flag ;
        }
        
        m_bExecute = true;
        return flag;
    }
}

// getter and setter:

int CReward::getExtarExp()
{
    return m_nExtarExp;
}
int CReward::getExtarCoin()
{
    return m_nExtarCoin;
}

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
int CReward::getFriendly()
{
    return m_nFriendly;
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
    m_nFriendly = 0;
    m_nExtarCoin = 0;
    m_nExtarExp = 0;
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
    m_nExtarCoin = inReward->getExtarCoin();
    m_nExtarExp = inReward->getExtarExp();
    
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
    m_nExtarExp += inAddReward->getExtarExp();
    m_nExtarCoin += inAddReward->getExtarCoin();

    
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
        m_nFriendly = GameTools::intForKey("friendly", inReward);
        
        m_pCards = (CCDictionary*)inReward->objectForKey("card");
        m_pProps = (CCDictionary*)inReward->objectForKey("item");
        
        CCDictionary *tmpBuffer = NULL;
        tmpBuffer =(CCDictionary*) inReward->objectForKey("buff");
        if (tmpBuffer)
        {
            m_nExtarExp = GameTools::intForKey("exp", tmpBuffer);
            m_nExtarCoin = GameTools::intForKey("coin", tmpBuffer);
        }
        
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
    CCLog("Coin: %d --- HP: %d --- Energy: %d --- EXP: %d --- add EXP : %d, add COIN: %d", m_nCoin, m_nHp, m_nEnergy, m_nExp, m_nExtarExp, m_nExtarCoin);
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
    memset(outString, 0, leng);

    snprintf(outString,leng,"Coin: %d --- HP: %d --- Energy: %d --- EXP: %d, add EXP : %d, add COIN: %d, card count: %d, prop count: %d", m_nCoin, m_nHp, m_nEnergy, m_nExp,
            
           m_nExtarExp, m_nExtarCoin ,m_pCards == NULL ? 0 : m_pCards->count(), m_pProps == NULL ? 0 : m_pProps->count());
}

