//
//  CActionItemLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-30.
//
//

#include "CActionItemLayer.h"
#include "CStructGameActionData.h"
#include "Utility.h"
#include "CPtPropConfigData.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"




CActionItemLayer::CActionItemLayer()
{
    size=CCDirector::sharedDirector()->getWinSize();
    height=380.0f;
}
CActionItemLayer::~CActionItemLayer()
{
    
}

CActionItemLayer *CActionItemLayer::Created(CStructGameActionData *data)
{
    CActionItemLayer *layer=new CActionItemLayer();
    if (!layer || !layer->initCreate(data)) {
        delete layer;
        layer=NULL;
        return NULL;
    }
    layer->autorelease();
    return layer;
}

bool  CActionItemLayer::initCreate(CStructGameActionData *data)
{
    if (!CCLayer::init()) {
        return false;
    }
    m_pData=data;
    createHead(data->getActionShowTime());
    createContext(data->getContent());
    switch (data->getActionShowType()) {
        case 1:
            //领奖
            createTips(data);
            break;
        case 2:
            //充值 //做一个充值界面
            createPng(data->getActionPic());
            createChongZhi(data);
            break;
        case 3:
            //创建副本 //需要添加的内容
            createPng(data->getActionPic());
            createHuben(data);
            break;
        case 4:
            //说明 什么都不加
            break;
        default:
            break;
    }
    if (height<0) {
        height=400;
    }
    else{
        height=380-height;
    }
    setContentSize(CCSizeMake(400,height ));
    return true;
}

void CActionItemLayer::createHead(string strHead)
{
    if (!getChildByTag(1)) {
        string word=Utility::getWordWithFile("word.plist", "huodongshijian");
        CCLabelTTF *labelTTF=CCLabelTTF::create(word.c_str(), "Arial", 20);
        labelTTF->setPosition(ccp(300,height));
        addChild(labelTTF,1,1);
        labelTTF->setColor(g_custom_color[13]);
        
    }
    if (!getChildByTag(2)) {
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 15);
        labelTTF->setPosition(ccp(300,height-30));
        addChild(labelTTF,1,2);
    }
    ((CCLabelTTF *)getChildByTag(2))->setString(strHead.c_str());
}

void CActionItemLayer::createContext(string  strContext)
{
    if (!getChildByTag(3)) {
        string word=Utility::getWordWithFile("word.plist", "huodongneirong");
        CCLabelTTF *labelTTF=CCLabelTTF::create(word.c_str(), "Arial", 20);
        labelTTF->setPosition(ccp(300,height-60));
        addChild(labelTTF,1,1);
        labelTTF->setColor(g_custom_color[13]);
    }
    if (!getChildByTag(4)) {
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 15,CCSizeMake(400, 0),kCCTextAlignmentCenter);
        labelTTF->setPosition(ccp(300,height-90));
        addChild(labelTTF,1,4);
    }
    ((CCLabelTTF *)getChildByTag(4))->setString(strContext.c_str());
    height-=(150+((CCLabelTTF *)getChildByTag(4))->getContentSize().height*0.5);
}

void CActionItemLayer::createTips(CStructGameActionData *data)
{
    string word=Utility::getWordWithFile("word.plist", "lingqujiangpin");
    if (data->getActionItemId1()!=0) {
        height-=80;
        //获取物品表里面的物品图片；
        CPtProp *m_itemProp=SinglePropConfigData::instance()->getPropById(data->getActionItemId1());
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath,m_itemProp->getIconName().c_str()));
        addChild(sprite,1,20);
        sprite->setPosition(ccp(100, height));
        //描述
        CCLabelTTF *labelTTF=CCLabelTTF::create(data->getActionItemTip1().c_str(), "Arial", 15);
        addChild(labelTTF,1,21);
        labelTTF->setPosition(ccp(180, height));
        CGameButtonControl *gamebutton =CGameButtonControl::createButton(TEXTMID, word.c_str(), "anniu2_Normal.png", "anniu2_Pressed.png");
        gamebutton->setPosition(ccp(400, height));
        addChild(gamebutton,1,22);

    }
    string word2=Utility::getWordWithFile("word.plist", "lingqujiangpin");
    if (data->getActionItemId2()!=0) {
        //获取物品表里面的物品图片；
        height-=60;
        CPtProp *m_itemProp=SinglePropConfigData::instance()->getPropById(data->getActionItemId2());
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath,m_itemProp->getIconName().c_str()));
        addChild(sprite,1,30);
        
        sprite->setPosition(ccp(100, height));
        //描述
        CCLabelTTF *labelTTF=CCLabelTTF::create(data->getActionItemTip2().c_str(), "Arial", 15);
        addChild(labelTTF,1,31);
         labelTTF->setPosition(ccp(180, height));
        CGameButtonControl *gamebutton =CGameButtonControl::createButton(TEXTMID, word2.c_str(), "anniu2_Normal.png", "anniu2_Pressed.png");
        gamebutton->setPosition(ccp(400, height));
        addChild(gamebutton,1,33);
        
    }
    string word3=Utility::getWordWithFile("word.plist", "lingqujiangpin");
    if (data->getActionItemId3()!=0) {
        
        CPtProp *m_itemProp=SinglePropConfigData::instance()->getPropById(data->getActionItemId3());
        if (m_itemProp)
        {
            height-=60;
            CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_propImagesPath,m_itemProp->getIconName().c_str()));
            addChild(sprite,1,40);
            sprite->setPosition(ccp(100, height));
            CCLabelTTF *labelTTF=CCLabelTTF::create(data->getActionItemTip3().c_str(), "Arial", 15);
            addChild(labelTTF,1,41);
            labelTTF->setPosition(ccp(180, height));
            CGameButtonControl *gamebutton =CGameButtonControl::createButton(TEXTMID, word3.c_str(), "anniu2_Normal.png", "anniu2_Pressed.png");
            gamebutton->setPosition(ccp(400, height));
            addChild(gamebutton,1,42);
            height+=120;
        }
        else{
            CCLog("data->getActionItemId3():%d",data->getActionItemId3());
        }
     
    }

}
void CActionItemLayer::createChongZhi(CStructGameActionData *data)
{
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, "进入", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(gamebutton,1,171);
    gamebutton->setPosition(ccp(200, height-60));
    height-=100;
}

void CActionItemLayer::createHuben(CStructGameActionData *data)
{
   
    CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, "进入", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(gamebutton,1,61);
    gamebutton->setPosition(ccp(200, height-60));
    height-=100;

}

void CActionItemLayer::createPng(string strPngName)
{
    CCLog("strPngName:%s",strPngName.c_str());
    if (!strPngName.empty()|| !strcmp(strPngName.c_str(),0)) {
        
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,strPngName.c_str()));
        if(sprite)
        {
            addChild(sprite,1,100);
            sprite->setPosition(ccp(180 , height-200));
            height-=(200+sprite->getContentSize().height*0.5);
        }
        else{
            CCLog("后台传输的PNG图片为NULL");
        }
    }
}
