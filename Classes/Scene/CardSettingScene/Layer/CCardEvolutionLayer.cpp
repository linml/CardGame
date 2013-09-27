//
//  CCardEvolutionLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-1.
//
//

#include "CCardEvolutionLayer.h"
#include "gamePlayer.h"
#include "CPtTool.h"
#include "PtHttpClient.h"
#include "CSaveConfirmLayer.h"
#include "PtJsonUtility.h"
#include "PtHttpURL.h"
CCardEvolutionLayer::CCardEvolutionLayer()
{
    m_bPropEnough = false;
    m_cMaps = NULL;
    m_pSaveButton = NULL;
    m_pSrcCard = NULL;
    m_pDesCard = NULL;
    m_pMapLayer[0] = NULL;
    m_pMapLayer[1] = NULL;
    m_pPlayer = SinglePlayer::instance();
    m_pStarData = SingleStarConfigData::instance();
    m_pPropConfigData = SinglePropConfigData::instance();
    m_nTouchTag = -1;
    m_nCostCoin = 0;
    m_nAddAtk = 0;
    m_nAddRvc = 0;
    m_nAddDef = 0;
    m_nAddHp = 0;
    for (int i = 0; i < 5; i++)
    {
        label[i] = NULL;
        m_pPropBg[i] = NULL;
    }

}
CCardEvolutionLayer::~CCardEvolutionLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    removeCard();
}

bool CCardEvolutionLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initEvolution();
        bRet = true;
    } while (0);
    return bRet;
}
bool CCardEvolutionLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pSrcCard, pTouch))
    {
        m_nTouchTag = 1;
        return true;
    }
    if (CPtTool::isInNode(m_pSaveButton, pTouch))
    {
        m_nTouchTag = 2;
        CCPoint point = m_pSaveButton->getAnchorPoint();
        CCTexture2D * pressed = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_pressed.png"));
        m_pSaveButton->initWithTexture(pressed);
        m_pSaveButton->setAnchorPoint(point);
        return true;
    }
    m_nTouchTag = -1;
    return false;
}
void CCardEvolutionLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CCardEvolutionLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_nTouchTag == 2)
    {
        CCPoint point = m_pSaveButton->getAnchorPoint();
        CCTexture2D * noraml = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_normal.png"));
        m_pSaveButton->initWithTexture(noraml);
        m_pSaveButton->setAnchorPoint(point);
       
    }
    handlerTouch(pTouch);
    
}
void CCardEvolutionLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
}

void CCardEvolutionLayer:: initEvolution()
{
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setScaleY(1.4f);
    
    addChild(bg, -1);
    bg->setPosition(ccp(10, 10));
    bg->setAnchorPoint(CCPointZero);
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "jinhua.plist"));
    m_pSaveButton =(CCSprite*) m_cMaps->getElementByTags("1,0,0");
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    
    int array[4]={1,0,1,4002};
    // 1,0, 164;
    for (int i = 0; i < 4; i++)
    {
        array[2] = 1;
        array[3] = 4002+i;
        label[i] = ((CCLabelTTF*)(m_cMaps->getElementByTags(array, 4)));
        array[2] = 164+i;
        m_pPropBg[i] = ((CCSprite*)m_cMaps->getElementByTags(array, 3));
    }
    
    array[2] = 168;
    m_pPropBg[4] = ((CCSprite*)m_cMaps->getElementByTags(array, 3));

    array[3]= 4006;
    array[2]=162;
    label[4] = ((CCLabelTTF*)(m_cMaps->getElementByTags(array, 4)));
    
    array[2]=137;
    for (int i = 0; i < 2; i++)
    {
        array[1]= i;
        m_aPoint[i] = ((CCSprite*)(m_cMaps->getElementByTags(array, 3)))->getPosition();
        m_pMapLayer[i] = ((CCLayer*)(m_cMaps->getElementByTags(array, 2)));
        CCLog("%d, x: %f, y: %f", m_aPoint[i].x, m_aPoint[i].y);
    }
   
    
    
}
void CCardEvolutionLayer::handlerTouch(CCTouch * pTouch)
{
    if (CPtTool::isInNode(m_pSrcCard, pTouch) && m_nTouchTag == 1)
    {
        removeCard();
        updateEvolution();
        return;
    }
    if (CPtTool::isInNode(m_pSaveButton, pTouch) && m_nTouchTag == 2)
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        saveOnClick();
    }
    
}

void CCardEvolutionLayer::updateEvolution()
{
    updateData();
    updateTexture();
    
}
void CCardEvolutionLayer:: updateData()
{
    // update data:
    if (m_pSrcCard && m_pDesCard)
    {
        m_nCostCoin = m_pStarData->getConstConin(m_pSrcCard->getCardData()->m_pCard->m_icard_id);
        m_nAddRvc =m_pDesCard->getCardData()->m_pCard->m_icard_leadership - m_pSrcCard->getCardData()->m_pCard->m_icard_leadership ;
        m_nAddAtk =m_pDesCard->getCardData()->m_attack - m_pSrcCard->getCardData()->m_attack ;
        m_nAddDef =m_pDesCard->getCardData()->m_defend - m_pSrcCard->getCardData()->m_defend ;
        m_nAddHp =m_pDesCard->getCardData()->m_iHp - m_pSrcCard->getCardData()->m_iHp ;
    }
    else
    {
        m_nCostCoin = 0;
        m_nAddAtk = 0;
        m_nAddRvc = 0;
        m_nAddDef = 0;
        m_nAddHp = 0;
    }
    
}
void CCardEvolutionLayer:: updateTexture()
{
    clearProps();
    if (m_pSrcCard == NULL || m_pDesCard == NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            label[i]->setString("");
        }
        return;
    }
    
    char buff[30]={0};
    // update view
    // conin:
    if (m_nCostCoin == 0)
    {
        label[4]->setString("");
    }else
    {
        sprintf(buff, "%d", m_nCostCoin);
        label[4]->setString(buff);
    }
    
    // atak
    sprintf(buff, "ATK : %d (+%d)", m_pSrcCard->getCardData()->m_attack, m_nAddAtk);
    label[0]->setString(buff);
    // def:
    sprintf(buff, "DEf : %d (+%d)", m_pSrcCard->getCardData()->m_defend, m_nAddDef);
    label[1]->setString(buff);
    // hp:
    sprintf(buff, "HP : %d (+%d)", m_pSrcCard->getCardData()->m_iHp, m_nAddHp);
    label[2]->setString(buff);
    // rvc:
    sprintf(buff, "RVC : %d (+%d)", m_pSrcCard->getCardData()->m_pCard->m_icard_leadership, m_nAddRvc);
    label[3]->setString(buff);
    
    // update prop icon:
    
    CCArray * array = SingleStarConfigData::instance()->getPropArrays(m_pSrcCard->getCardData()->m_pCard->m_icard_id);
    if (array)
    {
        
        m_bPropEnough = true;
        int hasCount = 0;
        char buff[10] = {0};
        PropItem * item = NULL;
        CCLabelTTF *tip = NULL;
        for (int i = 0; i < array->count(); i++)
        {
            item = (PropItem*)array->objectAtIndex(i);
            if(m_pPropConfigData->getPropDataById(item->propId))
            {
                hasCount = m_pPlayer->getPropCountFromBag(item->propId);
                sprintf(buff, "%d/%d", hasCount, item->propCount);
                tip = CCLabelTTF::create(buff, "arial", 11);
                if (hasCount < item->propCount)
                {
                    m_bPropEnough = false;
                    tip->setColor(ccRED);
                }else
                {
                    tip->setColor(ccGREEN);
                }
                tip->setPosition(ccp(5,5));
                
                
                
                CCLog("%s", m_pPropConfigData->getIconName().c_str());
                CCSprite * sprite = CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, m_pPropConfigData->getIconName().c_str()));
                if (sprite)
                {
                    sprite->setAnchorPoint(CCPointZero);
                    m_pPropBg[i]->addChild(sprite);
                    m_pPropBg[i]->addChild(tip);
                }
                
            }
            
        }

    }else
    {
        CCLog("no props array:");
    }
        
}

void CCardEvolutionLayer::clearProps()
{
    m_bPropEnough = false;
    for (int i = 0 ; i < 5; i++)
    {
        if (m_pPropBg[i])
        {
            m_pPropBg[i]->removeAllChildrenWithCleanup(true);
        }
    }
    
}

void CCardEvolutionLayer:: save()
{
    saveData();
    // change the fight
    if (m_pSrcCard && m_pDesCard && m_pFightCard)
    {
    
        int index = m_pSrcCard->getCardData()->getInWhichBattleArray()-1;
        if (index >= 0)
        {
            //save card in BattleArray:
        int t = m_pSrcCard->getCardData()->m_User_Card_ID;
        vector<CFightCard *> &tmpVect = m_pPlayer->getCardBattleArray().at(index);
             for (int i = 0; i< tmpVect.size(); i++)
             {
                 CCLog("user id: %d",tmpVect.at(i)->m_User_Card_ID );
                 if (tmpVect.at(i) && tmpVect.at(i)->m_User_Card_ID == t)
                 {
                     tmpVect.at(i)->updateFight(m_pFightCard->m_pCard, m_pFightCard->m_iCurrLevel);
                     break;
                 }
            }
      
        }
      
        //
        m_pSrcCard->updateCard(m_pFightCard);
        m_pSrcCard->getInCardBagPointer()->updateCard(m_pFightCard);
        if (m_pSrcCard->getCardData()->getInWhichBattleArray() != 0 || m_pFightCard->getEnConsume())
        {
            m_pSrcCard->getInCardBagPointer()->setDead();
        }
       
        //removeCard();
    }
    
}

void CCardEvolutionLayer::saveData()
{
    // sub: coin：
    CCLog("the sub before: %d", m_pPlayer->getCoin());
    m_pPlayer->subCoin(m_nCostCoin);
    CCLog("the sub after: %d", m_pPlayer->getCoin());
    // sub prop:
    CCArray * array = SingleStarConfigData::instance()->getPropArrays(m_pSrcCard->getCardData()->m_pCard->m_icard_id);
    if (array)
    {
        PropItem *propItem = NULL;
        for (int i = 0; i < array->count(); i++)
        {
            propItem = (PropItem*) array->objectAtIndex(i);
            m_pPlayer->subProp(propItem->propId, propItem->propCount);
        }
    }
    
}

void CCardEvolutionLayer::saveOnClick()
{
    CSaveConfirmLayer * layer = CSaveConfirmLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000, 2000);
    int cardItemId = 0;
    int cardGroup = 0;
    if (m_pSrcCard)
    {
        
        
        // 条件判定：
        /*
         * 1. 金币
         * 2. 道具
         * 3. 领导力
         * 4. 顶级
         */
        int index = m_pSrcCard->getCardData()->m_pCard->m_sicard_star;
        index--;
        int needLevel = g_aMaxLevel[index];
        
        if (m_pPlayer->getCoin() < m_nCostCoin)
        {
            // 金币不足
            layer->setResultCode(6);
            return;
        }
        else if(m_pSrcCard->getCardData()->m_pCard->m_ccard_next == 0)
        {
            
            //顶级
            layer->setResultCode(7);
            return;
        }
        else if(m_pSrcCard->getCardData()->m_iCurrLevel != needLevel)
        {
            layer->setResultCode(8);
            //级别不够
            return;
        }else if(m_bPropEnough == false)
        {
            layer->setResultCode(11);
        }

        // send message to server:
        //    api.php?m=Card&a=cardUpGrade&uid=194(用户ID)
        //    参数 ：
        //	sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&& tpye= 1(进化)&info="{"card_id"(当前进化卡牌id跟阵容ID(cardItemId_troop)，例1001_1):"","other":""}"(进化信息)
        
        cardItemId = m_pSrcCard->getCardData()->m_User_Card_ID;
        cardGroup = m_pSrcCard->getCardData()->getInWhichBattleArray();

    
        char buff[500]={0};
//xianbei modify        sprintf(buff, "sig=2ac2b1e302c46976beaab20a68ef95&&type=1&info=\"{\"card_id\":\"%d_%d\",\"other\":\"\"}",cardItemId, cardGroup);
        sprintf(buff, "sig=%s&&type=1&info=\"{\"card_id\":\"%d_%d\",\"other\":\"\"}",STR_USER_SIG, cardItemId, cardGroup);
    
        CCLog("%s",buff);
    
        ADDHTTPREQUESTPOSTDATA(STR_URL_UPGRADE_CARD(194),"cardEvolution","evolution", buff, callfuncO_selector(CCardEvolutionLayer::receiveCallBack));
      //  #define STR_URL_UPGRADE_CARD(UID)   URL_FACTORY("api.php?m=Card&a=cardUpGrade&uid=",UID)
      
    }else
    {
        layer->setResultCode(10);
    }

}
void  CCardEvolutionLayer::receiveCallBack(CCObject *pSender)
{
    // get replay from server: error or success:
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "cardEvolution");
    char *buff = (char*) pSender;
    CSaveConfirmLayer * layer = (CSaveConfirmLayer*) CCDirector::sharedDirector()->getRunningScene()->getChildByTag(2000);
    if (!buff)
    {
        layer->setResultCode(200);
        return;
    }
    CCDictionary* dic = PtJsonUtility::JsonStringParse(buff);
    
    int result = GameTools::intForKey("code", dic);
    
    layer->setResultCode(result);
    if (result == 0)
    {
        save();
    }else
    {
        CCLog("error");
    }
    
    
    delete[] buff;
}

/*
 * @breif: if m_pSrc == null ,add or, replace:
 */
void CCardEvolutionLayer::addCard(CPtDisPlayCard *inCard)
{
    if (inCard)
    {
        if (m_pSrcCard)
        {

            // remove:
            removeCard();
            // test:
        
        }
        // src card:
        m_pSrcCard = inCard;
        inCard->setPosition(m_aPoint[0]);
        inCard->setAnchorPoint(CCPointZero);
        m_pMapLayer[0]->addChild(inCard);
        int level = inCard->getCardData()->m_iCurrLevel;
        // des card:
        int nextCardId = inCard->getCardData()->m_pCard->m_ccard_next;
        CCard * nextCardData =SinglePlayer::instance()->getCardByCardId(nextCardId);
        if (nextCardData)
        {
            // if not evloution should delete fightCard:!!!
            m_pFightCard = new CFightCard(nextCardData,level);
            m_pFightCard->setInBattleArray(m_pSrcCard->getCardData()->getInWhichBattleArray());
            m_pFightCard->setEnConsume(true);
            m_pFightCard->m_iSuit = m_pSrcCard->getCardData()->m_iSuit;
            
            CPtDisPlayCard * card = CPtDisPlayCard::Create(m_pFightCard);
            card->setAnchorPoint(CCPointZero);
            card->setPosition(m_aPoint[1]);
            m_pMapLayer[1]->addChild(card);
            m_pDesCard = card;
            
           
        }else
        {
            CCLog("error.. cardevolution not has next card id");
        }
    }
    
    updateEvolution();
}
void CCardEvolutionLayer::removeCard()
{
    // remove
    if (m_pSrcCard == NULL)
    {
        return;
    }
    m_pSrcCard->getCardData()->setEnConsume(false);
    if (m_pSrcCard->getCardData()->getInWhichBattleArray() == 0)
    {
        m_pSrcCard->getInCardBagPointer()->setLive();

    }
    
    m_pSrcCard->removeFromParentAndCleanup(true);
    if (m_pDesCard)
    {
        m_pDesCard->removeFromParentAndCleanup(true);
        // deletd des: carddata:
        if (m_pFightCard)
        {
            delete m_pFightCard;
            m_pFightCard = NULL;
        }
      
    }
    m_pSrcCard = NULL;
    m_pDesCard = NULL;
  
}

