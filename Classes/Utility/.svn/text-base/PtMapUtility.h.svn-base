#ifndef PTMAPUTILITY_H
#define PTMAPUTILITY_H
 
#include "cocos2d.h"
#include "gameTools.h"
#include "Utility.h"
#include "gameConfig.h"

using namespace std;
using namespace cocos2d;
using namespace GameTools;


//node类型
const string type_sprite = "Sprite";
const string type_layer = "Layer";
const string type_color_layer = "ColorLayer";
const string type_labelttf = "LabelTTF";
const string type_touch = "Touch";
const string type_init_existnode = "initExistNode";
const string type_particle = "Particle";
const string type_init_exitttf = "initExistTTF";

class TouchRect;
class HBMap;

class ActionData {
public:
	string actionName;
	string actionType;
	string action;
};

namespace PtMapUtility {
    
    
    //脚本添加child 将脚本文件file解析，添加为father子节点
    void addChildFromScript(CCNode* father, const string& file);
    
    //脚本生成CClayer
    CCLayer* createLayerWithScript(CCDictionary* data);
    
    //脚本生成node
    CCNode* createNodeWithScript(CCNode* father, const string& nodeType, CCDictionary* data);
    
    //脚本生成精灵
    CCSprite* createSpriteWithScript(CCDictionary* nodeData);
    
    //脚本生成ColorLayer
    CCLayerColor* createColorLayerWithScript(CCDictionary* data);
    
    //脚本生成LabelTTF
    CCLabelTTF* createLabelTTFWithScript(CCDictionary* data);
    
    //初始化层信息
    void initLayerProperty(CCNode* layer, CCDictionary* data);
    
    //生成编辑器Layer
    CCLayer* layerWithFile(const string& file, bool isBackGround = false);
    
    //生成编辑器中精灵文件的sprite
    CCSprite* spriteWithSpriteFile(const string& file, const char* spriteName);
    
    //初始化特殊选项
    void initSpeOption(CCNode* father, CCNode* node, CCDictionary* data);
    
    //读取精灵初始化属性(initProperty)
    void initSpriteProperty(CCSprite* sprite, CCDictionary* data);
    
    //读取精灵相关信息（初始化属性，z，动作，runAction）
    void setSpriteDatInfo(CCSprite* sprite, CCDictionary* data);
    
    //添加相关地图信息到缓存中
    void addMapInfoToCache(HBMap* hbMap, const string& mapFileName, CCLayer* map);
    
    //添加touchRect
    void addTouchRectFromScript(const string& fileName, CCNode* node, vector<TouchRect>* vTouchRect);
    
    
    //读取精灵动作
//    void createActions(CCDictionary* data, vector<ActionData*>* vActions);

}

#endif