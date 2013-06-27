//
//  CCardFightingLayerScene.h
//  91.cube
//
//  Created by linminglu on 13-6-27.
//
//

#ifndef ___1_cube__CCardFightingLayerScene__
#define ___1_cube__CCardFightingLayerScene__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
class CFightCard;

class CCardFightingLayerScene : public CCLayer
{
public:
    CCardFightingLayerScene();
    ~CCardFightingLayerScene();
public:
    bool init();
    CREATE_FUNC(CCardFightingLayerScene);
public:
    void fSchudelUpdate(float t);
private:
    void createFightCard();
    void createMonsterCard();
private:
    vector<CFightCard *>m_vFightingCard;
    vector<CFightCard *>m_vMonsterCard;
    
    
    
    
};

#endif /* defined(___1_cube__CCardFightingLayerScene__) */
