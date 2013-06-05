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
using namespace  cocos2d;

class CJinengTeXiao :public CCObject
{
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster){};
};

class CJinengTeXiaoTexiao0:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao0(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        
    }
};

class CJinengTeXiaoTexiao1:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao1(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        
    }
};

class CJinengTeXiaoTexiao2:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao2(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        
    }
};

class CJinengTeXiaoTexiao3:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao3(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        
    }
};

class CJinengTeXiaoTexiao4:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao4(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
       
    }
};

class CJinengTeXiaoTexiao5:public CJinengTeXiao
{
public:
    CJinengTeXiaoTexiao5(){}
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node,CCNode *monster)
    {
        CCParticleSystemQuad  *particle=CCParticleSystemQuad::create((resRootPath+"particle/skill-texiao/26-xingxing.plist").c_str());
        particle->setPosition(ccp(200,500));
        layer->addChild(particle,100,288);
    }
};

class CJinengTeXiaoTexiao6:public CJinengTeXiao
{
    
public:
    virtual void showTexiao(CCLayer *layer,CCNode *node)
    {
        
    }
};



#endif /* defined(___1_cube__CJinengTeXiao__) */
