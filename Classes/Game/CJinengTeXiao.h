//
//  CJinengTeXiao.h
//  91.cube
//
//  Created by linminglu on 13-6-5.
//
//

#ifndef ___1_cube__CJinengTeXiao__
#define ___1_cube__CJinengTeXiao__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"
#include "CardLayer.h"
using namespace  cocos2d;

class CJinengTeXiao :public CCObject
{
public:
    virtual void showAnimation(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        this->m_pGongjiNode=node;
        this->m_pBeigongji=monster;
        this->showTexiao(layer);
    }
    virtual void showTexiao(CCLayer *layer){};
protected:
    CCNode *m_pGongjiNode;
    CCNode *m_pBeigongji;
    
};

class CJinengTeXiaoTexiao0:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao0(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
        
    }
};

class CJinengTeXiaoTexiao1:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao1(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
        
    }
};

class CJinengTeXiaoTexiao2:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao2(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
        
    }
};

class CJinengTeXiaoTexiao3:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao3(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
        
    }
};

class CJinengTeXiaoTexiao4:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao4(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
       
    }
};

class CJinengTeXiaoTexiao5:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao5(){}
public:
    virtual void showTexiao(CCLayer *layer)
    {
        if(m_pGongjiNode)
        {
  //          ((CCardLayer *)m_pGongjiNode)->animationHeroMoveRight();
        }
        CCParticleSystemQuad  *particle=CCParticleSystemQuad::create((resRootPath+"particle/skill-texiao/26-xingxing.plist").c_str());
        particle->setPosition(ccp(200,500));
        layer->addChild(particle,100,288);
        if(m_pBeigongji)
        {
//            ((CCardLayer *)m_pBeigongji)->animationBeiGongji(1);

        }
    }
};

class CJinengTeXiaoTexiao6:public CJinengTeXiao
{
    
public:
    virtual void showTexiao(CCLayer *layer)
    {
        
    }
};



#endif /* defined(___1_cube__CJinengTeXiao__) */
