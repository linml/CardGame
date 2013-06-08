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
#include "gameConfig.h"
#include "CFightLayerScene.h"
#include "LoadingScene.h"

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
            pDirector->replaceScene(CCTransitionFade::create(1.0f,pScene));
        }
        else{
            pDirector->runWithScene(CCTransitionFade::create(1.0f,pScene));
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
            pDirector->replaceScene(CCTransitionFade::create(1.0f,scene));
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,scene));
        }
    }
}

void SceneManager::runTargetScene(EN_CURRSCENE en_targetScene)
{
    switch (en_targetScene) {
        case EN_CURRSCENE_MAPSCENE :
            runMapScene();
            break;
        case  EN_CURRSCENE_TASKSCENE :
            runTaskScene();
            break;
        case   EN_CURRSCENE_CARDFACTORYSCENE:
            runCreateCardScene();
            break;
        case  EN_CURRSCENE_EXPLORATIONSCENE:
            runExplorationScene();
            break;
        case   EN_CURRSCENE_EVOLUTIONSCENE:
            runEvolutionScene();
            break;
        case   EN_CURRSCENE_UPGRADESCENE:
            runUpgradeScene();
            break;
        case   EN_CURRSCENE_FIGHTSCENE:
            runFightScene();
            break;
        default:
            break;
    }
}

bool SceneManager::runSceneSelect(int touch_tag)
{
    //需要声音否？
    EN_CURRSCENE targScene=EN_CURRSCENE_NONE;
    switch (touch_tag) {
        case 1009:
            targScene=EN_CURRSCENE_TASKSCENE;
            break;
        case 1003:
            targScene=EN_CURRSCENE_MAPSCENE;
            //            runMapScene();
            break;
        case 1005:
            targScene=EN_CURRSCENE_EXPLORATIONSCENE;
            //runExplorationScene();
            break;
        case 1006:
            targScene=EN_CURRSCENE_CARDFACTORYSCENE;
            //            runCreateCardScene();
            break;
        case 1004:
            targScene=EN_CURRSCENE_EVOLUTIONSCENE;
            // runEvolutionScene();
            break;
        case 1007:
            targScene=EN_CURRSCENE_FIGHTSCENE;
            //            runFightScene();
            break;
        case 1008:
            break;
        default:
            return false;
            break;
    }
    
    if(m_currscene!=targScene && targScene!=EN_CURRSCENE_NONE)
    {
        CCDirector::sharedDirector()->replaceScene(LoadingScene::sceneWithTargetScene(targScene));
    }
    return true;;
}

void SceneManager::runTaskScene()
{
    if(m_currscene!=EN_CURRSCENE_TASKSCENE)
    {
        m_currscene=EN_CURRSCENE_TASKSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        CCScene *pScene=CCScene::create();
        TaskSceneLayer *taskscene=TaskSceneLayer::Create<TaskSceneLayer>(scene_task.c_str());
        pScene->addChild(taskscene);
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(CCTransitionFade::create(1.0f,pScene));
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,pScene));
        }
    }
}

void SceneManager::runFightScene()
{
    if(m_currscene!=EN_CURRSCENE_FIGHTSCENE)
    {
        // create a scene. it's an autorelease object
        m_currscene=EN_CURRSCENE_FIGHTSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(CCTransitionFade::create(1.0f,CFightLayerScene::scene()));
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,CFightLayerScene::scene()));
        }
        
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
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(CCTransitionFade::create(1.0f,CEvolutionLayer::scene()));
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,CEvolutionLayer::scene()));
        }
    }
}

void SceneManager::runExplorationScene()
{
    if(m_currscene!=EN_CURRSCENE_EXPLORATIONSCENE)
    {
        m_currscene=EN_CURRSCENE_EXPLORATIONSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        if(pDirector->getRunningScene())
        {
            pDirector->replaceScene(CCTransitionFade::create(1.0f,CExplorationLayer::scene()));
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,CExplorationLayer::scene()));
        }
    }
}
