#include "HBActionScript.h"
#include "GameTools.h"
#include "ImageTools.h"
#include "HBAnimationCache.h"
#include <iostream>
#include "HBActionAniCache.h"
#include "SimpleAudioEngine.h"
#include "gameConfig.h"

using namespace GameTools;
using namespace ImageTools;

HBActionScript::HBActionScript():
nodeType("sprite"),
tag(-1),
anchorPosition(ccp(0.5,0.5)),
position(ccp(0,0)),
scale(1),
scaleX(1),
scaleY(1),
skewX(0),
skewY(0),
rotation(0),
visable(1),
zOrder(0),
texturePath(""),
textureRect(CCRectZero),
opacity(255),
color(ccc3(255,255,255)),
bolFlipX(false),
bolFlipY(false),
labelStr(""),
startCharMap('.'),
labelWidth(0),
labelHeight(0),
progressTimerType(kCCProgressTimerTypeRadialCCW),
percentage(0),
action(NULL),
actionForever(NULL),
hasAnchorPosition(false),
hasPosition(false),
hasScale(false),
hasScaleX(false),
hasScaleY(false),
hasSkewX(false),
hasSkewY(false),
hasRotation(false),
hasVisable(false),
hasZOrder(false),
hasTexturePath(false),
hasTextureRect(false),
hasOpacity(false),
hasColor(false),
hasBolFlipX(false),
hasBolFlipY(false),
hasLabelStr(false),
hasStartCharMap(false),
hasLabelWidth(false),
hasLabelHeight(false),
hasProgressTimerType(false),
hasPercentage(false),
bolResetData(false)
{
    //bq modify
    autorelease();
}


HBActionScript::HBActionScript(CCDictionary* data,bool bolResetData):
nodeType("sprite"),
tag(-1),
anchorPosition(ccp(0.5,0.5)),
position(ccp(0,0)),
scale(1),
scaleX(1),
scaleY(1),
skewX(0),
skewY(0),
rotation(0),
visable(1),
zOrder(0),
texturePath(""),
textureRect(CCRectZero),
opacity(255),
color(ccc3(255,255,255)),
bolFlipX(false),
bolFlipY(false),
labelStr(""),
startCharMap('.'),
labelWidth(0),
labelHeight(0),
progressTimerType(kCCProgressTimerTypeRadialCCW),
percentage(0),
action(NULL),
actionForever(NULL)
{
    //bq modify
    autorelease();
    
	//初始化全部flase
	this->bolResetData = true;
	hasAnchorPosition = false;
	hasPosition = false;
	hasScale = false;
	hasScaleX = false;
	hasScaleY = false;
	hasSkewX = false;
	hasSkewY = false;
	hasRotation = false;
	hasVisable = false;
	hasZOrder = false;
	hasTexturePath = false;
	hasTextureRect = false;
	hasOpacity = false;
	hasColor = false;
	hasBolFlipX = false;
	hasBolFlipY = false;
	hasLabelStr = false;
	//labelAtlas
	hasStartCharMap = false;
	hasLabelWidth = false;
	hasLabelHeight = false;
	hasProgressTimerType = false;
	hasPercentage = false;
	
	//判断类型
	if (data->objectForKey("nodeType")) 
		nodeType = valueForKey("nodeType",data);

	//公用属性
	if (data->objectForKey("tag")) tag = intForKey("tag",data);

	if (data->objectForKey("anchorPosition")) 
	{
		anchorPosition = pointForKey("anchorPosition",data);
		hasAnchorPosition = true;
	}
	if (data->objectForKey("position"))
	{
		position = pointForKey("position",data);
		hasPosition = true;
	}
	if (data->objectForKey("scale"))
	{
		scale = floatForKey("scale",data);
		hasScale = true;
	}
	if (data->objectForKey("scaleX"))
	{
		scaleX = floatForKey("scaleX",data);
		hasScaleX = true;
	}
	if (data->objectForKey("scaleY")) 
	{
		scaleY = floatForKey("scaleY",data);
		hasScaleY = true;
	}
	if (data->objectForKey("skewX")) 
	{
		skewX = floatForKey("skewX",data);
		hasSkewX = true;
	}
	if (data->objectForKey("skewY")) 
	{
		skewY = floatForKey("skewY",data);
		hasSkewY = true;
	}
	if (data->objectForKey("rotation")) 
	{
		rotation = floatForKey("rotation",data);
		hasRotation = true;
	}
	if (data->objectForKey("zOrder"))
	{
		zOrder = floatForKey("zOrder",data);
		hasZOrder = true;
	}
	if (data->objectForKey("visable")) 
	{
		visable = boolForKey("visable",data);
		hasVisable = true;
	}

	if (nodeType == "sprite")
	{
		if (data->objectForKey("opacity")) 
		{
			opacity = floatForKey("opacity",data);
			hasOpacity = true;
		}
		if (data->objectForKey("color")) 
		{
			color = ccc3ForKey("color",data);
			hasColor = true;
		}
		if (data->objectForKey("bolFlipX")) 
		{
			bolFlipX = boolForKey("bolFlipX",data);
			hasBolFlipX = true;
		}
		if (data->objectForKey("bolFlipY")) 
		{
			bolFlipY = boolForKey("bolFlipY",data);
			hasBolFlipY = true;
		}

		if (data->objectForKey("texturePath")) 
		{
			texturePath = string(valueForKey("texturePath",data));
            texturePath = g_ActionFilePath + texturePath;
			hasTexturePath = true;
			hasTextureRect = true;
		}
		if (data->objectForKey("textureRect")) 
		{
			textureRect = rectForKey("textureRect",data);
			hasTextureRect = true;
		}

		if (data->objectForKey("textureAliasTexParameters"))
		{
//			CCLOG("HBActionScript::HBActionScript path = %s",CSTR_FILEPTAH(g_ActionFilePath,texturePath.c_str()));
			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
			CCLOG("HBActionScript::HBActionScript path ok");
			texture->setAliasTexParameters();
		}
	}
	else if (nodeType == "particeSystem")
	{
		if (data->objectForKey("texturePath")) 
		{
			texturePath = string(valueForKey("texturePath",data));
            texturePath = g_ActionFilePath + texturePath;
			hasTexturePath = true;
		}
	}
	else if (nodeType == "labelTTF")
	{
		if (data->objectForKey("opacity")) 
		{
			opacity = floatForKey("opacity",data);
			hasOpacity = true;
		}
		if (data->objectForKey("color")) 
		{
			color = ccc3ForKey("color",data);
			hasColor = true;
		}
		if (data->objectForKey("labelStr")) 
		{
			labelStr = string(valueForKey("labelStr",data));
			hasLabelStr = true;
		}
	}
	else if (nodeType == "labelAtlas")
	{
		if (data->objectForKey("texturePath")) 
		{
			texturePath = string(valueForKey("texturePath",data));
            texturePath = g_ActionFilePath + texturePath;
			hasTexturePath = true;
		}
		if (data->objectForKey("labelStr")) 
		{
			labelStr = string(valueForKey("labelStr",data));
			hasLabelStr = true;
		}
		if (data->objectForKey("startCharMap")) 
		{
			startCharMap = *(valueForKey("startCharMap",data));
			hasStartCharMap = true;
		}
		if (data->objectForKey("labelWidth")) 
		{
			labelWidth = intForKey("labelWidth",data);
			hasLabelWidth = true;
		}
		if (data->objectForKey("labelHeight")) 
		{
			labelHeight = intForKey("labelHeight",data);
			hasLabelHeight = true;
		}
	}
	else if (nodeType == "progressTimer")
	{
		if (data->objectForKey("texturePath")) 
		{
			texturePath = string(valueForKey("texturePath",data));
            texturePath = g_ActionFilePath + texturePath;
			hasTexturePath = true;
		}
		if (data->objectForKey("progressTimerType")) 
		{
			int tmp = intForKey("progressTimerType",data);
			switch(tmp)
			{
			case 0:
				progressTimerType = kCCProgressTimerTypeRadialCCW;
				break;
			case 1:
				progressTimerType = kCCProgressTimerTypeRadialCW;
				break;
			case 2:
                progressTimerType = kCCProgressTimerTypeHorizontalBarLR;
				break;
			case 3:
				progressTimerType = kCCProgressTimerTypeHorizontalBarRL;
				break;
			case 4:
				progressTimerType = kCCProgressTimerTypeVerticalBarBT;
				break;
			case 5:
				progressTimerType = kCCProgressTimerTypeVerticalBarTB;
				break;
			}
			hasProgressTimerType = true;
		}
		if (data->objectForKey("percentage")) 
		{
			percentage = intForKey("percentage",data);
			hasPercentage = true;
		}
	}

	if (data->objectForKey("action")) action = creatAction(valueForKey("action",data));
	CC_SAFE_RETAIN(action);

	if (data->objectForKey("actionForever")) actionForever = creatAction(valueForKey("actionForever",data));
	CC_SAFE_RETAIN(actionForever);

	//是否重置元素数据，默认为true
	this->bolResetData = bolResetData;
	if (bolResetData)
	{
		hasAnchorPosition = true;
		hasPosition = true;
		hasScale = true;
		hasScaleX = true;
		hasScaleY = true;
		hasSkewX = true;
		hasSkewY = true;
		hasRotation = true;
		hasVisable = true;
		hasZOrder = true;
		hasTexturePath = true;
		hasTextureRect = true;
		hasOpacity = true;
		hasColor = true;
		hasBolFlipX = true;
		hasBolFlipY = true;
		hasLabelStr = true;
		hasStartCharMap = true;
		hasLabelWidth = true;
		hasLabelHeight = true;
		hasProgressTimerType = true;
		hasPercentage = true;
	}
}

HBActionScript::~HBActionScript(void)
{
//    CCLog("~HBActionScript action->retainCount = %d",action?action->retainCount():0);
//    CCLog("~HBActionScript actionForever->retainCount = %d",actionForever?actionForever->retainCount():0);
	CC_SAFE_RELEASE(action);
	CC_SAFE_RELEASE(actionForever);

	//清除CCString缓存
	for (unsigned int i = 0;i<strCache.size();i++)
	{
		CC_SAFE_DELETE(strCache[i]);
	}
	strCache.clear();
}

CCActionInstant* HBActionScript::creatInstantAction(const char* str)
{
	string mainStr = str;
	//字符串处理
	int maohaoPos = mainStr.find_first_of(":");
	string actionStr = mainStr.substr(1,maohaoPos-1);
	int size = mainStr.size()-maohaoPos-2;
	mainStr = mainStr.substr(maohaoPos+1,size);

	//action解析
	CCActionInstant* resultAction = NULL;
	vector<string> value;
	if (actionStr == "show")
	{
		resultAction = new CCShow();
		resultAction->autorelease();
	}
	else if (actionStr == "hide")
	{
		resultAction = new CCHide();
		resultAction->autorelease();
	}
	else if (actionStr == "place")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCPlace::create(ccp(atof(value[0].c_str()),atof(value[1].c_str())));
	}
	else if (actionStr == "flipX")
	{
		value = splitString(mainStr.c_str(),",");
		bool filp = false;
		if (atoi(value[0].c_str()) != 0) filp = true;
		resultAction = CCFlipX::create(filp);
	}
	else if (actionStr == "flipY")
	{
		value = splitString(mainStr.c_str(),",");
		bool filp = false;
		if (atoi(value[0].c_str()) != 0) filp = true;
		resultAction = CCFlipY::create(filp);
	}
	else if (actionStr == "toggleVisibility")
	{
		resultAction = CCToggleVisibility::create();
	}
	else if (actionStr == "script")
	{
		if (!mainStr.empty())
		{
			CCString* cstr = new CCString(mainStr.c_str());	
			strCache.push_back(cstr);
			resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::scriptCallBack),(void*)(cstr));
			//加载该脚本
			HBActionAniCache::sharedActionAniCache()->addActionAniWithFile(CSTR_FILEPTAH(g_ActionFilePath, mainStr.c_str()));
		}
	}
	else if (actionStr == "frame")
	{
		
		value = splitString(mainStr.c_str(),",");
		CCLOG("CCActionInstant* HBActionScript mainStr = %s,size = %d",mainStr.c_str(),value.size());
		if (value.size() == 2)
		{
			CCAnimation* ani = HBAnimationCache::sharedAnimationCache()->addAnimationWithFile(value[0].c_str());
			resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::frameCallBack1),(void*)ani->getFrames()->objectAtIndex(atoi(value[1].c_str())));
		}
		else
		{
			//string path = CCFileUtils::fullPathFromRelativePath(value[0].c_str());
			//CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());
			CCLOG("CCActionInstant* HBActionScript value[0].c_str() = %s",value[0].c_str());
			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(value[0].c_str());
			CCLOG("CCActionInstant* HBActionScript addImage %x",texture);
			resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::frameCallBack2),(void*)texture);
			CCLOG("CCActionInstant* HBActionScript actionWithTarget");
		}
	}
	else if (actionStr == "effectFrameWithFile")
	{
		value = splitString(mainStr.c_str(),",");
		CCTexture2D* texture = NULL;
		if (value.size() == 4)
		{
			texture = getColorTexture(value[0].c_str(),atoi(value[1].c_str()),atoi(value[2].c_str()),atoi(value[3].c_str()));
		}
		else if (value.size() == 2)
		{
			int effectType = atoi(value[1].c_str());
			if(effectType == 0) texture = getOppositionTexture(value[0].c_str());
			else if(effectType == 1) texture = getGrayTexture(value[0].c_str());
		}
		resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::frameCallBack2),(void*)texture);
	}
	else if (actionStr == "effectFrame")
	{
		CCString* cstr = new CCString(mainStr.c_str());	
		strCache.push_back(cstr);
		resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::effectframeCallBack2),(void*)(cstr));
	}
	else if (actionStr == "playSound")
	{
		CCString* cstr = new CCString(mainStr.c_str());
        cstr->m_sString = g_soundPath + cstr->m_sString;
		strCache.push_back(cstr);
		resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::soundCallBack),(void*)(cstr));
	}
	else if (actionStr == "playBackSound")
	{
		CCString* cstr = new CCString(mainStr.c_str());
        cstr->m_sString = g_soundPath + cstr->m_sString;
		strCache.push_back(cstr);
		resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::backSoundCallBack),(void*)(cstr));
	}
	else if (actionStr == "particeSystemOnOff")
	{
		CCString* cstr = new CCString(mainStr.c_str());
        cstr->m_sString = g_soundPath + cstr->m_sString;
		strCache.push_back(cstr);
		resultAction = CCCallFuncND::create(this, callfuncND_selector(HBActionScript::particeSystemCallBack),(void*)(cstr));
	}

	return resultAction;
}

CCActionInterval* HBActionScript::creatIntervalAction(const char* str)
{
	string mainStr = str;
	//字符串处理
	int maohaoPos = mainStr.find_first_of(":");
	string actionStr = mainStr.substr(1,maohaoPos-1);
	int size = mainStr.size()-maohaoPos-2;
	mainStr = mainStr.substr(maohaoPos+1,size);

	//action解析
	CCActionInterval* resultAction = NULL;
	vector<string> value;
	if (actionStr == "moveTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCMoveTo::create(atof(value[0].c_str()),ccp(atof(value[1].c_str()),atof(value[2].c_str())) );
	}
	else if (actionStr == "moveBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCMoveBy::create(atof(value[0].c_str()),ccp(atof(value[1].c_str()),atof(value[2].c_str())));
	}
	else if (actionStr == "scaleTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCScaleTo::create(atof(value[0].c_str()),atof(value[1].c_str()),atof(value[2].c_str()));
	}
	else if (actionStr == "scaleBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCScaleBy::create(atof(value[0].c_str()),atof(value[1].c_str()),atof(value[2].c_str()));
	}
	else if (actionStr == "rotateTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCRotateTo::create(atof(value[0].c_str()),atof(value[1].c_str()));
	}
	else if (actionStr == "rotateBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCRotateBy::create(atof(value[0].c_str()),atof(value[1].c_str()));
	}
	else if (actionStr == "bezierBy")
	{
		value = splitString(mainStr.c_str(),",");
		ccBezierConfig bezier;
		bezier.endPosition = ccp(atof(value[1].c_str()),atof(value[2].c_str()));
		bezier.controlPoint_1 = ccp(atof(value[3].c_str()),atof(value[4].c_str()));
		bezier.controlPoint_2 = ccp(atof(value[5].c_str()),atof(value[6].c_str()));

		resultAction = CCBezierBy::create(atof(value[0].c_str()),bezier);
	}
	else if (actionStr == "bezierTo")
	{
		value = splitString(mainStr.c_str(),",");
		ccBezierConfig bezier;
		bezier.endPosition = ccp(atof(value[1].c_str()),atof(value[2].c_str()));
		bezier.controlPoint_1 = ccp(atof(value[3].c_str()),atof(value[4].c_str()));
		bezier.controlPoint_2 = ccp(atof(value[5].c_str()),atof(value[6].c_str()));

		resultAction = CCBezierTo::create(atof(value[0].c_str()),bezier);
	}
	else if (actionStr == "fadeIn")
	{
		resultAction = CCFadeIn::create(atof(mainStr.c_str()));
	}
	else if (actionStr == "fadeOut")
	{
		resultAction = CCFadeOut::create(atof(mainStr.c_str()));
	}
	else if (actionStr == "fadeTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCFadeTo::create(atof(value[0].c_str()),atoi(value[1].c_str()));
	}
	else if (actionStr == "tintBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCTintBy::create(atof(value[0].c_str()),atoi(value[1].c_str()),atoi(value[2].c_str()),atoi(value[3].c_str()));
	}
	else if (actionStr == "tintTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCTintTo::create(atof(value[0].c_str()),atoi(value[1].c_str()),atoi(value[2].c_str()),atoi(value[3].c_str()));
	}
	else if (actionStr == "skewBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCSkewBy::create(atof(value[0].c_str()),atof(value[1].c_str()),atof(value[2].c_str()));
	}
	else if (actionStr == "skewTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCSkewTo::create(atof(value[0].c_str()),atof(value[1].c_str()),atof(value[2].c_str()));
	}
	else if (actionStr == "jumpBy")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCJumpBy::create(atof(value[0].c_str()),ccp(atof(value[1].c_str()),atof(value[2].c_str())),atoi(value[3].c_str()),atoi(value[4].c_str()));
	}
	else if (actionStr == "jumpTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCJumpTo::create(atof(value[0].c_str()),ccp(atof(value[1].c_str()),atof(value[2].c_str())),atoi(value[3].c_str()),atoi(value[4].c_str()));
	}
	else if (actionStr == "delayTime")
	{
		resultAction = CCDelayTime::create(atof(mainStr.c_str()));
	}
	else if (actionStr == "animate")
	{
		value = splitString(mainStr.c_str(),",");
		CCAnimation* ani;
        char* aniName=new char[128];
        memset(aniName, 0, 128);
		if (value.size() == 3)
		{
			ani = HBAnimationCache::sharedAnimationCache()->addAnimationWithFile(value[1].c_str(),&aniName);
			ani->setDelayPerUnit(atof(value[2].c_str()));
		}else
		{
			ani = HBAnimationCache::sharedAnimationCache()->addAnimationWithFile(value[1].c_str(),atoi(value[2].c_str()),atoi(value[3].c_str()),&aniName);
			ani->setDelayPerUnit(atof(value[4].c_str()));
		}

		if (value[0] == "1")
		{
//			ani = HBAnimationCache::sharedAnimationCache()->reverseAnimationByAni(ani);
            //edit xianbei
			ani = HBAnimationCache::sharedAnimationCache()->reverseAnimationByName(aniName);
		}
        
		resultAction = CCAnimate::create(ani);
        delete[] aniName;
        
	}
	else if (actionStr == "blink")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCBlink::create(atof(value[0].c_str()),atoi(value[1].c_str()));
	}
	
	else if (actionStr == "progressFromTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCProgressFromTo::create(atof(value[0].c_str()),atof(value[1].c_str()),atof(value[2].c_str()));
	}
	else if (actionStr == "progressTo")
	{
		value = splitString(mainStr.c_str(),",");
		resultAction = CCProgressTo::create(atof(value[0].c_str()),atof(value[1].c_str()));
	}
	else if (actionStr == "repeat")
	{
		value = splitStringSpecial(mainStr.c_str());
		CCFiniteTimeAction* act = creatFiniteTimeAction(value[0].c_str());
		if (!act) act = creatInstantAction(value[0].c_str());
		if (!act) act = creatIntervalAction(value[0].c_str());
		resultAction = CCRepeat::create(act,atoi(value[1].c_str()));
	}
	else if (actionStr == "repeatForever")
	{
		CCActionInterval* act = (CCActionInterval*)creatFiniteTimeAction(mainStr.c_str());
		if (!act) act = (CCActionInterval*)creatInstantAction(mainStr.c_str());
		if (!act) act = creatIntervalAction(mainStr.c_str());
		resultAction = CCRepeatForever::create(act);
	}
	//ease 速度相关action
	else if (actionStr == "easeIn")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseIn::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "easeOut")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseOut::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "easeInOut")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseInOut::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "easeExponentialIn")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseExponentialIn::create(act);
	}
	else if (actionStr == "easeExponentialOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseExponentialOut::create(act);
	}
	else if (actionStr == "easeExponentialInOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseExponentialInOut::create(act);
	}
	else if (actionStr == "easeSineIn")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseSineIn::create(act);
	}
	else if (actionStr == "easeSineOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseSineOut::create(act);
	}
	else if (actionStr == "easeSineInOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseSineInOut::create(act);
	}
	else if (actionStr == "eseElasticIn")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseElasticIn::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "eseElasticOut")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseElasticOut::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "eseElasticOut")
	{
		value = splitString(mainStr.c_str(),";");
		CCActionInterval* act = creatIntervalAction(value[0].c_str());
		CCEaseElasticInOut::create(act,atof(value[1].c_str()));
	}
	else if (actionStr == "easeBounceIn")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseBounceIn::create(act);
	}
	else if (actionStr == "easeBounceOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseBounceIn::create(act);
	}
	else if (actionStr == "easeBounceInOut")
	{
		CCActionInterval* act = creatIntervalAction(mainStr.c_str());
		CCEaseBounceInOut::create(act);
	}
	else
	{
		
	}

	return resultAction;
}

CCFiniteTimeAction* HBActionScript::creatFiniteTimeAction(const char* str)
{
	string mainStr = str;
	//字符串处理
	int maohaoPos = mainStr.find_first_of(":");
	string actionStr = mainStr.substr(1,maohaoPos-1);
	int size = mainStr.size()-maohaoPos-2;
	mainStr = mainStr.substr(maohaoPos+1,size);

	//action解析
	CCFiniteTimeAction* resultAction = NULL;
	vector<string> value;
	if (actionStr == "sequence")
	{
		
		value = GameTools::splitStringSpecial(mainStr.c_str());
		CCArray* actions = CCArray::createWithCapacity(value.size());
		for(int i = 0;i<value.size();i++)
		{
			CCAction* act = creatIntervalAction(value[i].c_str());
			if (!act) act = creatInstantAction(value[i].c_str());
			if (!act) act = creatFiniteTimeAction(value[i].c_str());
			actions->addObject(act);
		}
		resultAction = CCSequence::create(actions);
        //bq modify
//		actions->release();
	}
	else if (actionStr == "spawn")
	{
		value = splitStringSpecial(mainStr.c_str());
		CCArray* actions = CCArray::createWithCapacity(value.size());
		for(int i = 0;i<value.size();i++)
		{
			CCAction* act = creatIntervalAction(value[i].c_str());
			if (!act) act = creatInstantAction(value[i].c_str());
			if (!act) act = creatFiniteTimeAction(value[i].c_str());
			actions->addObject(act);
		}
		resultAction = CCSpawn::create(actions);
        //bq modify
//		actions->release();
	}

	return resultAction;
}

CCAction* HBActionScript::creatAction(const char* str)
{
    //CCLog("HBActionScript::creatAction 0 :%s",str);
	string mainStr = str;
	if (mainStr.find_first_of("(")!=0||mainStr.find_last_of(")")!=mainStr.size()-1)
	{
		cout<<mainStr+"语法错误，缺少括号"<<endl;
		return NULL;
	}

	CCAction* resultAction = NULL;
	//
	resultAction = creatFiniteTimeAction(str);
	if (!resultAction) resultAction = creatInstantAction(str);
	if (!resultAction) resultAction = creatIntervalAction(str);
	
	return resultAction;
}


void HBActionScript::runScriptBy(CCNode* node)
{
	CCNode* child = NULL;
	if (tag == -1)
	{
		child = node;
	}
	else
	{
		child = node->getChildByTag(tag);
	}

	if (nodeType == "sprite")
	{
		CCSprite* sprite = (CCSprite*)child;
		if (!sprite)
		{
			sprite = new CCSprite();
			sprite->init();
			sprite->autorelease();
			if (tag != -1) node->addChild(sprite,zOrder,tag);
		}
		else
		{
			if (tag != -1)
			{
				if(hasZOrder) node->reorderChild(sprite,zOrder);
				//精灵初始化时隐藏子节点
				if (bolResetData)
				{
					CCArray* children = sprite->getChildren();
					if (children)
					{
						for (int i = 0;i<children->count();i++)
						{
							CCNode* child = (CCNode*)children->objectAtIndex(i);
							child->setVisible(false);
							child->stopAllActions();
						}
					}
				}
			}
		}
		if (hasAnchorPosition) sprite->setAnchorPoint(anchorPosition);
		if (hasPosition) sprite->setPosition(position);
		if (hasRotation) sprite->setRotation(rotation);
		if (hasScale) sprite->setScale(scale);
		if (hasScaleX) sprite->setScaleX(scaleX);
		if (hasScaleY) sprite->setScaleY(scaleY);
		if (hasSkewX) sprite->setSkewX(skewX);
		if (hasSkewY) sprite->setSkewY(skewY);
		if (hasVisable) sprite->setVisible(visable);

		CCTexture2D* texture = NULL;
		if (hasTexturePath)
		{
			if(!texturePath.empty()) texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
			if(texture) sprite->setTexture(texture);
		}

		if (hasTextureRect)
		{
			if(texture) sprite->setTextureRect(CCRectMake(0,0,texture->getContentSizeInPixels().width,texture->getContentSizeInPixels().height));
			else sprite->setTextureRect(textureRect);
		}
		

		if (hasOpacity) sprite->setOpacity(opacity);
		if (hasColor) sprite->setColor(color);
		if (hasBolFlipX) sprite->setFlipX(bolFlipX);
		if (hasBolFlipY) sprite->setFlipY(bolFlipY);

		sprite->stopAllActions();
		//sprite->runAction(action);
		
		if(action) sprite->runAction((CCFiniteTimeAction*)action->copy()->autorelease());

		if(actionForever) sprite->runAction((CCFiniteTimeAction*)actionForever->copy()->autorelease());
	}
	else if (nodeType == "particeSystem")
	{
		if (child) {
            child->removeFromParentAndCleanup(true);
            child = NULL;
        }
//        CCParticleSystemQuad
		CCParticleSystemQuad* particle = (CCParticleSystemQuad*)child;
		if (!particle)
		{
			particle = CCParticleSystemQuad::create(texturePath.c_str());
//            particle->setScale(0.5);
			node->addChild(particle,zOrder,tag);
		}
		else
		{
//            particle->stopSystem();
			if (hasTexturePath) particle->initWithFile(texturePath.c_str());
			if (hasZOrder) node->reorderChild(particle,zOrder);
		}

		if (hasPosition) particle->setPosition(position);
		if (hasRotation) particle->setRotation(rotation);
		if (hasScale) particle->setScale(scale);
		if (hasVisable) 
		{
			particle->setVisible(visable);
		    if (visable) particle->resetSystem();
		    else particle->stopSystem();
		}

		particle->stopAllActions();

		if(action) particle->runAction((CCFiniteTimeAction*)action->copy()->autorelease());

		if(actionForever) particle->runAction((CCFiniteTimeAction*)actionForever->copy()->autorelease());
	}
	else if (nodeType == "labelTTF")
	{
		CCLabelTTF* label = (CCLabelTTF*)child;
		if (label)
		{
			if (hasZOrder) node->reorderChild(label,zOrder);
		}
		else 
		{
			label = CCLabelTTF::create(labelStr.c_str(),g_sFont.c_str(), g_iFontSize[48]);
			node->addChild(label,zOrder,tag);
		}
		//设置属性
		if (hasAnchorPosition) label->setAnchorPoint(anchorPosition);
		if (hasPosition) label->setPosition(position);
		if (hasRotation) label->setRotation(rotation);
		if (hasScale) label->setScale(scale);
		if (hasScaleX) label->setScaleX(scaleX);
		if (hasScaleY) label->setScaleY(scaleY);
		if (hasSkewX) label->setSkewX(skewX);
		if (hasSkewY) label->setSkewY(skewY);
		if (hasVisable) label->setVisible(visable);

		if (hasOpacity) label->setOpacity(opacity);
		if (hasColor) label->setColor(color);
		if (hasBolFlipX) label->setFlipX(bolFlipX);
		if (hasBolFlipY) label->setFlipY(bolFlipY);

		if (hasLabelStr) label->setString(labelStr.c_str());

		label->stopAllActions();
		if(action) label->runAction((CCFiniteTimeAction*)action->copy()->autorelease());

		if(actionForever) label->runAction((CCFiniteTimeAction*)actionForever->copy()->autorelease());
	}
	else if (nodeType == "labelAtlas")
	{
		if (child) {
			child->removeFromParentAndCleanup(true);
			child = NULL;
		}
		CCLabelAtlas* label = (CCLabelAtlas*)child;
		if (label)
		{
			label->initWithString(labelStr.c_str(),texturePath.c_str(),labelWidth,labelHeight,startCharMap);

			if (hasZOrder) node->reorderChild(label,zOrder);
		}
		else 
		{
			label = CCLabelAtlas::create(labelStr.c_str(),texturePath.c_str(),labelWidth,labelHeight,startCharMap);
			node->addChild(label,zOrder,tag);
		}
		//设置属性
		if (hasAnchorPosition) label->setAnchorPoint(anchorPosition);
		if (hasPosition) label->setPosition(position);
		if (hasRotation) label->setRotation(rotation);
		if (hasScale) label->setScale(scale);
		if (hasScaleX) label->setScaleX(scaleX);
		if (hasScaleY) label->setScaleY(scaleY);
		if (hasSkewX) label->setSkewX(skewX);
		if (hasSkewY) label->setSkewY(skewY);
		if (hasVisable) label->setVisible(visable);

		if (hasOpacity) label->setOpacity(opacity);
		if (hasColor) label->setColor(color);
		//if (hasBolFlipX) label->setFlipX(bolFlipX);
		//if (hasBolFlipY) label->setFlipY(bolFlipY);

		if (hasLabelStr) label->setString(labelStr.c_str());

		label->stopAllActions();
		if(action) label->runAction((CCFiniteTimeAction*)action->copy()->autorelease());

		if(actionForever) label->runAction((CCFiniteTimeAction*)actionForever->copy()->autorelease());
	}
	else if (nodeType == "progressTimer")
	{
		CCProgressTimer* progress = (CCProgressTimer*)child;
		if (progress)
		{
			if (hasTexturePath) progress->setSprite(CCSprite::create(texturePath.c_str()));
			if (hasZOrder) node->reorderChild(progress,zOrder);
		}
		else
		{
			progress = CCProgressTimer::create(CCSprite::create(texturePath.c_str()));
			node->addChild(progress,zOrder,tag);
		}

		//设置属性
		if (hasAnchorPosition) progress->setAnchorPoint(anchorPosition);
		if (hasPosition) progress->setPosition(position);
		if (hasRotation) progress->setRotation(rotation);
		if (hasScale) progress->setScale(scale);
		if (hasScaleX) progress->setScaleX(scaleX);
		if (hasScaleY) progress->setScaleY(scaleY);
		if (hasSkewX) progress->setSkewX(skewX);
		if (hasSkewY) progress->setSkewY(skewY);
		if (hasVisable) progress->setVisible(visable);
		//独有属性
		if (hasProgressTimerType)
        {
            setProgressType(&progress, progressTimerType);
        }
		if (hasPercentage) progress->setPercentage(percentage);

		progress->stopAllActions();
		if(action) progress->runAction((CCFiniteTimeAction*)action->copy()->autorelease());

		if(actionForever) progress->runAction((CCFiniteTimeAction*)actionForever->copy()->autorelease());
	}
}

void HBActionScript::setProgressType(CCProgressTimer** progress, PtProgressTimerType type)
{
    switch (type) {
        case kCCProgressTimerTypeRadialCCW:
            (*progress)->setType(kCCProgressTimerTypeRadial);
            (*progress)->setReverseProgress(true);
            break;
        case kCCProgressTimerTypeRadialCW:
            (*progress)->setType(kCCProgressTimerTypeRadial);
            break;
        case kCCProgressTimerTypeHorizontalBarLR:
            (*progress)->setType(kCCProgressTimerTypeBar);
            (*progress)->setMidpoint(ccp(0,0));
            (*progress)->setBarChangeRate(ccp(1, 0));
            break;
        case kCCProgressTimerTypeHorizontalBarRL:
            (*progress)->setType(kCCProgressTimerTypeBar);
            (*progress)->setMidpoint(ccp(1, 0));
            (*progress)->setBarChangeRate(ccp(1, 0));
            break;
        case kCCProgressTimerTypeVerticalBarBT:
            (*progress)->setType(kCCProgressTimerTypeBar);
            (*progress)->setMidpoint(ccp(0,0));
            (*progress)->setBarChangeRate(ccp(0, 1));
            break;
        case kCCProgressTimerTypeVerticalBarTB:
            (*progress)->setType(kCCProgressTimerTypeBar);
            (*progress)->setMidpoint(ccp(0, 1));
            (*progress)->setBarChangeRate(ccp(0, 1));
            break;
        default:
            break;
    }
}

void HBActionScript::scriptCallBack(CCNode* node,void* data)
{
	CCString* str = (CCString*)data;
	HBActionAni* ani = HBActionAniCache::sharedActionAniCache()->addActionAniWithFile(CSTR_FILEPTAH(g_ActionFilePath, str->m_sString) );
	ani->runAnimationBy(node);
}

void HBActionScript::frameCallBack1(CCNode* node,void* data)
{
	CCSpriteFrame* frame = (CCSpriteFrame*)data;
	CCSprite* sprite = (CCSprite*)node;
	sprite->setDisplayFrame(frame);
}

void HBActionScript::frameCallBack2(CCNode* node,void* data)
{
	CCLOG("HBActionScript::frameCallBack2");
	if(node == NULL) CCLOG("HBActionScript::frameCallBack2 node == NULL");
	if(data == NULL) CCLOG("HBActionScript::frameCallBack2 data == NULL");
	CCTexture2D* texture = (CCTexture2D*)data;
	CCSprite* sprite = (CCSprite*)node;
	sprite->setTexture(texture);
	sprite->setTextureRect(CCRectMake(0,0,texture->getContentSizeInPixels().width,texture->getContentSizeInPixels().height));
}

void HBActionScript::effectframeCallBack1(CCNode* node,void* data)
{
	CCSprite* sprite = (CCSprite*)node;
	CCTexture2D* texture = sprite->getTexture();
	sprite->setTexture(texture);
	sprite->setTextureRect(CCRectMake(0,0,texture->getContentSizeInPixels().width,texture->getContentSizeInPixels().height));
}

void HBActionScript::effectframeCallBack2(CCNode* node,void* data)
{
	CCString* str = (CCString*)data;
	int type = atoi(str->m_sString.c_str());
	CCSprite* sprite = (CCSprite*)node;
	CCTexture2D* texture = sprite->getTexture();

//    string name;
	string name = CCTextureCache::sharedTextureCache()->getKeyForTexture(texture);

	
	if (type == 0) texture = ImageTools::getOppositionTexture(name.c_str());
	else if (type == 1) texture = ImageTools::getGrayTexture(name.c_str());
	else if (type == 2) texture = ImageTools::getColorTexture(name.c_str(),255,255,255);
	else texture = ImageTools::getTexture(name.c_str());

	sprite->setTexture(texture);
	sprite->setTextureRect(CCRectMake(0,0,texture->getContentSizeInPixels().width,texture->getContentSizeInPixels().height));
}

void HBActionScript::backSoundCallBack(CCNode* node,void* data)
{
	CCString* str = (CCString*)data;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(str->m_sString.c_str());
}

void HBActionScript::soundCallBack(CCNode* node,void* data)
{
	CCString* str = (CCString*)data;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(str->m_sString.c_str());
}

void HBActionScript::particeSystemCallBack(CCNode* node,void* data)
{
	CCString* str = (CCString*)data;
	CCParticleSystemQuad* particleSystem = (CCParticleSystemQuad*)node;
//    particleSystem->setScale(0.5);
	if (str->m_sString == "0") 
	{
		particleSystem->stopSystem();
		particleSystem->setVisible(false);
	}
	else 
	{
		particleSystem->resetSystem();
		particleSystem->setVisible(true);
	}
}

//属性设置函数

void HBActionScript::setAction(CCAction* act)
{

	CC_SAFE_RELEASE(action);
	action = act;
	CC_SAFE_RETAIN(action);
}

void HBActionScript::setActionWith(const char* str)
{
	CC_SAFE_RELEASE(action);
	action = creatAction(str);
	CC_SAFE_RETAIN(action);
}

void HBActionScript::setNodeType(const char* str)
{
	nodeType = str;
}

void HBActionScript::setTag(int t)
{
	tag = t;
}

void HBActionScript::setAnchorPosition(CCPoint p)
{
	anchorPosition = p;
	hasAnchorPosition = true;
}

void HBActionScript::setPositionInPixels(CCPoint p)
{
	position = p;
	hasPosition = true;
}

void HBActionScript::setScale(float s)
{
	scale = s;
	hasScale = true;
}

void HBActionScript::setRotation(float r)
{
	rotation = r;
	hasRotation = true;
}

void HBActionScript::setVisable(bool v)
{
	visable = v;
	hasVisable = true;
}

void HBActionScript::setZOrder(int z)
{
	zOrder = z;
	hasZOrder = true;
}

void HBActionScript::setOpacity(GLubyte o)
{
	opacity = o;
	hasOpacity = true;
}

void HBActionScript::setColor(ccColor3B c)
{
	color = c;
	hasColor = true;
}

void HBActionScript::setBolFlipX(bool b)
{
	bolFlipX = b;
	hasBolFlipX = true;
}

void HBActionScript::setBolFlipY(bool b)
{
	bolFlipY = b;
	hasBolFlipY = true;
}

void HBActionScript::setTexturePath(const char* str)
{
	texturePath = str;
	hasTexturePath = true;
	hasTextureRect = true;
}

void HBActionScript::setLabelStr(const char* str)
{
	labelStr = str;
	hasLabelStr = true;
}

void HBActionScript::setStartCharMap(unsigned char c)
{
	startCharMap = c;
	hasStartCharMap = true;
}

void HBActionScript::setLabelWidth(unsigned int v)
{
	labelWidth = v;
	hasLabelWidth = true;
}
void HBActionScript::setLabelHeight(unsigned int v)
{
	labelHeight = v;
	hasLabelHeight = true;
}

void HBActionScript::setPercentage(float per)
{
	percentage = per;
	hasPercentage = true;
}
