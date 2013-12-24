#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_
#include <string>
#include "cocos2d.h"
#include "PtSoundTool.h"
#include "Utility.h"
#include "PtActionUtility.h"
#include "PtMapUtility.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#if defined LANGUAGE_CN
const string resRootPath = "resource_cn/";
#elif defined LANGUAGE_EN
const string resRootPath = "resource_en/";
#endif


#else

const string resRootPath = "resource_cn/";

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const string g_WordPath = "";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const string g_WordPath = "word/";
#endif

#define CSTR_FILEPTAH(ptah,fileName) ((ptah+fileName).c_str())

#define PT_ERR_LOG(msg)\
cerr << "PT Log :" << __FILE__ <<endl \
     << "        line : " << __LINE__ <<endl \
     << "        Compiled on " << __DATE__ \
     << " at " << __TIME__ <<endl \
     << "        message : " << msg <<endl  \

extern string g_serverUrl;

const string g_sysSoundPath = resRootPath+"sound/sys_sound_effect/";//系统音效
const string g_bgSoundPath = resRootPath+"sound/background/";//背景音乐
const string g_soundPath = resRootPath+"sound/";//音效路径

const string g_wordFilePath = "word.plist";
const string g_wordDirPath = resRootPath+"word/";

//路径
const std::string g_mapPath = resRootPath+"map/";
const std::string g_mapSpritePath = resRootPath+"sprite/";
const std::string g_mapImagesPath = resRootPath+"img/";
const std::string g_headImagesPath = resRootPath+"./";
const std::string plistPath=resRootPath +"uiScript/";

const std::string g_ActionFilePath = resRootPath+"action/";


const string g_sFont = "Helvetica";
const string g_sFont_title = "Georgia-BoldItalic";
const string g_sFont_btn = "Helvetica-Bold";
const std::string g_str_skillPathPlist=resRootPath +"particle/skill-texiao/";

const std::string g_propImagesPath = resRootPath+"img/prop/";
const std::string g_plistPath = resRootPath+"plist/";
const std::string g_chapterPath = resRootPath+"chapter/";

const std::string g_eventBoxPath= resRootPath+"eventbox/";
const std::string g_sellerPath = resRootPath+"img/seller/";
const std::string g_bufferIconPath = g_mapImagesPath+"bufferIcon/";
const std::string g_friendIconPath = g_mapImagesPath+"friend/";
const std::string g_loginRewardIconPath = g_mapImagesPath+"loginreward/";
const std::string g_rankIconPath = g_mapImagesPath+"rank/";

//注意 编辑器中使用大小 -1--就代表 T形按钮 100%
const int g_custom_font_size[] = {
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

const int g_iFontSize[] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,
    12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25};

//字体颜色
const ccColor3B g_color_white = {255,255,255};

//统一自定义的字体大小和颜色  但在代码中要写custom_color[i-1]
const ccColor3B g_custom_color[] = {
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
    {255,125,125}, //19黄色
    {255,0,0},//20
    {209,209,209},  //21每日签到灰色
    {0,255,0},      //22每日签到绿色
    {236,250,163},  //23每日签到黄色
    {255,255,255},  //24每日签到白色
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


const std::string g_str_level_up_config =  resRootPath+"card_level_up_config.plist";
const std::string g_str_star_config = resRootPath + "card_star_level_up_config.plist";
const std::string g_str_prop_config = resRootPath+"item_config.plist";

enum EN_ATKOBJECT {
    EN_ATKOBJECT_OWN,
    EN_ATKOBJECT_OWNTEAM,
    EN_ATKOBJECT_MONSTER,
    EN_ATKOBJECT_MONSTERTEAM,
    EN_ATKOBJECT_INITFIGHT,
};

enum EN_SEND_SKILL
{
    EN_SEND_SKILL_ANGRY,
    EN_SEND_SKILL_HELP,
    EN_SEND_SKILL_DEAD,
    EN_SEND_SKILL_BUFF,
};

enum EN_ANIMATIONTYPE
{
    EN_ANIMATIONTYPE_HERO,     // 当英雄攻击别人的时候发招的 动画
    EN_ANIMATIONTYPE_BUFFER,   // 当发现自身带buffer 的时候要展示动画的标志
    EN_ANIMATIONTYPE_STATUS,   // 当防卫的图标消失的时候
    EN_ANIMATIONTYPE_SKILL,
    EN_ANIMATIONTYPE_BUFFPLISTOWN,
    EN_ANIMATIONTYPE_BUFFPLISTOTHER,
    EN_ANIMATIONTYPE_DEADMOVE,
    EN_ANIMATIONTYPE_REMOVEPLIST,
};
enum EN_ANIMATIONTYPEREFACTOR
{
    EN_ANIMATIONTYPEREFACTOR_NONE,
    EN_ANIMATIONTYPEREFACTOR_SKILL,     // 当英雄攻击别人的时候发招的 动画
    EN_ANIMATIONTYPEREFACTOR_BUFFER_OWN,   // 当发现自身带buffer 的时候要展示动画的标志
    EN_ANIMATIONTYPEREFACTOR_BUFFER_MONSTER,
    EN_ANIMATIONTYPEREFACTOR_DEADMOVE,   // 当防卫的图标消失的时候
    EN_ANIMATIONTYPEREFACTOR_SHANGHAI,
    EN_ANIMATIONTYPEREFACTOR_REMOVEPLIST //删除 身上带的buffer的 tag;
};

enum  EN_ATKFIGHT_INDEX
{
    EN_ATKFIGHT_INDEX_NONE,
    EN_ATKFIGHT_INDEX_LEFT_LORD,
    EN_ATKFIGHT_INDEX_LEFT_SUPPORT,
    EN_ATKFIGHT_INDEX_RIGHT_LORD,
    EN_ATKFIGHT_INDEX_RIGHT_SUPPORT,
    EN_ATKFIGHT_INDEX_LEFT_MOVE,
    EN_ATKFIGHT_INDEX_RIGHT_MOVE
};
//处理的类型
enum EN_BUFF_FIELD_TYPE
{
    EN_BUFF_FIELD_TYPE_NONE=0,
    EN_BUFF_FIELD_TYPE_ATTACK,   //影响攻击力
    EN_BUFF_FIELD_TYPE_DEFEND,   //影响防御力
    EN_BUFF_FIELD_TYPE_ANGRY,    //怒气
    EN_BUFF_FIELD_TYPE_HP,       //影响HP，
    EN_BUFF_FIELD_TYPE_BINGFENG, //被冰封
    EN_BUFF_FIELD_TYPE_XUANYUN,  //眩晕
    EN_BUFF_FIELD_TYPE_DUYAO,    //毒药
    
};

enum EN_BUFF_ACTION
{
    EN_BUFF_ACTION_NONE,
    EN_BUFF_ACTION_ADD,
    EN_BUFF_ACTION_SUB
};
//乘以还是add
enum EN_BUFF_MARK
{
    EN_BUFF_MARK_NONE,
    EN_BUFF_MARK_MULTIPY,
    EN_BUFF_MARK_ADD
};

enum EN_BUFF_TARGET {
    EN_BUFF_TARGET_NONE,
    EN_BUFF_TARGET_OWNZERO,
    EN_BUFF_TARGET_OWNALL,
    EN_BUFF_TARGET_MONSTERZERO,
    EN_BUFF_TARGET_MONSTERALL
};


// max level values:
const int g_aMaxLevel[7] = {30, 45, 50, 70,85, 95,100};


enum EN_GAMEFIGHTSTATUS {
    EN_GAMEFIGHTSTATUS_WIN,   //胜利
    EN_GAMEFIGHTSTATUS_LOSE,  //失败，
    EN_GAMEFIGHTSTATUS_NONE,  //需要继续战斗，
};
enum ERROR_MSG {
    ERROR_MSG_NONE=1,
    ERROR_MSG_CONNECTSERVERERROR = 10000, //服务端链接失败
    ERROR_MSG_SERVERERROR =10001,
    ERROR_MSG_SERVERDATA=5035
};
enum EN_LEFTTEAMORRIGHTTEAM
{
    EN_RIGHTTEAM=0,
    EN_LEFTTEAM=1,
   
};

#define NOTIFYTAG_LEVELUP "NOTIFYTAG_LEVELUP"

#define CANNOTFIGHTEFFECTLOGICVALUE 8
#endif