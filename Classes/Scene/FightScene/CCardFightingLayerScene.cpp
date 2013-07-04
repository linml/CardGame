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
#include "CSkillManager.h"
#include "CCard.h"
#include "FightResultConfirm.h"
#include "Utility.h"
#include "PtActionUtility.h"

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
    G_SkillManager::instance()->clearAnimationList();
    CCScene *pScene=CCScene::create();
    CCardFightingLayerScene *fightScene = new CCardFightingLayerScene();
    fightScene->init();
    fightScene->autorelease();
    pScene->addChild(fightScene);
    return pScene;
    
}



CCardFightingLayerScene::CCardFightingLayerScene()
{
    m_iHuihe=1;
    m_bIsAnimationEnd=false;
    m_iAnimationIndex=0;
    m_vFightingCard.clear();
    m_vMonsterCard.clear();
    SinglePlayer::instance()->forTestMonsterCard();
}

CCardFightingLayerScene::~CCardFightingLayerScene()
{
    
    //删除特效；
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
    
    CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
    addChild(sprite,15,30001);
    sprite->setVisible(false);
    CCSprite *sprite2=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
    addChild(sprite2,15,30000);
    sprite2->setVisible(false);
    //CCSize winsize=CCDirector::sharedDirector()->getWinSize();
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
    
    //设置加血扣血的
    CCLabelTTF *labelTTf1=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf1,15,30002);
    labelTTf1->setVisible(false);
    labelTTf1->setColor(ccc3(255, 0, 0));
    
    CCLabelTTF *labelTTf2=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf2,15,30003);
    labelTTf2->setVisible(false);
    labelTTf2->setColor(ccc3(255, 0, 0));
    
    createFightCard();
    createMonsterCard();
   
    m_iFightingCardIndex=m_iMonsterCardIndex=0;
    schedule(schedule_selector(CCardFightingLayerScene::locgicSchudel));
    return true;
}

void CCardFightingLayerScene::basicAnimation(vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs)
{
    CGamesCard *m_tempCardSprite=(CGamesCard*)getChildByTag(ownFightCard[oWnindex]->tag);
   if(spfs->m_iHuihe!=0 && spfs->m_iHuihe!=-2 &&m_tempCardSprite->isAddTexiao==false)
    {
        m_tempCardSprite->isAddTexiao=true;

       Utility::runPtActionScript(m_tempCardSprite,"xuanzhe/xuanzhe.act",200);
    }
    
    cout<<"oWnindex:"<<spfs->m_spendAnimationAuthor<<" ownindex:"<<oWnindex<<" spfs->m_iShanghai"<<spfs->m_iShanghai<<" "<<m_tempCardSprite->getTag()<<endl;
    
    if(m_tempCardSprite)
    {
        CCLog("MonsteIndex:%d",MonsteIndex);
        if(spfs->m_iHuihe!=0 && spfs->m_iHuihe!=-2 )
        {
            CCLog("owniNDEX:%d",oWnindex);
            PtActionUtility::readSpriteActionFile("resource_cn/boss_icon_Effect.plist",m_tempCardSprite);
            m_tempCardSprite->runAction(CCSequence::create(
                                                           CCDelayTime::create(0.7f),
                                                           CCCallFuncN::create(m_tempCardSprite, callfuncN_selector(CCardFightingLayerScene::showJuqi)),
                                                           CCCallFuncND::create(this,callfuncND_selector(CCardFightingLayerScene::animationShouShang),(void *)&MonstFightCard[MonsteIndex]->tag), CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::setVistablHit)),CCCallFunc::create(m_tempCardSprite, callfunc_selector(CCardFightingLayerScene::hideJuqi)),
                                                           CCMoveBy::create(0.2f, ccp(0,-100)),
                                                           CCSpawn::create(
                                                                           CCCallFuncND::create(this, callfuncND_selector(CCardFightingLayerScene::animationHpFight),(void *)(ownFightCard[oWnindex])),
                                                                           CCCallFuncND::create(this, callfuncND_selector(CCardFightingLayerScene::animationHpMonster),(void *)(MonstFightCard[MonsteIndex])),NULL),
                                                           CCDelayTime::create(0.2f),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::updateSetHp)), CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::setHideHpAnimation)),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::AnimaitonEnd))  ,NULL));
        }
        else
        {
            CCLog("owniNDEX:%d",oWnindex);
            m_tempCardSprite->runAction(CCSequence::create(
                                                           CCMoveBy::create(0.2f, ccp(0,100)),
                                                           CCCallFuncND::create(this, callfuncND_selector(CCardFightingLayerScene::animationCardPanel),(void *)&ownFightCard[oWnindex]->tag),
                                                           CCCallFuncND::create(this,callfuncND_selector(CCardFightingLayerScene::animationShouShang),(void *)&MonstFightCard[MonsteIndex]->tag), CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::setVistablHit)),
                                                           CCMoveBy::create(0.2f, ccp(0,-100)),
                                                           CCSpawn::create(
                                                                           CCCallFuncND::create(this, callfuncND_selector(CCardFightingLayerScene::animationHpFight),(void *)(ownFightCard[oWnindex])),
                                                                           CCCallFuncND::create(this, callfuncND_selector(CCardFightingLayerScene::animationHpMonster),(void *)(MonstFightCard[MonsteIndex])),NULL),
                                                           CCDelayTime::create(0.2f),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::updateSetHp)), CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::setHideHpAnimation)),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::AnimaitonEnd)),NULL));
        }
    }
    
}

void CCardFightingLayerScene::animationSwf(int skillIndex,vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs)
{
    this->m_sfps=spfs;
    switch (skillIndex) {
        case 0:
        {
            CCLog("普通攻击");
            basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
        }
            break;
            default:
            break;
    }
}
void CCardFightingLayerScene::MoveCardSprite(vector<CFightCard *> &vCard)
{
    if(vCard.size()<=2)
    {
        return;
    }
    else
    {
        CCPoint  point;
        for (int i=0; i<vCard.size()-1; i++)
        {
            if(i==0)
            {
                point=getChildByTag(vCard[vCard.size()-2]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else if(i==vCard.size()-2)
            {
                point=getChildByTag(vCard[i-1]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::AnimaitonEnd)),NULL));
            }
            else
            {
                point=getChildByTag(vCard[i-1]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCMoveTo::create(0.2f,point)); 
            }
        }
    }
}
void CCardFightingLayerScene::fSchudelUpdate(float t)
{
    if(m_bIsAnimationEnd)
    {
        m_bIsAnimationEnd=false;
        if(m_iAnimationIndex <G_SkillManager::instance()->getAnimationVector().size())
        {
            SAnimationFps *saFps=G_SkillManager::instance()->getAnimationVector()[m_iAnimationIndex];
            switch (saFps->m_iHuihe)
            {
                case 1:
                case 0:
                    animationSwf(saFps->m_skillIndex,m_vFightingCard,m_vMonsterCard,saFps->m_spendAnimationAuthor,
                                 m_iMonsterCardIndex,saFps);
                    break;
                case -1:
                case -2:
                    animationSwf(saFps->m_skillIndex,m_vMonsterCard,m_vFightingCard,saFps->m_spendAnimationAuthor,m_iFightingCardIndex,saFps);
                    break;
                case 2:
                {
                    CGamesCard  *node= ((CGamesCard *) getChildByTag(m_vFightingCard[m_iFightingCardIndex]->tag));
                    if(node)
                    {
                        node->setDead();
                        
                    }
                    MoveCardSprite(m_vFightingCard);
                    if(node->isAddTexiao)
                    {
                        Utility::stopPtActionScript(node, 200);
                    }
                    m_iFightingCardIndex++;
                    
                }
                    break;
                case 3:
                {
                    CGamesCard  *node= ((CGamesCard *) getChildByTag(m_vMonsterCard[m_iMonsterCardIndex]->tag));
                    if(node)
                    {
                        node->setDead();
                    }
                    MoveCardSprite(m_vMonsterCard);
                    if(node->isAddTexiao)
                    {
                        Utility::stopPtActionScript(node, 200);
                    }
                    m_iMonsterCardIndex++;
                }
                default:
                    break;
            }
        }
        else{
            m_iWinStatus=getWinStats();
            if(m_iWinStatus==1)
            {
                winDialog();
            }
            else if(m_iWinStatus==-1)
            {
                loseDialog();
            }
            unschedule(schedule_selector(CCardFightingLayerScene::fSchudelUpdate));
        }
        
    } 
}


void CCardFightingLayerScene::loseDialog()
{
    int tmp = 0;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 100000);
    
    cout<<"lose"<<endl;
}

void CCardFightingLayerScene::winDialog()
{
    int tmp = 1;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 1000000);
    
    cout<<"win"<<endl;
}

int CCardFightingLayerScene::getWinStats()
{
    if(m_vFightingCard.size()==0)
    {
        return -1;
    }
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


void CCardFightingLayerScene::checkMonsterIsDeadAndMove()
{
    if (m_vMonsterCard[m_iMonsterCardIndex]->isDead)
    {
        G_SkillManager::instance()->fightDead(3);
        m_iMonsterCardIndex++;
    }
}
void CCardFightingLayerScene::checkOwnIsDeadAndMove()
{
    if (m_vFightingCard[m_iFightingCardIndex]->isDead)
    {
        G_SkillManager::instance()->fightDead(2);
        m_iFightingCardIndex++;
    }
}
void CCardFightingLayerScene::animationCardPanel(class CCNode *card, void *tag)
{
    if(getChildByTag(30000))
    {
        CCSprite *sprite=(CCSprite *)getChildByTag(30000);
        int spritetag=*(int*)tag;
        CCPoint point=getChildByTag(spritetag)->getPosition();
        sprite->setPosition(ccp(point.x+50,point.y+100));
        sprite->setVisible(true);
    }
    else
    {
        CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
        addChild(sprite,10,30000);
        CCPoint point=card->getPosition();
        sprite->setPosition(ccp(point.x+50,point.y+100));
    }
}

void CCardFightingLayerScene::AnimaitonEnd(CCObject *object)
{
    m_iAnimationIndex++;
    m_bIsAnimationEnd=true;
   
}
void CCardFightingLayerScene::showJuqi(CCNode *node,void *tag)
{
  Utility::runPtActionScript(node, "juqi1/juqi1.act", 100);
}
void CCardFightingLayerScene::animationShouShang(CCNode *node,void *tag)
{
    if(getChildByTag(30001))
    {
        CCSprite *sprite=(CCSprite *)getChildByTag(30001);
        int spritetag=*(int*)tag;
        sprite->setPosition(getChildByTag(spritetag)->getPosition());
        sprite->setVisible(true);
    }
    else
    {
        CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
        addChild(sprite,10,30001);
        int spritetag=*(int*)tag;
        sprite->setPosition(getChildByTag(spritetag)->getPosition());
    }
}
void CCardFightingLayerScene::setVistablHit(CCObject *object)
{
    if(getChildByTag(30000))
    {
    getChildByTag(30000)->setVisible(false);
    }
    if(getChildByTag(30001))
       {
            getChildByTag(30001)->setVisible(false);
       }
    
}
void CCardFightingLayerScene::hideJuqi(CCNode *node,void *tag)
{
    Utility::stopPtActionScript(node, 100);
}
void CCardFightingLayerScene::animationHpFight(CCNode *node,void *tag)
{
    int hpValue=m_sfps->m_iJiaxue;
    
    if(hpValue==0)
    {
        return;
    }
    CFightCard *sprite=(CFightCard *)tag;
    CCPoint point=getChildByTag(sprite->tag)->getPosition();
    CCLabelTTF *labelTTF=(CCLabelTTF *)getChildByTag(30002);
    char data[20];
    
    sprintf(data, "%d",hpValue*(-1));
    labelTTF->setString(data);
    labelTTF->setVisible(true);
    if(hpValue>0)
    {
        labelTTF->setPosition(ccp(point.x,point.y+100));
        labelTTF->runAction(CCMoveBy::create(0.2f,CCPoint(0, -100)));
    }
    else
    {
        labelTTF->setPosition(ccp(point.x,point.y));
        labelTTF->runAction(CCMoveBy::create(0.2f,CCPoint(0, 100)));
    }

}
void CCardFightingLayerScene::animationHpMonster(CCNode *node,void *tag)
{
    int hpValue=this->m_sfps->m_iShanghai;
    CFightCard *sprite=(CFightCard *)tag;
    if(hpValue==0)
    {
        return;
    }
    CCLog("sprite->tag:%d",sprite->tag);
    CCPoint point=this->getChildByTag(sprite->tag)->getPosition();// ();
    CCLabelTTF *labelTTF=(CCLabelTTF *)getChildByTag(30003);
    labelTTF->setVisible(true);
    char data[20];
    
    sprintf(data, "%d",hpValue*(-1));
    cout<<"hpvalue:"<<hpValue<<endl;
    labelTTF->setString(data);
    if(hpValue>0)
    {
        labelTTF->setPosition(ccp(point.x,point.y));
        labelTTF->runAction(CCMoveBy::create(0.2f,CCPoint(0, 100)));
    }
    else
    {
        
        labelTTF->setPosition(ccp(point.x,point.y+100));
        labelTTF->runAction(CCMoveBy::create(0.2f,CCPoint(0, -100)));
        
    }

}
void CCardFightingLayerScene::updateSetHp(CCObject *object)
{
    
}
void CCardFightingLayerScene::setHideHpAnimation(CCObject *obj)
{
    getChildByTag(30003)->setVisible(false);
    getChildByTag(30002)->setVisible(false);
}


static int tempaaaa=0;
void CCardFightingLayerScene::locgicSchudel(float t)
{

    int winStatus=getWinStats();
    //先判断是否全部死亡；
    if(winStatus==0)
    {
        if(!m_vFightingCard[m_iFightingCardIndex]->isDead && !m_vMonsterCard[m_iMonsterCardIndex]->isDead)
        {
            fightLogic(m_iHuihe);
        }
        else
        {
            checkOwnIsDeadAndMove();
            checkMonsterIsDeadAndMove();
        }
    }
    else
    {
        unschedule(schedule_selector(CCardFightingLayerScene::locgicSchudel));
       
        m_iFightingCardIndex=0;
        m_iMonsterCardIndex=0;
       
        if(G_SkillManager::instance()->getAnimationVector().size()>0)
        {
            m_bIsAnimationEnd=true;
            CCLog("cout :  winstatus=+-1");
            schedule(schedule_selector(CCardFightingLayerScene::fSchudelUpdate));
        }
    }
}

void CCardFightingLayerScene::fightLogic(int &iHuihe)
{
    switch (iHuihe)
    {
        case 1:
            G_SkillManager::instance()->refactorJisuan(0, m_vFightingCard, m_vMonsterCard, m_iFightingCardIndex, m_iMonsterCardIndex,1);
            break;
        case 0:
        {
            G_SkillManager::instance()->refactorJisuan(0, m_vFightingCard, m_vMonsterCard, m_vFightingCard.size()-1, m_iMonsterCardIndex,0);
        }
            break;
        case -1:
        {
            G_SkillManager::instance()->refactorJisuan(0, m_vMonsterCard,m_vFightingCard,m_iMonsterCardIndex, m_iFightingCardIndex,-1);
        }
            break;
        case -2:
        {
            G_SkillManager::instance()->refactorJisuan(0, m_vMonsterCard,m_vFightingCard,m_vMonsterCard.size()-1, m_iFightingCardIndex,-2);
        }
            break;
        default:
            break;
    }
    
    if(iHuihe==-2)
    {
        tempaaaa++;
//        cout<<"   huihe:"<<tempaaaa<<endl;
        iHuihe=1;
    }
    else
    {
        iHuihe--;
    }
}

void CCardFightingLayerScene::createFightCard()
{
    cout<<"SinglePlayer::instance()->m_hashmapFight.size():"<<SinglePlayer::instance()->m_hashmapFight.size()<<endl;
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    if(SinglePlayer::instance()->isLoadServer)
    {
        for (int i=0; i<SinglePlayer::instance()->m_hashmapFightingCard.size();i++)
        {
            m_vFightingCard.push_back(new CFightCard(*(SinglePlayer::instance()->m_hashmapFightingCard[i])));
        }

    }
    else
    {
        for (int i=0; i<SinglePlayer::instance()->m_hashmapFight.size();i++)
        {
            m_vFightingCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapFight[i]));
        }
    }
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        if(i!=m_vFightingCard.size()-1)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard,false);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            addChild(gameCard,i+5,m_vFightingCard[i]->tag);
            gameCard->setPosition(ccp(winsize.width/2-100-i*118,winsize.height*0.5));
        }
        else
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vFightingCard[i]->m_pCard,true);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            gameCard->setPosition(ccp(20,20));
            gameCard->setAnchorPoint(CCPointZero);
            addChild(gameCard,i+5,m_vFightingCard[i]->tag);
        }
    }
    
}

void CCardFightingLayerScene::createMonsterCard()
{
    CCSize wndsize=CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<SinglePlayer::instance()->m_hashmapMonster.size(); i++)
    {
        m_vMonsterCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapMonster[i]));
    }
    for (int  i=0; i<m_vMonsterCard.size(); i++) {
        if(i!=m_vMonsterCard.size()-1)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vMonsterCard[i]->m_pCard,false);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            addChild(gameCard,i+5, m_vMonsterCard[i]->tag);
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
            addChild(gameCard,i+5, m_vMonsterCard[i]->tag);
        }
    }
}