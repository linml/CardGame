#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

using namespace std;
using namespace cocos2d;


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#if defined LANGUAGE_CN
const string resRootPath = "resource_cn/";
#elif defined LANGUAGE_EN
const string resRootPath = "resource_en/";
#endif


#else

const string resRootPath = "resource_cn/";

#endif

const string sysSoundPath = resRootPath+"sound/sys_sound_effect/";//系统音效
const string bgSoundPath = resRootPath+"sound/background/";//背景音乐

const string wordFilePath = resRootPath+"word.plist";
const string wordDirPath = resRootPath+"word/";

//路径
const std::string mapPath = resRootPath+"map/";
const std::string mapSpritePath = resRootPath+"sprite/";
const std::string mapImagesPath = resRootPath+"img/";
const std::string headImagesPath = resRootPath+"./";
const std::string plistPath=resRootPath +"uiScript/";

const string sFont = "Helvetica";
const string sFont_title = "Georgia-BoldItalic";
const string sFont_btn = "Helvetica-Bold";

//注意 编辑器中使用大小 -1--就代表 T形按钮 100%
const int custom_font_size[] = {
    10,             //0  空值,不能使用这个参数
    13,             //1  T形按钮 100%
    12,             //2  T形按钮 95% 选中状态
    11,             //3  T形按钮 80 未选中状态
    
    16,             //4  棱角按钮 100%
    14,             //5  棱角按钮 85%  选中状态/未选中状态
    
    10,             //6  圆角按钮
    23,             //7  圆角“?”按钮
    15,             //8  蓝色按钮 --充值
    
    18,             //9  标题
    
    10,             //10 弹出对话框(二次确认的那种框体)
    
    14,             //11 中号标题
    
    14,             //12 装备列表选中
    
    8,              //13
};

//字体颜色
const ccColor3B color_white = {255,255,255};

//统一自定义的字体大小和颜色  但在代码中要写custom_color[i-1]
const ccColor3B custom_color[] = {
    {241,200,58},   //1
    {76,191,244},   //2
    {71,29,16},     //3
    {200,103,31},   //4
    {236,228,171},  //5
    {249,141,75},   //6
    {211,176,109},  //7
    {255,255,255},  //8
    {237, 212, 136},//9
    
    {255,210,153},  //10 T形按钮 95%  选中状态
    {160,184,188},   //11 T形按钮 80%  未选中状态
    
    {255,248,214},  //12 其他按钮     可点击状态
    {150,150,150},  //13 其他按钮     不可点击状态
    
    {124,247,255},  //14 蓝色按钮 --充值
    
    {255,162,0},    //15 标题
    
    {255,247,216},  //16 弹出对话框(二次确认的那种框体)
    
    {255,210,0},    //17 装备列表选中
    {0,0,0},    //18 black
};

typedef struct TPlayTask {
    string m_TaskName;
    int m_nTaskNumber;
    bool m_bTaskComplete;
} TPlayTask;

//define scene  plist
const std::string scene_datting =resRootPath+"uiScript/card.plist";
const std::string scene_suipian =resRootPath+"uiScript/kk.plist";
const std::string scene_task    =resRootPath+"uiScript/task.plist";
const std::string scene_exploration=resRootPath+"uiScript/room.plist";
const std::string scene_evolution=resRootPath + "uiScript/mall_evolution.plist";
const std::string scene_fightscene=resRootPath+"uiScript/card.plist";
const std::string layer_cardSprite=resRootPath +"uiScript/cubeCardSprite.plist";


/****
 * task  任务
 * exploration 探险
 * lineup  阵容
 * puzzle  拼图
 * shop    商城
 * email   邮件
 * option  选项
 */
const std::string g_str_taskScenePlist=plistPath+"taskScene.plist";
const std::string g_str_explorationPlist=plistPath+"explorationScene.plist";
const std::string g_str_lineupSettingPlist=plistPath+"lineupSettingScene.plist";
const std::string g_str_puzzlePlist=plistPath+"puzzleScene.plist";
const std::string g_str_shopPlist=plistPath+"shopScene.plist";
const std::string g_str_emailPlist=plistPath +"emailScene.plist";
const std::string g_str_optionPlist=plistPath+"optionScene.plist";



#endif