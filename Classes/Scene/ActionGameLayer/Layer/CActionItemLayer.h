//
//  CActionItemLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-30.
//
//

#ifndef ___1_cube__CActionItemLayer__
#define ___1_cube__CActionItemLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
using namespace std;
using namespace cocos2d;
class CStructGameActionData;

class CActionItemLayer:public cocos2d::CCLayer
{
public:
    static CActionItemLayer *Created(CStructGameActionData *data);
    bool  initCreate(CStructGameActionData *data);
public:
    CActionItemLayer();
    ~CActionItemLayer();
private:
    void createHead(string strHead);//创建一个 标题
    void createContext(string  strContext);//创建一个内容
    void createPng(string strPngName);
    void createTips(CStructGameActionData *data);
    void createHuben(CStructGameActionData *data);
    void createChongZhi(CStructGameActionData *data);
private:
    CCSize size;
    CStructGameActionData *m_pData;
    float height;
   
    
};

#endif /* defined(___1_cube__CActionItemLayer__) */
