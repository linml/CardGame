//
//  LoadingScene.h
//  91.cube
//
//  Created by linminglu on 13-6-6.
//
//

#ifndef ___1_cube__LoadingScene__
#define ___1_cube__LoadingScene__

#include <iostream>
#include "cocos2d.h"
#include "SceneManager.h"
class CFightingCardLayerLogic;
using namespace std;

class LoadingScene: public CCScene {
public:
    LoadingScene();
    ~LoadingScene();
    static LoadingScene* sceneWithTargetScene(EN_CURRSCENE currScene,EN_CURRSCENE targetScene);
private:
    virtual void update(float dt);
    void doScheule();
    bool initWithTargetScene(EN_CURRSCENE currScene,EN_CURRSCENE targetScene);
    void addParticle();
    void delParticle();
    void fightlogic(float tTimes);
    EN_CURRSCENE targetScene_;
    EN_CURRSCENE m_currScene_;
public:
    void  getMonsterInfo(CCObject *object);
private:
    CFightingCardLayerLogic *m_clFightLogic;
};


#endif /* defined(___1_cube__LoadingScene__) */
