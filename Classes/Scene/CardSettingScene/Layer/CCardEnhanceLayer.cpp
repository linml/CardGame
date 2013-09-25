

//
//  CCardEvolutionLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-23.
//
//

#include "CCardEnhanceLayer.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "CPtLevelConfigData.h"
#include "gamePlayer.h"
#include "PtJsonUtility.h"
#include "PtHttpClient.h"
#include "CSaveConfirmLayer.h"
#include "PtHttpURL.h"
#include "CBattleArrayLayer.h"
CCardEnhanceLayer::CCardEnhanceLayer()
{
    m_pSelectCard = NULL;
    m_nCurrentExp = 0;
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        m_pMaterialCards[i] = NULL;
    }
    m_nAddAtak = 0;
    m_nAddDef = 0;
    m_nAddHp = 0;
    m_nAddRvc = 0;
    m_nCurrentLevel = -1;
    m_pLevelConfig = SingleLevleConfigData::instance();
    m_nCostConin = 0;
    m_pPlayer = SinglePlayer::instance();
    m_pCardBag = NULL;
   
}

CCardEnhanceLayer::~CCardEnhanceLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    resetMaterialCard();
}

bool CCardEnhanceLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCCardEvolution();
        bRet = true;
    } while (0);
    return bRet;
}

bool CCardEnhanceLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    CCLog("CCardEnhanceLayer::ccTouchBegan");
    if(m_pSelectCard && CPtTool::isInNode(m_pSelectCard, pTouch))
    {
        m_nTouchTag = 2;
        return true;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CCNode * tmp = m_pMaterialCards[i];
        if (tmp && CPtTool::isInNode(tmp, pTouch))
        {
            m_nTouchTag = 3;
            return true;
        }
    }
    CCNode * node = m_cMaps->getElementByTags("1,0,0");
   
    if (node&& CPtTool::isInNode(node, pTouch))
    {
        m_nTouchTag = 1;
        CCSprite *m_pSaveButton = (CCSprite*)node;
        CCPoint point = m_pSaveButton->getAnchorPoint();
        CCTexture2D * pressed = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_pressed.png"));
        m_pSaveButton->initWithTexture(pressed);
        m_pSaveButton->setAnchorPoint(point);
        return true;
    }

    return  false;
}
void CCardEnhanceLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
        
}
void CCardEnhanceLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag == 1)
    {
        CCNode * node = m_cMaps->getElementByTags("1,0,0");
        CCSprite *m_pSaveButton = (CCSprite*)node;
        CCPoint point = m_pSaveButton->getAnchorPoint();
        CCTexture2D * pressed = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_normal.png"));
        m_pSaveButton->initWithTexture(pressed);
        m_pSaveButton->setAnchorPoint(point);
        
    }
    handlerTouch(pTouch);
    
}
void CCardEnhanceLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void CCardEnhanceLayer::initCCardEvolution()
{
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setScaleY(1.4f);
    
    addChild(bg, -1);
    bg->setPosition(ccp(10, 10));
    bg->setAnchorPoint(CCPointZero);
  
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "qianghua.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    m_cMaps->getTouchRects(m_cTouches);
    
    
    updateTexture();
}

void CCardEnhanceLayer::handlerTouch(CCTouch * pTouch)
{
    CCLog("CCardEnhanceLayer::ccTouchBegan");
    if(m_pSelectCard && CPtTool::isInNode(m_pSelectCard, pTouch))
    {
        removeCard(-2);
        return;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CCNode * tmp = m_pMaterialCards[i];
        if (tmp && CPtTool::isInNode(tmp, pTouch))
        {
            removeCard(i);
            return;
        }
    }
    
    CCNode * node = m_cMaps->getElementByTags("1,0,0");
    if (node&& CPtTool::isInNode(node, pTouch))
    {
        CCLog("save");
       // save();
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        saveOnClick();
    }

}

int CCardEnhanceLayer::getEmptyCardSlide()
{
    if (m_pSelectCard == NULL)
    {
        return  -2;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i] == NULL)
        {
            return  i;
        }
    }
    return  -1;
}
void CCardEnhanceLayer::addCard(const int& inIndex, CPtDisPlayCard * inCard)
{
    
    
    if (inCard != NULL )
    {
        if ((inIndex >= 0 && inIndex < MATERIALCARDCOUNT) || inIndex == -2)
        {
            if (inIndex == -2)
            {
                if (inCard->getCardData()->getInWhichBattleArray() == 0)
                {
                    inCard->getInCardBagPointer()->setDead();
                }
                addCardAction(inIndex, inCard);
                m_pSelectCard = inCard;
                m_nCurrentLevel = inCard->getCardData()->m_iCurrLevel;
             
            }else if(m_pMaterialCards[inIndex] == NULL)
            {
                               
                // data change:
                m_pMaterialCards[inIndex] = inCard;
                inCard->getInCardBagPointer()->setDead();
                inCard->getCardData()->setEnConsume(true);
                // action:
                addCardAction(inIndex, inCard);
                
            }
            updateEnhance();
        }
    
    }
}

void CCardEnhanceLayer::removeCard(const int &inIndex)
{
    if (inIndex == -2)
    {
        if (m_pSelectCard && m_pSelectCard->getCardData()->getInWhichBattleArray() == 0)
        {
            m_pSelectCard->getInCardBagPointer()->setLive();
        }
    
        removeCardAction(inIndex);
        m_pSelectCard = NULL;
        m_nCurrentLevel = -1;
    
    }
    else if(inIndex >= 0 && inIndex < MATERIALCARDCOUNT && m_pMaterialCards[inIndex])
    {
        if (m_pMaterialCards[inIndex]->getCardData()->getInWhichBattleArray() == 0)
        {
            m_pMaterialCards[inIndex]->getInCardBagPointer()->setLive();
        }
        m_pMaterialCards[inIndex]->getCardData()->setEnConsume(false);

        removeCardAction(inIndex);
        m_pMaterialCards[inIndex] = NULL;
    }
    updateEnhance();
}
bool CCardEnhanceLayer::canClickCard(CPtDisPlayCard * inCard)
{
    bool bRet = true;
    if (m_pSelectCard)
    {
        if (m_pSelectCard->getCardData()->m_User_Card_ID == inCard->getCardData()->m_User_Card_ID)
        {
            bRet =  false;
        }
        if (inCard->getCardData()->getInWhichBattleArray() != 0)
        {
           bRet = false;
        }

    }

    
    return bRet;
}

void CCardEnhanceLayer::addCardAction(const int& inIndex, CPtDisPlayCard * inCard)
{
    if (inIndex == -2)
    {
        inCard->retain();
        inCard->removeFromParentAndCleanup(true);
        inCard->setPosition(ccp(10, 10));
        inCard->setAnchorPoint(CCPointZero);
        m_cMaps->getElementByTags("1,0, 137")-> addChild(inCard);
        inCard->release();
        return;
    }
    
    int i = (6+inIndex)%8 + 18;
    int array[3] = {1,0,0};
    array[2] = i;
    CCNode * node = m_cMaps->getElementByTags(array,3);
    if (node)
    {
        inCard->retain();
        inCard->removeFromParentAndCleanup(true);
        CCPoint point = node->boundingBox().origin;
        point.x += 3;
        point.y += 4;
        inCard->setPosition(point);
        inCard->setAnchorPoint(CCPointZero);
        inCard->setScale(0.71f);
        addChild(inCard, 22, 8000+inIndex);
        inCard->release();
    }
    
}
void CCardEnhanceLayer::removeCardAction(const int& inIndex)
{
    if (inIndex == -2)
    {
        m_cMaps->getElementByTags("1,0, 137")->removeChild(m_pSelectCard, true);
        return;
    }
    else if(inIndex >= 0 && inIndex < MATERIALCARDCOUNT)
    {
        removeChildByTag(8000+inIndex, true);
    }
        
}

void CCardEnhanceLayer::updateEnhance()
{
    updateData();
    updateTexture();
}
void CCardEnhanceLayer::updateData()
{

    m_nCurrentExp = 0;
    m_nAddAtak  = 0;
    m_nAddDef  = 0;
    m_nAddHp = 0;
    m_nAddRvc = 0;
    m_nCostConin = 0;
    // get suport exp:
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CGamesCard * tmp = m_pMaterialCards[i];
        if (tmp)
        {
            m_nCurrentExp += tmp->getCardData()->getSupportValue(1);
            m_nCostConin += tmp->getCardData()->getCostCoin();
        }
    }
    
    
    if (m_pSelectCard)
    {
        int  exp = m_pSelectCard->getCardData()->m_iCurrExp;
        exp += m_nCurrentExp;
        int currentLevel = m_pSelectCard->getCardData()->m_iCurrLevel;
        int level = m_pLevelConfig->getCurrentLevel(exp,m_pSelectCard->getCardData()->m_pCard->m_sicard_star ,currentLevel);
        level = level >= m_pSelectCard->getCardData()->m_pCard->m_ileve_max ? m_pSelectCard->getCardData()->m_pCard->m_ileve_max:level;
        CCLog("current-----: %d, level: %d, currentLevel: %d", exp, level, m_nCurrentLevel);
        if (level >= currentLevel )
        {
            CCLog("can qianghua");
            CFightCard * tmp = m_pSelectCard->getCardData();
            m_nAddAtak = tmp->getAddValue(level, 1) - tmp->m_attack;
            m_nAddDef =tmp->getAddValue(level, 2)- tmp->m_defend;
            m_nAddHp  = tmp->getAddValue(level, 3) - tmp->m_iHp;
            m_nAddRvc = tmp->getAddValue(level, 4) - 0;
            m_nCurrentLevel = level;
            
            // cclog
            CCLog("%d %d", tmp->getAddValue(level, 1), tmp->m_attack);
        }
       
       
    }else
    {
        
        m_nAddAtak  = 0;
        m_nAddDef  = 0;
        m_nAddHp = 0;
        m_nAddRvc = 0;
        m_nCostConin = 0;
    }

}
void CCardEnhanceLayer::updateTexture()
{
    int maxLevel = 0;
    if (m_pSelectCard)
    {
        m_nCurrentAtk = m_pSelectCard->getCardData()->m_attack;
        m_nCurrentDef = m_pSelectCard->getCardData()->m_defend;
        m_nCurrentHp = m_pSelectCard->getCardData()->m_iCurrHp;
        m_nCurrentRv = m_pSelectCard->getCardData()->m_nLeadShip;
        maxLevel = m_pSelectCard->getCardData()->m_pCard->m_ileve_max;
    }
    else
    {
        m_nCurrentAtk = 0;
        m_nCurrentDef = 0;
        m_nCurrentHp = 0;
        m_nCurrentRv = 0;
       
    }
        //init zero

   
    int array[4]={1,0,1,4001};
    char buff[30] = {0};
    CCLabelTTF * label = NULL;
    // atak
    array[3]=4001;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
       
        sprintf(buff, "ATK: %d (+ %d)",m_nCurrentAtk, m_nAddAtak);
        label->setString(buff);
    }
    
    // hp
  
    array[3]=4002;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        
        sprintf(buff, "HP: %d (+ %d)",m_nCurrentHp, m_nAddHp);
        label->setString(buff);
    }

    // def:
    array[3]=4003;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        
        sprintf(buff, "DEF: %d (+ %d)",m_nCurrentDef , m_nAddDef);
        label->setString(buff);
    }

    // rvc:
     array[3]=4004;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
       
        sprintf(buff, "RVC: %d (+ %d)",m_nCurrentRv, m_nAddRvc);
        label->setString(buff);
    }
    
    array[2]=160;
    array[3]= 4007;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        if (m_nCurrentLevel == -1)
        {
             label->setString("");
        }else
        {
             sprintf(buff, "%d/%d",m_nCurrentLevel, maxLevel);
             label->setString(buff);
        }
       
       
    }

    // conin:
    array[2]=129;
    array[3]=4005;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label && maxLevel != -1)
    {
        sprintf(buff, "%d",m_nCostConin);
        label->setString(buff);
    }
    

}


void CCardEnhanceLayer::clearMaterialCard()
{
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
    
          
            m_pMaterialCards[i]->removeFromParentAndCleanup(true);
            m_pMaterialCards[i] = NULL;
        }
    }
    if (m_pSelectCard)
    {
          
    }
    updateEnhance();
    
}
void CCardEnhanceLayer::resetMaterialCard()
{
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
            m_pMaterialCards[i]->getCardData()->setEnConsume(false);
            m_pMaterialCards[i]->getInCardBagPointer()->setLive();
        }
    }
    if (m_pSelectCard)
    {
        m_pSelectCard->getCardData()->setEnConsume(false);
        m_pSelectCard->getInCardBagPointer()->setLive();
    }


}
bool CCardEnhanceLayer::verifyConin()
{
    int conin = m_pPlayer->getCoin();
    bool bRet = false;
    if (m_nCostConin <= conin)
    {
        bRet = true;
    }
    return bRet;
}



void CCardEnhanceLayer:: save()
{
       // save data:
    CCLog("sub coin: %d", m_nCostConin);
    m_pPlayer->subCoin(m_nCostConin);
    m_nCostConin = 0;
    
    vector<CFightCard *> &r_CardBag = m_pPlayer->getCardBagVector();
    CCArray * array = m_pCardBag->getItems();
    list<int> ids;

    
    CBattleArrayLayer *parent =(CBattleArrayLayer*) getParent();
    CPtDisPlayCard *preCardManifier = parent->getPreCardManifier();
    int index = -1;
    for (int i = 0; i < MATERIALCARDCOUNT; i ++)
    {
        if (m_pMaterialCards[i])
        {
            index = m_pMaterialCards[i]->getIndex();
            ids.push_back(index);
            
            if(preCardManifier &&preCardManifier->getCardData() == m_pMaterialCards[i]->getCardData())
            {
                parent->setPreCardManifier(NULL);
            }
        }
    }
    ids.sort();
    for (std::list<int>::reverse_iterator i = ids.rbegin(); i != ids.rend(); i++)
    {
        index = *i;
        CCLog("index:%d",index);
        array->removeObjectAtIndex(index);
        std::vector<CFightCard*>::iterator j = r_CardBag.begin()+index;
        delete *j;
        r_CardBag.erase(j);
    }
    
    
    if(m_pSelectCard)
    {
        m_pSelectCard->updateToLevel(m_nCurrentLevel);
        m_pSelectCard->getInCardBagPointer()->updateToLevel(m_nCurrentLevel);
    
    }
    m_pCardBag->reload();
 
    clearMaterialCard();
}

void CCardEnhanceLayer::saveOnClick()
{
    CSaveConfirmLayer * layer =  CSaveConfirmLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000, 2000);

    if (m_pSelectCard == 0 || m_nCostConin == 0)
    {
      //  CCLog("没有材料卡或被强化的卡");
        layer->setResultCode(5);
        CCLog("22333");
        return;
    }
    bool bRet =verifyConin();
    if (bRet == false)
    {
        CCLog("金币不足");
        layer->setResultCode(6);
        return ;
    }
    char buff[200]={0};
    char param[200]={0};
//xianbei modify    sprintf(buff, "sig=2ac2b1e302c46976beaab20a68ef95&type=%d&info={\"card_id\":\"%d\",\"other\":[",0,m_pSelectCard->getCardData()->m_User_Card_ID);
    sprintf(buff, "sig=%s&type=%d&info={\"card_id\":\"%d\",\"other\":[",STR_USER_SIG,0,m_pSelectCard->getCardData()->m_User_Card_ID);
    char tmp [20];
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
            sprintf(tmp, "\"%d_%d\",",m_pMaterialCards[i]->getCardData()->m_User_Card_ID, m_pMaterialCards[i]->getCardData()->getInWhichBattleArray());
            strncat(param, tmp, strlen(tmp));
        }
    }
    CCLog("%s", param);
    strncat(buff, param, strlen(param)-1);
   // strncat(buff, "]}", strlen(param)-1);
    strcat(buff, "]}");
    CCLog("%s",buff);
   
    ADDHTTPREQUESTPOSTDATA(STR_URL_ADVANCE_CARD(194) , "cardenhance","enhancel", buff, callfuncO_selector(CCardEnhanceLayer::receiveCallBack));
   // #define STR_URL_ADVANCE_CARD(UID)   URL_FACTORY("api.php?m=Card&a=cardUpGrade&uid=",UID)
    
}
void CCardEnhanceLayer::receiveCallBack(CCObject *pSender)
{

    char *buffer = (char *) pSender;
    CCLog("callback: %s", buffer);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "cardenhance");
    CSaveConfirmLayer * layer = (CSaveConfirmLayer * ) CCDirector::sharedDirector()->getRunningScene()->getChildByTag(2000);
    if(!buffer)
    {
        layer->setResultCode(200);
        return;
    }
    CCDictionary* dic = PtJsonUtility::JsonStringParse(buffer);
 
    int result = GameTools::intForKey("code", dic);
    layer->setResultCode(result,true);
    if (result == 0)
    {
        save();
    }else
    {
        CCLog("error");
    }
    delete [] buffer;

}