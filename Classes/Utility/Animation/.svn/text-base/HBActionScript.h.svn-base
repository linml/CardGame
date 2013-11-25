#pragma once
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;


    /** Types of progress
     @since v0.99.1
     */
    typedef enum {
        /// Radial Counter-Clockwise
        kCCProgressTimerTypeRadialCCW,
        /// Radial ClockWise
        kCCProgressTimerTypeRadialCW,
        /// Horizontal Left-Right
        kCCProgressTimerTypeHorizontalBarLR,
        /// Horizontal Right-Left
        kCCProgressTimerTypeHorizontalBarRL,
        /// Vertical Bottom-top
        kCCProgressTimerTypeVerticalBarBT,
        /// Vertical Top-Bottom
        kCCProgressTimerTypeVerticalBarTB,
    } PtProgressTimerType;
    
class HBActionScript : public CCObject
{
public:
	HBActionScript();
	HBActionScript(CCDictionary* data,bool bolResetData);
	~HBActionScript(void);

	

	CCAction* creatAction(const char* str);

	CCActionInstant* creatInstantAction(const char* str);

	CCFiniteTimeAction* creatFiniteTimeAction(const char* str);

	CCActionInterval* creatIntervalAction(const char* str);

	void runScriptBy(CCNode* node);

	void scriptCallBack(CCNode* node,void* data);

	void frameCallBack1(CCNode* node,void* data);

	void frameCallBack2(CCNode* node,void* data);

	void effectframeCallBack1(CCNode* node,void* data);

	void effectframeCallBack2(CCNode* node,void* data);

	void soundCallBack(CCNode* node,void* data);

	void backSoundCallBack(CCNode* node,void* data);

	void particeSystemCallBack(CCNode* node,void* data);

	//属性设置函数
	void setAction(CCAction* act);
	void setActionWith(const char* str);

	void setNodeType(const char* str);
	void setTag(int t);
	void setAnchorPosition(CCPoint p);
	void setPositionInPixels(CCPoint p);
	void setScale(float s);
	void setRotation(float r);
	void setVisable(bool v);
	void setZOrder(int z);
	void setOpacity(GLubyte o);
	void setColor(ccColor3B c);
	void setBolFlipX(bool b);
	void setBolFlipY(bool b);

	void setTexturePath(const char* str);

	void setLabelStr(const char* str);

	void setStartCharMap(unsigned char c);

	void setLabelWidth(unsigned int v);
	void setLabelHeight(unsigned int v);

	void setPercentage(float per);
    
    void setProgressType(CCProgressTimer** progress, PtProgressTimerType type);

	string nodeType;
	int tag;

	CCPoint anchorPosition;
	CCPoint position;
	float scale;
	float scaleX;
	float scaleY;
	float skewX;
	float skewY;
	float rotation;
	bool visable;
	int zOrder;
	//sprite
	string texturePath;
	CCRect textureRect;
	GLubyte opacity;
	ccColor3B color;
	bool bolFlipX;
	bool bolFlipY;

	//labelTTF
	string labelStr;

	//labelAtlas
	unsigned char startCharMap;
	unsigned int labelWidth;
	unsigned int labelHeight;

	//progressTimer
	PtProgressTimerType progressTimerType;
	float percentage;

	bool bolResetData;

	//属性是否设置
	bool hasAnchorPosition;
	bool hasPosition;
	bool hasScale;
	bool hasScaleX;
	bool hasScaleY;
	bool hasSkewX;
	bool hasSkewY;
	bool hasRotation;
	bool hasVisable;
	bool hasZOrder;
	//sprite
	bool hasTexturePath;
	bool hasTextureRect;
	bool hasOpacity;
	bool hasColor;
	bool hasBolFlipX;
	bool hasBolFlipY;

	//labelTTF
	bool hasLabelStr;
	//labelAtlas
	unsigned char hasStartCharMap;
	unsigned int hasLabelWidth;
	unsigned int hasLabelHeight;


	//progressTimer
	bool hasProgressTimerType;
	bool hasPercentage;


	CCAction* action;

	CCAction* actionForever;

	vector<CCString*> strCache;
};
