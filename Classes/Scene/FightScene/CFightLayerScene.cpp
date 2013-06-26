//
//  CFightLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#include "CFightLayerScene.h"
#include "CardLayer.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "CJinengTeXiao.h"
#include "CSkillManager.h"

#include "CCardPanel.h"


#include "FightResultConfirm.h"


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
bool SortByM1( const SFightCardSprite *v1, const SFightCardSprite  *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    if(!v1->isDead)
    {
        return false;
    }
    return true;
}
/*序号, 技能, 伤害值得 */
/*****序号gongjizhe
 0 技能 普通攻击
 1 技能 触发%200的攻击
 2 技能 格挡
 */
//应该是用
const int fightdata[10][7]={
    
    {0,0,7,0,0,0,0}, //普通攻击
    {4,5,0,0,0,0,3}, //吸血伤害
    {5,0,20,0,0,0,0}, //无视防御伤害20 //下一张卡牌的
    {9,2,0,0,0,0,0}, //
    {0,8,3,0,0,0,0},
    {2,0,0,0,0,0,0},
    {1,0,0,0,0,0,0},
    {2,0,0,0,0,0,2},
    {0,1,1,0,0,0,1},
    {2,0,0,0,0,0,0}
};



CFightLayerScene::CFightLayerScene()
{
    m_vfightCardSprite.clear();
    m_vMonsterCardSprite.clear();
    gongjiHuiHe=1;
    isOwnActionEnd=false;
    isMonsterEnd=false;
}


CFightLayerScene::~CFightLayerScene()
{
    DELETE_POINT_VECTOR(m_vfightCardSprite,vector<SFightCardSprite *>);
    DELETE_POINT_VECTOR(m_vMonsterCardSprite,vector<SFightCardSprite *>);
    
    SinglePlayer::instance()->forTestDeleteMonster();
}

CCScene *CFightLayerScene::scene()
{
    CCScene *pScene=CCScene::create();
    CFightLayerScene *mapLayer = new CFightLayerScene();
    mapLayer->setUserData((void*)(scene_fightscene.c_str())); //没神马用。 
    mapLayer->init();
    mapLayer->autorelease();
    pScene->addChild(mapLayer);
    return pScene;
}

void CFightLayerScene::createMonster()
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


void CFightLayerScene::createOwnFightCardPosition()
{
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    // 玩家 战斗时候的卡牌
    for (vector<CCardSprite *>::iterator it=SinglePlayer::instance()->m_vZhangdouCardSprite.begin(); it!=SinglePlayer::instance()->m_vZhangdouCardSprite.end(); it++)
    {
        SFightCardSprite *fightCard=new SFightCardSprite;
        fightCard->cardsprite=new  CCardSprite(**it);// 使用的时候 应该是复制 起卡牌的属性
        fightCard->isDead=false;
        //std::cout<<fightCard->cardsprite->m_cardData.m_iYongHuJineng<<endl;
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
            this->addChild(sprite1,1,m_vfightCardSprite[i]->tag);
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

bool CFightLayerScene::init()
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    currOwnCardSprite=NULL;
    monsterCardSprite=NULL;
    m_vFightCardIndex=0;
    m_vMonsterCardIndex=0;
    m_iZhujiangHuihe=0;
    m_iMonsterZhujiangHuihe=0;
	CCLog("MapScene::init");
	//init bg;
    
    CCSprite *bgSprite=CCSprite::create((g_mapImagesPath+"fighting/battle_bg11.png").c_str());
    assert(bgSprite!=NULL);
    bgSprite->setPosition(ccp(winsize.width*0.5,winsize.height*0.5));
    addChild(bgSprite,0);
    
    //init Touxiang
    //    CCSprite *leftHeadImage =CCSprite::create((g_mapImagesPath+"fighting/head_left.png").c_str());
    //    leftHeadImage->setPosition(ccp(40,winsize.height-40));
    //    addChild(leftHeadImage,1);
    //
    //
    //    CCSprite *rightHeadImage =CCSprite::create((g_mapImagesPath+"fighting/head_right2.png").c_str());
    //    rightHeadImage->setPosition(ccp(winsize.width-40,winsize.height-40));
    //    addChild(rightHeadImage,1);
    
    //血量数值
    CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 35);
    labelttf->setPosition(ccp(150,700));
    labelttf->setColor(ccc3(255, 0, 0));
    this->addChild(labelttf,10,20000);
    CCLabelTTF *labelttfMonster=CCLabelTTF::create("", "Arial", 35);
    labelttfMonster->setPosition(ccp(624,700));
    labelttfMonster->setColor(ccc3(255, 0, 0));
    addChild(labelttfMonster,10,20001);
    //定义更具global的 appdata 值得 添加了 cccardlayer的 类别
    createOwnFightCardPosition();
    createMonster();
    
    setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite, labelttf);
    setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite, labelttfMonster);
    initAssignHuihe();
    
    CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
    addChild(sprite,15,30001);
    sprite->setVisible(false);
    CCSprite *sprite2=CCSprite ::create((g_mapImagesPath+"fighting/skill_1.png").c_str());
    addChild(sprite2,15,30000);
    sprite2->setVisible(false);
  
    scheduleOnce(schedule_selector(CFightLayerScene::updateGetGameDataToGetServerRandow), 1.0f);
    return true;
}

void CFightLayerScene::setText(const char *data)
{
    CCLabelTTF *labelttf=(CCLabelTTF *) this->getChildByTag(20002);
    labelttf->setString(data);
}


void CFightLayerScene::updateHp(CCardSprite *pCardSprite,CCardSprite *pMonsterCardSprite)
{
    CCLabelTTF *pLabel=(CCLabelTTF *)this->getChildByTag(20000);
    setHp(pCardSprite, pLabel);
    CCLabelTTF *pMonsterLabel=(CCLabelTTF *)this->getChildByTag(20001);
    setHp(pMonsterCardSprite, pMonsterLabel);
    
}

void CFightLayerScene::setHp(CCardSprite *pCardSprite, cocos2d::CCLabelTTF *labelttf)
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

void CFightLayerScene::initOwnHuihe()
{
    cout<<"m_iZhujiangHuihe:"<<m_iZhujiangHuihe<<endl;
    m_iZhujiangHuihe=G_SkillManager::instance()->getSkillByIndex(m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng)->m_ihuihe;
}



void CFightLayerScene::initMonsterHuihe()
{
    cout<<"m_iMonsterZhujiangHuihe:"<<m_iMonsterZhujiangHuihe<<endl;
    m_iMonsterZhujiangHuihe=G_SkillManager::instance()->getSkillByIndex(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iJiChuJineng)->m_ihuihe;
    
}

void CFightLayerScene::initAssignHuihe()
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

int CFightLayerScene::getWinStats()
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
void CFightLayerScene::callBackOwnAnimationChangeFlag(CCObject *pSend)
{
    cout<<(this->isOwnActionEnd?"isOwnActionEnd:true":"isOwnActionEnd:false")<<endl;
    this->isOwnActionEnd=true;
}

void CFightLayerScene::callBackMonsterAnimationChangeFlag(CCObject *pSend)
{
     cout<<(this->isMonsterEnd?"isMonsterEnd:true":"isMonsterEnd:false")<<endl;
     this->isMonsterEnd=true;
}

void CFightLayerScene::checkOwnIsDeadAndMove()
{
    if (m_vfightCardSprite[m_vFightCardIndex]->isDead)
    {
       isOwnActionEnd=false;
       ((CCardPanel *)(this->getChildByTag(m_vfightCardSprite[m_vFightCardIndex]->tag)))->setDead();
       
         G_SkillManager::instance()->dealWithSkillShanghai(m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iZhangHunJiachi, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex,m_vMonsterCardIndex, jiaHp, JianHp,this);
        m_vFightCardIndex++;
         initOwnHuihe();
        
    }

}
void CFightLayerScene::animationMoveToLast(class CCObject  *pSend)
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
                getChildByTag(m_vfightCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightLayerScene::callBackOwnAnimationChangeFlag)),NULL));
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
                getChildByTag(m_vMonsterCardSprite[i]->tag)->runAction(CCSequence::create(CCMoveTo::create(0.2f,point),CCCallFunc::create(this, callfunc_selector(CFightLayerScene::callBackMonsterAnimationChangeFlag)),NULL));
            }
            setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite,(CCLabelTTF *) getChildByTag(20001));
        }

    }
}
void CFightLayerScene::checkMonsterIsDeadAndMove()
{
    if (m_vMonsterCardSprite[m_vMonsterCardIndex]->isDead)
    {
        ((CCardPanel *)(this->getChildByTag(m_vMonsterCardSprite[m_vMonsterCardIndex]->tag)))->setDead();
        isMonsterEnd=false;
         G_SkillManager::instance()->dealWithSkillShanghai(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iZhangHunJiachi,m_vMonsterCardSprite,m_vfightCardSprite,m_vMonsterCardIndex,m_vFightCardIndex, jiaHp, JianHp,this);
       m_vMonsterCardIndex++;
       initMonsterHuihe();
   }
}
void CFightLayerScene::secheudelUpdateToDoDealWith(float times)
{
    cout<<"="<<gongjiHuiHe<<isOwnActionEnd<<isMonsterEnd<<endl;
    if( isMonsterEnd && isOwnActionEnd)
    {
         unschedule(schedule_selector(CFightLayerScene::secheudelUpdateToDoDealWith));
         fightLogic(gongjiHuiHe); 
    }
}

void CFightLayerScene::dealWithFight(CCObject *object)
{
    
    //先判断当前的双方的主将是否是死亡的 如果是四万的 就要切换位置

    int winStatus=getWinStats();
    cout<<"winStatus="<<winStatus<<endl;
    //先判断是否全部死亡；
    if(winStatus==-1)
    {
        // //输了
        loseDialog();
    }
    else if(winStatus==0)
    {
        if(!m_vfightCardSprite[m_vFightCardIndex]->isDead && !m_vMonsterCardSprite[m_vMonsterCardIndex]->isDead)
        {
            fightLogic(gongjiHuiHe);
            cout<<"aaaaaaaaaa"<<endl;
        }
        else
        {
            cout<<"bbbbbbbbbb"<<endl;
            isMonsterEnd=true;
            isOwnActionEnd=true;
            checkOwnIsDeadAndMove();
            checkMonsterIsDeadAndMove();
            schedule(schedule_selector(CFightLayerScene::secheudelUpdateToDoDealWith), 0.2f);
        }
    }
    else
    {
        //赢了
        winDialog();
    }
    
}

void CFightLayerScene::loseDialog()
{
    int tmp = 0;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 100000);
    
    cout<<"lose"<<endl;
}

void CFightLayerScene::winDialog()
{
    int tmp = 1;
    FightResultConfirm * resultConfirm = new FightResultConfirm();
    resultConfirm->setUserData((void*)tmp);
    resultConfirm->init();
    resultConfirm->autorelease();
    addChild(resultConfirm, 1000000);

    cout<<"win"<<endl;
}

void CFightLayerScene::animationCardPanel(class CCardPanel *card, void *tag)
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

void CFightLayerScene::animationShouShang(class CCardPanel *card,void *tag)
{
    if(getChildByTag(30001))
    {
        CCSprite *sprite=(CCSprite *)getChildByTag(30001);
        int spritetag=*(int*)tag;
        sprite->setPosition(getChildByTag(spritetag)->getPosition());
        sprite->setVisible(true);
    }
    else{
        CCSprite *sprite=CCSprite ::create((g_mapImagesPath+"fighting/hiten_1.png").c_str());
        addChild(sprite,10,30001);
        int spritetag=*(int*)tag;
        sprite->setPosition(getChildByTag(spritetag)->getPosition());
    }
}

void CFightLayerScene::animationMoveBack(class CCardPanel *card)
{
    card->runAction(CCMoveBy::create(0.2f,ccp(0,-100)));
}

void CFightLayerScene::setVistablHit()
{
    getChildByTag(30000)->setVisible(false);
    getChildByTag(30001)->setVisible(false);
    
}

void CFightLayerScene::fightLogic(int  huihe)
{
   cout<<"huihe:"<<huihe<<endl;
    switch (huihe) {
        case 1:
            //主将发动攻击
               gongjiHuiHe--;
            if(m_iZhujiangHuihe!=0)
            {
                 cout<<"huihe1:"<<huihe<<endl;
                m_iZhujiangHuihe--;
             
                //动画效果
                G_SkillManager::instance()->dealWithSkillShanghai(0, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex, m_vMonsterCardIndex,jiaHp,JianHp,this);
                setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite,(CCLabelTTF*) getChildByTag(20001));
            
            }
            else
            { cout<<"huihe2:"<<huihe<<" "<<m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng<<endl;
               // gongjiHuiHe--;
                G_SkillManager::instance()->dealWithSkillShanghai(m_vfightCardSprite[m_vFightCardIndex]->cardsprite->m_cardData.m_iJiChuJineng, m_vfightCardSprite, m_vMonsterCardSprite, m_vFightCardIndex, m_vMonsterCardIndex,jiaHp,JianHp,this);
                setHp(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite,(CCLabelTTF*) getChildByTag(20001));
                initOwnHuihe();
            }
            break;
        case 0:
        {
            //拥护技能
            //动画效果
            int indexYonghu=m_vfightCardSprite[4]->cardsprite->m_cardData.m_iYongHuJineng;
            gongjiHuiHe--;
            cout<<"indexYonghu:"<<indexYonghu<<endl;
            G_SkillManager::instance()->dealWithSkillShanghai(indexYonghu, m_vfightCardSprite, m_vMonsterCardSprite, 4, m_vMonsterCardIndex,jiaHp,JianHp,this);
            
        }
            break;
        case -1:
        {
            gongjiHuiHe--;

            if(m_iMonsterZhujiangHuihe!=0)
            {
                m_iMonsterZhujiangHuihe--;
                                //动画效果
                G_SkillManager::instance()->dealWithSkillShanghai(0, m_vMonsterCardSprite,m_vfightCardSprite,m_vMonsterCardIndex, m_vFightCardIndex,jiaHp,JianHp,this);
                setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite,(CCLabelTTF*) getChildByTag(20001));
            }
            else
            {
                cout<<"===>>>>"<<m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iJiChuJineng<<endl;
                initMonsterHuihe();
                G_SkillManager::instance()->dealWithSkillShanghai(m_vMonsterCardSprite[m_vMonsterCardIndex]->cardsprite->m_cardData.m_iJiChuJineng, m_vMonsterCardSprite,m_vfightCardSprite,m_vMonsterCardIndex, m_vFightCardIndex,jiaHp,JianHp,this);
                setHp(m_vfightCardSprite[m_vFightCardIndex]->cardsprite,(CCLabelTTF*) getChildByTag(20001));
            }
        }
            break;
        case -2:
        {
            int indexYonghu=m_vMonsterCardSprite[4]->cardsprite->m_cardData.m_iYongHuJineng;
            gongjiHuiHe=1;
            cout<<"indexYonghu:"<<indexYonghu<<endl;
            G_SkillManager::instance()->dealWithSkillShanghai(indexYonghu, m_vMonsterCardSprite,m_vfightCardSprite , 4, m_vFightCardIndex,jiaHp,JianHp,this);
        }
            break;
        default:
            break;
    }
}

void CFightLayerScene::updateGetGameDataToGetServerRandow()
{
    //延时 执行了这个函数
    dealWithFight(this);
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)