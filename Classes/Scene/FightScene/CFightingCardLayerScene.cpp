//
//  CFightingCardLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-7-11.
//
//

#include "CFightingCardLayerScene.h"
#include "CSkillManager.h"
#include "gameConfig.h"
#include "PtMapUtility.h"
#include "gamePlayer.h"
#include "CCard.h"
#include "CGamesCard.h"
#include "CFightingCardLayerLogic.h"

CCScene *CFightingCardLayerScene::scene()
{
    G_SkillManager::instance()->clearAnimationList();
    CCScene *pScene=CCScene::create();
    CFightingCardLayerScene *fightScene = new CFightingCardLayerScene();
    fightScene->init();
    fightScene->autorelease();
    pScene->addChild(fightScene);
    return pScene;
    
}

CFightingCardLayerScene::CFightingCardLayerScene()
{
    m_friendFightLogic=new  CFightingCardLayerLogic();
}
CFightingCardLayerScene::~CFightingCardLayerScene()
{
    if(m_friendFightLogic)
    {
        delete m_friendFightLogic;
        m_friendFightLogic=NULL;
    }
    
}
bool CFightingCardLayerScene::init()
{
    m_iTotalHuihe=0;
	CCLog("CCardFightingLayerScene::init");
    initBggroudPng();
    
    PtMapUtility::addChildFromScript(this, plistPath+"zhandouui.plist");
    
    initText();
    initHitText();
    createFightCard();
    createMonsterCard();
    //计算战斗
    initGame();
    m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    schedule(schedule_selector(CFightingCardLayerScene::locgicSchudel));
    return true;
}

void  CFightingCardLayerScene::locgicSchudel(float t)
{
  ///1  判断 是否战斗超过30回合，2  判断是否胜利 输赢， 3  否则一直回调。
    EN_GAMEFIGHTSTATUS winStatus=getWinStatus();
    //先判断是否全部死亡；
    if(winStatus==EN_GAMEFIGHTSTATUS_NONE)
    {
        logicFighting();
    }
    else
    {
        cout<<"end logic game"<<endl;
        m_enWinStatus=winStatus;
        unschedule(schedule_selector(CFightingCardLayerScene::locgicSchudel));
    }
    
}

void CFightingCardLayerScene::logicFighting()
{
    m_enHuiheIndex++;
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD)
    {
        CCLog("LEFT%d-------->RIGHT%d",m_iFightCardIndex,m_iMonsterCardIndex);
       // CCAssert(m_vFightingCard[m_iFightCardIndex], "战斗队列中居然有空位的卡牌");
        m_friendFightLogic->logicFightGame(m_vFightingCard, m_vMonsterCard, m_vFightingCard[m_iFightCardIndex], this);
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
    {
        CCLog("LEFT5-------->RIGHT%d",m_iMonsterCardIndex);
         m_friendFightLogic->logicFightGame(m_vFightingCard, m_vMonsterCard, m_vFightingCard[4], this);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD)
    {
        
        CCLog("RIGHT%d-------->LEFT%d",m_iMonsterCardIndex,m_iFightCardIndex);
           CCAssert(m_vMonsterCard[m_iMonsterCardIndex], "敌人队列中居然有空位的卡牌");
        m_friendFightLogic->logicFightGame(m_vMonsterCard, m_vFightingCard, m_vMonsterCard[m_iMonsterCardIndex], this);

    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_friendFightLogic->logicFightGame(m_vMonsterCard, m_vFightingCard, m_vMonsterCard[4], this);
        CCLog("RIGHT5-------->LEFT%d",m_iFightCardIndex);
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    }
    if(m_enHuiheIndex>EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    }
    checkIsDead();
}
void CFightingCardLayerScene::checkIsDead()
{
    if (m_iFightCardIndex<m_vFightingCard.size()-1) {
        
        if(m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            m_vFightingCard[m_iFightCardIndex]->m_iCurrHp=0;
            m_vFightingCard[m_iFightCardIndex]->isDead=true;
            //发动死亡技能
            do {
                m_iFightCardIndex++;
            } while (m_vFightingCard[m_iFightCardIndex]==NULL &&m_iFightCardIndex<m_vFightingCard.size()-1);
        }
    }
    if (m_iMonsterCardIndex<m_vMonsterCard.size()-1) {
        
        if(m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            //发动死亡技能
            m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp=0;
            m_vMonsterCard[m_iMonsterCardIndex]->isDead=true;
            do {
                m_iMonsterCardIndex++;
            } while (m_vMonsterCard[m_iMonsterCardIndex]==NULL &&m_iMonsterCardIndex<m_vMonsterCard.size()-1);
        }
    }
}


EN_GAMEFIGHTSTATUS CFightingCardLayerScene::getWinStatus()
{
    if (m_iTotalHuihe>=31) {
        return EN_GAMEFIGHTSTATUS_LOSE;
    }
    bool isCardAllDead=true;
    for (int i=m_iFightCardIndex; i<m_vFightingCard.size()-1; i++)
    {
        if(m_vFightingCard[i]&&!m_vFightingCard[i]->isDead) //没有死亡 拥户不算
        {
            isCardAllDead=false;
            break;
        }
    }
    //都死了
    if(isCardAllDead)
    {
        return EN_GAMEFIGHTSTATUS_LOSE;
    }
    //先算自己是否全部
    for (int i=m_iMonsterCardIndex; i<m_vMonsterCard.size()-1; i++)
    {
        if (m_vMonsterCard[i]&&!m_vMonsterCard[i]->isDead)
        {
            return EN_GAMEFIGHTSTATUS_NONE;
        }
    }
    
    return EN_GAMEFIGHTSTATUS_WIN;
}


void CFightingCardLayerScene::initGame()
{
    m_iFightCardIndex=0;
    m_iMonsterCardIndex=0;
}

void CFightingCardLayerScene::loadFromServerTest()
{
    if(SinglePlayer::instance()->isLoadEndCardTeam &&SinglePlayer::instance()->m_vvBattleArray.size()>0&&SinglePlayer::instance()->m_vvBattleArray[0].size()>0)
    {
        for (int i=0; i<SinglePlayer::instance()->m_vvBattleArray[0].size();i++)
        {
            if(SinglePlayer::instance()->m_vvBattleArray[0][i])
            {
                m_vFightingCard.push_back(new CFightCard(*(SinglePlayer::instance()->m_vvBattleArray[0][i])));
            }
            else
            {
                m_vFightingCard.push_back(NULL);
            }
        }
    }
    else
    {
        for (int i=0; i<SinglePlayer::instance()->m_hashmapFight.size();i++)
        {
            m_vFightingCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapFight[i]));
        }
    }
}
void CFightingCardLayerScene::createFightCard()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    loadFromServerTest();
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        if(i!=m_vFightingCard.size()-1 &&m_vFightingCard[i])
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            addChild(gameCard,8-i,m_vFightingCard[i]->tag);
            gameCard->setPosition(getCardPoint(i,true));
            if(i!=0)
            {
                gameCard->setScale(0.6);
            }
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
        }
        else if(m_vFightingCard[i])
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            gameCard->setPosition(ccp(20,20));
            gameCard->setAnchorPoint(CCPointZero);
            addChild(gameCard,8-i,m_vFightingCard[i]->tag);
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
        }
    }

}

void CFightingCardLayerScene::createMonsterCard()
{
    CCSize wndsize=CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<SinglePlayer::instance()->m_hashmapMonster.size(); i++)
    {
        m_vMonsterCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapMonster[i]));
    }
    for (int  i=0; i<m_vMonsterCard.size(); i++)
    {
        if(i!=m_vMonsterCard.size()-1 && m_vMonsterCard[i]!=NULL)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]->m_pCard);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            addChild(gameCard,9-i, m_vMonsterCard[i]->tag);
            gameCard->setPosition(getCardPoint(i, false));
            gameCard->setFlipX(true);
            if(i!=0)
            {
                
                gameCard->setScale(0.6);
            }
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
            
        }
        else if(m_vMonsterCard[i])
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]->m_pCard);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            gameCard->setPosition(ccp(wndsize.width-200,20));
            gameCard->setFlipX(true);
            gameCard->setAnchorPoint(CCPointZero);
            addChild(gameCard,9-i, m_vMonsterCard[i]->tag);
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
            
        }
    }

}
CCPoint CFightingCardLayerScene::getCardPoint(int index, bool isLeftCard)
{
        CCPoint point;
        int value=isLeftCard?-1:1;
        CCSize  size=CCDirector::sharedDirector()->getWinSize();
        if(index==0)
        {
            return CCPoint(size.width*0.5+100*value,140);
        }
        else
        {
            return CCPoint(size.width*0.5+200*value+30*value*(index-1),100);
        }
        
}

bool CFightingCardLayerScene::initHitText()
{
    //设置加血扣血的
    CCLabelTTF *labelTTf1=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf1,15,30002);
    labelTTf1->setVisible(false);
    labelTTf1->setColor(ccc3(255, 0, 0));
    
    CCLabelTTF *labelTTf2=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf2,15,30003);
    labelTTf2->setVisible(false);
    labelTTf2->setColor(ccc3(255, 0, 0));
    return true;
}

bool CFightingCardLayerScene::initText()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    //血量数值
    CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 35);
    labelttf->setPosition(ccp(150,700));
    labelttf->setColor(ccc3(255, 0, 0));
    this->addChild(labelttf,10,20000);
    
    CCLabelTTF *labelttfMonster=CCLabelTTF::create("", "Arial", 35);
    labelttfMonster->setPosition(ccp(624,700));
    labelttfMonster->setColor(ccc3(255, 0, 0));
    addChild(labelttfMonster,10,20001);
    
    
    CCLabelTTF *labelttfVersion=CCLabelTTF::create("", "Arial", 50);
    labelttfVersion->setPosition(ccp(winsize.width*0.5,winsize.height-100));
    labelttfVersion->setColor(ccc3(0, 0, 255));
    addChild(labelttfVersion,10,20003);
    return true;
}



bool CFightingCardLayerScene::initAtkPng()
{
    CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
    addChild(sprite,15,30001);
    sprite->setVisible(false);
    CCSprite *sprite2=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
    addChild(sprite2,15,30000);
    sprite2->setVisible(false);
    return true;
}

bool CFightingCardLayerScene::initBggroudPng()
{
    CCSize  winsize=CCDirector::sharedDirector()->getWinSize();
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/bgm.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    return  true;
}