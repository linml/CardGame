//
//  SceneManager.cpp
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#include "SceneManager.h"

#include "MapScene.h"
#include "CardFactoryLayer.h"
#include "TaskSceneLayer.h"
#include "ExplorationLayer.h"
#include "CEvolutionLayer.h"
#include "ScenePlistConfig.h"
#include  "CFightLayer.h"

SceneManager::SceneManager()
{
    m_currscene=EN_CURRSCENE_NONE;
}
void SceneManager::runMapScene()
{
    if(m_currscene!=EN_CURRSCENE_MAPSCENE)
    {
        // create a scene. it's an autorelease object
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *pScene = CCScene::create();
        MapScene *mapLayer = new MapScene();
        mapLayer->setUserData((void*)(scene_datting.c_str()));
        mapLayer->init();
        mapLayer->autorelease();
        pScene->addChild(mapLayer);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(pScene);
        }
        else{
            pDirector->runWithScene(pScene);
        }
        m_currscene=EN_CURRSCENE_MAPSCENE;
    }
    
}

void SceneManager::runCreateCardScene()
{
    if(m_currscene!=EN_CURRSCENE_CARDFACTORYSCENE)
    {
        m_currscene=EN_CURRSCENE_CARDFACTORYSCENE;
        
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *scene=CCScene::create();
        CardFactoryLayer *cardfactory=CardFactoryLayer::Create<CardFactoryLayer>(scene_suipian.c_str());
        scene->addChild(cardfactory);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(scene);
            
        }
        else{
            pDirector->runWithScene(scene);
        }
    }
}
bool SceneManager::runSceneSelect(int touch_tag)
{
    //需要声音否？
    switch (touch_tag) {
        case 1009:
            runTaskScene();
            break;
        case 1003:
            runMapScene();
            break;
        case 1005:
            runExplorationScene();
            break;
        case 1006:
            runCreateCardScene();
            break;
        case 1004:
            runEvolutionScene();
            break;
        case 1007:
            runFightScene();
            break;
        case 1008:
            break;            
        default:
            return false;
            break;
    }
    return true;;
}

void SceneManager::runTaskScene()
{
    if(m_currscene!=EN_CURRSCENE_TASKSCENE)
    {
        m_currscene=EN_CURRSCENE_TASKSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *scene=CCScene::create();
        TaskSceneLayer *taskscene=TaskSceneLayer::Create<TaskSceneLayer>(scene_task.c_str());
        scene->addChild(taskscene);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(scene);
        }
        else
        {
            pDirector->runWithScene(scene);
        }
    }
}

void SceneManager::runFightScene()
{
    if(m_currscene!=EN_CURRSCENE_FIGHTSCENE)
    {
        // create a scene. it's an autorelease object
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *pScene = CCScene::create();
        CFightLayer *mapLayer = new CFightLayer();
        mapLayer->setUserData((void*)(scene_fightscene.c_str()));
        mapLayer->init();
        mapLayer->autorelease();
        pScene->addChild(mapLayer);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(pScene);
        }
        else{
            pDirector->runWithScene(pScene);
        }
        m_currscene=EN_CURRSCENE_FIGHTSCENE;
    }

}

void SceneManager::runUpgradeScene()
{
//    if(m_currscene!=EN_CURRSCENE_EVOLUTIONSCENE)
//    {
//        m_currscene=EN_CURRSCENE_EVOLUTIONSCENE;
//        CCDirector *pDirector=CCDirector::sharedDirector();
//        CCScene *scene=CCScene::create();
//        CEvolutionLayer *taskscene=CEvolutionLayer::Create<CEvolutionLayer>(scene_evolution.c_str());
//        scene->addChild(taskscene);
//        if(pDirector->getRunningScene())
//        {
//            pDirector->replaceScene(scene);
//        }
//        else
//        {
//            pDirector->runWithScene(scene);
//        }
//    }

}

void SceneManager::runEvolutionScene()
{
    if(m_currscene!=EN_CURRSCENE_EVOLUTIONSCENE)
    {
        m_currscene=EN_CURRSCENE_EVOLUTIONSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *scene=CCScene::create();
        CEvolutionLayer *taskscene=CEvolutionLayer::Create<CEvolutionLayer>(scene_evolution.c_str());
        scene->addChild(taskscene);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(scene);
        }
        else
        {
            pDirector->runWithScene(scene);
        }
    }
}

void SceneManager::runExplorationScene()
{
    if(m_currscene!=EN_CURRSCENE_EXPLORATIONSCENE)
    {
        m_currscene=EN_CURRSCENE_EXPLORATIONSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *scene=CCScene::create();
        ExplorationLayer *taskscene=ExplorationLayer::Create<ExplorationLayer>(scene_exploration.c_str());
        scene->addChild(taskscene);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(scene);
        }
        else
        {
            pDirector->runWithScene(scene);
        }
    }
}
