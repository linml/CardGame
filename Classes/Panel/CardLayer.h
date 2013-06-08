//
//  CardLayer.h
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#ifndef ___1_cube__CardLayer__
#define ___1_cube__CardLayer__

#include <iostream>

#include "CBaseLayer.h"
#include "CardSprite.h"
#include "GameSpriteConfig.h"

class CCardLayer :public CBaseLayer
{
public:
    CCardLayer();
    ~CCardLayer();
public:
    template <class T>
    static T *CreateSprite(const char *fileName,CCardSprite *spriteProperty);
    
    bool initWithMapFile(const char *fileName);
    void showFightLayer();
    void showCardWholeLayer();
    int getHp();
    int getMp();
    void showAtk();
    void showDef();

    void showName(std::string str);
    void showStarLevel(int level);
    CCSprite *getHero();
    void  animationHeroMoveRight();
    void  animationHeroMoveLeft();
    void  animationBeiGongji(int isRight);
    bool  setSpriteProperty(CCardSprite *spriteProperty);
    CCLayer *getMapByTag();
private:
    void animationHeroMove(int fangxiang);
private:
    string mapFileName;
    CCardSprite *m_cCardSpriteProperty;
    
};
template <class T>
T *CCardLayer::CreateSprite(const char *fileName,CCardSprite *spriteProperty)
{
    T *layer=new T;
    if(!layer || !layer->setSpriteProperty(spriteProperty)||!layer->initWithMapFile(fileName))
    {
        delete layer;
        layer=NULL;
        return NULL;
    }
    layer->autorelease();
    return layer;
}


#endif /* defined(___1_cube__CardLayer__) */
