//
//  CBattleArrayLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-8.
//
//

#include "CBattleArrayLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "gamePlayer.h"
#include "CGamesCard.h"
#include "CPtTableItem.h"
#include "CPtBatterArray.h"
#include "CPtTool.h"

static CPtBattleArray * s_currentBattleArray = NULL;

static CCard cards[200];
static int indexCard = 0;
static CGamesCard* testCard();

CGamesCard* testCard()
{
    static bool flag = true;
    // should be delet:
    CCard *card = &cards[indexCard++];
    card->m_icard_id = 100000;
    card->m_scard_name = "卡牌1";
    card->m_ccard_next = 0;  ///背景底色
    card->m_sicard_star = rand()%6+1;
    card->m_icard_stirps = 1;   //种族
    card->m_icard_suit = rand()%52;      //随机数值
    card->m_icard_leadership = 4+rand()%2;
    card->m_icard_exp = 150;    //吃掉该卡牌的 经验
    card->m_icard_price = 1000;  //卖掉该卡佩的 价格
    card->m_icard_attack = 110+rand()%20; //攻击力
    card->m_icard_defend = 22+rand()%5; //防御力
    card->m_icardhp = 1056+rand()%100;    //卡牌的总的HP
    card->m_iusually_attack = 0;
    card->m_iskillLine = 0;
    card->m_iskillHelp = 0; //拥护技能
    card->m_iskillDead = 0; //死亡技能
    card->m_iskillBuff = 0; //增幅技能
    card->m_scard_tips = "card_tips_001"; //描述
    if (flag)
    {
        flag = false;
         card->m_scard_resources="peo.png";  //资源
    }else
    {
        flag = true;
        card->m_scard_resources="peo1.png";  //资源
    }
   
    card->m_scard_head="";  //头像
    card->m_scard_groud="bg1.png"; //
    card->m_scard_role="";
    
    CGamesCard * diplay = CGamesCard::Create(card);    

    return diplay;
}


// implement class of CPtBattleArrayItem:


bool CPtBattleArrayItem::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
 
 
    CCLog("CPtBattleArrayItem::ccTouchBegan");
    if(m_pDelegateLayer ==  NULL)
    {
        return false;
        
    }

    
    m_pDelegateLayer->m_bReplace = false;
    s_currentBattleArray = m_pDelegateLayer->panel->getBattleArray(pTouch);
;
    
    // new add  or replace:
    
    CGamesCard * displace =dynamic_cast<CGamesCard *>(this->getDisplayView());
    m_pDelegateLayer->m_pMoveCard = displace;

    if(displace)
    {
        if (CPtTool::isInNode(displace, pTouch))
        {
            // create new sprite:
            CCPoint worldPostion = displace->getPosition();
            worldPostion = displace->getParent()->convertToWorldSpace(worldPostion);
            m_pDelegateLayer->m_pMoveCard = displace->getCopy();
            displace->setDead();
            m_pDelegateLayer->m_pMoveCard->setAnchorPoint(CCPointZero);
            m_pDelegateLayer->m_pMoveCard->setPosition(worldPostion);
            m_pDelegateLayer->addChild(m_pDelegateLayer->m_pMoveCard, 6000, 100);
            m_pDelegateLayer->m_cPrePoint = m_pDelegateLayer->m_pMoveCard->getPosition();
            m_pDelegateLayer->m_nCurrentIndex = m_pDelegateLayer->m_nCount;
            return true;
        }
        
    }
    return false;

}


void CPtBattleArrayItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pDelegateLayer->m_pMoveCard)
    {
      m_pDelegateLayer->m_pMoveCard->retain();
     // m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
      
    }
    

    CPtBattleArray * battles =  s_currentBattleArray;  // m_pDelegateLayer->panel->getBattleArray(pTouch);
    s_currentBattleArray = NULL;
    if (battles)
    {
//        if (battles != s_currentBattleArray && s_currentBattleArray!=NULL)
//        {
//            s_currentBattleArray->updateBattleArray();
//        }
        
        // remove:
        
        if (m_pDelegateLayer->m_pMoveCard)
        {
            CCRect rect1 = m_pDelegateLayer->m_pMoveCard->boundingBox();
            rect1.origin = battles->getParent()->convertToNodeSpace(rect1.origin);
            CCRect rect =  battles->boundingBox();
            
            //rect.origin = this->convertToNodeSpace(rect.origin);
            
            if (
                !rect.intersectsRect(rect1))
            {
                m_pDelegateLayer->m_pMoveCard->release();
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                
                battles->updateBattleArray();
                return;
            }
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            
            
        }
      

        
        //replace and add:
        
        int replace = battles->getReplaceCard(m_pDelegateLayer->m_pMoveCard);
        if (replace != -1)
        {
            CCLog("the replace: %d", replace);
          
            battles->replaceCard(m_pDelegateLayer->m_pMoveCard, replace);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();
       
            return;
        }
        
        if (battles->isAssistantCard(m_pDelegateLayer->m_pMoveCard, false))
        {
            battles->addCard(m_pDelegateLayer->m_pMoveCard, 4);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();
            return;
        }
        
        int index = battles->getInsertIndex();
        CCLog("the index: %d", index);
        if (index != -1 )
        {
            
             battles->addCard(m_pDelegateLayer->m_pMoveCard, index);
             m_pDelegateLayer->m_pMoveCard->release();
        }
        
        battles->updateBattleArray();
        
    }else
    {
        m_pDelegateLayer->m_pMoveCard->release();
        m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
    }
    

}

//implement class of CBattleArrayLayer

bool CBattleArrayLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
       // load();
    initCBattleArray();

        bRet = true;
    } while (0);
    return bRet ;
}


void CBattleArrayLayer::initCards()
{
  
  //  vector<CFightCard *> & bag =  m_pGamePlayer->m_vCardBag;
    CCArray * array = CCArray::create();
    for (int i = 0; i < 200/* bag.size()*/; i++)
    {
      
        
  //      CGamesCard * card = CGamesCard::Create(bag.at(i)->m_pCard);
        
     CCSprite * tmp = testCard();//CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"card_template.png")); // //; //(CCSprite*)card->getChildByTag(100);
     //   tmp->initWithFile(CSTR_FILEPTAH(g_mapImagesPath,"fighting/card_res_001.png"));
        tmp->setAnchorPoint(CCPointZero);
     //   card->setContentSize(tmp->getContentSize());
      //  card->getChildByTag(20)->setPosition(80, 40);
      //  CCLog("the card: x, y: %f, %f", card->getPositionX(), card->getPositionY());
        if (tmp)//(card)
        {
            //CPtTableItem * item = CPtTableItem::create();
            CPtBattleArrayItem * item = CPtBattleArrayItem::create();
             item->setDisplayView(tmp);
             item->setTouchNode(tmp);
//            item->addChild(tmp, 0, 1);
//            item->setAnchorPoint(CCPointZero);
//            item->setContentSize(tmp->getContentSize());
            item->setDelegate(this);
            item->setDelegateLayer(this);
            array->addObject(item);
        }
    }
    
   
    m_pCards = CPtListViewWidget::create(array, CCSizeMake(476, 632), kCCScrollViewDirectionVertical , CCSizeMake(5, 2) , 3);
    dynamic_cast<TableView *>(m_pCards->getTableView())->setDelayMode(true);
    
    m_pCards->setAnchorPoint(CCPointZero);
    m_pCards->setPosition(ccp(567, 10.0f));
    CCSprite *bg_header = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardBg_header.png"));
    bg_header->setAnchorPoint(CCPointZero);
    bg_header->setPosition(ccp(557, 635));
    addChild(bg_header, 2);
    addChild(m_pCards,1);
  
}

void CBattleArrayLayer::initBatterArray()
{
    if (!m_pGamePlayer)
    {
        m_pGamePlayer = SinglePlayer::instance();
    }
    if (m_pGamePlayer)
    {
       //
    }
}


void CBattleArrayLayer::initCBattleArray()
{
    // init cardBag:
    initCards();
    
    // init left:
    panel = CPtBattleArrayPanel::create(CCSizeMake(534, 640));
    panel->setPosition(ccp(10,10));
    addChild(panel, 30);
}


bool CBattleArrayLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    s_currentBattleArray = NULL;
    return false;
}
void CBattleArrayLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // move the copy Card:
    CPtTool::drag(m_pMoveCard, pTouch);
    CPtBattleArray * battles = NULL;
    if (s_currentBattleArray)
    {
        battles =  s_currentBattleArray;
    }else
    {
        battles = panel->getCurrentArray();//getBattleArray(pTouch);
    }
   
    if (m_pMoveCard)
    {
        if (battles)
        {
            if (s_currentBattleArray== NULL)
            {
                s_currentBattleArray = battles;
            }
            
            battles->insertMoveCard(m_pMoveCard);
        }
    }

}
void CBattleArrayLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
     
}
void CBattleArrayLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}





void CBattleArrayLayer::load()
{
    m_pGamePlayer = SinglePlayer::instance();
    m_pGamePlayer->loadGamesConfig();
    m_pGamePlayer->loadServerCardBag();
    this->schedule(schedule_selector(CBattleArrayLayer::callback));
    
}
void CBattleArrayLayer::callback(float dt)
{
    if (m_pGamePlayer->isLoadCardBagEnd)
    {
        initCBattleArray();
        this->unschedule(schedule_selector(CBattleArrayLayer::callback));
    }
}

void CBattleArrayLayer::removeCallBack(CCNode *pNode)
{
    
  
}



