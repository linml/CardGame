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
#include "FightResultConfirm.h"
#include "SEveryATKData.h"
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
    
}

CFightingCardLayerScene::~CFightingCardLayerScene()
{
    SinglePlayer::instance()->onFightExitScene();
    g_FightSkillManager::instance()->clearAnimationList();
    //释放特效文件 
}

bool CFightingCardLayerScene::init()
{
    CCLog("CCardFightingLayerScene::init");
    initBggroudPng();
    PtMapUtility::addChildFromScript(this, plistPath+"zhandouui.plist");
    initText();
    initHitText();
    createHpText();
    createEngryText();
    createFightCard();
    createMonsterCard();
    deleteBackGamePlayerFightMonsterCard();
    createHero();
    initHpEngry();
    isAnimationEnd=0;
    animationAndex=0;
    hpUpdateIndex=0;
    isAnimationEnd=true;
    m_itotalAnimation=g_FightSkillManager::instance()->m_animationVector.size();
    schedule(schedule_selector(CFightingCardLayerScene::animationSchudel));
    return true;
}

void CFightingCardLayerScene::animationSchudel(float t)
{
    if (isAnimationEnd && animationAndex<m_itotalAnimation) {
        CCLog("animationAndex:%d,%d",animationAndex,m_itotalAnimation);
        isAnimationEnd=false;
        CAnimationSpriteGameFight *fightAnimation=g_FightSkillManager::instance()->m_animationVector[animationAndex];
        this->m_currCAnimationHP=fightAnimation;
        animationSwf(fightAnimation);
    }
    else  if(animationAndex==m_itotalAnimation)
    {
        unschedule(schedule_selector(CFightingCardLayerScene::animationSchudel));
        CCLog("end animation");
        m_enWinStatus=SinglePlayer::instance()->m_enWinStatus;
        if(m_enWinStatus==EN_GAMEFIGHTSTATUS_WIN)
        {
            winDialog();
        }
        else if(m_enWinStatus==EN_GAMEFIGHTSTATUS_LOSE)
        {
            loseDialog();
        }
    }
}

void CFightingCardLayerScene::winDialog()
{
    int tmp = 1;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 1000000);
    
    cout<<"win"<<endl;
}
void CFightingCardLayerScene::loseDialog()
{
    int tmp = 0;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 100000);
    
    cout<<"lose"<<endl;
}

void CFightingCardLayerScene::initSetUpdateHp(int iCurrHp,int TotalHp,int currEngry,bool isLeft)
{
    if(isLeft)
    {
        char data[20];
        sprintf(data, "HP:%d/%d",iCurrHp,TotalHp);
        CCLabelTTF *labelTTF=(CCLabelTTF*)getChildByTag(77777);
        labelTTF->setString(data);
        sprintf(data, "ENGRY:%d",currEngry);
        labelTTF=(CCLabelTTF*)getChildByTag(87777);
        labelTTF->setString(data);
        
    }
    else{
        char data[20];
        sprintf(data, "HP:%d/%d",iCurrHp,TotalHp);
        CCLabelTTF *labelTTF=(CCLabelTTF*)getChildByTag(77778);
        labelTTF->setString(data);
        sprintf(data, "ENGRY:%d",currEngry);
        labelTTF=(CCLabelTTF*)getChildByTag(87778);
        labelTTF->setString(data);
    }
}

void CFightingCardLayerScene::initHpEngry()
{
    int i=0;
    for (int i=0; i<m_vFightHero.size(); i++) {
        if(m_vFightHero[i])
        {
            m_vFightHero[i]->setVisible(true);
            initSetUpdateHp(m_vFightingCard[i]->m_iCurrHp,m_vFightingCard[i]->m_iHp,m_vFightingCard[i]->m_iCurrEngry,true);
            break;
        }
    }
    for (int i=0; i<m_vMonsterHero.size(); i++) {
        if(m_vMonsterHero[i])
        {
            m_vMonsterHero[i]->setVisible(true);
            initSetUpdateHp(m_vMonsterCard[i]->m_iCurrHp,m_vMonsterCard[i]->m_iHp,m_vMonsterCard[i]->m_iCurrEngry,true);
            break;
        }
    }
}

void CFightingCardLayerScene::updateHpAndAngry()
{
    cout<<"update====="<<hpUpdateIndex<<"/"<<SinglePlayer::instance()->m_vHpAngry.size()<<endl;
    if(hpUpdateIndex<SinglePlayer::instance()->m_vHpAngry.size())
    {
        SEveryATKData *eveyatk=SinglePlayer::instance()->m_vHpAngry[hpUpdateIndex];
        if (eveyatk)
        {
            SSpriteStatus *spriteleft =eveyatk->data[0];
            SSpriteStatus *spriteRight=eveyatk->data[1];
            if (spriteleft)
            {
                initSetUpdateHp(spriteleft->m_iCurrHp,spriteleft->m_iCurrTotalHp,spriteleft->m_iEngry,true);
            }
            if (spriteRight)
            {
                initSetUpdateHp(spriteRight->m_iCurrHp,spriteRight->m_iCurrTotalHp,spriteRight->m_iEngry,false);
            }
        }
        hpUpdateIndex++;
    }
    else
    {
        cout<<"aaaaaaaaaaaaaaa"<<endl;
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
            FILE *fp = fopen(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath((g_strresource+m_vFightingCard[i]->m_pCard->m_scard_role+".png").c_str()), "r");
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
                CCSprite *sprite=CCSprite::create((g_strresource+m_vFightingCard[i]->m_pCard->m_scard_role+".png").c_str());
                addChild(sprite,1,m_vFightingCard[i]->tag+10);
                sprite->setPosition(ccp(wndSize.width*0.5-300,wndSize.height*0.5));
                sprite->setVisible(false);
                m_vFightHero[i]=sprite;
            }
        }
    }
    for (int i=0; i<m_vMonsterCard.size(); i++) {
        if(m_vMonsterCard[i])
        {
            FILE *fp = fopen(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath((g_strresource+m_vMonsterCard[i]->m_pCard->m_scard_role+".png").c_str()), "r");
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
                CCSprite *sprite=CCSprite::create((g_strresource+m_vMonsterCard[i]->m_pCard->m_scard_role+".png").c_str());
                addChild(sprite,1,m_vMonsterCard[i]->tag+10);
                sprite->setPosition(ccp(wndSize.width*0.5+300,wndSize.height*0.5));
                sprite->setVisible(false);
                sprite->setFlipX(true);
                m_vMonsterHero[i]=sprite;
            }
        }
    }
}
void CFightingCardLayerScene::showSkillBuffer(cocos2d::CCSprite *pFightSprite, cocos2d::CCSprite *pMonsterSprite2, int skillid, CAnimationSpriteGameFight *fightAnimation)
{
    
    if(pMonsterSprite2)
    {
        string filePath=fightAnimation->m_sBufferPlist;
        //filePath += ".plist";
        CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
        CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
        pMonsterSprite2->runAction(CCSequence::create((CCFiniteTimeAction*)animation,endAnimation,NULL));
    }
    
}
void CFightingCardLayerScene::showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSpriteGameFight *fightAnimation)
{
    if(pFightSprite ==pMonsterSprite2)
    {
        
    }
    else
    {
        if(pFightSprite)
        {
#ifdef AAAAFOROSMACHINE
            CSkillData *pSkilldata=SinglePlayer::instance()->getSkillBySkillId(skillid);
            PtActionUtility::readSpriteActionFile(pSkilldata->effect_plist,pFightSprite,"FightCard");
            if(pMonsterSprite2)
            {
                PtActionUtility::readSpriteActionFile(pSkilldata->effect_plist, pMonsterSprite2,"MonsterCard");
            }
#else
            if(hpUpdateIndex==43)
            {
                
            }
            string filePath="";
            CSkillData *pSkilldata=SinglePlayer::instance()->getSkillBySkillId(skillid);
            if (!pSkilldata) {
                AnimaitonEnd(NULL);
                return;
            }
            CCLog("===============>%d,%s",pSkilldata->skill_id,pSkilldata->effect_plist.c_str());
            filePath=pSkilldata->effect_plist;
            if (fightAnimation->m_enAtkFightIndex == EN_ATKFIGHT_INDEX_RIGHT_LORD)
            {
                filePath += "_r.plist";
                CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
                CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonsterSprite2);
                CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::showHpAnimation));
                CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
                pMonsterSprite2->runAction(CCSequence::create((CCFiniteTimeAction*)animation,nd,callback,endAnimation,NULL));
            }
            else if(fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_LEFT_LORD)
            {
                filePath += "_l.plist";
                CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
                CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonsterSprite2);
                CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::showHpAnimation));
                CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
                pFightSprite->runAction(CCSequence::create((CCFiniteTimeAction*)animation,nd,callback,endAnimation,NULL));


            }
            else if(fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
            {
                if(m_vFightingCard[4])
                {
                    filePath += "_l.plist";
                    CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
                    CGamesCard *gamecard=(CGamesCard *)getChildByTag(m_vFightingCard[4]->tag);
                    CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonsterSprite2);
                    CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::showHpAnimation));
                    CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
                    gamecard->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(0,100)),(CCFiniteTimeAction*)animation,nd,callback,CCMoveBy::create(0.2, ccp(0, -100)), endAnimation,NULL));
                }
               
            }
            else if(fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
            {
                if(m_vMonsterCard[4])
                {
                    filePath += "_r.plist";
                    CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
                    CGamesCard *gamecard=(CGamesCard *)getChildByTag(m_vMonsterCard[4]->tag);
                    CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonsterSprite2);
                    CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::showHpAnimation));
                    CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd));
                    gamecard->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(0,100)),(CCFiniteTimeAction*)animation,nd,callback,CCMoveBy::create(0.2, ccp(0, -100)), endAnimation,NULL));
                }
            }
#endif
        }
        else{
            AnimaitonEnd(NULL);
        }
    }
}

void  CFightingCardLayerScene::animationShouShang(CCNode *node,void *tag)
{
    if(getChildByTag(30001))
    {
        CCSprite *sprite=(CCSprite *)getChildByTag(30001);
        CCSprite * spritetag=(CCSprite *)tag;
        if(spritetag)
        {
            CCMoveBy *move=CCMoveBy::create(0.1, ccp(20, 0));
            spritetag->runAction(CCSequence::create(CCDelayTime::create(0.01),move,move->reverse(),NULL));
            sprite->setPosition(spritetag->getPosition());
            sprite->runAction(CCSequence::create(CCShow::create(),CCDelayTime::create(0.2),CCHide::create(),NULL));
        }
    }
    else
    {
        CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
        addChild(sprite,10,30001);
        CCSprite * spritetag=(CCSprite *)tag;
        CCMoveBy *move=CCMoveBy::create(0.1, ccp(20, 0));
         spritetag->runAction(CCSequence::create(CCDelayTime::create(0.01),move,move->reverse(),NULL));
        sprite->setPosition(spritetag->getPosition());
        sprite->runAction(CCSequence::create(CCShow::create(),CCDelayTime::create(0.2),CCHide::create(),NULL));
    }
    
}
void CFightingCardLayerScene::skillHelpAnimationSwf(CAnimationSpriteGameFight *fightAnimation, cocos2d::CCSprite *pFight, cocos2d::CCSprite *pMonster)
{
    switch (fightAnimation->m_enAtkFightIndex)
    {
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
        {
            if(m_vFightingCard[4])
            {
               // CGamesCard *gameCard=(CGamesCard *)getChildByTag(m_vFightingCard[4]->tag);
                //gameCard->runAction(<#cocos2d::CCAction *action#>)
                
            }
        }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
            
            break;
            
        default:
            break;
    }
}


void CFightingCardLayerScene::skillAnimationSwf(CAnimationSpriteGameFight *fightAnimation,CCSprite *pFight,CCSprite *pMonster)
{
    CCLOG("SKILL ID=  %d",fightAnimation->m_iSKillId);
    switch (fightAnimation->m_enAnimationType)
    {
        case EN_ANIMATIONTYPE_HERO:
            updateHpAndAngry();
            showSkill(pFight,pMonster,fightAnimation->m_iSKillId, fightAnimation);
            //显示扣血的函数
            break;
        case EN_ANIMATIONTYPE_BUFFER:
            showSkill(pFight,NULL,fightAnimation->m_iSKillId, fightAnimation);
            break;
        case EN_ANIMATIONTYPE_STATUS:
            showSkill(pFight,NULL,fightAnimation->m_iSKillId,fightAnimation);
            break;
        case EN_ANIMATIONTYPE_SKILL:
            textSkillInfo(fightAnimation);
            break;
        case  EN_ANIMATIONTYPE_DEADMOVE:
            break;
        case EN_ANIMATIONTYPE_BUFFPLISTOTHER:
        {
            showSkillBuffer(pFight,pMonster,0,fightAnimation);
            break;
        }
        case EN_ANIMATIONTYPE_BUFFPLISTOWN:
        {
            showSkillBuffer(pMonster,pFight,0,fightAnimation);
            break;
        }
        default:
            break;
    }
    
}

void CFightingCardLayerScene::animationSwf(CAnimationSpriteGameFight *fightAnimation)
{
    CCLog("animationSwf,%d",(int)fightAnimation->m_enAtkFightIndex);
    switch (fightAnimation->m_enAtkFightIndex)
    {
        case EN_ATKFIGHT_INDEX_LEFT_LORD:
        {
            if(m_vFightHero[fightAnimation->m_iATKindex])
            {
                CCLog("%d/%d",fightAnimation->m_iATKindex,fightAnimation->m_iDefIndex);
                CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
                CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
                skillAnimationSwf(fightAnimation,sprite,pMonster);
            }
            else
            {
                AnimaitonEnd(NULL);
                return;
            }
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
        {
            if(m_vFightHero[4])
            {
           
                CCSprite *sprite=m_vFightHero[fightAnimation->m_iATKindex];
                CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_iDefIndex];
                skillAnimationSwf(fightAnimation,sprite,pMonster);
            }
            else
            {
                AnimaitonEnd(NULL);
                return;
            }
            
        }
        break;
        case EN_ATKFIGHT_INDEX_RIGHT_LORD:
        {
            if(m_vMonsterHero[fightAnimation->m_iATKindex])
            {
                CCSprite *sprite=m_vMonsterHero[fightAnimation->m_iATKindex];
                CCSprite *pMonster=m_vFightHero[fightAnimation->m_iDefIndex];
                skillAnimationSwf(fightAnimation,pMonster,sprite);
            }
            else{
                AnimaitonEnd(NULL);
                return;
            }
            
        }
        break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
        {
            if(m_vMonsterHero[4])
            {
                CCSprite *sprite=m_vMonsterHero[fightAnimation->m_iATKindex];
                CCSprite *pMonster=m_vFightHero[fightAnimation->m_iDefIndex];
                yinCangRenWu(m_vFightHero,pMonster);
                yinCangRenWu(m_vMonsterHero,sprite);
                skillAnimationSwf(fightAnimation,pMonster,sprite);
            }
            else
            {
                AnimaitonEnd(NULL);
                return;
            }
            
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_MOVE:
            m_vFightHero[m_currCAnimationHP->m_iATKindex]->setVisible(false);
            moveCardSprite(m_vFightingCard,m_currCAnimationHP->m_iATKindex,true);//移动 card
            if(m_currCAnimationHP->m_iATKindex+1<m_vMonsterHero.size()-1 &&m_vFightingCard[m_currCAnimationHP->m_iATKindex+1])
            {
                m_vFightHero[m_currCAnimationHP->m_iATKindex+1]->setVisible(true);
            }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_MOVE:
                m_vMonsterHero[m_currCAnimationHP->m_iATKindex]->setVisible(false);
                moveCardSprite(m_vMonsterCard,m_currCAnimationHP->m_iATKindex,false);//移动 card
                if(m_currCAnimationHP->m_iATKindex+1<m_vMonsterHero.size()-1 &&m_vMonsterHero[m_currCAnimationHP->m_iATKindex+1])
                {
                    m_vMonsterHero[m_currCAnimationHP->m_iATKindex+1]->setVisible(true);
                }
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
        for (int i=0 ; i<vectemp.size(); i++)
        {
            if (vectemp[i])
            {
                if(isLeft)
                {
                    sprintf(data, "left%0.2d",i);
                }
                else
                {
                    sprintf(data, "right%0.2d",i);
                    
                }
                CCLog("%s,%d,0x%x",data,vectemp[i]->tag,(CCSprite *)getChildByTag(vectemp[i]->tag));
                PtActionUtility::readSpriteActionFile(g_ActionFilePath+"movecard.plist",(CCSprite *)getChildByTag(vectemp[i]->tag),string(data));
            }
            
       }
        ((CGamesCard *)getChildByTag(vCard[goIndex]->tag))->setDead();
        runAction(CCSequence::create(CCDelayTime::create(0.3f),CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd)),NULL));
    }
}
void CFightingCardLayerScene::showHp(int leftHp,int RightHp)
{
    cout<<"left hp:"<<leftHp<<"Right hp:"<<RightHp<<endl;
    if (leftHp==0 &&RightHp==0) {
        return;
    }
    CCPoint point;
    CCLabelTTF *labelTTF=NULL;
    char data[20];
    int hpValue=leftHp;
    if(hpValue!=0)
    {
        
        CCPoint point=ccp((CCDirector::sharedDirector()->getWinSize().width*0.5-200),(CCDirector::sharedDirector()->getWinSize().height*0.5));
        CCLabelTTF *labelTTF=(CCLabelTTF *)getChildByTag(30002);
        
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
    hpValue=RightHp;
    if(hpValue!=0)
    {
        
        point=ccp((CCDirector::sharedDirector()->getWinSize().width*0.5+200),(CCDirector::sharedDirector()->getWinSize().height*0.5));
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

EN_ATKFIGHT_INDEX CFightingCardLayerScene::getHuiHeIndex()
{
    return m_enHuiheIndex;
}
void CFightingCardLayerScene::loadFromServerTest()
{
    for (int i=0; i<SinglePlayer::instance()->m_hashmapFightingCard.size(); i++)
    {
        if(SinglePlayer::instance()->m_hashmapFightingCard[i])
        {
            m_vFightingCard.push_back(new CFightCard(*SinglePlayer::instance()->m_hashmapFightingCard[i]));
        } else{
            m_vFightingCard.push_back(NULL);
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
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]);
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
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]);
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

        for (int i=0; i<SinglePlayer::instance()->m_hashmapMonsterCard.size(); i++)
        {
            if(SinglePlayer::instance()->m_hashmapMonsterCard[i])
            {
                m_vMonsterCard.push_back(new CFightCard(*SinglePlayer::instance()->m_hashmapMonsterCard[i]));
            }
            else{
                m_vMonsterCard.push_back(NULL);
            }
        }
    for (int  i=0; i<m_vMonsterCard.size(); i++)
    {
        if(i!=m_vMonsterCard.size()-1 && m_vMonsterCard[i]!=NULL)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]);
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
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]);
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

void CFightingCardLayerScene::deleteBackGamePlayerFightMonsterCard()
{
    SinglePlayer::instance()->deleteFightMonsterCard();
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

void CFightingCardLayerScene::textSkillInfo(CAnimationSpriteGameFight *fight)
{
    if (getChildByTag(20003)) {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(20003);
        char data[20];
        sprintf(data,"发送技能%d",fight->m_iSKillId);//,
        labelttf->setString(data);
        labelttf->runAction(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),CCCallFunc::create(this, callfunc_selector(CFightingCardLayerScene::AnimaitonEnd)),NULL));
        
    }
}

void CFightingCardLayerScene::yinCangRenWu(vector<CCSprite *>vsprite,CCSprite *sprite)
{
    if (!sprite)
    {
        for (int i=0; i<vsprite.size(); i++)
        {
            if(vsprite[i])
            {
                vsprite[i]->setVisible(false);
            }
        }
    }
    else
    {
        for (int i=0; i<vsprite.size(); i++)
        {
            if(vsprite[i])
            {
                if(vsprite[i]==sprite)
                {
                    vsprite[i]->setVisible(true);
                }
                else
                {
                    vsprite[i]->setVisible(false);
                }
            }
        }

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

// 创建显示文本比如，怒气，Hp信息;
void CFightingCardLayerScene::createEngryText()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("1", "Arail", 25);
    addChild(labelttf,10,87777);
    labelttf->setPosition(ccp(300,600));
    labelttf=CCLabelTTF::create("2", "Arail", 25);
    addChild(labelttf,10,87778);
    labelttf->setPosition(ccp(874,600));
}

//创建HPtext
void CFightingCardLayerScene::createHpText()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("1", "Arail", 25);
    addChild(labelttf,10,77777);
    labelttf->setPosition(ccp(150,600));
    labelttf=CCLabelTTF::create("2", "Arail", 25);
    addChild(labelttf,10,77778);
    labelttf->setPosition(ccp(724,600));
}

bool CFightingCardLayerScene::initText()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    //中间显示战斗时候的界面 显示信息
    CCLabelTTF *labelttfVersion=CCLabelTTF::create("", "Arial", 50);
    labelttfVersion->setPosition(ccp(winsize.width*0.5,winsize.height-100));
    labelttfVersion->setColor(ccc3(0, 0, 255));
    addChild(labelttfVersion,10,20003);
    return true;
}

//攻击的时候 显示的图片
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
// 初始化 背景图片
bool CFightingCardLayerScene::initBggroudPng()
{
    CCSize  winsize=CCDirector::sharedDirector()->getWinSize();
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/bgm.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    return  true;
}