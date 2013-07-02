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
    m_iHuihe=1;
    m_bIsAnimationEnd=false;
    m_iAnimationIndex=0;
    m_vFightingCard.clear();
    m_vMonsterCard.clear();
    SinglePlayer::instance()->forTestMonsterCard();
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
    schedule(schedule_selector(CCardFightingLayerScene::locgicSchudel));
    return true;
}

void CCardFightingLayerScene::basicAnimation(vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs)
{
  
    /*
    CCardPanel *m_tempCardSprite=(CCardPanel*)getChildByTag(ownFightCard[oWnindex]->tag);
//    if(spfs->m_iHuihe!=0 && spfs->m_iHuihe!=-2 &&m_tempCardSprite->isAddTexiao==false)
//    {
//        m_tempCardSprite->isAddTexiao=true;
//
//        Utility::runPtActionScript(m_tempCardSprite,"xuanzhe/xuanzhe.act",200);
//    }
    
    cout<<"oWnindex"<<spfs->m_iJiaxue<<"spfs->m_iShanghai"<<spfs->m_iShanghai<<" "<<m_tempCardSprite<<endl;
    
    if(m_tempCardSprite)
    {
        if(spfs->m_iHuihe!=0 && spfs->m_iHuihe!=-2 )
        {
            m_tempCardSprite->runAction(CCSequence::create(
                                                           CCMoveBy::create(0.2f, ccp(0,100)),
                                                           CCCallFuncN::create(m_tempCardSprite, callfuncN_selector(CFightingLayerScene::showJuqi)),
                                                           CCCallFuncND::create(this,callfuncND_selector(CFightingLayerScene::animationShouShang),(void *)&MonstFightCard[MonsteIndex]->tag), CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::setVistablHit)),CCCallFunc::create(m_tempCardSprite, callfunc_selector(CFightingLayerScene::hideJuqi)),
                                                           CCMoveBy::create(0.2f, ccp(0,-100)),
                                                           CCSpawn::create(
                                                                           CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::animationHpFight),(void *)(ownFightCard[m_vFightCardIndex])),
                                                                           CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::animationHpMonster),(void *)(MonstFightCard[MonsteIndex])),NULL),
                                                           CCDelayTime::create(0.2f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::updateSetHp)), CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::setHideHpAnimation)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation))  ,NULL));
        }
        else
        {
            m_tempCardSprite->runAction(CCSequence::create(
                                                           CCMoveBy::create(0.2f, ccp(0,100)),
                                                           CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::animationCardPanel),(void *)&ownFightCard[oWnindex]->tag),
                                                           CCCallFuncND::create(this,callfuncND_selector(CFightingLayerScene::animationShouShang),(void *)&MonstFightCard[MonsteIndex]->tag), CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::setVistablHit)),
                                                           CCMoveBy::create(0.2f, ccp(0,-100)),
                                                           CCSpawn::create(
                                                                           CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::animationHpFight),(void *)(ownFightCard[m_vFightCardIndex])),
                                                                           CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::animationHpMonster),(void *)(MonstFightCard[MonsteIndex])),NULL),
                                                           CCDelayTime::create(0.2f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::updateSetHp)), CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::setHideHpAnimation)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
        }
    }
     */
}

void CCardFightingLayerScene::animationSwf(int skillIndex,vector<CFightCard *>ownFightCard,vector<CFightCard *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs)
{
    switch (skillIndex) {
        case 0:
        {
            //c("普通攻击");
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
        for (int i=0; i<=vCard.size()-1; i++)
        {
            if(i==0)
            {
                point=getChildByTag(vCard[vCard.size()-2]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else if(i!=vCard.size()-2)
            {
                point=getChildByTag(vCard[i-1]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else
            {
                point=getChildByTag(vCard[i-1]->tag)->getPosition();
                getChildByTag(vCard[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CCardFightingLayerScene::AnimaitonEnd)),NULL));
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
                        
//                        if(((CGamesCard*)(node))->isAddTexiao)
//                        {
//                            Utility::stopPtActionScript(node, 200);
//                        }
                    }
                    MoveCardSprite(m_vFightingCard);
                    m_iFightingCardIndex++;
//                    setHp(m_vfightCardSprite[m_iFightingCardIndex],(CCLabelTTF *) getChildByTag(20000));
                    
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
                    m_iMonsterCardIndex++;
                }
                default:
                    break;
            }

            
            m_iAnimationIndex++;
            
        }
        else{
            unschedule(schedule_selector(CCardFightingLayerScene::fSchudelUpdate));
        }
        
    } 
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
        //initMonsterHuihe();
    }
}
void CCardFightingLayerScene::checkOwnIsDeadAndMove()
{
    if (m_vFightingCard[m_iFightingCardIndex]->isDead)
    {
        G_SkillManager::instance()->fightDead(2);
        m_iFightingCardIndex++;
        //initOwnHuihe();
    }
}

void CCardFightingLayerScene::AnimaitonEnd(CCObject *object)
{
    m_bIsAnimationEnd=true;
}

static int tempaaaa=0;
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
        else
        {
            checkOwnIsDeadAndMove();
            checkMonsterIsDeadAndMove();
        }
    }
    else
    {
        unschedule(schedule_selector(CCardFightingLayerScene::locgicSchudel));
        CCLog("cout :  winstatus=+-1");
        m_iFightingCardIndex=0;
        m_iMonsterCardIndex=0;
        if(G_SkillManager::instance()->getAnimationVector().size()>0)
        {
            schedule(schedule_selector(CCardFightingLayerScene::fSchudelUpdate));
        }
    }
}

void CCardFightingLayerScene::fightLogic(int &iHuihe)
{
    cout<<"huihe:"<<iHuihe<<endl;
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
        cout<<"   huihe:"<<tempaaaa<<endl;
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