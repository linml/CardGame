//
//  CFightingLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-18.
//
//

#include "CFightingLayerScene.h"
#include "CCardPanel.h"
#include "CardSprite.h"
#include "gamePlayer.h"
#include "gameConfig.h"
#include "CSkillManager.h"
#include "FightResultConfirm.h"

#include "HBActionAni.h"
#include "HBActionAniCache.h"

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

/**
 *技能值列表   1 , 火焰燃烧
 2 攻击添加50%
 3 冰冻效果
 4
 5
 7 死亡技能 +1000
 */
CFightingLayerScene::CFightingLayerScene()
{
    m_vfightCardSprite.clear();
    m_vMonsterCardSprite.clear();
    gongjiHuiHe=1;
    isOwnActionEnd=false;
    isMonsterEnd=false;
    isDoingAnimation=false;
}


CFightingLayerScene::~CFightingLayerScene()
{
    DELETE_POINT_VECTOR(m_vfightCardSprite,vector<SFightCardSprite *>);
    DELETE_POINT_VECTOR(m_vMonsterCardSprite,vector<SFightCardSprite *>);
    SinglePlayer::instance()->forTestDeleteMonster();
}

CCScene *CFightingLayerScene::scene()
{
    CCScene *pScene=CCScene::create();
    CFightingLayerScene *mapLayer = new CFightingLayerScene();
    mapLayer->setUserData((void*)(scene_fightscene.c_str())); //没神马用。
    mapLayer->init();
    mapLayer->autorelease();
    pScene->addChild(mapLayer);
    return pScene;
}

void CFightingLayerScene::createMonster()
{
    SinglePlayer::instance()->forTestMonster();
    CCSize wndsize=CCDirector::sharedDirector()->getWinSize();
    // 玩家 战斗时候的卡牌
    for (vector<CCardSprite *>::iterator it=SinglePlayer::instance()->m_vMonsterCardSprite.begin(); it!=SinglePlayer::instance()->m_vMonsterCardSprite.end(); it++)
    {
        CCLog("Monster");
        SFightCardSprite *fightCard=new SFightCardSprite;
        fightCard->cardsprite=new  CCardSprite(**it);
        fightCard->isDead=false;
        if ((*it)->m_cardData.m_unCurrentHp==0) {
            fightCard->isDead=true;
        }
        m_vMonsterCardSprite.push_back(fightCard);
    }
    //布阵
    for (int i=0;i<m_vMonsterCardSprite.size();i++)
    {
        m_vMonsterCardSprite[i]->tag=1000+i;
        if(i==m_vMonsterCardSprite.size()-1)//设置最后一张为 辅助英雄
        {
            //放在 拥护的位置
            CCardPanel *sprite1=CCardPanel::Create((g_mapImagesPath+"fighting/"+m_vMonsterCardSprite[i]->cardsprite->m_cardData.m_sResourceName+"_3.png").c_str(), m_vMonsterCardSprite[i]->cardsprite->m_cardData.m_sPlayerCardName.c_str()) ;
            this->addChild(sprite1,1,m_vMonsterCardSprite[i]->tag);
            cout<<"m_vfightCardSprite[i]->tag:"<<m_vfightCardSprite[i]->tag<<endl;
            sprite1->setPosition(ccp(wndsize.width-200,20));
            sprite1->setAnchorPoint(ccp(0,0));
            sprite1->setFlipX(true);
        }
        else
        {
            CCardPanel *sprite1=CCardPanel::Create((g_mapImagesPath+"fighting/"+m_vMonsterCardSprite[i]->cardsprite->m_cardData.m_sResourceName+"_2.png").c_str(), m_vMonsterCardSprite[i]->cardsprite->m_cardData.m_sPlayerCardName.c_str()) ;
            this->addChild(sprite1,1,m_vMonsterCardSprite[i]->tag);
            sprite1->setPosition(ccp(wndsize.width*0.5+100+i*118,wndsize.height*0.5));
            sprite1->setFlipX(true);
        }
    }
}


void CFightingLayerScene::createOwnFightCardPosition()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    // 玩家 战斗时候的卡牌
    for (vector<CCardSprite *>::iterator it=SinglePlayer::instance()->m_vZhangdouCardSprite.begin(); it!=SinglePlayer::instance()->m_vZhangdouCardSprite.end(); it++)
    {
        CCLog("dddd");
        SFightCardSprite *fightCard=new SFightCardSprite;
        fightCard->cardsprite=new  CCardSprite(**it);// 使用的时候 应该是复制 起卡牌的属性
        fightCard->isDead=false;
        std::cout<<fightCard->cardsprite->m_cardData.m_iYongHuJineng<<endl;
        m_vfightCardSprite.push_back(fightCard);
    }
    //布阵
    for (int i=0;i<m_vfightCardSprite.size();i++)
    {
        m_vfightCardSprite[i]->tag=100+i;
        
        if(i==m_vfightCardSprite.size()-1)
        {
            //放在 拥护的位置
            CCardPanel *sprite1=CCardPanel::Create((g_mapImagesPath+"fighting/"+m_vfightCardSprite[i]->cardsprite->m_cardData.m_sResourceName+"_3.png").c_str(), m_vfightCardSprite[i]->cardsprite->m_cardData.m_sPlayerCardName.c_str()) ;
            this->addChild(sprite1,5,m_vfightCardSprite[i]->tag);
            sprite1->setPosition(ccp(20,20));
            sprite1->setAnchorPoint(CCPointZero);
        }
        else
        {
            CCardPanel *sprite1=CCardPanel::Create((g_mapImagesPath+"fighting/"+m_vfightCardSprite[i]->cardsprite->m_cardData.m_sResourceName+"_2.png").c_str(), m_vfightCardSprite[i]->cardsprite->m_cardData.m_sPlayerCardName.c_str()) ;
            this->addChild(sprite1,5-i,m_vfightCardSprite[i]->tag);
            sprite1->setPosition(ccp(winsize.width/2-100-i*118,winsize.height*.5));
        }
    }
}
void CFightingLayerScene::setText(const char *data)
{
    CCLabelTTF *labelttf=(CCLabelTTF *) this->getChildByTag(20003);
    labelttf->setString(data);
}

void CFightingLayerScene::updateSetHp()
{
    updateHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite, m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite);
}


void CFightingLayerScene::updateHp(CCardSprite *pCardSprite,CCardSprite *pMonsterCardSprite)
{
    CCLabelTTF *pLabel=(CCLabelTTF *)this->getChildByTag(20000);
    setHp(pCardSprite, pLabel);
    CCLabelTTF *pMonsterLabel=(CCLabelTTF *)this->getChildByTag(20001);
    setHp(pMonsterCardSprite, pMonsterLabel);
    
}

void CFightingLayerScene::setHp(CCardSprite *pCardSprite, cocos2d::CCLabelTTF *labelttf)
{
    if(pCardSprite==NULL)
    {
        labelttf->setString("point is null");
        return;
    }
    
    char data[20];
    sprintf(data, "%d/%d",pCardSprite->m_cardData.m_unCurrentHp,pCardSprite->m_cardData.m_unHp);
    labelttf->setString(data);
}

void CFightingLayerScene::initOwnHuihe()
{
    cout<<"m_iZhujiangHuihe:"<<m_iZhujiangHuihe<<endl;
    m_iZhujiangHuihe=G_SkillManager::instance()->getSkillByIndex(m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng)->m_ihuihe;
}



void CFightingLayerScene::initMonsterHuihe()
{
    cout<<"m_iMonsterZhujiangHuihe:"<<m_iMonsterZhujiangHuihe<<endl;
    m_iMonsterZhujiangHuihe=G_SkillManager::instance()->getSkillByIndex(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iJiChuJineng)->m_ihuihe;
    
}
void CFightingLayerScene::showJuqi(CCNode *node)
{
    Utility::runPtActionScript(node, "juqi1/juqi1.act", 100);//(<#cocos2d::CCNode *node#>, <#int tag#>)
}

void CFightingLayerScene::hideJuqi(CCNode *node)
{
    Utility::stopPtActionScript(node, 100);//(node, "juqi1/juqi1.act", 100);//
}
void CFightingLayerScene::initAssignHuihe()
{
    if( (m_vMonsterCardSprite.size()< m_vFightCardIndex) ||
       (m_vMonsterCardIndex>m_vMonsterCardSprite.size()))
    {
        return;
    }
    initOwnHuihe();
    //获得 普通攻击的 主将的回合数字
    initMonsterHuihe();
    
}

int CFightingLayerScene::getWinStats()
{
    //先判断
    bool isCardAllDead=true;
    for (int i=0; i<m_vfightCardSprite.size()-1; i++)
    {
        if(!m_vfightCardSprite[i]->isDead) //没有死亡 拥户不算
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
    for (int i=0; i<m_vMonsterCardSprite.size()-1; i++)
    {
        if (!m_vMonsterCardSprite[i]->isDead)
        {
            return 0;
        }
    }
    return 1;
}
//攻击序列
void CFightingLayerScene::callBackOwnAnimationChangeFlag(CCObject *pSend)
{
    cout<<(this->isOwnActionEnd?"isOwnActionEnd:true":"isOwnActionEnd:false")<<endl;
    this->isOwnActionEnd=true;
}

void CFightingLayerScene::callBackMonsterAnimationChangeFlag(CCObject *pSend)
{
    cout<<(this->isMonsterEnd?"isMonsterEnd:true":"isMonsterEnd:false")<<endl;
    this->isMonsterEnd=true;
}

void CFightingLayerScene::checkOwnIsDeadAndMove()
{
    if (m_vfightCardSprite[m_vFightCardIndex]->isDead)
    {
        int zhanHunSkillIndex=m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iZhangHunJiachi;
        if(zhanHunSkillIndex!=-1)
        {
            G_SkillManager::instance()->dealWithSkillShanghaiList(zhanHunSkillIndex, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex, m_vMonsterCardIndex,1);
        }
        G_SkillManager::instance()->fightDead(2);
        m_vFightCardIndex++;
        initOwnHuihe();
    }
}

void CFightingLayerScene::animationMoveToLast(class CCObject  *pSend)
{
    if(!isOwnActionEnd)
    {
        CCPoint point;
        for (int i=0; i<=3; i++)
        {
            if(i==0)
            {
                point=getChildByTag(m_vfightCardSprite[3]->tag)->getPosition();
                getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else if(i!=3)
            {
                point=getChildByTag(m_vfightCardSprite[i-1]->tag)->getPosition();
                getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else
            {
                point=getChildByTag(m_vfightCardSprite[i-1]->tag)->getPosition();
                getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::callBackOwnAnimationChangeFlag)),NULL));
            }
            setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20000));
        }
        
    }
    if(!isMonsterEnd)
    {
        CCPoint point;
        for (int i=0; i<=3; i++)
        {
            if(i==0)
            {
                point=getChildByTag(m_vMonsterCardSprite[3]->tag)->getPosition();
                getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else if(i!=3)
            {
                point=getChildByTag(m_vMonsterCardSprite[i-1]->tag)->getPosition();
                getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
            }
            else
            {
                point=getChildByTag(m_vMonsterCardSprite[i-1]->tag)->getPosition();
                getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::callBackMonsterAnimationChangeFlag)),NULL));
            }
            setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20001));
        }
        
    }
}
void CFightingLayerScene::checkMonsterIsDeadAndMove()
{
    if (m_vMonsterCardSprite[m_vMonsterCardIndex]->isDead)
    {
         int zhanHunSkillIndex=m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iZhangHunJiachi;
        if(zhanHunSkillIndex!=-1)
        {
        G_SkillManager::instance()->dealWithSkillShanghaiList(zhanHunSkillIndex, m_vMonsterCardSprite, m_vfightCardSprite, m_vMonsterCardIndex, m_vFightCardIndex,-1);
        }
        G_SkillManager::instance()->fightDead(3);
        m_vMonsterCardIndex++;
        initMonsterHuihe();
    }
}
void CFightingLayerScene::secheudelUpdateToDoDealWith(float times)
{
    cout<<"="<<gongjiHuiHe<<isOwnActionEnd<<isMonsterEnd<<endl;
    if( isMonsterEnd && isOwnActionEnd)
    {
        unschedule(schedule_selector(CFightingLayerScene::secheudelUpdateToDoDealWith));
        fightLogic(gongjiHuiHe);
    }
}

void CFightingLayerScene::dealWithFight(CCObject *object)
{
    
    //先判断当前的双方的主将是否是死亡的 如果是四万的 就要切换位置
    
    int winStatus=getWinStats();
    cout<<"winStatus="<<winStatus<<endl;
    //先判断是否全部死亡；
    if(winStatus==0)
    {
        if(!m_vfightCardSprite[m_vFightCardIndex]->isDead && !m_vMonsterCardSprite[m_vMonsterCardIndex]->isDead)
        {
            fightLogic(gongjiHuiHe);
        }
        else
        {
            checkOwnIsDeadAndMove();
            checkMonsterIsDeadAndMove();
        }
    }
    else
    {
        unschedule(schedule_selector(CFightingLayerScene::updateGetGameDataToGetServerRandow));
        currentSwfIndex=0;
        initBufferEnd=true;
        m_vFightCardIndex=0;
        m_vMonsterCardIndex=0;
        
        if(G_SkillManager::instance()->getAnimationVector().size()>0)
        {
            CCLog("AAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
            schedule(schedule_selector(CFightingLayerScene::startAnimationSwf));
        }
    }
    
}
void CFightingLayerScene::removeSprite(CCNode *node,void *tag)
{
    CCSprite *sprite=(CCSprite*)tag;
    removeChild(sprite, true);
}
void CFightingLayerScene::showFightAnimation(CCNode *pSend,void *tag)
{
    CCLog("aaaa");
 
}

void CFightingLayerScene::animationHpFight(class CCNode *card,void *tag)
{
    int hpValue=this->m_sfps->m_iJiaxue;
   
    if(hpValue==0)
    {
        return;
    }
     SFightCardSprite *sprite=(SFightCardSprite *)tag;
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

void CFightingLayerScene::animationHpMonster(class CCNode *card,void *tag)
{
    cout<<card<<"   "<<this<<endl;
        int hpValue=this->m_sfps->m_iShanghai;
    SFightCardSprite *sprite=(SFightCardSprite *)tag;
// ->m_iShanghaie;
    if(hpValue==0)
    {
        return;
    }
    CCPoint point=getChildByTag(sprite->tag)->getPosition();
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

void CFightingLayerScene::setHideHpAnimation()
{
    getChildByTag(30003)->setVisible(false);
    getChildByTag(30002)->setVisible(false);
}
void CFightingLayerScene::nextAnimation(CCNode *node)
{
    this->currentSwfIndex++;
    this->isDoingAnimation=false;
}
void CFightingLayerScene::basicAnimation(vector<SFightCardSprite *>ownFightCard,vector<SFightCardSprite *>MonstFightCard,int  oWnindex,int  MonsteIndex,class SAnimationFps * spfs)
{
    
    CCardPanel *m_tempCardSprite=(CCardPanel*)getChildByTag(ownFightCard[oWnindex]->tag);
    if(spfs->m_iHuihe!=0 && spfs->m_iHuihe!=-2 &&m_tempCardSprite->isAddTexiao==false)
    {
        m_tempCardSprite->isAddTexiao=true;
        //        HBActionAni* ani = HBActionAniCache::sharedActionAniCache()->addActionAniWithFile(CSTR_FILEPTAH(g_ActionFilePath,"xuanzhe/xuanzhe.act"));
        //        ani->runAnimationBy(m_tempCardSprite);
        Utility::runPtActionScript(m_tempCardSprite,"xuanzhe/xuanzhe.act",200);
    }
    
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
}

void CFightingLayerScene::animationSwf(int skillIndex,vector<SFightCardSprite *>ownFightCard,vector<SFightCardSprite *>MonstFightCard,int  oWnindex,int  MonsteIndex, SAnimationFps * spfs)
{
    CCLOG("%d\n",skillIndex);
    switch (skillIndex) {
        case 0:
        {
            setText("普通攻击");
            basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
        }
           break;
        case 1:
            setText("女神之枪");
            break;
        case 2:
        {
            setText("女神战魂");
            for (int i=oWnindex+1; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                addChild(sprite,1000,ownFightCard[i]->tag+100);
                CCPoint point=getChildByTag(ownFightCard[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                }
                
            }

        }
            break;
        case 3:
            setText("女神拥护");
            basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 4:
            setText("寒霜号角");
            basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 5:
            for (int i=oWnindex+1; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_1.png").c_str());
                addChild(sprite,1000,ownFightCard[i]->tag+100);
                CCPoint point=getChildByTag(ownFightCard[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                }
                
            }

            break;
        case 6:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 7:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 8:
            for (int i=oWnindex+1; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                addChild(sprite,1000,ownFightCard[i]->tag+100);
                CCPoint point=getChildByTag(ownFightCard[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                }
                
            }

            break;
        case 9:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 10:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 11:
            for (int i=oWnindex+1; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_3.png").c_str());
                addChild(sprite,1000,ownFightCard[i]->tag+100);
                CCPoint point=getChildByTag(ownFightCard[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                }
                
            }

            break;
        case 12:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 13:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 14:
            for (int i=oWnindex; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                addChild(sprite,1000,ownFightCard[i]->tag+100);
                CCPoint point=getChildByTag(ownFightCard[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CFightingLayerScene::removeSprite), (void*)(sprite)),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                }
                
            }

            break;
        case 15:
        {
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
        }

            break;
        case 16:
        {
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
        }
            break;
        case 17:
                        basicAnimation(ownFightCard,MonstFightCard,oWnindex,MonsteIndex,spfs);
            break;
        case 18:
            break;
        case 19:
            break;
        case 20:
            break;
        default:
            break;
    }
}

void CFightingLayerScene::startAnimationSwf(float data)
{
    if(currentSwfIndex < G_SkillManager::instance()->getAnimationVector().size() && currentSwfIndex>=0)
    {
        if(!isDoingAnimation)
        {
            isDoingAnimation=true;
            SAnimationFps *saFps=G_SkillManager::instance()->getAnimationVector()[currentSwfIndex];
            this->m_sfps=saFps;
            switch (saFps->m_iHuihe)
            {
                case 1:
                case 0:
                      animationSwf(saFps->m_skillIndex,m_vfightCardSprite,m_vMonsterCardSprite,saFps->m_spendAnimationAuthor,m_vMonsterCardIndex,saFps);
                    break;
                case -1:
                case -2:
                      animationSwf(saFps->m_skillIndex,m_vMonsterCardSprite,m_vfightCardSprite,saFps->m_spendAnimationAuthor,m_vFightCardIndex,saFps);
                    break;
                case 2:
                {
                    CCardPanel *node= ((CCardPanel *) getChildByTag(m_vfightCardSprite[m_vFightCardIndex]->tag));
                    if(node)
                    {
                        node->setDead();

                        if(((CCardPanel*)(node))->isAddTexiao)
                        {
                            Utility::stopPtActionScript(node, 200);
                        }
                    }
                    CCPoint point;
                    for (int i=0; i<=3; i++)
                    {
                        if(i==0)
                        {
                            point=getChildByTag(m_vfightCardSprite[3]->tag)->getPosition();
                            getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
                        }
                        else if(i!=3)
                        {
                            point=getChildByTag(m_vfightCardSprite[i-1]->tag)->getPosition();
                            getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
                        }
                        else
                        {
                            point=getChildByTag(m_vfightCardSprite[i-1]->tag)->getPosition();
                            getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                        }
                      }
                    m_vFightCardIndex++;
                    setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20000));

                }
                    break;
                case 3:
                {
                    CCardPanel *node=((CCardPanel *) getChildByTag(m_vMonsterCardSprite[m_vMonsterCardIndex]->tag));
                    if(node)
                    {
                        node->setDead(); 
                        if(((CCardPanel*)(node))->isAddTexiao)
                        {
                            Utility::stopPtActionScript(node, 200);
                        }
                    }
                    CCPoint point;
                    for (int i=0; i<=3; i++)
                    {
                        if(i==0)
                        {
                            point=getChildByTag(m_vMonsterCardSprite[3]->tag)->getPosition();
                            getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
                        }
                        else if(i!=3)
                        {
                            point=getChildByTag(m_vMonsterCardSprite[i-1]->tag)->getPosition();
                            getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCMoveTo::create(0.2f,point));
                        }
                        else
                        {
                            point=getChildByTag(m_vMonsterCardSprite[i-1]->tag)->getPosition();
                            getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::nextAnimation)),NULL));
                        }
                    }
                    m_vMonsterCardIndex++;
                    setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20000));
                }
                default:
                    break;
            }
        }
    }
    else if(currentSwfIndex==G_SkillManager::instance()->getAnimationVector().size())
    {
        unschedule(schedule_selector(CFightingLayerScene::startAnimationSwf));
        if(getWinStats()==1)
        {
            winDialog();
        }
        else if(getWinStats()==-1){
            loseDialog();
        }
    }
}



void CFightingLayerScene::loseDialog()
{
    int tmp = 0;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 100000);
    
    cout<<"lose"<<endl;
}

void CFightingLayerScene::winDialog()
{
    int tmp = 1;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 1000000);
    
    cout<<"win"<<endl;
}

void CFightingLayerScene::animationCardPanel(class CCardPanel *card, void *tag)
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

void CFightingLayerScene::animationShouShang(class CCardPanel *card,void *tag)
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

void CFightingLayerScene::animationMoveBack(class CCardPanel *card)
{
    card->runAction(CCMoveBy::create(0.2f,ccp(0,-100)));
}

void CFightingLayerScene::setVistablHit()
{
    getChildByTag(30000)->setVisible(false);
    getChildByTag(30001)->setVisible(false);
    
}

void CFightingLayerScene::fightLogic(int  huihe)
{
    cout<<"huihe:"<<huihe<<endl;
    switch (huihe)
    {
        case 1:
            //主将发动攻击
            gongjiHuiHe--;
            if(m_iZhujiangHuihe!=0)
            {
                cout<<"huihe1:"<<huihe<<endl;
                m_iZhujiangHuihe--;
                G_SkillManager::instance()->dealWithSkillShanghaiList(0, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex, m_vMonsterCardIndex,1);               
            }
            else
            { cout<<"huihe2:"<<huihe<<" "<<m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng<<endl;
                G_SkillManager::instance()->dealWithSkillShanghaiList(m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex, m_vMonsterCardIndex,1);  
            }
            break;
        case 0:
        {
            //拥护技能
            //动画效果
            int indexYonghu=m_vfightCardSprite[m_vfightCardSprite.size()-1]->cardsprite->m_cardData.m_iYongHuJineng;
            gongjiHuiHe--;
            cout<<"indexYonghu:"<<indexYonghu<<endl;
            G_SkillManager::instance()->dealWithSkillShanghaiList(indexYonghu, m_vfightCardSprite, m_vMonsterCardSprite, m_vfightCardSprite.size()-1, m_vMonsterCardIndex,0);
            
        }
            break;
        case -1:
        {
            gongjiHuiHe--;
            if(m_iMonsterZhujiangHuihe!=0)
            {
                m_iMonsterZhujiangHuihe--;
                //动画效果
                G_SkillManager::instance()->dealWithSkillShanghaiList(0, m_vMonsterCardSprite,m_vfightCardSprite,m_vMonsterCardIndex, m_vFightCardIndex,-1);
            }
            else
            {
               int HuiheJineng=m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iJiChuJineng;
                G_SkillManager::instance()->dealWithSkillShanghaiList(HuiheJineng, m_vMonsterCardSprite,m_vfightCardSprite,m_vMonsterCardIndex, m_vFightCardIndex,-1);
             }
        }
            break;
        case -2:
        {
            int indexYonghu=m_vMonsterCardSprite[m_vMonsterCardSprite.size()-1]->cardsprite->m_cardData.m_iYongHuJineng;
            gongjiHuiHe=1;
            cout<<"indexYonghu:"<<indexYonghu<<endl;
            G_SkillManager::instance()->dealWithSkillShanghaiList(indexYonghu, m_vMonsterCardSprite,m_vfightCardSprite ,m_vMonsterCardSprite.size()-1, m_vFightCardIndex,-2);
        }
            break;
        default:
            break;
    }
}

void CFightingLayerScene::updateGetGameDataToGetServerRandow()
{
        dealWithFight(this);
}


bool CFightingLayerScene::init()
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    currOwnCardSprite=NULL;
    monsterCardSprite=NULL;
    m_vFightCardIndex=0;
    m_vMonsterCardIndex=0;
    m_iZhujiangHuihe=0;
    m_iMonsterZhujiangHuihe=0;
	CCLog("CFightingLayerScene::init");
    
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/battle_bg11.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    
    
    initText();
    //定义更具global的 appdata 值得 添加了 cccardlayer的 类别
    createOwnFightCardPosition();
    createMonster();
    
    setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20000));
    setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite, (CCLabelTTF *) getChildByTag(20001));

    
    initAssignHuihe();

   Utility::addPtActionScript("juqi1/juqi1.act");
    
    CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
    addChild(sprite,15,30001);
    sprite->setVisible(false);
    CCSprite *sprite2=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
    addChild(sprite2,15,30000);
    sprite2->setVisible(false);
    
    G_SkillManager::instance()->clearAnimationList();
    this->runAction(CCSequence::create(CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(CFightingLayerScene::runSchedule)),NULL));
    return true;
}

void CFightingLayerScene::runSchedule()
{
    schedule(schedule_selector(CFightingLayerScene::updateGetGameDataToGetServerRandow));
}

void CFightingLayerScene::initText()
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
    
    //设置加血扣血的
    CCLabelTTF *labelTTf1=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf1,15,30002);
    labelTTf1->setVisible(false);
    labelTTf1->setColor(ccc3(255, 0, 0));
    
    CCLabelTTF *labelTTf2=CCLabelTTF::create("", "Arial", 45);
    addChild(labelTTf2,15,30003);
    labelTTf2->setVisible(false);
    labelTTf2->setColor(ccc3(255, 0, 0));
}

