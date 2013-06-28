//
//  CCardFightingLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-27.
//
//

#include "CCardFightingLayerScene.h"
#include "gamePlayer.h"
#include "CGamesCard.h"
#include "gameConfig.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CCScene *CCardFightingLayerScene::scene()
{
    CCScene *pScene=CCScene::create();
    CCardFightingLayerScene *fightScene = new CCardFightingLayerScene();
    fightScene->init();
    fightScene->autorelease();
    pScene->addChild(fightScene);
    return pScene;

}



CCardFightingLayerScene::CCardFightingLayerScene()
{
    
}

CCardFightingLayerScene::~CCardFightingLayerScene()
{
    DELETE_POINT_VECTOR(m_vFightingCard,vector<CFightCard *>);
    DELETE_POINT_VECTOR(m_vMonsterCard,vector<CFightCard *>);
}

bool CCardFightingLayerScene::init()
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCLog("CCardFightingLayerScene::init");
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/battle_bg11.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    createFightCard();
    createMonsterCard();
    m_iFightingCardIndex=m_iMonsterCardIndex=0;
    scheduleOnce(schedule_selector(CCardFightingLayerScene::locgicSchudel), 1.0f);
    return true;
}

void CCardFightingLayerScene::fSchudelUpdate(float t)
{
    
}
int CCardFightingLayerScene::getWinStats()
{
    //先判断
    bool isCardAllDead=true;
    for (int i=0; i<m_vFightingCard.size()-1; i++)
    {
        if(!m_vFightingCard[i]->isDead) //没有死亡 拥户不算
        {
            isCardAllDead=false;
            break;
        }
    }
    //都死了
    if(isCardAllDead)
    {
        return -1;
    }
    //先算自己是否全部
    for (int i=0; i<m_vMonsterCard.size()-1; i++)
    {
        if (!m_vMonsterCard[i]->isDead)
        {
            return 0;
        }
    }
    return 1;
}


void CCardFightingLayerScene::locgicSchudel(float t)
{
    int winStatus=getWinStats();
    cout<<"winStatus="<<winStatus<<endl;
    //先判断是否全部死亡；
    if(winStatus==0)
    {
        if(!m_vFightingCard[m_iFightingCardIndex]->isDead && !m_vMonsterCard[m_iMonsterCardIndex]->isDead)
        {
            fightLogic(m_iHuihe);
        }

    }
}
void CCardFightingLayerScene::fightLogic(int iHuihe)
{
    
}

void CCardFightingLayerScene::createFightCard()
{
    cout<<"SinglePlayer::instance()->m_hashmapFight.size():"<<SinglePlayer::instance()->m_hashmapFight.size();
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<SinglePlayer::instance()->m_hashmapFight.size();i++)
    {
        m_vFightingCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapFight[i]));
    }
    for (int i=0; i<m_vFightingCard.size(); i++) {
        if(i!=m_vFightingCard.size()-1)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard,false);
        m_vFightingCard[i]->tag=100+i;
        gameCard->setTag(m_vFightingCard[i]->tag);
        addChild(gameCard,i+5);
        gameCard->setPosition(ccp(winsize.width/2-100-i*118,winsize.height*0.5));
        }
        else
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard,true);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            gameCard->setPosition(ccp(20,20));
            gameCard->setAnchorPoint(CCPointZero);
            addChild(gameCard,i+5);
        }
    }
    
}

void CCardFightingLayerScene::createMonsterCard()
{
    CCSize wndsize=CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<SinglePlayer::instance()->m_hashmapMonster.size(); i++) {
        m_vMonsterCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapMonster[i]));
    }
    for (int  i=0; i<m_vMonsterCard.size(); i++) {
        if(i!=m_vMonsterCard.size()-1)
        {
        CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]->m_pCard,false);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            addChild(gameCard,i+5);
            gameCard->setPosition(ccp(wndsize.width*0.5+100+i*118,wndsize.height*0.5));
            gameCard->setFlipX(true);
        }
        else
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]->m_pCard,true);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            gameCard->setPosition(ccp(wndsize.width-200,20));
          //  gameCard->setAnchorPoint(ccp(0,0));
            gameCard->setFlipX(true);
            addChild(gameCard,i+5);
        }
    }
}