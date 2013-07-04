//
//  LoadingScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-6.
//
//

#include "LoadingScene.h"
#include "SceneManager.h"
#include "PtHttpClient.h"
#include "gamePlayer.h"
#include "Utility.h"
#include "gamePlayer.h"

LoadingScene::LoadingScene()
{
    
}

LoadingScene *LoadingScene::sceneWithTargetScene(EN_CURRSCENE currScene,EN_CURRSCENE targetScene)
{
    // 以下代码生成了一个当前类的自动释放对象
    LoadingScene *aLoadingScene = new LoadingScene();
    aLoadingScene->initWithTargetScene(currScene,targetScene);
    aLoadingScene->autorelease();
    return aLoadingScene;
}

void LoadingScene::addParticle()
{
    Utility::addPtActionScript("juqi1/juqi1.act");
    for (int i=0; i< SinglePlayer::instance()->m_hashmapFight.size() ; i++)
    {
        
    }
    for (int i=0; i< SinglePlayer::instance()->m_hashmapMonster.size() ; i++)
    {
        
    }
    //加载例子特效
}

void LoadingScene::delParticle()
{
    
}

bool LoadingScene::initWithTargetScene(EN_CURRSCENE currScene,EN_CURRSCENE targetScene) {
    if (CCScene::init()) {
        targetScene_ = targetScene;
        m_currScene_ = currScene;
        //创建一个Label标签放在屏幕中央
        CCLabelTTF* label = CCLabelTTF::create("Loading ...",
                                                        "Arial", 64);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        label->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(label);
        // 必须在下一帧才加载目标场景！
        this->runAction(CCSequence::create(CCDelayTime::create(1.0),CCCallFunc::create(this, callfunc_selector(LoadingScene::doScheule)),NULL));
        return true;
    }
    
    return false;
}


void LoadingScene::doScheule()
{
    if(targetScene_==EN_CURRSCENE_FIGHTSCENE)
    {
        cout<<"this  will get tiem  load  server monster"<<endl;
        SinglePlayer::instance()->forTestMonsterCard();
    }
    this->scheduleUpdate();
}

void LoadingScene::update(float delta) {
    
    if(targetScene_==EN_CURRSCENE_FIGHTSCENE)
    {
        //loading the  例子效果
        addParticle();
        //addFightResource();
    }
        //终止所有的预定信息
        this->unscheduleAllSelectors();
        // 通过TargetScenes这个枚举类型来决定加载哪个场景
        SingleSceneManager::instance()->runTargetScene(targetScene_);
}