//
//  CGameRoleAnimation.cpp
//  91.cube
//
//  Created by linminglu on 13-9-5.
//
//

#include "CGameRoleAnimation.h"
#include "CGameTalkDialog.h"
#include "gameConfig.h"
#include "CGameStoryLayer.h"
CGameRoleAnimation::CGameRoleAnimation()
{
    wndSize=CCDirector::sharedDirector()->getWinSize();
}

CGameRoleAnimation::~CGameRoleAnimation()
{
    
}
void CGameRoleAnimation::dealAnimation()
{
    float value;
    CCAction *action=NULL;;
    switch (m_pGameDialog->getGameTalkUiPlan())
    {
        case 1:
            action=animation1(value);
            break;
        case 2:
            action=animation2(value);
            break;
        case 3:
            action=animation3(value);
            break;
        case 4:
            action=animation4(value);
            break;
        case 5:
            action=animation5(value);
            break;
        case 6:
            action=animation6(value);
            break;
        case 7:
            action=animation7(value);
            break;
        case 8:
            action=animation8(value);
            break;
        case 9:
            action=animation9(value);
            break;
            
        default:
            break;
    }
    bool isLeft=(m_pGameDialog->getGameTalkUiPlan()%2==1?true:false);
    startDealAction(action,value,isLeft);
    
}

void CGameRoleAnimation::endShowText(float seconed, cocos2d::CCNode *node)
{
    node->runAction(CCSequence::create(CCDelayTime::create(seconed),CCCallFunc::create(this, callfunc_selector(CGameRoleAnimation::showEnd)),NULL));
}

void CGameRoleAnimation::setDialogPosition(cocos2d::CCNode *node)
{
    if(node->isVisible())
    {
        CCPoint oldpoint=node->getPosition();
        node->setPosition(ccp(oldpoint.x,180));
    }
}




void CGameRoleAnimation::runActionAnimation(CCLayer *layer,CGameTalkDialog *newTalkDialog,CGameTalkDialog *oldDialog)
{
    m_Player=layer;
    m_pGameDialog=newTalkDialog;
    if(!oldDialog)
    {
        dealAnimation();
    }
    else{
        if (oldDialog==newTalkDialog)
        {
            //            //直接输出语句
            //            if (newTalkDialog->getGameTalkUiPlan()%2==0)
            //            {
            //                CCLabelTTF *labelttf=(CCLabelTTF *)(m_Player->getChildByTag(201)->getChildByTag(202));
            //                labelttf->setString(m_pGameDialog->getGameTalkDialogWord().c_str());
            //            }
            //            else{
            CCLabelTTF *labelttf=(CCLabelTTF *)(m_Player->getChildByTag(101)->getChildByTag(102));
            labelttf->setString(m_pGameDialog->getGameTalkDialogWord().c_str());
            //            }
        }
        else{
            
            disappearDialog();
            
            //            if(oldDialog->getGameTalkUiPlan()%2==0)
            //            {
            //                setDialogPosition(m_Player->getChildByTag(101));
            //                m_Player->getChildByTag(201)->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp(0, 300)),CCCallFunc::create(this, callfunc_selector(CGameRoleAnimation::moveDialogToUpEnd) ),NULL));
            //            }
            //            else{
            //                setDialogPosition(m_Player->getChildByTag(201));
            //                m_Player->getChildByTag(101)->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp(0, 300)),CCCallFunc::create(this, callfunc_selector(CGameRoleAnimation::moveDialogToUpEnd) ),NULL));
            //            }
            
        }
    }
}

void CGameRoleAnimation::startDealAction(CCAction *action, float startLableValue,bool isLeft)
{
    string str=CSTR_FILEPTAH(g_mapImagesPath,m_pGameDialog->getGameTalkDialogPng().c_str());
    CCLog("%s",str.c_str());
    CCSprite *sprite=(CCSprite *)m_Player->getChildByTag(100);
    CCTextureCache::sharedTextureCache()->removeTexture(sprite->getTexture());
    sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(str.c_str()));
    sprite->setVisible(true);
    if (isLeft) {
        sprite->setPosition(ccp(0,500));
    }
    else{
        sprite->setPosition(ccp(wndSize.width,500));
    }
    
    sprite->runAction(action);
    CCLabelTTF *labelttf=(CCLabelTTF *)(m_Player->getChildByTag(101)->getChildByTag(102));
    labelttf->setString(m_pGameDialog->getGameTalkDialogWord().c_str());
    CCLayerColor *layerColor=(CCLayerColor *)(m_Player->getChildByTag(101));
    if(isLeft)
    {
        layerColor->setPosition(ccp(280,500));
    }
    else{
        layerColor->setPosition(ccp(420,500));
    }
    layerColor->runAction(CCSequence::create(CCDelayTime::create(startLableValue),CCShow::create(),CCFadeTo::create(0.2, 125),CCCallFunc::create(this, callfunc_selector(CGameRoleAnimation::showEnd)),NULL));
}

void CGameRoleAnimation::disappearDialog()
{
    m_Player->getChildByTag(100)->runAction(CCFadeOut::create(0.2));
    m_Player->getChildByTag(101)->runAction(CCSequence::create(CCFadeOut::create(0.2),CCCallFunc::create(this, callfunc_selector(CGameRoleAnimation::moveDialogToUpEnd) ),NULL));
}
void CGameRoleAnimation::moveDialogToUpEnd()
{
    dealAnimation();
}



void CGameRoleAnimation::showEnd(CCObject *object)
{
    
    ((CGameStoryLayer *)m_Player)->setCaneTouch();
}

CCAction * CGameRoleAnimation::animation1(float &value)
{
    value=0.3f;
    return CCSequence::create(CCFadeIn::create(0.1f),CCMoveBy::create(0.2, ccp(100,0)),NULL);
}
CCAction * CGameRoleAnimation::animation2(float &value)
{
    value=0.3f;
    return CCSequence::create(CCFadeIn::create(0.1f),CCMoveBy::create(0.2, ccp(-100,0)),NULL);

}
CCAction * CGameRoleAnimation::animation3(float &value)
{
    value=0.3f;
    return  CCSequence::create(CCFadeIn::create(0.1f),CCJumpBy::create(0.2, ccp(100,0), 80, 3),NULL);
}
CCAction * CGameRoleAnimation::animation4(float &value)
{
    value=0.3f;
    return  CCSequence::create(CCFadeIn::create(0.1f),CCJumpBy::create(0.2, ccp(-100,0), 80, 3),NULL);
}
CCAction * CGameRoleAnimation::animation5(float &value)
{
    value=0.3f;
    return CCSequence::create(CCFadeIn::create(0.1f),CCSpawn::create(CCSequence::create(CCRotateBy::create(0.1, 50),CCRotateBy::create(0.1, -50),NULL),CCMoveBy::create(0.2, ccp(100,0)),NULL),NULL);
}

CCAction * CGameRoleAnimation::animation6(float &value)
{
     value=0.3f;
     return CCSequence::create(CCFadeIn::create(0.1f),CCSpawn::create(CCSequence::create(CCRotateBy::create(0.1, -50),CCRotateBy::create(0.1, +50),NULL),CCMoveBy::create(0.2, ccp(-100,0)),NULL),NULL);
}

CCAction * CGameRoleAnimation::animation7(float &value)
{
    value=0.3f;
  return   CCSequence::create(CCFadeIn::create(0.1f),CCEaseBackOut::create(CCMoveBy::create(0.2, ccp(100,0))),NULL);
}

CCAction * CGameRoleAnimation::animation8(float &value)
{
    value=0.3f;
    return   CCSequence::create(CCFadeIn::create(0.1f),CCEaseBackOut::create(CCMoveBy::create(0.2, ccp(-100,0))),NULL);

}
CCAction * CGameRoleAnimation::animation9(float &value)
{
    value=0.3f;
    return   CCSequence::create(CCFadeIn::create(0.1f),CCEaseBackIn::create(CCMoveBy::create(0.2, ccp(100,0))),NULL);

}
CCAction * CGameRoleAnimation::animation10(float &value)
{
    value=0.3f;
    return   CCSequence::create(CCFadeIn::create(0.1f),CCEaseBackIn::create(CCMoveBy::create(0.2, ccp(-100,0))),NULL);

}
