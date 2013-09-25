//
//  SceneManager.h
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#ifndef __en_cube__SceneManager__
#define __en_cube__SceneManager__

#include <iostream>
#include "cocos2d.h"
#include "CSingleton.h"
using namespace std;
using namespace cocos2d;
enum  EN_CURRSCENE
{
    EN_CURRSCENE_NONE=-1,
    EN_CURRSCENE_MAPSCENE = 0,
    EN_CURRSCENE_TASKSCENE ,
    EN_CURRSCENE_CARDFACTORYSCENE,
    EN_CURRSCENE_EXPLORATIONSCENE,
    EN_CURRSCENE_EVOLUTIONSCENE,
    EN_CURRSCENE_UPGRADESCENE,
    EN_CURRSCENE_FIGHTSCENE,
    EN_CURRSCENE_HALLSCENE,
    EN_CURRSCENE_LOGINSCENE,
    EN_CURRSCENE_CARDSETTINGSCENE
    
};
class SceneManager
{
    SceneManager();
public:
    void runMapScene();

    bool runSceneSelect(int touch_tag);
    void runTargetScene(EN_CURRSCENE en_targetScene, int inParam = 0);
private:
    void runCreateCardScene();
    void runTaskScene();
    void runExplorationScene();
    void runEvolutionScene();
    void runUpgradeScene();
    void runFightScene();
    void runLoginScene();
    void runHallScene(int inOpenType = 0);
    void runCardSettingScene();
    void runSceneAnimation(CCScene *tagerScene);
    //test
public:
    void runTest();

public:
   DECLARE_SINGLETON_CLASS(SceneManager)
private:
    EN_CURRSCENE m_currscene;

};

typedef Singleton<SceneManager> SingleSceneManager;

#endif /* defined(__en_cube__SceneManager__) */
