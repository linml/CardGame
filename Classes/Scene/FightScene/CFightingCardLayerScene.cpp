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
#include "CAnimationSpriteGameFight.h"
#include "CFightSkillManager.h"
#include "CSkillData.h"
#include "Utility.h"
#include "PtActionUtility.h"
#define AAAAFOROSMACHINE 
static string  g_strresource=g_mapImagesPath+"fighting/";
static string g_testtemp[5]={
    "001",
    "002",
    "003",
    "purple_princess",
    "red_wolf"
};
#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}


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
    m_vAnimation.clear();
}
CFightingCardLayerScene::~CFightingCardLayerScene()
{
    if(m_friendFightLogic)
    {
        delete m_friendFightLogic;
        m_friendFightLogic=NULL;
    }
    DELETE_POINT_VECTOR(m_vAnimation,vector<CAnimationSpriteGameFight *>);
    
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
    createHero();
    m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    g_FightSkillManager::instance()->clearAnimationList();
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
        animationAndex=0;
        isAnimationEnd=true;
        schedule(schedule_selector(CFightingCardLayerScene::animationSchudel));
    }
}

void CFightingCardLayerScene::animationSchudel(float t)
{
    if (isAnimationEnd && animationAndex<g_FightSkillManager::instance()->m_animationVector.size()) {
        isAnimationEnd=false;
        CAnimationSpriteGameFight *fightAnimation=g_FightSkillManager::instance()->m_animationVector[animationAndex];
        this->m_currCAnimationHP=fightAnimation;
        animationSwf(fightAnimation);
    }
    if(animationAndex==g_FightSkillManager::instance()->m_animationVector.size())
    {
        unschedule(schedule_selector(CFightingCardLayerScene::animationSchudel));
        CCLog("end animation");
    }
}

void CFightingCardLayerScene::createHero()
{
    m_vFightHero.resize(5);//(5, NULL);
    m_vMonsterHero.resize(5);//, NULL);
    CCSize wndSize=CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        if(m_vFightingCard[i])
        {
            FILE *fp = fopen((g_strresource+m_vFightingCard[i]->m_pCard->m_scard_resources+".png").c_str(), "r");
            if(fp==NULL)
            {
                string cardfile=g_strresource+"card_res_"+g_testtemp[rand()%3]+"_000"+".png";
                CCSprite *sprite=CCSprite::create(cardfile.c_str());
                addChild(sprite,1,m_vFightingCard[i]->tag+10);
                sprite->setPosition(ccp(wndSize.width*0.5-300,wndSize.height*0.5));
                sprite->setVisible(false);
                m_vFightHero[i]=sprite;
            }
            else
            {
                fclose(fp);
                CCSprite *sprite=CCSprite::create((g_strresource+m_vFightingCard[i]->m_pCard->m_scard_resources+".png").c_str());
                addChild(sprite,1,m_vFightingCard[i]->tag+10);
                m_vFightHero[i]=sprite;
            }
        }
    }
    if(m_vFightHero.size()>0)
    {
        m_vFightHero[0]->setVisible(true);
    }
    for (int i=0; i<m_vMonsterCard.size(); i++) {
        if(m_vMonsterCard[i])
        {
            FILE *fp = fopen((g_strresource+m_vMonsterCard[i]->m_pCard->m_scard_resources+".png").c_str(), "r");
            if(fp==NULL)
            {
                string cardfile=g_strresource+"card_res_"+g_testtemp[rand()%3]+"_000"+".png";
                CCSprite *sprite=CCSprite::create(cardfile.c_str());
                addChild(sprite,1,m_vMonsterCard[i]->tag+10);
                sprite->setPosition(ccp(wndSize.width*0.5+300,wndSize.height*0.5));
                sprite->setVisible(false);
                m_vMonsterHero[i]=sprite;
            }
            else
            {
                fclose(fp);
                CCSprite *sprite=CCSprite::create((g_strresource+m_vMonsterCard[i]->m_pCard->m_scard_resources+".png").c_str());
                addChild(sprite,1,m_vMonsterCard[i]->tag+10);
                m_vMonsterHero[i]=sprite;
            }
        }
    }
    if(m_vMonsterHero.size()>0)
    {
        m_vMonsterHero[0]->setVisible(true);
    }
}

void CFightingCardLayerScene::showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSpriteGameFight *fightAnimation)
{
    if(pFightSprite ==pMonsterSprite2)
    {
        
    }
    else
    {
        CSkillData *pSkilldata=SinglePlayer::instance()->getSkillBySkillId(skillid);
        if(pFightSprite)
        {
#ifndef AAAAFOROSMACHINE
            PtActionUtility::readSpriteActionFile(pSkilldata->effect_plist,pFightSprite,"FightCard");
            if(pMonsterSprite2)
            {
                PtActionUtility::readSpriteActionFile(pSkilldata->effect_plist, pMonsterSprite2,"MonsterCard");
            }
#else
  //          Utility::runPtActionScript(pFightSprite, "resource_cn/action/juqi1/juqi1.act", 100);
            CCAction *animation=PtActionUtility::getRunActionWithActionFile("resource_cn/boss_icon_Effect.plist");//,pFightSprite);
            CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonsterSprite2);
            CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::showHpAnimation));
            CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
            pFightSprite->runAction(CCSequence::create((CCFiniteTimeAction*)animation,nd,callback,endAnimation,NULL));
#endif
        }
    }
    
}
void  CFightingCardLayerScene::animationShouShang(CCNode *node,void *tag)
{
    if(getChildByTag(30001))
    {
        CCSprite *sprite=(CCSprite *)getChildByTag(30001);
        CCSprite * spritetag=(CCSprite *)tag;
        sprite->setPosition(spritetag->getPosition());
        sprite->runAction(CCSequence::create(CCShow::create(),CCDelayTime::create(0.2),CCHide::create(),NULL));
    }
    else
    {
        CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
        addChild(sprite,10,30001);
        CCSprite * spritetag=(CCSprite *)tag;
        sprite->setPosition(spritetag->getPosition());
        sprite->runAction(CCSequence::create(CCShow::create(),CCDelayTime::create(0.2),CCHide::create(),NULL));
    }
    
}
void CFightingCardLayerScene::skillAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster)
{
    switch (fightAnimation->m_enAnimationType)
    {
        case EN_ANIMATIONTYPE_HERO:
            showSkill(pFight,pMonster,fightAnimation->m_iSKillId, fightAnimation);
            //显示扣血的函数
            break;
        case EN_ANIMATIONTYPE_BUFFER:
            showSkill(pFight,NULL,fightAnimation->m_iSKillId, fightAnimation);
            break;
        case EN_ANIMATIONTYPE_STATUS:
            showSkill(pFight,NULL,fightAnimation->m_iSKillId,fightAnimation);
            break;
        default:
            break;
    }
    
}

void CFightingCardLayerScene::animationSwf(CAnimationSpriteGameFight *fightAnimation)
{
    CCLog("animationSwf");
    switch (fightAnimation->m_enAtkFightIndex)
    {
        case EN_ATKFIGHT_INDEX_LEFT_LORD:
        {
            CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
            CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
            skillAnimationSwf(fightAnimation,sprite,pMonster);
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
        {
            CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
            CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
            skillAnimationSwf(fightAnimation,sprite,pMonster);
            
        }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_LORD:
        {
            CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
            CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
            skillAnimationSwf(fightAnimation,pMonster,sprite);
            
        }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
        {
            CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
            CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
            skillAnimationSwf(fightAnimation,pMonster,sprite);
            
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_MOVE:
            m_vFightHero[m_currCAnimationHP->m_iATKindex]->setVisible(false);
            m_vFightHero[m_currCAnimationHP->m_iDefIndex]->setVisible(true);
            moveCardSprite(m_vFightingCard,m_currCAnimationHP->m_iATKindex,true);
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_MOVE:
            m_vMonsterHero[m_currCAnimationHP->m_iATKindex]->setVisible(false);
            m_vMonsterHero[m_currCAnimationHP->m_iDefIndex]->setVisible(true);
             moveCardSprite(m_vMonsterCard,m_currCAnimationHP->m_iATKindex,false);
            break;
        default:
            break;
    }
}

void CFightingCardLayerScene::moveCardSprite(vector<CFightCard *> &vCard,int goIndex,bool isLeft)
{
    if(vCard.size()<=2)
    {
        return;
    }
    else
    {
        vector<CFightCard *>vectemp;
        vectemp.clear();
        for (int i=goIndex; i<=vCard.size()-2; i++) {
            vectemp.push_back(vCard[i]);
        }
        for (int i=goIndex-1; i>=0; i--) {
            vectemp.push_back(vCard[i]);
        }
        char data[20];
        for (int i=0 ; i<vectemp.size(); i++) {
            if(isLeft)
            {
                sprintf(data, "left%0.2d",i);
            }
            else{
                sprintf(data, "right%0.2d",i);
                
            }
            CCLog("%s,%d,0x%x",data,vectemp[i]->tag,(CCSprite *)getChildByTag(vectemp[i]->tag));
            PtActionUtility::readSpriteActionFile(g_ActionFilePath+"movecard.plist",(CCSprite *)getChildByTag(vectemp[i]->tag),string(data));
        }
       runAction(CCSequence::create(CCDelayTime::create(0.3f),CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd)),NULL));
    }
}

void CFightingCardLayerScene::logicFighting()
{
    m_enHuiheIndex++;
    if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_LORD)
    {
        CCLog("LEFT%d-------->RIGHT%d,LEFTHP :%d,RIHGHP: %d",m_iFightCardIndex,m_iMonsterCardIndex,m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp);
        // CCAssert(m_vFightingCard[m_iFightCardIndex], "战斗队列中居然有空位的卡牌");
        m_friendFightLogic->logicFightGame(m_vFightingCard, m_vMonsterCard, m_iFightCardIndex,m_iMonsterCardIndex,m_vFightingCard[m_iFightCardIndex],this);
        
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
    {
        CCLog("LEFT5-------->RIGHT%d,LEFTHP :%d,RIHGHP: %d",m_iMonsterCardIndex,m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp);
        m_friendFightLogic->logicFightGame(m_vFightingCard, m_vMonsterCard,m_iFightCardIndex,m_iMonsterCardIndex, m_vFightingCard[4], this);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_LORD)
    {
        
        CCLog("RIGHT%d-------->LEFT%d,LEFTHP :%d,RIHGHP: %d",m_iMonsterCardIndex,m_iFightCardIndex,m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp);
        //CCAssert(m_vMonsterCard[m_iMonsterCardIndex], "敌人队列中居然有空位的卡牌");
        m_friendFightLogic->logicFightGame(m_vMonsterCard, m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex, m_vMonsterCard[m_iMonsterCardIndex], this);
    }
    else if(m_enHuiheIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_friendFightLogic->logicFightGame(m_vMonsterCard, m_vFightingCard,m_iMonsterCardIndex,m_iFightCardIndex, m_vMonsterCard[4], this);
        CCLog("RIGHT5-------->LEFT%d,LEFTHP :%d,RIHGHP: %d",m_iFightCardIndex,m_vFightingCard[m_iFightCardIndex]->m_iCurrHp,m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp);
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
        m_iTotalHuihe++;
        CCLog("========>%d",m_iTotalHuihe);
    }
    if(m_enHuiheIndex>EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
    {
        m_enHuiheIndex=EN_ATKFIGHT_INDEX_NONE;
    }
    checkIsDead();
}

void CFightingCardLayerScene::showHp(int leftHp,int RightHp)
{
    int hpValue=leftHp;
    if(hpValue==0)
    {
        return;
    }
    CCPoint point=ccp((CCDirector::sharedDirector()->getWinSize().width*0.5-100),(CCDirector::sharedDirector()->getWinSize().height*0.5));
    CCLabelTTF *labelTTF=(CCLabelTTF *)getChildByTag(30002);
    char data[20];
    sprintf(data, "%d",hpValue*(-1));
    labelTTF->setString(data);
    labelTTF->setVisible(true);
    if(hpValue>0)
    {
        labelTTF->setPosition(ccp(point.x,point.y+100));
        labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.2f,CCPoint(0, -100)),CCHide::create(),NULL));
    }
    else
    {
        labelTTF->setPosition(ccp(point.x,point.y));
        labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.2f,CCPoint(0, 100)),CCHide::create(),NULL));
    }
    hpValue=RightHp;
    if(hpValue==0)
    {
        return;
    }
     point=ccp((CCDirector::sharedDirector()->getWinSize().width*0.5+100),(CCDirector::sharedDirector()->getWinSize().height*0.5));
    labelTTF=(CCLabelTTF *)getChildByTag(30003);
    sprintf(data, "%d",hpValue*(-1));
    labelTTF->setString(data);
    labelTTF->setVisible(true);
    if(hpValue>0)
    {
        labelTTF->setPosition(ccp(point.x,point.y+100));
        labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.2f,CCPoint(0, -100)),CCHide::create(),NULL));
    }
    else
    {
        labelTTF->setPosition(ccp(point.x,point.y));
        labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.2f,CCPoint(0, 100)),CCHide::create(),NULL));
    }
}

void CFightingCardLayerScene::AnimaitonEnd(CCObject *object)
{
    animationAndex++;
    isAnimationEnd=true;
    
}


void CFightingCardLayerScene::showHpAnimation(CCObject *object)
{
    if (m_currCAnimationHP)
    {
        switch (m_currCAnimationHP->m_enAtkFightIndex)
        {
            case EN_ATKFIGHT_INDEX_LEFT_LORD:
                showHp(m_currCAnimationHP->m_iAddHp,m_currCAnimationHP->m_iSubHp);
                break;
            case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
                showHp(m_currCAnimationHP->m_iAddHp,m_currCAnimationHP->m_iSubHp);
                break;
            case EN_ATKFIGHT_INDEX_RIGHT_LORD:
                showHp(m_currCAnimationHP->m_iSubHp,m_currCAnimationHP->m_iAddHp);
                break;
            case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
               showHp(m_currCAnimationHP->m_iSubHp,m_currCAnimationHP->m_iAddHp);
                break;
            default:
                break;
        }
    }
}

void CFightingCardLayerScene::checkIsDead()
{
    if (m_iFightCardIndex<m_vFightingCard.size()-1) {
        
        
        if(m_vFightingCard[m_iFightCardIndex]->m_iCurrHp<=0)
        {
            int backIndex=m_iFightCardIndex;
            m_vFightingCard[m_iFightCardIndex]->m_iCurrHp=0;
            m_vFightingCard[m_iFightCardIndex]->isDead=true;
            //m_vFightHero[m_iFightCardIndex]->setVisible(false);
           
            //发动死亡技能
            do {
                m_iFightCardIndex++;
            } while (m_vFightingCard[m_iFightCardIndex]==NULL &&m_iFightCardIndex<m_vFightingCard.size()-1);
             g_FightSkillManager::instance()->appendAnimation(backIndex, m_iFightCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_HERO, EN_ATKFIGHT_INDEX_LEFT_MOVE);
            //m_vFightHero[m_iFightCardIndex]->setVisible(true);
        }
    }
    if (m_iMonsterCardIndex<m_vMonsterCard.size()-1)
    {
        
        if(m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp<=0)
        {
            //发动死亡技能
            int backIndex=m_iMonsterCardIndex;
            m_vMonsterCard[m_iMonsterCardIndex]->m_iCurrHp=0;
            m_vMonsterCard[m_iMonsterCardIndex]->isDead=true;
            //m_vMonsterHero[m_iMonsterCardIndex]->setVisible(false);
            do {
                m_iMonsterCardIndex++;
            } while (m_vMonsterCard[m_iMonsterCardIndex]==NULL &&m_iMonsterCardIndex<m_vMonsterCard.size()-1);
            //m_vMonsterHero[m_iMonsterCardIndex]->setVisible(true);
         g_FightSkillManager::instance()->appendAnimation(backIndex, m_iMonsterCardIndex, 0, 0, 0, 0, 0, EN_ANIMATIONTYPE_HERO, EN_ATKFIGHT_INDEX_RIGHT_MOVE);
        }
    }
}

EN_ATKFIGHT_INDEX CFightingCardLayerScene::getHuiHeIndex()
{
    return m_enHuiheIndex;
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