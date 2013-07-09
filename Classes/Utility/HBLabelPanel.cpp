#include "HBLabelPanel.h"
#include "GameTools.h"
#include "GameConfig.h"
//#include "HBDataBase.h"
using namespace GameTools;

HBLabelPanel::HBLabelPanel(int w,int region):CCSprite()
{
	init();
	autorelease();

	maxWidth = w;

	labeTegion = region;

	maxHeight = 0;
	currenWidth = 0;
	currenHight = 0;

	setAnchorPoint(ccp(0,1));
}

HBLabelPanel::~HBLabelPanel(void)
{

}

void HBLabelPanel::addSpriteWithData(CCDictionary* data)
{
	string path = valueForKey("path",data);
	int offsetY = intForKey("offsetY",data);
	bool bolCenter = boolForKey("center",data);

	addSprite(path.c_str(),offsetY,bolCenter);
}
void HBLabelPanel::addLabelTTFWithData(CCDictionary* data)
{
	ccColor3B color = ccc3ForKey("color",data);
	int fontSize = intForKey("fontSize",data);
	int offsetY = intForKey("offsetY",data);
	string mainStr = valueForKey("string",data);
	bool bolCenter = boolForKey("center",data);
    //add bq
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    if(iPlatfrom == PLATFORM_IPHONE) {
//        fontSize /= 2;
//    }       
//#endif
    
	addLabelTTF(mainStr.c_str(),fontSize,color,offsetY,bolCenter);
}

void HBLabelPanel::addLabelTTFWithStr(const char* str,int fontSize,ccColor3B color,int offsetY,bool bolCenter)
{
	CCLabelTTF* label;

	label = CCLabelTTF::create(str,g_sFont.c_str(),fontSize);
	label->setAnchorPoint(ccp(0,1));

	//居中
	if (bolCenter)
	{
		label->setPosition(ccp((maxWidth-label->getContentSize().width)/2,-currenHight-offsetY));
	}
	else
	{
		label->setPosition(ccp(currenWidth,-currenHight-offsetY));
	}

	label->setColor(color);

	addChild(label);

	currenWidth += label->getContentSize().width;
	maxHeight = MAX(maxHeight,label->getContentSize().height+offsetY);
	currentNodeList.push_back(label);
}

//void HBLabelPanel::addLabelPanelWithData(CCDictionary* data)
//{
//	int w = intForKey("maxWidth",data);
//	int rowSpace = intForKey("rowSpace",data);
//
//	HBLabelPanel* label;
//	label = new HBLabelPanel(w,rowSpace);
//	label->setAnchorPoint(ccp(0,1));
//	label->setPosition(ccp(currenWidth,-currenHight));
//
//	CCDictionary* tmpdata = (CCDictionary*)data->objectForKey("panel");
//	if (tmpdata)
//	{
//		label->resetPanelWithDict(tmpdata);
//	}
//	else
//	{
//		label->resetPanelWithFile(valueForKey("panelFile",data),valueForKey("panelKey",data));
//	}
//
//	
//
//	addChild(label);
//
//	currenWidth += label->getWidth();
//	maxHeight = MAX(maxHeight,label->getHeight());
//	currentNodeList.push_back(label);
//}

void HBLabelPanel::addSprite(const char* path,int offsetY,bool bolCenter)
{
	CCSprite* sprite = CCSprite::create(path);
	sprite->setAnchorPoint(ccp(0,1));

	//居中
	if (bolCenter)
	{
		sprite->setPosition(ccp((maxWidth-sprite->getContentSize().width)/2,-currenHight-offsetY));
	}
	else
	{
		sprite->setPosition(ccp(currenWidth,-currenHight-offsetY));
	}
	
	addChild(sprite);
	currenWidth += sprite->getContentSize().width*sprite->getScaleX();
	maxHeight = MAX(maxHeight,sprite->getContentSize().height*sprite->getScaleY()+offsetY);
	currentNodeList.push_back(sprite);
}

void HBLabelPanel::addLabelTTF(const char* str,int fontSize,ccColor3B color,int offsetY,bool bolCenter)
{
	string mainStr = str;

	CCLabelTTF* label = CCLabelTTF::create(mainStr.c_str(),g_sFont.c_str(),fontSize);
        //add bq
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    if(iPlatfrom == PLATFORM_IPHONE) {
//        fontSize *= 2;
//    }      
//#endif
	vector<string> subString;
	int tmpWidth = currenWidth;

	int maxw = 0;
	int lenth = 0;
	while (tmpWidth + label->getContentSize().width>maxWidth)
	{
		maxw = maxWidth-tmpWidth;
		lenth = maxw/(fontSize/2);
		//越界判定
		lenth = lenth>mainStr.size()-1?mainStr.size()-1:lenth;

		bool bolChinese = false;  //中文标识
		vector<int> canSub;
		for (unsigned int i = 0;i<mainStr.size();i++)
		{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(mainStr[i]>0xa1)
			{
				canSub.push_back(true);
				canSub.push_back(false);
				canSub.push_back(false);
				i+=2;
				bolChinese = true;
			}
			else 
				canSub.push_back(true);
#else
            if(mainStr[i]<0||mainStr[i]>254)
			{
				canSub.push_back(true);
				canSub.push_back(false);
				canSub.push_back(false);
				i+=2;
				bolChinese = true;
			}
			else 
				canSub.push_back(true);
#endif
		}

		bool bolBreak = false;
		bool ret = true;
		do 
		{
			//中文字符后两位不可切割
			if(canSub[lenth])
			{
				label->setString(mainStr.substr(0,lenth).c_str());
				
				if (label->getContentSize().width+fontSize>maxw)
				{
					ret = false;
				}
				else if (label->getContentSize().width>maxw)
				{
					lenth--;
					bolBreak = true;
				}
				else
				{
					//int tmp = maxw - label->getContentSizeInPixels().width;
					//lenth += tmp/(fontSize/2);
					if (bolBreak) ret = false;
					else 
					{
						//越界判定
						if (lenth+3>canSub.size()-1) ret = false;
						else lenth += 3;
					}
				}

				//英文版，断字处理
				if (!bolChinese)
				{
					if (!ret&&lenth>0)
					{
						if (mainStr[lenth] == ' ')
						{
							lenth++;
						}
						else
						{
							while(!(mainStr[lenth-1] == ' '))
							{
								lenth--;
								if (lenth == 0) 
								{
									//lenth = mainStr.length();
									break;
								}
									
							}
						}

					}
				}
			}
			else
			{
				lenth--;
			}
		} while (ret);
        
        if (lenth > 0)
        {
            subString.push_back(mainStr.substr(0,lenth));
        }
        else
        {
            nextLine(true);
        }
		mainStr = mainStr.substr(lenth);
		label->setString(mainStr.c_str());
		for (unsigned int i = 0;i<lenth;i++)
		{
			canSub.erase(canSub.begin()+0);
		}


		tmpWidth = 0;
		// 		maxw = maxWidth-tmpWidth;
		// 		lenth = maxw/(fontSize/2);
	}

	subString.push_back(mainStr);
    //add bq
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    if(iPlatfrom == PLATFORM_IPHONE) {
//        fontSize /= 2;
//    }    
//#endif



	for (unsigned int i = 0;i<subString.size();i++)
	{
		addLabelTTFWithStr(subString[i].c_str(),fontSize,color,offsetY,bolCenter);
		if (i<subString.size()-1)
			nextLine(true);
	}
}

void HBLabelPanel::addSplitLabelTTF(const char* str,int fontSize,ccColor3B color,int offsetY,int w,int rowSpace)
{

	HBLabelPanel* label;
	label = new HBLabelPanel(w,rowSpace);
	label->setAnchorPoint(ccp(0,1));
	label->setPosition(ccp(currenWidth,-currenHight-offsetY));

	label->addLabelTTF(str,fontSize,color,0);

	addChild(label);

	currenWidth += label->getWidth();
	maxHeight = MAX(maxHeight,label->getHeight());
	currentNodeList.push_back(label);
}




//增加外部引入的精灵
void HBLabelPanel::addSpriteWithSprite(CCSprite* sprite,int offsetX,int offsetY)
{
	currenWidth += offsetX;

	sprite->setAnchorPoint(ccp(0,1));
	sprite->setPosition(ccp(currenWidth,-currenHight-offsetY));

	addChild(sprite);

	currenWidth += sprite->getContentSize().width*sprite->getScaleX();
	maxHeight = MAX(maxHeight,sprite->getContentSize().height*sprite->getScaleY()+offsetY);
	currentNodeList.push_back(sprite);

	nextLine(false);
}

void HBLabelPanel::addLabelTTFWithLabel(CCLabelTTF* label,int offsetX,int offsetY,bool bolCenter)
{
	currenWidth += offsetX;

	label->setAnchorPoint(ccp(0,1));
	//居中
	if (bolCenter)
	{
		label->setPosition(ccp((maxWidth-label->getContentSize().width)/2,-currenHight-offsetY));
	}
	else
	{
		label->setPosition(ccp(currenWidth,-currenHight-offsetY));
	}

	addChild(label);

	currenWidth += label->getContentSize().width;
	maxHeight = MAX(maxHeight,label->getContentSize().height+offsetY);
	currentNodeList.push_back(label);

	nextLine(false);
}

void HBLabelPanel::addSpace(int n)
{
	currenWidth += n;
	nextLine(false);
}

void HBLabelPanel::addLine(ccColor3B color,int lineNum)
{
	nextLine(true);

	CCSprite* line = new CCSprite();
	line->init();
    line->autorelease();
	line->setTextureRect(CCRectMake(0,0,maxWidth-10,1));
	line->setColor(color);
	line->setAnchorPoint(ccp(0.5,1));
	line->setPosition(ccp(maxWidth/2,-(currenHight+lineNum)));
	addChild(line);

	maxHeight = line->getContentSize().height+2*lineNum;

	nextLine(true);
}

void HBLabelPanel::addLine()
{
	nextLine(true);

	CCSprite* line = new CCSprite();
	line->init();
    line->autorelease();
	line->setTextureRect(CCRectMake(0,0,maxWidth-10,1));
	line->setColor(ccc3(52,66,115));
	line->setAnchorPoint(ccp(0.5,1));
	line->setPosition(ccp(maxWidth/2,-(currenHight)));
	addChild(line);

	maxHeight = line->getContentSize().height;

	nextLine(true);
}

void HBLabelPanel::addEnter()
{
	nextLine(true);
}


void HBLabelPanel::centerCurrentLine()
{
	//居中
// 	for (unsigned int i = 0;i<currentNodeList.size();i++)
// 	{
// 		currentNodeList[i]->setPositionInPixels(ccp(currentNodeList[i]->getPositionInPixels().x,-(currenHight+labeTegion/2)));
// 	}
	
	//setContentSizeInPixels(CCSizeMake(maxWidth,currenHight));
	//setTextureRect(CCRectMake(0,0,maxWidth,currenHight+maxHeight+LABELREGION));
}

void HBLabelPanel::nextLine(bool bolchange)
{
	if (currenWidth>=maxWidth||bolchange)
	{
		centerCurrentLine();
		currenWidth = 0;
		currenHight += maxHeight+labeTegion;
		maxHeight = 0;
		currentNodeList.clear();
	}
}


int HBLabelPanel::getWidth()
{
	return maxWidth;
}
int HBLabelPanel::getHeight()
{
	int h = currenHight;
	if (maxHeight != 0)
	{
		h += maxHeight+labeTegion;
	}

	return h;
}