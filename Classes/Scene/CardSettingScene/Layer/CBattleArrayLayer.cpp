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
#include "CCardEnhanceLayer.h"
#include "CCardSettingScene.h"
static CPtBattleArray * s_currentBattleArray = NULL;

// implement class of CPtBattleArrayItem:



bool CPtBattleArrayItem::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
 
 
    CCLog("CPtBattleArrayItem::ccTouchBegan");
    if(m_pDelegateLayer ==  NULL)
    {
        return false;
        
    }
    

    
    if (m_pDelegateLayer->getTableClickEnable() == false)
    {
        m_pDelegateLayer->m_bReplace = false;
        s_currentBattleArray = m_pDelegateLayer->panel->getBattleArray(pTouch);
    }
      
    // new add  or replace:
    
    CPtDisPlayCard * displace =dynamic_cast<CPtDisPlayCard *>(this->getDisplayView());
    m_pDelegateLayer->m_pMoveCard = displace;

    if(displace)
    {
        if (CPtTool::isInNode(displace, pTouch))
        {
           
            // onclik mode:
            if (m_pDelegateLayer->getTableClickEnable())
            {
                m_pDelegateLayer->setTableClickMove(false);
                return true;
            }
            
            
            if (displace->getCardData()->getInBattleArray())
            {
                //
                CCLog("it's in battleArray");
                return false;
            }
            // ondrag mode:
            // create new sprite:
            CCPoint worldPostion = displace->getPosition();
            worldPostion = displace->getParent()->convertToWorldSpace(worldPostion);
            m_pDelegateLayer->m_pMoveCard = displace->getCopy();
            displace->setDead();
            m_pDelegateLayer->m_pMoveCard->setAnchorPoint(CCPointZero);
            m_pDelegateLayer->m_pMoveCard->setPosition(worldPostion);
            m_pDelegateLayer->addChild(m_pDelegateLayer->m_pMoveCard, 6000, 100);
            m_pDelegateLayer->m_cPrePoint = m_pDelegateLayer->m_pMoveCard->getPosition();
            dynamic_cast<CPtDisPlayCard*>(m_pDelegateLayer->m_pMoveCard)->setInCardBagPointer(displace);
            return true;
        }
        
    }
    return false;

}


void CPtBattleArrayItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pDelegateLayer)
    {
        CCLog("onTableClick");
        // onclick:
        if (m_pDelegateLayer->m_pEnhancePanel && m_pDelegateLayer->getTableClickEnable())
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
                return;
            }
            int i = m_pDelegateLayer->m_pEnhancePanel->getEmptyCardSlide();
            if (i != -1)
            {
                CGamesCard * displace =dynamic_cast<CGamesCard *>(this->getDisplayView());
              
                
                if(displace)
                {
                    if (CPtTool::isInNode(displace, pTouch))
                    {
                        
                                            
                        
                        if (displace->getCardData()->getInBattleArray())
                        {
                            //
                            CCLog("it's in battleArray");
                            
                        }
                        // ondrag mode:
                        // create new sprite:
                        
                        CGamesCard *node = displace->getCopy();
                        m_pDelegateLayer->m_pEnhancePanel->addCard(i, node);

                    }
                    
                }

                
            }
            return;
        }
      
    }
    
    
    
    if (m_pDelegateLayer->m_pMoveCard)
    {
      m_pDelegateLayer->m_pMoveCard->retain();
      
    }
    

    CPtBattleArray * battles =  s_currentBattleArray; 
    s_currentBattleArray = NULL;
    if (battles)
    {

        
        // remove:
        
        if (m_pDelegateLayer->m_pMoveCard)
        {
            CCRect rect1 = m_pDelegateLayer->m_pMoveCard->boundingBox();
            rect1.origin = battles->getParent()->convertToNodeSpace(rect1.origin);
            CCRect rect =  battles->boundingBox();
            
            
            if (!rect.intersectsRect(rect1))
            {
                dynamic_cast<CPtDisPlayCard*>(m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();
                m_pDelegateLayer->m_pMoveCard->release();
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                
                battles->updateBattleArray();
                return;
            }
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            
            
        }
      

        
        //replace and add:
        
        int replace = battles->getReplaceCard(m_pDelegateLayer->m_pMoveCard);
        int index = battles->getInsertIndex();
        if (replace != -1)
        {
            CCLog("the replace: %d", replace);
          
            battles->replaceCard(m_pDelegateLayer->m_pMoveCard, replace);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();
    
        }else if (battles->isAssistantCard(m_pDelegateLayer->m_pMoveCard, false))
        {
            battles->addCard(m_pDelegateLayer->m_pMoveCard, 4);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();

        }
        else if (index != -1 )
        {
            
             battles->addCard(m_pDelegateLayer->m_pMoveCard, index);
             m_pDelegateLayer->m_pMoveCard->release();
        }else
        {
            dynamic_cast<CPtDisPlayCard*>(m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();
         //   dynamic_cast<CPtDisPlayCard*>(m_pDelegateLayer->m_pMoveCard)->getCardData()->setInCardBagPoint(NULL);
            m_pDelegateLayer->m_pMoveCard->release();
        }
        
        battles->updateBattleArray();
        
    }else
    {
        dynamic_cast<CPtDisPlayCard*>(m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();

        m_pDelegateLayer->m_pMoveCard->release();
        m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
       
    }
    

}

void CPtBattleArrayItem::handlerEnhanceOnCliked()
{
    if (m_pDelegateLayer && m_pDelegateLayer->m_pEnhancePanel)
    {
        
    }
}

//implement class of CBattleArrayLayer

CBattleArrayLayer::CBattleArrayLayer()
{

}
CBattleArrayLayer::~CBattleArrayLayer()
{
    
}

bool CBattleArrayLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());

        initCards();
        bRet = true;
    } while (0);
    return bRet ;
}


void CBattleArrayLayer::initCards()
{
    m_bTableClikEnable = false;
    m_pGamePlayer = SinglePlayer::instance();
    vector<CFightCard *> & bag =  m_pGamePlayer->m_vCardBag;
    CCArray * array = CCArray::create();
    for (int i = 0; i < bag.size() ; i++)
    {
        CCLog("i: %d", i);
        // change cardData:
        CFightCard * cardData = bag.at(i);
    
//        if (cardData && cardData->m_pCard && cardData->m_pCard->m_icard_suit== 0)
//        {
//            CCLog("reset resource: %d , %d", cardData->m_User_Card_ID, cardData->m_pCard->m_icard_id);
//            // test: change the tmp resource value:
//            cardData->m_pCard->m_icard_suit= cardData->m_iSuit;
//        //    cardData->m_pCard->m_sicard_star = rand()%7 +1;
//            cardData->m_pCard-> m_icard_stirps = rand()%5 +1;   //种族_
//            cardData->m_pCard->m_scard_head="";  //头像
//            cardData->m_pCard->m_scard_ground="bg1.png"; //
//            cardData->m_pCard->m_scard_role="";
//            if ((rand()%200)%2 == 0)
//            {
//                cardData->m_pCard->m_scard_resources="peo.png";
//
//            }else
//            {
//                 cardData->m_pCard->m_scard_resources="peo1.png";
//            }
//        }

        CCLog("reset resource: %d , %d", cardData->m_User_Card_ID, cardData->m_iSuit);
        
     
        CGamesCard * card = CPtDisPlayCard::Create(cardData);
        card->setAnchorPoint(CCPointZero);
        if (card)//(card)
        {
            if (cardData->getInBattleArray())
            {
                  card->setDead();
             //   cardData->setInCardBagPoint(card);
                if (CCardSettingScene::s_pBattleArrayCards)
                {
                    CCardSettingScene::s_pBattleArrayCards->setObject(card, cardData->m_User_Card_ID);
                }
            }
         //

            CPtBattleArrayItem * item = CPtBattleArrayItem::create();
            item->setDisplayView(card);
            item->setTouchNode(card);
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
    
    //
    vector<vector<CFightCard *> > &tmpFightArray = m_pGamePlayer->m_vvBattleArray;
    for (int i = 0; i < tmpFightArray.size(); i++)
    {
        vector<CFightCard * > &tmpFights = tmpFightArray.at(i);
        for (int j = 0; j < tmpFights.size(); j++)
        {
            if (tmpFights.at(j) == NULL)
            {
                CCLog("the fightArray: %d, %d is NULL",i,j);
            }
        
        }
        CCLog("the fightArray: size %d, %d",i,tmpFights.size());
    }
}



bool CBattleArrayLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    s_currentBattleArray = NULL;
    return false;
}
void CBattleArrayLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // onclik mode:
    if (m_bTableClikEnable)
    {
        setTableClickMove(true);
        return;
    }
    
    // ondrag mode:
    // move the copy Card:
    CPtTool::drag(m_pMoveCard, pTouch);
    CPtBattleArray * battles = NULL;
    if (s_currentBattleArray)
    {
        battles =  s_currentBattleArray;
    }else
    {
        battles = panel->getCurrentArray();
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



void CBattleArrayLayer::removeCallBack(CCNode *pNode)
{
    
  
}

void CBattleArrayLayer::addTeamArrayPanel()
{
    panel = CPtBattleArrayPanel::create(CCSizeMake(534, 640));
    panel->setPosition(ccp(10,10));
    addChild(panel, 30, 4000);
    dynamic_cast<TableView *>(m_pCards->getTableView())->setDelayMode(true);
}

void CBattleArrayLayer::addEnhance()
{
    m_pEnhancePanel = CCardEnhanceLayer::create();
    addChild(m_pEnhancePanel, 1, 4000);
    dynamic_cast<TableView *>(m_pCards->getTableView())->setDelayMode(false);

}


void CBattleArrayLayer::removeLeft()
{
    if (panel)
    {
        panel->resetBattleArrays();
    }
    removeChildByTag(4000, true);
    panel = NULL;
    m_pEnhancePanel = NULL;
}



