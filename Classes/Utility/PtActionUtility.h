//
//  PtActionUtility.h
//  91.cube
//
//  Created by xianbei1987 on 13-6-8.
//
//

#ifndef _1_cube_PtActionUtility_h
#define _1_cube_PtActionUtility_h

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

//动作指令
const std::string act_hide = "Hide";
const std::string act_show = "Show";
const std::string act_place = "Place";
const std::string act_delay = "Delay";
const std::string act_move_to = "MoveTo";
const std::string act_move_by = "MoveBy";
const std::string act_scale_to = "ScaleTo";
const std::string act_scale_by = "ScaleBy";
const std::string act_rotate_to = "RotateTo";
const std::string act_rotate_by = "RotateBy";
const std::string act_fade_in = "FadeIn";
const std::string act_fade_out = "FadeOut";
const std::string act_fade_to = "FadeTo";
const std::string act_skew_to = "SkewTo";
const std::string act_skew_by = "SkewBy";
const std::string act_blink = "Blink";
const std::string act_tint_to = "TintTo";
const std::string act_tint_by = "TintBy";
const std::string act_animation = "Animation";
const std::string act_reverse = "Reverse";
const std::string act_repeat = "Repeat";
const std::string act_repeatforever = "RepeatForever";
const std::string act_sequence = "Sequence";
const std::string act_spawn = "Spawn";
const std::string act_over = "ActOver"; //踩格子效果动作结束专用
const std::string act_remove_self = "RemoveSelf";//删除本身
const std::string act_reset_zorder="ResetZorder";
const std::string act_animateplist="Animate";


class ActionData {
public:
	string actionName;
	string actionType;
	string action;
};

class ActionCallFun {
public:
	//标记动作结束
	void actionOver();
    //删除本身
    void removeSelf(CCNode* node,void* data);
    //按钮执行动作结束
    void btnActOver();
    void resetZorder(CCNode *node,void *data);
    void callTexiaoFile(CCNode *node,void *data);
};


namespace PtActionUtility {
    
    //读取动作(actions)
    void createActions(CCDictionary* data, vector<ActionData*>* vActions);
    
    //解析actionName-逐层解析 生成最后动作
    void setAction(CCNode* sprite, const string& actionName, const vector<ActionData*>* vActions);
    
    //迭代解析字符串-动作
    CCAction* decodeStringToAction(const string& sAction, const vector<ActionData*>* vActions);

    //fine action by name
    string getSActionWithName(const string& actionName, const vector<ActionData*>* vActions);
    
    //载入动画文件，生成动画
    CCAnimation* createAniWithFile(const char* file, const char* aniName, float delay);
    
    //载入动画文件，生成动画
    void createAniWithFile(const string& file);
    
    //读取动作文件(runAction and actions)
    void readSpriteActionFile(const string& file, CCSprite* sprite,const string& key = "");
    
    //取得脚本在的一个组合动画
    CCAction* getRunActionWithActionFile(const string& file);
    
}

#endif
