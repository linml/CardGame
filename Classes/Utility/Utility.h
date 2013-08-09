#ifndef UTILITY_H
#define UTILITY_H

#include "cocos2d.h"
#include "gameTools.h" 

using namespace std;
using namespace cocos2d;
using namespace GameTools;


#define PTACTIONANI_TAG 361008

class TouchRect;

namespace Utility {
    
    /************添加区域***************/
    
    //修改触摸区域 将 vTouchRect 下目标为 touch_tag 的响应区域更改为 rect
    void setTouchRect(CCRect rect, int touch_tag, vector<TouchRect>& vTouchRect);
    
    //添加触摸区域 将 node 添加到 vTouchRect 中，目标为 touch_tag 区域为 rect
    void addTouchRect(CCRect rect, int touch_tag, CCNode* node, vector<TouchRect>& vTouchRect);
    
    //添加触摸区域 将 node 添加到 vTouchRect 中，目标为 touch_tag
    void addTouchRect(int touch_tag, CCNode* node, vector<TouchRect>& vTouchRect);
    
    //添加触摸区域 将 fatherNode 下 sTag 子节点添加到 vTouchRect，目标为 touch_tag 
    void addTouchRect(int touch_tag, CCNode* fatherNode, const string& sTag, vector<TouchRect>& vTouchRect);
    
    //删除触摸区域 将目标 touch_tag 从 vTouchRect 删除
    void removeTouchRect(int touch_tag, vector<TouchRect>& vTouchRect);
    
    //迭代取目标tag在屏幕上的rect
    CCRect getNodeTouchRect(CCNode* node);

    //获取子节点 通过 father 从 index 开始，遍历迭代至容器 vTag 中最后一个元素，获取子节点
    CCNode* getNodeByTag(CCNode* father, const vector<int> vTag, int index = 0);

    //获取子节点 通过 father 从 index 开始，遍历至字符串 sTag 中最后一个整型字符子串，获取子节点。 此方法主要用于脚本解析
    CCNode* getNodeByTag(CCNode* father, const string& sTag, int index = 0);

    /************声音播放***************/
    
    //获取 node 父节点 X方向缩放值
    float getParentScaleX(CCNode* node);
    //获取 node 父节点 Y方向缩放值
    float getParentScaleY(CCNode* node);
    
    CCPoint getPositionByTag(CCNode* father, CCPoint pos, const vector<int> vTag,int index = 0);
    CCPoint getPositionByTag(CCNode* father, CCPoint pos, const string& sTag,int index = 0);
    
    CCRect getNodeRectInScreen(CCNode* father, const vector<int> vTag, int index = 0);
    CCRect getNodeRectInScreen(CCNode* node,CCPoint pos,const vector<int> vTag,int index = 0);
    CCRect getNodeRectInScreen(CCNode* node,CCPoint pos,const string& sTag,int index = 0);
    
    //生成文字
    string getWordWithFile(const string& file, const char* str);
    string getWordWithFile(const char* str);

    int runPtActionScript(CCNode* node, const char* filePtah,int tag);
    int stopPtActionScript(CCNode* node,int tag);
    int addPtActionScript(const char* filePtah);
    
    //统一按钮处理
    //sprite---按钮精灵即touchRect内的精灵，
    //target --- 后面回调的指针 selector -- 回调
    //anchorPoint -- 锚点
    //soundFile -- 音效文件  不需要播放传入“”
    //actionFile -- 针对图标按钮 传入动作文件“btnAction”
    int handleBtnCallBack(CCSprite* sprite,CCObject* target,SEL_CallFunc selector,CCPoint anchorPoint = ccp(-1,-1),const string& soundFile = "UI_click.wav");
    int handleBtnCallBack(CCSprite* sprite,CCObject* target,SEL_CallFuncND selector, void* d,CCPoint anchorPoint = ccp(0.5,0.5),const string& soundFile = "UI_click.wav");
    int handleBtnCallBack(const string& actionFile,CCSprite* sprite,CCObject* target,SEL_CallFunc selector,CCPoint anchorPoint = ccp(-1,-1),const string& soundFile = "UI_click.wav");
    int handleBtnCallBack(const string& actionFile,CCSprite* sprite,CCObject* target,SEL_CallFuncND selector, void* d,CCPoint anchorPoint = ccp(-1,-1),const string& soundFile = "UI_click.wav");
    
    //设置已在layer中精灵的锚点
    void setExistSpriteAnchorPoint(CCSprite* sprite,CCPoint anchorPoint);


}

enum {
    CALL_BACK_TYPE_NULL = 0,//空
    CALL_BACK_TYPE_CLOSE = 1,//关闭
    CALL_BACK_TYPE_PASS = 2,//传递touch 并关闭
    CALL_BACK_TYPE_PASS_NOTCLOSE = 5,//传递 不关闭
    CALL_BACK_TYPE_SCRIPT_MODIFY = 3,//调用脚本  在原有的脚本上修改
    CALL_BACK_TYPE_SCRIPT_NEW = 4,//调用脚本  删除原有脚本,添加新脚本
    
};

class TouchRect {
    
public:
	CCRect rect;
	int tag;
    CCNode* node;
    string call_back;
    string nextPath;
public:
    
	TouchRect();
	TouchRect(CCRect rect, int tag,CCNode* node = NULL, string callback = "");
    //解析callback
    vector<int> getCallBackType();
    string getNextScriptPath();
    static int SearchTouchTag(CCPoint pos, vector<TouchRect>& vTouchRect, CCSprite** btnSprite = NULL);
    static void AppendCurrTouchTag(int touchTag, vector<TouchRect>& vTouchRect,  vector<TouchRect*> & vCurrTouch);
    
};

#endif