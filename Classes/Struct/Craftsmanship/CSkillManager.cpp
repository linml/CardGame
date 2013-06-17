//
//  GongjiPutongJineng.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "CSkillManager.h"
#include "gameConfig.h"
#include "cocos2d.h"
#include "CCardPanel.h"
#include "CFightLayerScene.h"
using namespace cocos2d;
/*
 class CCraftsmanship gstr_sskillList[]=
 {
 {EN_SKILL_BASICSKILL,1,"女神之枪"},
 {EN_SKILL_BLESSINGSKILL,2,"女神之拥"},
 {EN_SKILL_ADVOCACYSKILL,3,"英灵颂歌"},
 {EN_SKILL_BASICSKILL,4,"寒霜号角"},
 {EN_SKILL_BLESSINGSKILL,5,"水晶龙甲"},
 {EN_SKILL_ADVOCACYSKILL,6,"岩石技能"},
 {EN_SKILL_BASICSKILL,7,"藤编"},
 {EN_SKILL_BLESSINGSKILL,8,"冰雪森林"},
 {EN_SKILL_ADVOCACYSKILL,9,"回复术"},
 {EN_SKILL_BASICSKILL,10,"铁锤挥击"},
 {EN_SKILL_BLESSINGSKILL,11,"大地守护"},
 {EN_SKILL_ADVOCACYSKILL,12,"精锻"},
 {EN_SKILL_BASICSKILL,13,"霜爪击"},
 {EN_SKILL_BLESSINGSKILL,14,"狼魂"},
 {EN_SKILL_ADVOCACYSKILL,15,"撕咬"},
 };
 */

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}
inline  int  RangeLand(int min=1,int max=100)
{
    return  rand() % (max-min+1) + min;
}
CSkillManager::CSkillManager()
{
    m_skillList.push_back(new BasicCCraftsmanship(1,"女神之枪",g_str_skillPathPlist+"0-sanmang.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(2,"女神之拥",g_str_skillPathPlist+"0-xingxing.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(3,"英灵颂歌",g_str_skillPathPlist+"7-zhongji.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(4,"寒霜号角",g_str_skillPathPlist+"10-chouxue.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(5,"水晶龙甲",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(6,"岩石技能",g_str_skillPathPlist+"18-xingxing.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(7,"藤编",g_str_skillPathPlist+"26-xingxing.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(8,"冰雪森林",g_str_skillPathPlist+"32-xuanyunbuff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(9,"回复术",g_str_skillPathPlist+"35-xingxing.plist",0,0.5));//,
    m_skillList.push_back(new BasicCCraftsmanship(10,"铁锤挥击",g_str_skillPathPlist+"32-xuanyunbuff.plist",3,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(11,"大地守护",g_str_skillPathPlist+"penxue.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(12,"精锻",g_str_skillPathPlist+"home-jiesuo.plist",0,1));//,
    m_skillList.push_back(new BasicCCraftsmanship(13,"霜爪击",g_str_skillPathPlist+"changjing-yan.plist",2,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(14,"狼魂",g_str_skillPathPlist+"32-xuanyunbuff.plist",0,1));//,
    m_skillList.push_back(new AdvocacyCCraftsmanship(15,"撕咬",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new BasicCCraftsmanship(16,"爪击",g_str_skillPathPlist+"14-buff.plist",0,1));//,
    m_skillList.push_back(new DeadCCraftsmanship(17,"寒月守护",g_str_skillPathPlist+"14-buff.plist",0,1));//,
}

CSkillManager::~CSkillManager()
{
    DELETE_POINT_VECTOR(m_skillList,vector<CCraftsmanship* >);
}


CCraftsmanship *CSkillManager::getSkillByIndex(int index)
{
    if (index>=1&&index<m_skillList.size())
    {
        return m_skillList[index-1];
    }
    return NULL;
}

int  CSkillManager::puTongGongJi(SFightCardSprite ** ownSprite, SFightCardSprite **enemySprite)
{
    if((*ownSprite)->isCannotATK())
    {
        return -1;
    }
    else
    {
        int oldHp=(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp;
        //普通攻击    A攻击者攻击- B敌人的防御  =伤害  需要加上  防护的伤害
        (*enemySprite)->cardsprite->m_cardData.m_unCurrentHp -=(* ownSprite)->cardsprite->m_cardData.m_unPlayerCardAttack-(*enemySprite)->cardsprite->m_cardData.m_unPlayerCardDefence;
        if((*enemySprite)->cardsprite->m_cardData.m_unCurrentHp<0)
        {
            (*enemySprite)->cardsprite->m_cardData.m_unCurrentHp=0;
        }
        (*enemySprite)->dealLastHp();
        (*enemySprite)->fantanShanghai(ownSprite);
        (*enemySprite)->initShangHai();
        int retHp=oldHp-(*enemySprite)->cardsprite->m_cardData.m_unCurrentHp ;
        return retHp>=0?retHp:0;
    }
}
void CSkillManager::removeSprite(cocos2d::CCNode *node,void *tag)
{
    CCSprite *sprite=(CCSprite *)(tag);
    if(layer)
    {
        layer->removeChild(sprite, true);
    }
}

void CSkillManager::animationDealWithSkillShanghai(vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianhp,cocos2d::CCLayer *layer)
{
    CCardPanel * fightSprite=(CCardPanel *)(layer->getChildByTag(ownCardProperty[ownIndex]->tag));
    fightSprite->runAction(CCSequence::create(
                                              CCMoveBy::create(0.2f,ccp(0,100)),CCCallFuncND::create(layer, callfuncND_selector(CFightLayerScene::animationCardPanel),(void *)&ownCardProperty[ownIndex]->tag),
                                                    CCDelayTime::create(0.1f),
                                                    CCCallFuncND::create(layer, callfuncND_selector(CFightLayerScene::animationShouShang),(void *)&enemyCardpropert[enemyIndex]->tag),  CCDelayTime::create(0.1f),CCMoveBy::create(0.2f,ccp(0,-100)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::setVistablHit)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)),NULL
                                              ));
}

void  CSkillManager::dealWithSkillShanghai(int skillIndex,vector<SFightCardSprite *>ownCardProperty,vector<SFightCardSprite *> enemyCardpropert,int  ownIndex,int enemyIndex,int &jiaHp,int &jianHp,cocos2d::CCLayer *layer)
{
    jianHp=-1;
    jiaHp=-1;
    this->layer=layer;
    if(ownIndex>ownCardProperty.size() ||enemyIndex >enemyCardpropert.size() )
    {
        cout<<"AAAAAAAAAAAAAAAA"<<endl;
        PT_ERR_LOG("error");
        return ;
    }
    cout<<"skillIndex===>"<<skillIndex<<endl;
    switch (skillIndex)
    {
        case 0:
        {
            jianHp=puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
            if(jianHp<=0)
            {
                jianHp=0;
            }
            animationDealWithSkillShanghai(ownCardProperty,enemyCardpropert,ownIndex,enemyIndex,jiaHp,jianHp,layer);
        }
            break;
        case 1:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                layer->getChildByTag(ownCardProperty[ownIndex]->tag)->runAction(CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)));
                return ;
            }
            jianHp= enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            //回合攻击
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack*2-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence;
            enemyCardpropert[enemyIndex]->dealLastHp();
            enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
            enemyCardpropert[enemyIndex]->initShangHai();
            jianHp-=enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            animationDealWithSkillShanghai(ownCardProperty,enemyCardpropert,ownIndex,enemyIndex,jiaHp,jianHp,layer);
            
        }
            break;
        case 2:
        {
            //死亡加成
            for (int i=0; i<ownCardProperty.size(); i++) {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack+=5;
                }
                for (int i=ownIndex; i<=3; i++) {
                    CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_1.png").c_str());
                    this->layer->addChild(sprite,1000,3100);
                    CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                    sprite->setPosition(ccp(point.x,point.y+200));
                    if(i!=3)
                    {
                        sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                    }
                    else
                    {
                        cout<<"run dealwithFight"<<endl;
                        sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
                    }

                    
                }
            }
        }
            break;
        case 3:
        {
            //拥护攻击
            if(RangeLand()>=50)
            {
                //吸血效果
                ownCardProperty[ownIndex]->cardsprite->m_cardData.setAddCurrentHp(2);
                enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -=2;
                 animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
                
            }
            else
            {
                puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
                animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            }
        }
            break;
        case 4:
        {
            enemyCardpropert[enemyIndex]->m_ibingdong+=1; //回合 发动冰冻技能
          CCardPanel *card=(CCardPanel *)layer->getChildByTag(ownCardProperty[ownIndex]->tag)  ;
            card->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp(0,100)),CCDelayTime::create(0.2f),CCMoveBy::create(0.2f, ccp(0,-100)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)),NULL));
        }
            break;
        case 5:
        {
            for (int i=0; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardDefence+=3;
                }
            }
            for (int i=ownIndex; i<=3; i++)
            {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                this->layer->addChild(sprite,1000,ownCardProperty[i]->tag);
                CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
                }

            }
        }
            break;
        case 6:
        {
            if(RangeLand()<=50)
            {
                jianHp =puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
                animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            }
            else
            {
                
                for (int i=0; i<=3; i++) {
                    
                    if(!ownCardProperty[i]->isDead)
                    {
                        ownCardProperty[i]->m_iHuduanshanghai+=2;
                        CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                        this->layer->addChild(sprite,1000,11111);
                        CCPoint point=layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                        sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)),NULL));
                        break;
                    }
                    
                }
                
                
            }
        }
            break;
        case 7:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                return ;
            }
            jianHp= enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            //回合攻击
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= 3;
            enemyCardpropert[enemyIndex]->dealLastHp();
            enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
            enemyCardpropert[enemyIndex]->initShangHai();
            jianHp-=enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
        }
            break;
        case 8:
        {
            
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.setAddCurrentHp(3);
                }
            }
            for (int i=ownIndex; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                this->layer->addChild(sprite,1000,ownCardProperty[i]->tag);
                CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
                }

            }
            
        }
            break;
        case 9:
        {
            if(RangeLand()<=50)
            {
                jianHp =puTongGongJi(&ownCardProperty[ownIndex],&enemyCardpropert[enemyIndex]);
                animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            }
            else
            {
                ownCardProperty[ownIndex]->cardsprite->m_cardData.setAddCurrentHp(3); //两倍伤害
                animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            }
        }
            break;
        case 10:
        {
            ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardDefence+=3;
            CCSprite *sprite=CCSprite::create((g_mapImagesPath +"/fighting/buff_2.png").c_str());
            CCPoint point=layer->getChildByTag(ownCardProperty[ownIndex]->tag)->getPosition();
            sprite->setPosition(ccp(point.x,point.y+200));
            sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)),NULL));
        }
            break;
        case 11:
        {
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack +=1;
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardDefence+=1;
                }
            }
            for (int i=ownIndex; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                this->layer->addChild(sprite,1000,ownCardProperty[i]->tag);
                CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
                }

            }
            
        }
            break;
        case 12:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                return ;
            }
            jianHp= enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            //回合攻击
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= (ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence)*2;
            enemyCardpropert[enemyIndex]->dealLastHp();
            enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
            enemyCardpropert[enemyIndex]->initShangHai();
            jianHp-=enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            
        }
            break;
        case 13:
        {
            if(ownCardProperty[ownIndex]->isCannotATK())
            {
                return ;
            }
            jianHp= enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
            //回合攻击
            enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp -= (ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unPlayerCardAttack-enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unPlayerCardDefence)*1.5;
            enemyCardpropert[enemyIndex]->dealLastHp();
            enemyCardpropert[enemyIndex]->fantanShanghai(&ownCardProperty[ownIndex]);
            enemyCardpropert[enemyIndex]->initShangHai();
            jianHp-=enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp;
             animationDealWithSkillShanghai(ownCardProperty, enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            
        }
            break;
        case 14:
        {
            for (int i=ownIndex; i<ownCardProperty.size(); i++)
            {
                if(!ownCardProperty[i]->isDead)
                {
                    ownCardProperty[i]->cardsprite->m_cardData.m_unPlayerCardAttack +=2;
                    ownCardProperty[i]->cardsprite->m_cardData.m_unCurrentHp+=1;
                }
            }
            for (int i=ownIndex; i<=3; i++) {
                CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                this->layer->addChild(sprite,1000,ownCardProperty[i]->tag);
                CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                sprite->setPosition(ccp(point.x,point.y+200));
                if(i!=3)
                {
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                }
                else
                {
                    cout<<"run dealwithFight"<<endl;
                    sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
                }

            }
            
            
        }
            break;
        case 15:
        {
            for (int i=0; i<ownCardProperty.size()-1; i++)
            {
                cout<<"===="<<endl;
                if(!ownCardProperty[i]->isDead)
                {
                    cout<<"un dead"<<i<<endl;
                    ownCardProperty[i]->cardsprite->m_cardData.setAddCurrentHp(1);
                    CCSprite *sprite =CCSprite::create((g_mapImagesPath+"/fighting/buff_2.png").c_str());
                    layer->addChild(sprite,100,i+400+ownCardProperty[i]->tag);
                    CCPoint point=this->layer->getChildByTag(ownCardProperty[i]->tag)->getPosition();
                    sprite->setPosition(ccp(point.x,point.y+200));
                    if(i!=3)
                    {
                        sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f), CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),NULL));
                    }
                    else
                    {
                        cout<<"run dealwithFight"<<endl;
                        sprite->runAction(CCSequence::create(CCMoveTo::create(0.2f, point),CCDelayTime::create(0.2f),CCCallFuncND::create(this, callfuncND_selector(CSkillManager::removeSprite), (void*)(sprite)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::dealWithFight)),NULL));
                    }
                    
                } 
            }
        }
            break;
        case 16:
            animationDealWithSkillShanghai(ownCardProperty,enemyCardpropert, ownIndex, enemyIndex, jiaHp, jianHp, layer);
            break;
        case 17:
        {
            CCardPanel *sprite=(CCardPanel *)layer->getChildByTag(ownCardProperty[ownIndex]->tag);
            sprite->runAction(CCSequence::create(CCMoveBy::create(0.2f,ccp(0,100)),CCDelayTime::create(0.2f),CCMoveBy::create(0.2f,ccp(0,-100)),CCCallFunc::create(layer, callfunc_selector(CFightLayerScene::animationMoveToLast)),NULL));
        }
            break;
        default:
            break;
    }
    if(enemyCardpropert[enemyIndex]->cardsprite->m_cardData.m_unCurrentHp==0)
    {
        enemyCardpropert[enemyIndex]->isDead=true;
    }
    if(ownCardProperty[ownIndex]->cardsprite->m_cardData.m_unCurrentHp==0)
    {
        ownCardProperty[ownIndex]->isDead=true;
    }
}

