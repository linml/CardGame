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
#include "LoadingScene.h"
#include "ExplorationScene.h"
#include "LoginScene.h"
#include "HallScene.h"
#include "CFightingCardLayerScene.h"
#include "CCardSettingScene.h"
#include "CFightingLayerScene.h"

SceneManager::SceneManager()
{
    srand(time(0));
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
           runSceneAnimation(pScene);
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
            runSceneAnimation(scene);
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,scene));
        }
    }
}

void SceneManager::runTargetScene(EN_CURRSCENE en_targetScene ,int inParam)
{
    switch (en_targetScene) {
            
        case EN_CURRSCENE_LOGINSCENE:
            runLoginScene();
            break;
        case EN_CURRSCENE_HALLSCENE:
            runHallScene(inParam);
            break;
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
        case EN_CURRSCENE_CARDSETTINGSCENE:
            runCardSettingScene(inParam);
        default:
            break;
    }
}

bool SceneManager::runSceneSelect(int touch_tag)
{
    //需要声音否？
    EN_CURRSCENE targScene=EN_CURRSCENE_NONE;
    targScene=(EN_CURRSCENE)touch_tag;
    if(m_currscene!=targScene && targScene!=EN_CURRSCENE_NONE)
    {
        CCDirector::sharedDirector()->replaceScene(LoadingScene::sceneWithTargetScene(m_currscene,targScene));
    }
    return true;;
}


void SceneManager::runLoginScene()
{
    if(m_currscene!=EN_CURRSCENE_LOGINSCENE)
    {
        m_currscene=EN_CURRSCENE_LOGINSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        
        CCScene *pScene = CLoginScene::scene();
        
        if(pDirector->getRunningScene())
        {
             runSceneAnimation(pScene);
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,pScene));
        }
    }

}
void SceneManager::runHallScene(int inOpenType)
{
    if(m_currscene!=EN_CURRSCENE_HALLSCENE)
    {
        m_currscene=EN_CURRSCENE_HALLSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        
        CCScene *pScene = CHallScene::scene(inOpenType);
        
        if(pDirector->getRunningScene())
        {
             runSceneAnimation(pScene);
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,pScene));
        }
    }

}

void SceneManager::runCardSettingScene(int inLastSceneTag)
{
    if(m_currscene!=EN_CURRSCENE_CARDSETTINGSCENE)
    {
        m_currscene=EN_CURRSCENE_CARDSETTINGSCENE;
        CCDirector *pDirector=CCDirector::sharedDirector();
        
        CCScene *pScene = CCardSettingScene::scene(inLastSceneTag);
        
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
             runSceneAnimation(pScene);
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
             runSceneAnimation(CFightingLayerScene::scene());
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,CFightingLayerScene::scene()));
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
            runSceneAnimation(CEvolutionLayer::scene());
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
            runSceneAnimation(CExploration::scene());
        }
        else
        {
            pDirector->runWithScene(CCTransitionFade::create(1.0f,CExploration::scene()));
        }
    }
}

void SceneManager::runSceneAnimation(CCScene *tagerScene)
{
    int iSceneIndex=1;
    CCDirector * pDirector=CCDirector::sharedDirector();
    switch (iSceneIndex) {
        case 1:
            pDirector->replaceScene(CCTransitionFade::create(1.0f,tagerScene));
            break;
        case 2:
            pDirector->replaceScene(CCTransitionShrinkGrow::create(1.0, tagerScene));
        case 3:
            pDirector->replaceScene(CCTransitionSplitRows::create(1.0, tagerScene));
        case 4:
            pDirector->replaceScene(CCTransitionSplitCols::create(1.0, tagerScene));
        default:
            break;
    }
}

// test:
void SceneManager::runTest()
{
    CCScene *scene = CLoginScene::scene();
    CCDirector::sharedDirector()->runWithScene(scene);
}
