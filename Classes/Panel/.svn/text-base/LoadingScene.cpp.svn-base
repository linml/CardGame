//
//  LoadingScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-6.
//
//

#include "LoadingScene.h"
#include "SceneManager.h"

LoadingScene::LoadingScene()
{
    
}

LoadingScene *LoadingScene::sceneWithTargetScene(EN_CURRSCENE targetScene)
{
    // 以下代码生成了一个当前类的自动释放对象
    LoadingScene *aLoadingScene = new LoadingScene();
    aLoadingScene->initWithTargetScene(targetScene);
    aLoadingScene->autorelease();
    return aLoadingScene;
}

bool LoadingScene::initWithTargetScene(EN_CURRSCENE targetScene) {
    if (CCScene::init()) {
        targetScene_ = targetScene;
        //创建一个Label标签放在屏幕中央
        CCLabelTTF* label = CCLabelTTF::labelWithString("Loading ...",
                                                        "Arial", 64);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        label->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(label);
        // 必须在下一帧才加载目标场景！
        this->runAction(CCSequence::create(CCDelayTime::create(1.0),CCCallFunc::create(this, callfunc_selector(LoadingScene::doscheule)),NULL));
        
        return true;
    }
    
    return false;
}
void LoadingScene::doscheule()
{
    this->scheduleUpdate();
}

void LoadingScene::update(float delta) {
    //终止所有的预定信息
    this->unscheduleAllSelectors();
    // 通过TargetScenes这个枚举类型来决定加载哪个场景
    SingleSceneManager::instance()->runTargetScene(targetScene_);
}