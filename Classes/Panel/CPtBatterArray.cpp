#include "CPtBatterArray.h"
#include "CPtTool.h"
#include "gameConfig.h"
#include "CGamesCard.h"
#include <cmath>
#include "CCard.h"
#include "PtHttpClient.h"


// test:
#define ADDHTTPREQUESTPOSTDATA(URL,NOTIFICATIONTAG,HTTPREQUESTTAG,__POSTSTR__,CALLBACK)\
{\
stcRequestInf inf;\
inf.m_pchURL = URL; \
cout<<inf.m_pchURL<<endl; \
inf.m_RequestType=CCHttpRequest::kHttpPost;\
inf.m_pSelector = NOTIFICATIONTAG;\
inf.m_pchTag = HTTPREQUESTTAG;\
inf.m_pchData = __POSTSTR__;\
CPtHttpClient::sharePtHttpClient()->addRequest(inf);\
CCNotificationCenter::sharedNotificationCenter()->addObserver(this,CALLBACK, inf.m_pSelector, NULL);\
}

using namespace CPtTool;

// implement CPtDisPlayCard:
CPtDisPlayCard  * CPtDisPlayCard::Create(CFightCard *card)
{
    CPtDisPlayCard *cardSprite=new CPtDisPlayCard();
    if(cardSprite ==NULL|| !cardSprite->initCreate(card))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;

}


// implement CPtBattleArray:
CPtBattleArray* CPtBattleArray::create(vector<CFightCard *> &fightArray, const cocos2d::CCSize &size, const cocos2d::CCPoint &point, int Tag /*= 1*/)
{
	CPtBattleArray * batterarray = new CPtBattleArray();
    
	if (batterarray)
	{
        batterarray->inTag = Tag;
        batterarray->initMap();
        batterarray->setTouchEnabled(true);
        batterarray->setTouchMode(kCCTouchesOneByOne);
		batterarray->autorelease();
        batterarray->initSize(size, point);
        batterarray->initBattleArrayFromServer(fightArray);
    
	}
	return batterarray;
}

CPtBattleArray::CPtBattleArray()
{
	initData();
}

CPtBattleArray::~CPtBattleArray()
{
}

void CPtBattleArray::initSize(const cocos2d::CCSize &size, const cocos2d::CCPoint &point)
{
    float width = size.width;
    m_cPositions[0] = point;
    for (int i = 1; i < CARDCOUNT-1; i++)
    {
        m_cPositions[i] = ccp(m_cPositions[i-1].x+width/2, m_cPositions[i-1].y);
    }
    m_cPositions[4] = ccp(25, 150);
    
}

void CPtBattleArray::initBattleArrayFromServer(vector<CFightCard *> &fightArray)
{
    CFightCard * tmp = fightArray.at(fightArray.size()-1);
    CCLog("size: %d", fightArray.size());
    if (tmp)
    {
        // test: change the tmp resource value:
    
//        tmp->m_pCard->m_icard_suit= rand()%52+1;
//        tmp->m_pCard->m_scard_head="";  //头像
//        tmp->m_pCard->m_scard_groud="bg1.png"; //
//        tmp->m_pCard->m_scard_role="";
//        tmp->m_pCard->m_scard_resources="peo.png";
        // testEnd;
          CCLog("id, %d", tmp->m_pCard->m_icard_id);
        CGamesCard * card = CPtDisPlayCard::Create(tmp);
        addCard(card, 4);
    }
    int index = 0;
    for (int i = 3; i >= 0 ;i--)
    {
        tmp = NULL;
        tmp = fightArray.at(i);
        if (tmp)
        {
            CCLog("fight: %d",i);
            // test: change the tmp resource value:
//            tmp->m_pCard->m_icard_suit= rand()%52+1;
//            tmp->m_pCard->m_scard_head="";  //头像
//            tmp->m_pCard->m_scard_groud="bg1.png"; //
//            tmp->m_pCard->m_scard_role="";
//            tmp->m_pCard->m_scard_resources="peo.png";
            // testEnd;
             CCLog("position:%d,id, %d", i,tmp->m_pCard->m_icard_id);
            CGamesCard * card = CPtDisPlayCard::Create(tmp);
            addCard(card, index++);
        }else
        {
            CCLog("fight: %d null",i);
        }
    }
    updateBattleArray();
}

void CPtBattleArray::createSuitLogo(const int& inSuit, const int &inSequence, const int &inPositionIndex)
{
    char buff[10] ={0};
    sprintf(buff, "suit%d.png", inSuit);
    CCSprite *suitSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, buff));
    suitSprite->setAnchorPoint(CCPointZero);
    suitSprite->setPosition(ccp(5,5));
    m_pSuitLogo[inPositionIndex]->cocos2d::CCNode::removeAllChildrenWithCleanup(true);
    m_pSuitLogo[inPositionIndex]->addChild(suitSprite,300);
   
    m_aSuitArray[inPositionIndex] = inSuit;
    m_aSequenceArray[inPositionIndex] = inSequence;
}

/*
 * @function: bool addCard(CCObject *inCard, const int & inCardType)
 * @brief: add or replace card to the batterarray.
 * @param inCard : the card to the batterarray.
 * @param inCardType: the card type: from 0 - 4, when the value is 4, it means the card act as intervene role.
 * @return ture | false 
 */

bool CPtBattleArray::addCard(CCNode *inCard, const int & inCardType , const bool &inAppendEnable /*= false*/)
{
    
	bool bRet = false;

	if(inCardType == 4)
	{
        if (m_pCardArray[inCardType] != NULL)
        {
           return replaceCard(inCard, inCardType);
        }
		// add the intervene card
		// 1. first distinguish the card's kind , if it'kind is intervene. continue, else return false.
        
        // update ui:
        m_pCardArray[inCardType] = inCard;
      
        if (m_pCardArray[inCardType])
		{
            dynamic_cast<CPtDisPlayCard*>(inCard)->getCardData()->setInBattleArray(true);
            inCard->setAnchorPoint(CCPointZero);
            inCard->setPosition(m_cPositions[inCardType]);
            this->addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
            
		}


	}

    // append:
   	if (inCardType >= 0 && inCardType < 4)
	{
        
		if (m_pCardArray[inCardType])
		{
			// remove the former card, set the former card state.
            CCLog("have");
		
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        
        if (m_pCardArray[inCardType])
		{
            inCard->setAnchorPoint(CCPointZero);
            inCard->setPosition(m_cPositions[inCardType]);
            this->addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
            dynamic_cast<CGamesCard*>(inCard)->getCardData()->setInBattleArray(true);
		}
		
	}
	else
	{
		// the illegality card
	}

	updateBattleArray();

	return bRet;
}

bool CPtBattleArray::replaceCard(CCNode *inCard, const int & inCardType)
{
    bool bRet = false;


	if(inCardType == 4)
	{
		// add the intervene card
		// 1. first distinguish the card's kind , if it'kind is intervene. continue, else return false.
        
        // update
        if (m_pCardArray[inCardType])
		{
            
            CPtDisPlayCard * tmpCard = dynamic_cast<CPtDisPlayCard*>(m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
                tmpCard->getCardData()->setInBattleArray(false);
              //  tmpCard = tmpCard->getCardData()->getInCardBagPoint();
                if (tmpCard)
                {
               //     tmpCard->getCardData()->setInCardBagPoint(NULL);
                }
            }

            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            dynamic_cast<CGamesCard*>(inCard)->getCardData()->setInBattleArray(true);
            addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
           
        }

        
	}
    
	if (inCardType >= 0 && inCardType < 4)
	{
		if (m_pCardArray[inCardType])
		{
			// remove the former card, set the former card state.
            CPtDisPlayCard * tmpCard = dynamic_cast<CPtDisPlayCard*>(m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
                tmpCard->getCardData()->setInBattleArray(false);
              //  tmpCard = tmpCard->getCardData()->getInCardBagPoint();
                if (tmpCard)
                {
                  //  tmpCard->getCardData()->setInCardBagPoint(NULL);
                }
            }
            
            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
            
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            dynamic_cast<CGamesCard*>(inCard)->getCardData()->setInBattleArray(true);
            addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
		}
		
	}
	else
	{
		// the illegality card
	}
    
	updateBattleArray();
    
	return bRet;

    
}

int CPtBattleArray::getReplaceCard(CCNode *inCard)
{
    CCRect rect = inCard->boundingBox();
    rect.origin = this->convertToNodeSpace(rect.origin);
    

    for (int i = 0; i < CARDCOUNT; i++)
    {
        CCNode *tmp = m_pCardArray[i];
        if (tmp)
        {
            float dt = abs(ccpDistance(ccp(rect.getMidX(), rect.getMidY()), ccp(tmp->boundingBox().getMidX(), tmp->boundingBox().getMidY())));
            if (dt < 8.0f)
            {
                return i;
            }
        }
    }
    return -1;
}

bool CPtBattleArray::isAssistantCard(CCNode *inCard, bool inRemove)
{
    CCRect rect ;
    rect.origin = m_cPositions[4];
    rect.size = CCSizeMake(139, 200);
    CCPoint point = inCard->getPosition();
    point = this->convertToNodeSpace(point);
    if (rect.intersectsRect (CCRectMake(point.x, point.y, 139, 200)))
    {
        if (inRemove)
        {
            return true;
        }
        else
        {
            if ((point.x-rect.origin.x) < 100)
            {
                return true;
            }
        }

    }
    return false;
}

bool CPtBattleArray::isAssistantCard(CCTouch *pTouch)
{
   
    if (m_pCardArray[CARDCOUNT-1])
    {
        CCRect rect = m_pCardArray[CARDCOUNT-1]->boundingBox();
        CCPoint point =  this->convertTouchToNodeSpace(pTouch);
        if (rect.containsPoint(point))
        {
            return true;
        }
    }
    return false;

}

bool CPtBattleArray::removeCard(const int& inCardType)
{
	bool bRet = false;
	if (inCardType >= 0 && inCardType < 5)
	{
		if (m_pCardArray[inCardType])
		{
			// set card state:            
			// set null

            CPtDisPlayCard * tmpCard = dynamic_cast<CPtDisPlayCard*>(m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
             //   tmpCard = tmpCard->getCardData()->getInCardBagPoint();
                if (tmpCard)
                {
                    tmpCard->getCardData()->setInBattleArray(false);
                //    tmpCard->getCardData()->setInCardBagPoint(NULL);
                }
            }

            removeChild(m_pCardArray[inCardType], true);
			m_pCardArray[inCardType] = NULL;
            m_pSuitLogo[inCardType]->removeAllChildrenWithCleanup(true);
            m_aSequenceArray[inCardType] = -1;
            m_aSuitArray[inCardType] = -1;
			updateBattleArray();
		}
		
	}

	return bRet;
}

int CPtBattleArray::getFightPower()
{
	return m_nFightPower;
}

int CPtBattleArray::getLeaderShip()
{
	return m_nLeadership;
}

int CPtBattleArray::getGrowthValue()
{
	return m_nGrowthValue;
}

CardSuitKind CPtBattleArray::getCardSuitKind()
{
	return m_SuitKind;
}


CCArray * CPtBattleArray::getCardArray()
{
	CCArray * array =  CCArray::create();
	for (int i = 0; i < CARDCOUNT; ++i)
	{
		if (m_pCardArray[i])
		{
			array->addObject(m_pCardArray[i]);
		}
	}
	return array;
	
}

int  CPtBattleArray::resortFightCard()
{
    int next = 0;
    for (int i  = 0;  i < CARDCOUNT -1; i++)
    {
       
        
        if (m_pCardArray[i])
        {
            if (next != i)
            {
                m_pCardArray[next] = m_pCardArray[i];
                m_pCardArray[i] = NULL;
               
            }
            m_pCardArray[next]->setPosition(m_cPositions[next]);
            this->reorderChild(m_pCardArray[next], 1000+next);
            next++;
            CCLog("the card:%d", i);
        }
    }
    if (m_pCardArray[4])
    {
        m_pCardArray[4]->setPosition(m_cPositions[4]);
    }
    return next;
}

int CPtBattleArray::getInsertIndex()
{
   return insertWhichNode(m_pCardArray, this, CARDCOUNT);
}

void CPtBattleArray::initMap()
{
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "zhenxingfudong.plist"));
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setAnchorPoint(CCPointZero);
    this->addChild(bg, -1);
    this->setContentSize(bg->getContentSize());
    int tags[3]={2,0,3};
    for (int i = 0 ; i < CARDCOUNT; i++)
    {
        tags[2] = 3+ i;
        m_pSuitLogo[i] =(CCSprite *) m_cMaps->getElementByTags(tags, 3);
    }
    
    if (inTag == 2)
    {
       dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,0,1"))->setString("Team 2 Arrage") ;
    }else if(inTag == 3)
    {
        dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,0,1"))->setString("Defens Team") ;
    }
    
    
    m_pSaveBtn =dynamic_cast<CCSprite *>(m_cMaps->getElementByTags("2,0,8"));
    
  
}
void CPtBattleArray::initData()
{
    m_bTouchEnable = false;
	m_nLeadership = 0;
	m_nFightPower = 0;
    m_nDefense = 0;
    m_nHP = 0;
    m_nSuitAtk = 0;
    m_nSuitDef = 0;
    
	m_nGrowthValue = 0;
    // init logo:
    for (int i = 0; i < CARDCOUNT; i++)
    {
        m_pSuitLogo[i] = NULL;
        m_pCardArray[i] = NULL;
        m_aSuitArray[i] = -1;
        m_aSequenceArray[i] = -1;
    }
}


void CPtBattleArray::updateData()
{
    m_nLeadership = 0;
	m_nFightPower = 0;
    m_nDefense = 0;
    m_nHP = 0;
    
    if (m_pCardArray[CARDCOUNT-1])
    {
        m_nLeadership = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_pCard->m_icard_leadership;
        m_nFightPower = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_attack;

        m_nDefense = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_defend;
        m_nHP = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_iHp;
    }
    
    for (int i = 0; i < CARDCOUNT-1 ;i++ )
    {
        
        if (m_pCardArray[i])
        {
           CFightCard *cardData = dynamic_cast<CGamesCard *>(m_pCardArray[i])->getCardData();
           m_nLeadership += cardData-> m_pCard->m_icard_leadership;
           m_nFightPower += cardData-> m_attack; //攻击力
           m_nDefense    += cardData-> m_defend; //防御力
           m_nHP    += cardData-> m_iHp;    //卡牌
        }
        else
        {
            break;
        }
    }
    
}

void CPtBattleArray::updateBattleArray()
{
  
    
    resortFightCard();
    updateCardSuitKind();
    updateData();
    updateLabel();
}



void CPtBattleArray::updateCardSuitKind()
{
    //
    
    m_nSuitAtk = 0;
    m_nSuitDef = 0;
    
    int outSuit, outSequence;
    for (int i = 0; i < CARDCOUNT-1; i++)
    {
        if (m_pCardArray[i])
        {
            if (dynamic_cast<CGamesCard *>(m_pCardArray[i])->getSuit(outSuit, outSequence))
                createSuitLogo(outSuit, outSequence, i+1);
        }else
        {
             m_pSuitLogo[i+1]->removeAllChildrenWithCleanup(true);
             m_aSuitArray[i+1] = -1;
             m_aSequenceArray[i+1] = -1;
        }
       
    }
    if (m_pCardArray[CARDCOUNT-1])
    {
        if (dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getSuit(outSuit, outSequence))
            createSuitLogo(outSuit, outSequence, 0);
    }else
    {
        m_pSuitLogo[0]->removeAllChildrenWithCleanup(true);
        m_aSuitArray[0] = -1;
        m_aSequenceArray[0] = -1;
    }
    
    // test
    
    m_nSuitAtk = 10;
    m_nSuitDef = 5;
}

void CPtBattleArray::updateLabel()
{
    CCLabelTTF *label = NULL;
    char buff[40]={0};
    
    // atk:
    sprintf(buff, "ATK: %d (+%d)",m_nFightPower, m_nSuitAtk);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,3"));
    label->setString(buff);
    
    // hp
    sprintf(buff, "HP: %d",m_nHP);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,4"));
    label->setString(buff);
    
    // def
    sprintf(buff, "DEF: %d (-%d)",m_nDefense,m_nSuitDef);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,5"));
    label->setString(buff);
    
    // rvc
    sprintf(buff, "RVC: %d",m_nLeadership);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,6"));
    label->setString(buff);
    
    // suit
    sprintf(buff, "ATK +%d DEF -%d",m_nSuitAtk, m_nSuitDef);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,2,2"));
    label->setString(buff);
    
    
}

static CCNode * selectNode  = NULL;
static int selectIndex = -1;

bool CPtBattleArray::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bOnClick = false;
    m_bTouchEnable = false;
    CCLog("CPtBattleArray %d ::ccTouchBegan",inTag+1);
   
    if (CPtTool::isInNode(m_pSaveBtn, pTouch))
    {
        m_bOnClick = true;
       // m_bTouchEnable = true;
        return true;
    }
    
    if (isAssistantCard(pTouch))
    {
        selectIndex = 4;
        selectNode = m_pCardArray[4];
        m_bTouchEnable = true;
        return true;
    }

    CCPoint point = this->getParent()->convertTouchToNodeSpace(pTouch);
    if (this->boundingBox().containsPoint(point))
    {
       
        selectIndex = whichNode(m_pCardArray, this, pTouch, resortFightCard());
        CCLog("the selectIndex: %d", selectIndex);
        if (selectIndex == -1)
        {
            return false;
        }
        selectNode = m_pCardArray[selectIndex];
        this->reorderChild(selectNode, 1020);
        m_bTouchEnable = true;
        return true;
        
    }
    
      return false;
}
void CPtBattleArray::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchMoved");
    
    if(m_bOnClick == true)
    {
        return;
    }
    
    if (selectNode)
    {
        drag(selectNode, pTouch);
       
        CCNode *right = selectIndex+1 < 4? m_pCardArray[selectIndex+1] : NULL;
        CCNode *left = selectIndex -1 >= 0? m_pCardArray[selectIndex-1]:NULL;
        if (isNeedMove(selectNode, right, 1))
        {
            moveHalfStep(right, 2);
            m_pCardArray[selectIndex] = right;
            selectIndex++;
            m_pCardArray[selectIndex] = selectNode;
        }else if(isNeedMove(selectNode, left, 2))
        {
            moveHalfStep(left, 1);
            m_pCardArray[selectIndex] = left;
            selectIndex--;
            m_pCardArray[selectIndex] = selectNode;
        }
    }

}
void CPtBattleArray::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchEnded");
    
 
    if (m_bOnClick)
    {
        save();
        return;
    }
    
    if (selectNode)
    {
//        CCRect rect1 = selectNode->boundingBox();
//        rect1.origin = this->getParent()->convertToNodeSpace(rect1.origin);
//        CCRect rect = this->boundingBox();
        
        //rect.origin = this->convertToNodeSpace(rect.origin);

        CCRect rect1 = selectNode->boundingBox();
        rect1.origin = selectNode->getParent()->convertToWorldSpace(rect1.origin);
        CCRect rect = this->boundingBox();
        rect1.origin = this->convertToNodeSpace(rect1.origin);
        rect.origin = CCPointZero;
        if (selectIndex == 4 && !isAssistantCard(selectNode,true))
        {
            removeCard(4);
        }
        else if(!rect.intersectsRect(rect1))
        {
            removeCard(selectIndex);
        }
     
    }
    updateBattleArray();
    
    for (int i = 0; i < CARDCOUNT-1; i++)
    {
        if (m_pCardArray[i])
        {
            CCLog("the card: %d",i);
        }
    }
    
}
void CPtBattleArray::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayer::ccTouchCancelled(pTouch, pEvent);
    ccTouchEnded(pTouch, pEvent);
}

void CPtBattleArray::insertMoveCard(CCNode *inCard)
{
    insertMoveNode(m_pCardArray, this, inCard, CARDCOUNT-1);
}

void CPtBattleArray::save()
{
    
    CCLog("save()");
    save(SinglePlayer::instance()->m_vvBattleArray.at(inTag-1));
    // save data:
    int count = 0;
    for (int i = 0; i < CARDCOUNT; i++)
    {
        if (m_aSuitArray[i] != -1)
        {
            count++;
        }
        CCLog("i%d, %d", m_aSequenceArray[i], m_aSuitArray[i]);
    }
    int suit = getSuitResult(m_aSuitArray, m_aSequenceArray, count);
    int array[5]={0};
    CPtDisPlayCard * tmp = NULL;
    if (m_pCardArray[4] != NULL)
    {
        CPtDisPlayCard * tmp = dynamic_cast<CPtDisPlayCard*>(m_pCardArray[4]) ;
        array[4] =tmp->getCardData()->m_User_Card_ID;
    }
    int index = 0;
   
    for (int i = 3; i >= 0 ;i--)
    {
        tmp = NULL;
        tmp =  dynamic_cast<CPtDisPlayCard*>(m_pCardArray[i]) ;
        if (tmp)
        {
            array[index++] = tmp->getCardData()->m_User_Card_ID;
        }
    }
    
    char buffer[200]={0};
    sprintf(buffer, "sig=2ac2b1e302c46976beaab20a68ef95&troops=%d&card_color_id=%d&card_team_param={"
            ,inTag,suit);
            string std = buffer;
    bool flag = true;
    for (int i = 0; i < 5; i++)
    {
        CCLog("the index: %d", array[i]);
        if (array[i] != 0)
        {
            if (flag)
            {
                sprintf(buffer, "\"%d\":\"%d\"", i+1, array[i]);
                flag = false;
            }
            else
            {
                sprintf(buffer, ",\"%d\":\"%d\"", i+1, array[i]);
            }

            
            std = std + buffer;
        }
        
    }
    std = std+"}";
    
   
  //  ADDHTTPREQUESTPOSTDATA("http://cube.games.com/api.php?m=Card&a=saveCardTeam&uid=194", "helloworld1","ehll", std.c_str(), callfuncO_selector(CPtBattleArray::callBack));
   CCLog("suit: %d, %s, %s", suit, buffer, std.c_str());
}
void CPtBattleArray::callBack(CCObject *pSender)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "helloworld1");
 
    char * buffer = (char *)pSender;
    save(SinglePlayer::instance()->m_vvBattleArray.at(inTag-1));
    CCLog("callback: %s", buffer);
}

void CPtBattleArray::save(vector<CFightCard *> & infightArray)
{
    CGamesCard * tmp = dynamic_cast<CGamesCard*>(m_pCardArray[4]);
    if (tmp)
    {
         infightArray.at(4) = tmp->getCardData() ;
    }
    else
    {
        infightArray.at(4) = NULL;
    }
    
    int j = 0;
    for (int i =  3; i >= 0; i--)
    {
        if (m_pCardArray[i])
        {
            infightArray.at(j++) = dynamic_cast<CGamesCard*>(m_pCardArray[i])->getCardData();
        }
        else
        {
            infightArray.at(i) = NULL;
        }

    }
}

// implement class of CPtBatterArrayPanel

CPtBattleArrayPanel* CPtBattleArrayPanel::create(CCSize size, CCNode* container)
{
    
    CPtBattleArrayPanel* pRet = new CPtBattleArrayPanel();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->init();
        pRet->autorelease();
       
        
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;

}

CPtBattleArrayPanel::CPtBattleArrayPanel()
{
    
    gamePlayer = SinglePlayer::instance();
}


bool CPtBattleArrayPanel::init()
{
    
    CCLayer *layer = CCLayer::create();
    //CCLayerColor *layer = CCLayerColor::create(ccc4(125, 0, 0, 125));
    layer->ignoreAnchorPointForPosition(false);
    

  
    for (int i = 2; i >= 0; i--)
    {
        CPtBattleArray * battleArray = CPtBattleArray::create (gamePlayer->m_vvBattleArray.at(i) ,CCSizeMake(139, 200), ccp(175, 150) ,i+1);
        layer->addChild(battleArray);
        battleArray->setPosition(ccp(0, (2-i) * 520 + 85));
        m_pBatterArrays[i] = battleArray;
       // battleArray->initSize(CCSizeMake(139, 200), ccp(175, 150));
    }
    setContainer(layer);
    setDirection(kCCScrollViewDirectionVertical);
    setContentSize(CCSizeMake(534, 1560 +85));
    m_pContainer->setPosition(ccp(0, -1000));
    m_pCurrentBatterArray = m_pBatterArrays[0];
    return true;
}

CPtBattleArray * CPtBattleArrayPanel::getBattleArray(CCTouch *pTouch)
{
    CPtBattleArray * tmp ;
    for (int i = 0; i < BATTERARRAYCOUNT; i++)
    {
        tmp = m_pBatterArrays[i];
        if (tmp)
        {
            if(CPtTool::isInNode(tmp, pTouch))
            {
                return tmp;
            }
        }
    }
    return NULL;
}


// test:
static CCPoint prePoint;
bool CPtBattleArrayPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    prePoint = m_pContainer->getPosition();
    CCLog("logo");
    for (int i = 0; i < BATTERARRAYCOUNT ; i++)
    {
        CPtBattleArray * tmp =  m_pBatterArrays[i];
        if (tmp)
        {
            if (tmp->m_bTouchEnable)
            {
                return  false;
            }
        }
    }
    
    m_cBeginOffset = getContentOffset();
    
 return   CCScrollView::ccTouchBegan(pTouch, pEvent);
 
}
void CPtBattleArrayPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
}
void CPtBattleArrayPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    CCScrollView::ccTouchEnded(pTouch, pEvent);
    
    CCPoint m_EndOffset=getContentOffset();
//    if (m_cBeginOffset.equals(m_EndOffset))
//    {
//       // onclick:
//        return ;
//    }
    adjustScrollView(m_cBeginOffset,m_EndOffset);
    updatePanel();

    
    
}
void CPtBattleArrayPanel::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
}

CPtBattleArrayPanel::~CPtBattleArrayPanel()
{


}

/*
 * just the view poistion
 */

void CPtBattleArrayPanel::adjustScrollView(const CCPoint& begin, const CCPoint &end)
{
    CCLog("begin: %f, %f, end: %f, %f", begin.x, begin.y, end.x, end.y);
    
  //   int nPage = abs(begin.x / (int)m_CellSize.width);
    int nPage = abs((begin.y) / (int)500);
    int nAdjustPage = 0;
    int nDis = end.y-begin.y;

    if (nDis<-getViewSize().height/3)
    {
        nAdjustPage=nPage+1;
    }
    else if (nDis > getViewSize().height/3)
    {
        nAdjustPage=nPage-1;
    }
    else
    {
        nAdjustPage=nPage;
    }

    nAdjustPage=MIN(nAdjustPage,3-1);
    nAdjustPage=MAX(nAdjustPage,0);
    scrollToPage(nAdjustPage);
    CCLog("the page: %d, %d", nAdjustPage, nPage);
}



void CPtBattleArrayPanel::scrollToPage( int nPage )
{
    m_nCurrentPage = 2-nPage;
        // 关闭CCScrollView中的自调整
    unscheduleAllSelectors();
    CCPoint adjustPos=ccp(0,-500*nPage);
    // 调整位置
    setContentOffsetInDuration(adjustPos, 0.2f);
 
}




void CPtBattleArrayPanel::initData()
{
	// initData
	for (int i = 0; i < BATTERARRAYCOUNT; ++i)
	{
		m_pBatterArrays[i] = NULL;
		
	}
	m_nPlayerLeadership = 0;
	m_nPlayerFightPower = 0;
	m_nCurrentPage = -1;
	m_pCurrentBatterArray = NULL;

}



void CPtBattleArrayPanel::updatePanel()
{
	// refesh
    if (m_nCurrentPage <= 2 && m_nCurrentPage >=0)
    {
        m_pCurrentBatterArray = m_pBatterArrays[m_nCurrentPage];
    }
}