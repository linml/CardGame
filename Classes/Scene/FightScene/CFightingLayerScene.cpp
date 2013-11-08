//
//  CFightingLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 11/5/13.
//
//

#include "CFightingLayerScene.h"
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
#include "CFightCardBufferData.h"
#include "CGameCardBuffer.h"
#include "CPtTool.h"
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


CCScene *CFightingLayerScene::scene()
{
    CCScene *pScene=CCScene::create();
    CFightingLayerScene *fightScene = new CFightingLayerScene();
    fightScene->init();
    fightScene->autorelease();
    pScene->addChild(fightScene);
    return pScene;
    
}

CFightingLayerScene::CFightingLayerScene()
{
    m_gamePlayer=SinglePlayer::instance();
    m_pSFightData=new SFightResultData();
    if(m_gamePlayer->getFightKuaijin())
    {
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(2.0);
    }

}

CFightingLayerScene::~CFightingLayerScene()
{
    if(m_pSFightData)
    {
        delete m_pSFightData;
    }
    m_gamePlayer->onFightExitScene();
    G_FightSkillManager::instance()->clearAnimationList();
    //释放特效文件
}

bool CFightingLayerScene::init()
{
    m_nAnimationAndex=0;
    m_nHpEngryIndex=0;
    m_nBufferIndex=0;
    m_bAnimationEnd=true;
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
    createKuaiJin();
    createShowFightUid();

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(1);
    m_nTotalAnimation=G_FightSkillManager::instance()->m_vAnimationStrip.size();
    runAction(CCSequence::create(CCDelayTime::create(1.0f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::delayToSchude)),NULL));
    return true;
}
void CFightingLayerScene::createShowFightUid() //显示对战适合的 对方阵容的UID
{
    char data[20];
    sprintf(data, "FIHGTUID:%d",m_gamePlayer->getFightUid());
    CCLabelTTF *labelttf=CCLabelTTF::create(data, "Arial", 20);
    labelttf->setPosition(ccp(800, 700));
    addChild(labelttf,2,1000000);
    CCFadeIn *fadein = CCFadeIn::create(1.0f);//(1.2f,ccp(0,10));
    labelttf->runAction(CCRepeatForever::create(static_cast<CCSequence *>(CCSequence::create(fadein,fadein->reverse(),NULL))));
}

void CFightingLayerScene::delayToSchude(CCObject *object)
{
    schedule(schedule_selector(CFightingLayerScene::animationSchudel));
}

void CFightingLayerScene::onEnter()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(
                                                                  this,
                                                                  callfuncO_selector(CFightingLayerScene::rePlayerRecode), // 处理的消息的回调函数
                                                                  "CONGTOUBOFANG", // 感兴趣的消息名称
                                                                  NULL); // 保存消息中传递的数据
    CCLayer::onEnter();
}
void CFightingLayerScene::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(
                                                                     this,
                                                                     "CONGTOUBOFANG" // 感兴趣的消息名称
                                                                     ); // 保存消息中传递的数据
    CCLayer::onExit();
    
}
void CFightingLayerScene::rePlayerRecode()
{
    m_nAnimationAndex=0;
    m_nHpEngryIndex=0;
    m_nBufferIndex=0;
    resetCardPosition();
    hideAllHero();
    initHpEngry();
    clearUpVectorBuffer();
    m_bAnimationEnd=true;
    schedule(schedule_selector(CFightingLayerScene::animationSchudel));
    
}

bool CFightingLayerScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CFightingLayerScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CFightingLayerScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(getChildByTag(911), pTouch))
    {
        if((int)(CCDirector::sharedDirector()->getScheduler()->getTimeScale()+0.0005)==1)
        {
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(2.0);
            m_gamePlayer->setFightKuaijin(true);
        }
        else
        {
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0);
            m_gamePlayer->setFightKuaijin(false);
        }
    }
}
void CFightingLayerScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

CCPoint CFightingLayerScene::getBufferIconPostion(int index,bool isLeft)
{
    CCPoint point=CCPointZero;
    if(isLeft)
    {
        point.y=700;
        point.x=100+50*index;
    }
    else{
        point.y=700;
        point.x=800-50*index;
    }
    return point;
}

void CFightingLayerScene::showSkill(CCSprite *pFightSprite,CCSprite *pMonsterSprite2,int skillid,CAnimationSctrip *fightAnimation)
{
    if(pFightSprite)
    {
        string filePath;
        if (fightAnimation->m_sRunActionFile.empty())
        {
            CCLog("策划没有配置该技能的特效文件");
            AnimaitonEnd();
            return;
        }
        if (fightAnimation->m_enAnimationType!=EN_ANIMATIONTYPEREFACTOR_BUFFER_OWN &&fightAnimation->m_enAnimationType!=EN_ANIMATIONTYPEREFACTOR_BUFFER_MONSTER )
        {
            if (fightAnimation->m_enAtkFightIndex == EN_ATKFIGHT_INDEX_RIGHT_LORD ||
                fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_RIGHT_SUPPORT)
            {
                filePath = fightAnimation->m_sRunActionFile+"_r.plist";
            }
            else if(fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_LEFT_LORD ||
                fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_LEFT_SUPPORT)
            {
                filePath =fightAnimation->m_sRunActionFile+ "_l.plist";
            }
        }
        else{
            filePath=fightAnimation->m_sRunActionFile;
        }
        //播放特效文件
        CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath);
        CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd));
        if (fightAnimation->m_enAtkFightIndex == EN_ATKFIGHT_INDEX_LEFT_LORD || fightAnimation->m_enAtkFightIndex ==EN_ATKFIGHT_INDEX_RIGHT_LORD)
        {
            reorderChild(pFightSprite, 2);
            CCCallFuncN *rebb=CCCallFuncN::create(this, callfuncN_selector(CFightingLayerScene::actionReorderZorderNode));
            pFightSprite->runAction(CCSequence::create((CCFiniteTimeAction*)animation,CCDelayTime::create(0.6f),rebb,endAnimation,NULL));
        }
        else
        {
             pFightSprite->runAction(CCSequence::create((CCFiniteTimeAction*)animation,CCDelayTime::create(0.6f),endAnimation,NULL));
        }

        if(pMonsterSprite2)
        {
            CCAction *action=PtActionUtility::getRunActionWithActionFile(filePath,"shoushang");
            if (action)
            {
                pMonsterSprite2->runAction(action);
            }
        }
    }
    else
    {
        AnimaitonEnd();
    }
}

void CFightingLayerScene::skillAnimationSwf(CAnimationSctrip *fightAnimation,CCSprite *pFight,CCSprite *pMonster)
{
    
    switch (fightAnimation->m_enAnimationType)
    {
            
        case    EN_ANIMATIONTYPEREFACTOR_SKILL:     // 当英雄攻击别人的时候发招的 动画
        case    EN_ANIMATIONTYPEREFACTOR_BUFFER_OWN:   // 当发现自身带buffer 的时候要展示动画的标志
                showSkill(pFight,pMonster,fightAnimation->m_nSKillId, fightAnimation);
            break;
        case    EN_ANIMATIONTYPEREFACTOR_BUFFER_MONSTER:
                showSkill(pMonster,pFight,fightAnimation->m_nSKillId, fightAnimation);
            break;
        case EN_ANIMATIONTYPEREFACTOR_REMOVEPLIST:
            if(pFight)
            {
                CCLog("fightAnimation->m_nSpritetag %d",fightAnimation->m_nSpritetag);
                pFight->removeChildByTag(fightAnimation->m_nSpritetag, true);
                AnimaitonEnd();
            }
            break;
        case    EN_ANIMATIONTYPEREFACTOR_DEADMOVE:   // 当防卫的图标消失的时候
        {
            if (fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_RIGHT_MOVE)
            {
                 moveCardSprite(m_vMonsterCard,m_currCAnimationHP->m_nATKindex,true);
            }
            else if(fightAnimation->m_enAtkFightIndex==EN_ATKFIGHT_INDEX_LEFT_MOVE)
            {

                moveCardSprite(m_vFightingCard,m_currCAnimationHP->m_nATKindex,true);
            }
            
        }
            break;
        case    EN_ANIMATIONTYPEREFACTOR_SHANGHAI:
                showHpShanghai(pFight,pMonster,fightAnimation->m_nSKillId,fightAnimation);
            break;
        default:
            break;
    }
}


void CFightingLayerScene::showHpShanghai(cocos2d::CCSprite *pFightSprite, cocos2d::CCSprite *pMonsterSprite2, int skillid, CAnimationSctrip *fightAnimation)
{
    switch (fightAnimation->m_enAtkFightIndex)
    {
        case EN_ATKFIGHT_INDEX_LEFT_LORD:
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
            showHp(fightAnimation->m_nAddHp, fightAnimation->m_nSubHp);
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
        case EN_ATKFIGHT_INDEX_RIGHT_LORD:
            showHp(fightAnimation->m_nSubHp, fightAnimation->m_nAddHp);
        default:
            break;
    }
}


void CFightingLayerScene::animationSwf(CAnimationSctrip *fightAnimation)
{
    CCLog("animationSwf,%d",(int)fightAnimation->m_enAtkFightIndex);
    switch (fightAnimation->m_enAtkFightIndex)
    {
        case EN_ATKFIGHT_INDEX_LEFT_LORD:
        {
            if(m_vFightHero[fightAnimation->m_nATKindex])
            {
                CCLog("%d/%d",fightAnimation->m_nATKindex,fightAnimation->m_nDEFindex);
                CCSprite *pFightsprite=m_vFightHero[fightAnimation->m_nATKindex];
                CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_nDEFindex];
                skillAnimationSwf(fightAnimation,pFightsprite,pMonster);
            }
            else
            {
                AnimaitonEnd();
                return;
            }
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_SUPPORT:
        {
            if(m_vFightHero[4])
            {
                //CCSprite *sprite=m_vFightHero[fightAnimation->m_nATKindex];
                CCSprite *pMonster=m_vMonsterHero[fightAnimation->m_nDEFindex];
                CGamesCard *gamecard=(CGamesCard *)getChildByTag(m_vFightingCard[4]->tag);
                skillAnimationSwf(fightAnimation,gamecard,pMonster);
            }
            else
            {
                AnimaitonEnd();
                return;
            }
            
        }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_LORD:
        {
            if(m_vMonsterHero[fightAnimation->m_nATKindex])
            {
                CCSprite *sprite=m_vMonsterHero[fightAnimation->m_nATKindex];
                CCSprite *pMonster=m_vFightHero[fightAnimation->m_nDEFindex];
                skillAnimationSwf(fightAnimation,sprite,pMonster);
            }
            else{
                AnimaitonEnd();
                return;
            }
            
        }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_SUPPORT:
        {
            if(m_vMonsterHero[4])
            {
                //CCSprite *sprite=m_vMonsterHero[fightAnimation->m_nATKindex];
                CCSprite *pMonster=m_vFightHero[fightAnimation->m_nDEFindex];
                 CGamesCard *gamecard=(CGamesCard *)getChildByTag(m_vMonsterCard[4]->tag);
                skillAnimationSwf(fightAnimation,gamecard,pMonster);
            }
            else
            {
                AnimaitonEnd();
                return;
            }
            
        }
            break;
        case EN_ATKFIGHT_INDEX_LEFT_MOVE:
            CCLog("animationAndex====%d",m_nAnimationAndex);
            updateHpAndAngry();
            updateBuffer();
            m_vFightHero[m_currCAnimationHP->m_nATKindex]->setVisible(false);
            moveCardSprite(m_vFightingCard,m_currCAnimationHP->m_nATKindex,true);//移动 card
            if(m_currCAnimationHP->m_nATKindex+1<m_vMonsterHero.size()-1 &&m_vFightingCard[m_currCAnimationHP->m_nATKindex+1])
            {
                m_vFightHero[m_currCAnimationHP->m_nATKindex+1]->setVisible(true);
            }
            break;
        case EN_ATKFIGHT_INDEX_RIGHT_MOVE:
        {
            updateHpAndAngry();
            updateBuffer();
            m_vMonsterHero[m_currCAnimationHP->m_nATKindex]->setVisible(false);
            moveCardSprite(m_vMonsterCard,m_currCAnimationHP->m_nATKindex,false);//移动 card
            int valueIndex=m_currCAnimationHP->m_nATKindex+1;
            while (m_vMonsterHero[valueIndex]==NULL)
            {
                valueIndex++;
                if (valueIndex==m_vMonsterHero.size()-1) {
                    break;
                }
            }
            if(valueIndex<m_vMonsterHero.size()-1)
            {
                m_vMonsterHero[valueIndex]->setVisible(true);
                break;
            }
            
        }
            break;
        default:
            break;
    }
    m_nAnimationAndex++;
    updateHpAndAngry(); //每次攻击可能增加怒气
    updateBuffer();

}
void CFightingLayerScene::animationSchudel(float t)
{
    if (m_bAnimationEnd && m_nAnimationAndex<m_nTotalAnimation) {

        CCLog("animationAndex:%d,%d",m_bAnimationEnd,m_nAnimationAndex);
        m_bAnimationEnd=false;
        CAnimationSctrip *fightAnimation=G_FightSkillManager::instance()->m_vAnimationStrip[m_nAnimationAndex];
        m_currCAnimationHP=fightAnimation;
        animationSwf(fightAnimation);

    }
    else  if(m_nAnimationAndex==m_nTotalAnimation)
    {
        unschedule(schedule_selector(CFightingLayerScene::animationSchudel));
        CCLog("end animation");
        m_enWinStatus=m_gamePlayer->getWinOrLoseStatus();
        CPtTool::logMemoryInfo();
        clearAllHeroTexiao();
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

void CFightingLayerScene::winDialog()
{
    
    if(!m_pSFightData->getHuiFang())
    {
        m_pSFightData->setFightResult(1);
    }
    
    // int tmp = 1;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)m_pSFightData);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 1000000);
    cout<<"win"<<endl;
}
void CFightingLayerScene::loseDialog()
{
    if(!m_pSFightData->getHuiFang())
    {
        m_pSFightData->setFightResult(0);
    }
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)m_pSFightData);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 100000);
    cout<<"lose"<<endl;
}

void CFightingLayerScene::initSetUpdateHp(int iCurrHp,int TotalHp,int currEngry,bool isLeft)
{
    int hpTag=77777;
    int engryTag=87777;
    if(!isLeft)
    {
        hpTag=77778;
        engryTag=87778;
    }
    char data[20];
    sprintf(data, "HP:%d/%d",iCurrHp,TotalHp);
    CCLabelTTF *labelTTF=(CCLabelTTF*)getChildByTag(hpTag);
    labelTTF->setString(data);
    sprintf(data, "ENGRY:%d",currEngry);
    labelTTF=(CCLabelTTF*)getChildByTag(engryTag);
    labelTTF->setString(data);

}

void CFightingLayerScene::hideAllHero()
{
    for (int i=0; i<m_vFightHero.size(); i++) {
        if(m_vFightHero[i])
        {
            m_vFightHero[i]->setVisible(false);
        }
    }
    for (int i=0; i<m_vMonsterHero.size(); i++)
    {
        if(m_vMonsterHero[i])
        {
            m_vMonsterHero[i]->setVisible(false);
        }
    }
}

void CFightingLayerScene::initHpEngry()
{
    updateHpAndAngry();
    for (int i=0; i<m_vFightHero.size(); i++)
    {
        if(m_vFightHero[i])
        {
            m_vFightHero[i]->setVisible(true);
            break;
        }
    }
    for (int i=0; i<m_vMonsterHero.size(); i++)
    {
        if(m_vMonsterHero[i])
       {
            m_vMonsterHero[i]->setVisible(true);
            break;
        }
    }
}
void CFightingLayerScene::showVectorBuffer()
{
    CCLog("png count %d, %d",m_leftBuffer.size(),m_rightBuffer.size());
    for (int i=0; i<m_leftBuffer.size(); i++) {
        m_leftBuffer[i]->setPosition(getBufferIconPostion(i, true));
        addChild(m_leftBuffer[i],100);
    }
    for (int i=0; i<m_rightBuffer.size(); i++) {
        m_rightBuffer[i]->setPosition(getBufferIconPostion(i, false));
        addChild(m_rightBuffer[i],100);
    }
    
}
void CFightingLayerScene::clearUpVectorBuffer()
{
    for (int i=0; i<m_leftBuffer.size(); i++)
    {
        if(m_leftBuffer[i])
        {
            removeChild(m_leftBuffer[i],true);
        }
        
    }
    for (int i=0; i<m_rightBuffer.size(); i++)
    {
        if (m_rightBuffer[i])
        {
            removeChild(m_rightBuffer[i],true)  ;
        }
    }
    
    m_leftBuffer.erase(m_leftBuffer.begin(), m_leftBuffer.end());
    m_leftBuffer.clear();
    m_rightBuffer.erase(m_rightBuffer.begin(), m_rightBuffer.end());
    m_rightBuffer.clear();
}

void CFightingLayerScene::updateBuffer()
{
    
    if(m_nBufferIndex < m_gamePlayer->getFightCardFightingBuffer().size())
    {
        CFightCardFightingBuffer *eveyatk=m_gamePlayer->getFightCardFightingBuffer()[m_nBufferIndex];
        if(!eveyatk||eveyatk->m_nActionSkillIndex!=m_nAnimationAndex)
        {
            if (!eveyatk) {
                while (!m_gamePlayer->getFightCardFightingBuffer()[m_nBufferIndex]) {
                    m_nBufferIndex++;
                }
            }
           return ;
        }
        if(eveyatk)
        {
            clearUpVectorBuffer();
            CGamePlayer *player=m_gamePlayer;
            for (int i=0; i<eveyatk->m_vbufferList.size(); i++)
            {
                if(eveyatk->m_vbufferList[i]->isLeft)
                {
                    const char *pngIcon=player->getBufferPngByEffectId(eveyatk->m_vbufferList[i]->m_iEffectid).c_str();
                    if(strlen(pngIcon)>0)
                    {
                        CGameCardBuffer *gameBuffer=CGameCardBuffer::CreateBuffer(pngIcon, eveyatk->m_vbufferList[i]->m_iValue);
                        this->m_leftBuffer.push_back(gameBuffer);
                    }
                    else{
                        CCLog("eveyatk->m_vbufferList[i]->m_iEffectid:%d",eveyatk->m_vbufferList[i]->m_iEffectid);
                    }
                }
                else{
                    const char *pngIcon=player->getBufferPngByEffectId(eveyatk->m_vbufferList[i]->m_iEffectid).c_str();
                    //CCLog("png path:%s,%d",pngIcon,eveyatk->m_vbufferList[i]->m_iEffectid);
                    if(strlen(pngIcon)>0)
                    {
                        CGameCardBuffer *gameBuffer=CGameCardBuffer::CreateBuffer(pngIcon, eveyatk->m_vbufferList[i]->m_iValue);
                        this->m_rightBuffer.push_back(gameBuffer);
                    }
                }
            }
        }
        showVectorBuffer();
        do {
            m_nBufferIndex++;
        } while (!m_gamePlayer->getFightCardFightingBuffer()[m_nBufferIndex]);
        
    }
    
}
void CFightingLayerScene::updateHpAndAngry()
{
    

    cout<<"update====="<<m_nHpEngryIndex<<"/"<<m_gamePlayer->getHpAngryVectory().size()<<endl;
    if(m_nHpEngryIndex < m_nHpEngryIndex<m_gamePlayer->getHpAngryVectory().size())
    {
        SEveryATKData *eveyatk=m_gamePlayer->getHpAngryVectory()[m_nHpEngryIndex];
        if(eveyatk->m_nCurrAction!=m_nAnimationAndex)
        {
            return ;
        }
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
        m_nHpEngryIndex++;
    }
    
}

void CFightingLayerScene::createHero()
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

/*
void CFightingLayerScene::showSkillBuffer(cocos2d::CCSprite *pFightSprite, cocos2d::CCSprite *pMonsterSprite2, int skillid, CAnimationSpriteGameFight *fightAnimation)
{
    
    if(pMonsterSprite2)
    {
        CCNode *node=pMonsterSprite2->getChildByTag(fightAnimation->spritetag);
        if(!node)
        {
            string filePath=fightAnimation->m_sBufferPlist;
            CCAction *animation=PtActionUtility::getRunActionWithActionFile(filePath.c_str());
            CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd));
            pMonsterSprite2->runAction(CCSequence::create((CCFiniteTimeAction*)animation,endAnimation,NULL));
        }
        else{
            AnimaitonEnd();
        }
    }
    
}
*/
/*
void CFightingLayerScene::actionPFightSkill(const char *fightName,CCSprite *pFight,CCSprite *pMonster)
{
    CCLog("fightName:%s",fightName);
    CCAction *animation=PtActionUtility::getRunActionWithActionFile(fightName);
    CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::showHpAnimation));
    CCCallFuncN *rebb=CCCallFuncN::create(this, callfuncN_selector(CFightingLayerScene::actionReorderZorderNode));
    CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd));
    reorderChild(pFight,2);
    pFight->runAction(CCSequence::create((CCFiniteTimeAction*)animation,callback,rebb,CCDelayTime::create(0.6f),endAnimation,NULL));
    if(pMonster)
    {
        pMonster->runAction(PtActionUtility::getRunActionWithActionFile(fightName,"shoushang"));
    }
}
*/
/*
void CFightingLayerScene::actionHelpSprite(const char *fightName,CCSprite *pFight,CCSprite *pMonster)
{
    
    CCAction *animation=PtActionUtility::getRunActionWithActionFile(fightName);
    //    CCCallFuncND *nd=CCCallFuncND::create(this,callfuncND_selector(CFightingCardLayerScene::animationShouShang),(void *)pMonster);
    CCCallFunc *callback=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::showHpAnimation));
    CCCallFunc *endAnimation=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd));
    pFight->runAction(CCSequence::create((CCFiniteTimeAction*)animation,callback,CCDelayTime::create(0.6),endAnimation,NULL));
    pMonster->runAction(PtActionUtility::getRunActionWithActionFile(fightName,"shoushang"));
}
*/

void CFightingLayerScene::moveCardSprite(vector<CFightCard *> &vCard,int goIndex,bool isLeft)
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
        runAction(CCSequence::create(CCDelayTime::create(0.3f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd)),NULL));
    }
}
void CFightingLayerScene::showHp(int leftHp,int RightHp)
{
    cout<<"left hp:"<<leftHp<<"   Right hp:"<<RightHp<<endl;
    if (leftHp==0 &&RightHp==0)
    {
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
        sprintf(data, "%d",hpValue*(1));
        labelTTF->setString(data);
        labelTTF->setVisible(true);
        if(hpValue>0)
        {
            labelTTF->setPosition(ccp(point.x,point.y+100));
            labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.5f,CCPoint(0, -100)),CCHide::create(),NULL));
        }
        else
        {
            labelTTF->setPosition(ccp(point.x,point.y));
            labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.5f,CCPoint(0, 100)),CCHide::create(),NULL));
        }
    }
    hpValue=RightHp;
    if(hpValue!=0)
    {
        
        point=ccp((CCDirector::sharedDirector()->getWinSize().width*0.5+200),(CCDirector::sharedDirector()->getWinSize().height*0.5));
        labelTTF=(CCLabelTTF *)getChildByTag(30003);
        sprintf(data, "%d",hpValue*(1));
        labelTTF->setString(data);
        labelTTF->setVisible(true);
        if(hpValue>0)
        {
            labelTTF->setPosition(ccp(point.x,point.y+100));
            labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.5f,CCPoint(0, -100)),CCHide::create(),NULL));
        }
        else
        {
            labelTTF->setPosition(ccp(point.x,point.y));
            labelTTF->runAction(CCSequence::create(CCMoveBy::create(0.5f,CCPoint(0, 100)),CCHide::create(),NULL));
        }
    }
    CCCallFunc *callfunc=CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::AnimaitonEnd));
    this->runAction(CCSequence::create(CCDelayTime::create(0.5),callfunc,NULL));
}
void CFightingLayerScene::actionReorderZorderNode(CCObject *object)
{
    if(object &&this)
    {
        
        this->reorderChild((CCSprite *)object, 1);
    }
}
void CFightingLayerScene::actionReorderZorder(CCNode *object,void *tag)
{
    CCLog("this->%x",this);
    CCLog("object:%x",object);
    if(object &&this)
    {
        CCSprite *sprite=(CCSprite *)tag;
        this->reorderChild(sprite, 1);
    }
}

void CFightingLayerScene::AnimaitonEnd()
{
    m_bAnimationEnd=true;
}
EN_ATKFIGHT_INDEX CFightingLayerScene::getHuiHeIndex()
{
    return m_enHuiheIndex;
}

void CFightingLayerScene::loadFromServerTest()
{
    for (int i=0; i<m_gamePlayer->m_hashmapFightingCard.size(); i++)
    {
        if(m_gamePlayer->m_hashmapFightingCard[i])
        {
            m_vFightingCard.push_back(new CFightCard(*m_gamePlayer->m_hashmapFightingCard[i]));
        } else{
            m_vFightingCard.push_back(NULL);
        }
    }
}

void CFightingLayerScene::createFightCard()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    loadFromServerTest();
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        CGamesCard *gameCard=NULL;
        if(i!=m_vFightingCard.size()-1 &&m_vFightingCard[i])
        {
            gameCard=CGamesCard::Create(m_vFightingCard[i]);
            m_vFightingCard[i]->tag=100+i;
            gameCard->setTag(m_vFightingCard[i]->tag);
            addChild(gameCard,8-i,m_vFightingCard[i]->tag);
            gameCard->setPosition(getCardPoint(i,true));
            if(i!=0)
            {
                gameCard->setScale(0.8);
            }
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
        }
        else if(m_vFightingCard[i])
        {
            gameCard=CGamesCard::Create(m_vFightingCard[i]);
            m_vFightingCard[i]->tag=100*(i+1);
            gameCard->setTag(m_vFightingCard[i]->tag);
            gameCard->setPosition(ccp(20,20));
            gameCard->setAnchorPoint(CCPointZero);
            gameCard->setScale(0.8);
            addChild(gameCard,8-i,m_vFightingCard[i]->tag);
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
        }
        if (gameCard) {
            gameCard->hideGameCardData();
        }
    }
    
}

void CFightingLayerScene::createMonsterCard()
{
    CCSize wndsize=CCDirector::sharedDirector()->getWinSize();
    
    for (int i=0; i<m_gamePlayer->m_hashmapMonsterCard.size(); i++)
    {
        if(m_gamePlayer->m_hashmapMonsterCard[i])
        {
            m_vMonsterCard.push_back(new CFightCard(*m_gamePlayer->m_hashmapMonsterCard[i]));
        }
        else{
            m_vMonsterCard.push_back(NULL);
        }
    }
    for (int  i=0; i<m_vMonsterCard.size(); i++)
    {
        CGamesCard *gameCard=NULL;
        if(i!=m_vMonsterCard.size()-1 && m_vMonsterCard[i]!=NULL)
        {
            gameCard=CGamesCard::Create(m_vMonsterCard[i]);
            m_vMonsterCard[i]->tag=1000*(i+1);
            gameCard->setTag(m_vMonsterCard[i]->tag);
            addChild(gameCard,9-i, m_vMonsterCard[i]->tag);
            gameCard->setPosition(getCardPoint(i, false));
            gameCard->setFlipX(true);
            if(i!=0)
            {
                gameCard->setScale(0.8);
            }
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
            
        }
        else if(m_vMonsterCard[i])
        {
            
            gameCard=CGamesCard::Create(m_vMonsterCard[i]);
            m_vMonsterCard[i]->tag=1000+i;
            gameCard->setTag(m_vMonsterCard[i]->tag);
            gameCard->setPosition(ccp(wndsize.width-150,20));
            gameCard->setFlipX(true);
            gameCard->setAnchorPoint(CCPointZero);
            gameCard->setScale(0.8);
            addChild(gameCard,9-i, m_vMonsterCard[i]->tag);
            CCLog("%f,%f",gameCard->getPosition().x,gameCard->getPosition().y);
        }
        if (gameCard) {
            gameCard->hideGameCardData();
        }
        
    }
    
}

void CFightingLayerScene::deleteBackGamePlayerFightMonsterCard()
{
    m_gamePlayer->deleteFightMonsterCard();
}

CCPoint CFightingLayerScene::getCardPoint(int index, bool isLeftCard)
{
    CCPoint point;
    int value=isLeftCard?-1:1;
    CCSize  size=CCDirector::sharedDirector()->getWinSize();
    if(index==0)
    {
        return CCPoint(size.width*0.5+100*value,120);
    }
    else
    {
        return CCPoint(size.width*0.5+200*value+30*value*(index-1),100);
    }
    
}

void CFightingLayerScene::textSkillInfo(CAnimationSctrip *fight)
{
    if (getChildByTag(20003)) {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(20003);
        char data[20];
        sprintf(data,"发送技能%d",fight->m_nSKillId);//,
        labelttf->setString(data);
        labelttf->runAction(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(1.0),NULL));
        
    }
}

void CFightingLayerScene::yinCangRenWu(vector<CCSprite *>vsprite,CCSprite *sprite)
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

bool CFightingLayerScene::initHitText()
{
    //设置加血扣血的
    CCLabelTTF *labelTTf1=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf1,45,30002);
    labelTTf1->setVisible(false);
    
    CCLabelTTF *labelTTf2=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf2,45,30003);
    labelTTf2->setVisible(false);
    return true;
}

void CFightingLayerScene::clearAllHeroTexiao()
{
    for (int i=0; i<m_vFightHero.size();i++ ) {
        if(m_vFightHero[i])
            m_vFightHero[i]->removeAllChildrenWithCleanup(true);
    }
    for (int i=0; i<m_vMonsterHero.size();i++ ) {
        if(m_vMonsterHero[i])
            m_vMonsterHero[i]->removeAllChildrenWithCleanup(true);
    }
}

void CFightingLayerScene::resetCardPosition()
{
    
    //clearAllHeroTexiao();
    for (int i=0; i<m_vFightingCard.size(); i++)
    {
        if(i!=m_vFightingCard.size()-1 &&m_vFightingCard[i])
        {
            CGamesCard *gameCard=(CGamesCard*)getChildByTag(m_vFightingCard[i]->tag);
            gameCard->setPosition(getCardPoint(i,true));
            gameCard->setLive();
            if(i!=0)
            {
                gameCard->setScale(0.8);
            }
            else
            {
                gameCard->setScale(1.0f);
            }
        }
    }
    for (int i=0; i<m_vMonsterCard.size(); i++)
    {
        if(i!=m_vMonsterCard.size()-1 &&m_vMonsterCard[i])
        {
            CGamesCard *gameCard=(CGamesCard*)getChildByTag(m_vMonsterCard[i]->tag);
            gameCard->setPosition(getCardPoint(i,false));
            gameCard->setLive();
            if(i!=0)
            {
                gameCard->setScale(0.8);
            }
            else
            {
                gameCard->setScale(1.0f);
            }
            
        }
    }
}

//
void CFightingLayerScene::createKuaiJin()
{
    CCSprite *sprite=CCSprite::create("Icon-Small@2x.png");
    addChild(sprite,100,911);
    sprite->setPosition(ccp(512,700));
}

// 创建显示文本比如，怒气，Hp信息;
void CFightingLayerScene::createEngryText()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("1", "Arail", 25);
    addChild(labelttf,99999,87777);
    labelttf->setPosition(ccp(300,750));
    labelttf=CCLabelTTF::create("2", "Arail", 25);
    addChild(labelttf,99999,87778);
    labelttf->setPosition(ccp(874,750));
}


//创建HPtext
void CFightingLayerScene::createHpText()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("", "Arail", 25);
    addChild(labelttf,99999,77777);
    labelttf->setPosition(ccp(150,750));
    labelttf=CCLabelTTF::create("", "Arail", 25);
    addChild(labelttf,99999,77778);
    labelttf->setPosition(ccp(724,750));
}

bool CFightingLayerScene::initText()
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
bool CFightingLayerScene::initAtkPng()
{
    //    CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
    //    addChild(sprite,15,30001);
    //    sprite->setVisible(false);
    CCSprite *sprite2=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
    addChild(sprite2,15,30000);
    sprite2->setVisible(false);
    return true;
}
// 初始化 背景图片
bool CFightingLayerScene::initBggroudPng()
{
    CCSize  winsize=CCDirector::sharedDirector()->getWinSize();
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/bgm.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    return  true;
}