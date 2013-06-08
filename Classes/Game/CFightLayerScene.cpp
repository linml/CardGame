//
//  CFightLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#include "CFightLayerScene.h"
#include "PtMapUtility.h"
#include "CardLayer.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "CJinengTeXiao.h"

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
    mapLayer->setUserData((void*)(scene_fightscene.c_str()));
    mapLayer->init();
    mapLayer->autorelease();
    pScene->addChild(mapLayer);
    return pScene;
}

void CFightLayerScene::createMonster()
{
    SinglePlayer::instance()->forTestMonster();
    // 玩家 战斗时候的卡牌
    for (vector<CCardSprite *>::iterator it=SinglePlayer::instance()->m_vMonsterCardSprite.begin(); it!=SinglePlayer::instance()->m_vMonsterCardSprite.end(); it++)
    {
        CCLog("Monster");
        SFightCardSprite *fightCard=new SFightCardSprite;
        fightCard->cardsprite=*it;
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
        if(i==0)
        {
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vMonsterCardSprite[i]->cardsprite) ;
            this->addChild(sprite1,1,m_vMonsterCardSprite[i]->tag);
            sprite1->setPosition(ccp(1024-400,300));
            sprite1->setAnchorPoint(CCPointZero);
            CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 35);
            labelttf->setPosition(ccp(624,700));
            labelttf->setColor(ccc3(255, 0, 0));
            this->addChild(labelttf,1,20001);
            setHp(m_vfightCardSprite[i]->cardsprite,labelttf);
        }
        else  if(i==4)
        {
            //放在 拥护的位置
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vMonsterCardSprite[i]->cardsprite);
            this->addChild(sprite1,1,m_vMonsterCardSprite[i]->tag);
            sprite1->setPosition(ccp(1024-200,170));
            sprite1->setAnchorPoint(CCPointZero);
            sprite1->setScale(0.6);
        }
        else
        {
            //放在 拥护的位置
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vMonsterCardSprite[i]->cardsprite);
            this->addChild(sprite1,1,m_vMonsterCardSprite[i]->tag);
            sprite1->setPosition(ccp(520+(60+i*120),100));
            sprite1->setAnchorPoint(CCPointZero);
            sprite1->setScale(0.5);
        }
    }
}


void CFightLayerScene::createOwnFightCardPosition()
{
    
    // 玩家 战斗时候的卡牌
    for (vector<CCardSprite *>::iterator it=SinglePlayer::instance()->m_vZhangdouCardSprite.begin(); it!=SinglePlayer::instance()->m_vZhangdouCardSprite.end(); it++)
    {
        CCLog("dddd");
        SFightCardSprite *fightCard=new SFightCardSprite;
        fightCard->cardsprite=*it;
        fightCard->isDead=false;
        m_vfightCardSprite.push_back(fightCard);
    }
    //布阵
    for (int i=0;i<m_vfightCardSprite.size();i++)
    {
        m_vfightCardSprite[i]->tag=100+i;
        if(i==0)
        {
            CCLog("ccc");
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vfightCardSprite[i]->cardsprite) ;
            this->addChild(sprite1,1,m_vfightCardSprite[i]->tag);
            sprite1->setPosition(ccp(150,300));
            sprite1->setAnchorPoint(CCPointZero);
            CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 35);
            labelttf->setPosition(ccp(150,700));
            labelttf->setColor(ccc3(255, 0, 0));
            this->addChild(labelttf,1,20000);
            setHp(m_vfightCardSprite[i]->cardsprite,labelttf);
        }
        else  if(i==4)
        {
            //放在 拥护的位置
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vfightCardSprite[i]->cardsprite);
            this->addChild(sprite1,1,m_vfightCardSprite[i]->tag);
            sprite1->setPosition(ccp(0,170));
            sprite1->setAnchorPoint(CCPointZero);
            sprite1->setScale(0.6);
        }
        else{
            //放在 拥护的位置
            CCardLayer *sprite1=CCardLayer::CreateSprite<CCardLayer>(layer_cardSprite.c_str(), m_vfightCardSprite[i]->cardsprite);
            this->addChild(sprite1,1,m_vfightCardSprite[i]->tag);
            sprite1->setPosition(ccp(60+i*120,100));
            sprite1->setAnchorPoint(CCPointZero);
            sprite1->setScale(0.5);
        }
    }
}

bool CFightLayerScene::init()
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    currOwnCardSprite=NULL;
    monsterCardSprite=NULL;
	CCLog("MapScene::init");
	bool bRet = false;
	do
	{
		char *tmp = NULL;
		CC_BREAK_IF(!CCLayer::init());
		tmp = (char *)this->getUserData();
        CCLog("CFightLayer:%s",tmp);
		PtMapUtility::addChildFromScript(this,tmp);
		bRet = true;
	} while (0);
    //定义更具global的 appdata 值得 添加了 cccardlayer的 类别
    createOwnFightCardPosition();
    createMonster();
    CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 25);
    this->addChild(labelttf,10,20002);
    labelttf->setPosition(ccp(512,650));
    labelttf->setColor(ccc3(255, 0, 0));
    setText("对战ing");
    currIndexBegin=currIndexEnd=0;
    currOwnCardSprite=(CCardLayer *)getChildByTag(m_vfightCardSprite[0]->tag);
    monsterCardSprite=(CCardLayer *)getChildByTag(m_vMonsterCardSprite[0]->tag);
    
    scheduleOnce(schedule_selector(CFightLayerScene::updateGetGameDataToGetServerRandow), 1.0f);
	return bRet;
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

int CFightLayerScene::getWinStats()
{
    //先判断
    bool isCardAllDead=true;
    for (int i=0; i<m_vfightCardSprite.size(); i++)
    {
        if(!m_vfightCardSprite[i]->isDead)
        {
            isCardAllDead=false;
            break;
        }
    }
    
    if(isCardAllDead)
    {
        return -1;
    }
    
    if(m_vMonsterCardSprite.size()==5)
    {
        //先算自己是否全部
        for (int i=0; i<m_vMonsterCardSprite.size()-1; i++)
        {
            if (!m_vMonsterCardSprite[i]->isDead) {
                return 0;
            }
        }
        return 1;
        
    }
    else
    {
        for (int i=0; i<m_vMonsterCardSprite.size(); i++)
        {
            if (!m_vMonsterCardSprite[i]->isDead)
            {
                return 0;
            }
        }
        return 1;
    }
}


void CFightLayerScene::dealWithFight(CCObject *object)
{
    //当开局的时候
    if (currIndexBegin!=0 || currIndexBegin==currIndexEnd)
    {
        return;
    }
    if(currIndexEnd== )
    

       
}

void CFightLayerScene::updateGetGameDataToGetServerRandow()
{
    //延时 执行了这个函数
    dealWithFight(this);
}

//#undef DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE)