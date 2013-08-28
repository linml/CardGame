//
//  CGameErrorConfig.h
//  91.cube
//
//  Created by linminglu on 13-8-15.
//
//

#ifndef ___1_cube__CGameErrorConfig__
#define ___1_cube__CGameErrorConfig__

#include <iostream>
#include <string>
#include <map>
#include "cocos2d.h"
#include <vector>
#include "Utility.h"
using namespace std;
/**
 该文件准备定义一个 服务端错误的表述信息
 */
enum EN_DIALOGTYPE {
    EN_DIALOGTYPE_NONE=0,
    EN_DIALOGTYPE_ONLYOK ,
    EN_DIALOGTYPE_ONLYCANCEL,
    EN_DIALOGTYPE_ONLYOKANDCANCEL
    };


class CErrorDialog :public cocos2d::CCLayer {
public:
    //std::string str,CCObject *, SEL_CallFunc okFunction,SEL_CallFunc cancelFunction
    CErrorDialog();
    ~CErrorDialog();
    static void CreateByMsgId(std::string strKeyId,EN_DIALOGTYPE entypeButton);
    static CCScene* scene()
    {
        // 'scene' is an autorelease object
        CCScene *scene = CCScene::create();
        
        // 'layer' is an autorelease object
        CErrorDialog *layer = new CErrorDialog ();
        layer->initCreate("10000",EN_DIALOGTYPE_ONLYCANCEL);
        layer->autorelease();
        // add layer as a child to scene
        scene->addChild(layer);
        
        // return the scene
        return scene;
    }
private:
    void removeTagArray(string str);
    bool initCreate(std::string str,EN_DIALOGTYPE entypeButton);
public:
    CREATE_FUNC(CErrorDialog);
private:
    vector<TouchRect> m_cTouches;
    EN_DIALOGTYPE m_enDialogType;
};
#endif /* defined(___1_cube__CGameErrorConfig__) */
