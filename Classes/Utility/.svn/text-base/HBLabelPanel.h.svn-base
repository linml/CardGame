#pragma once
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;


class HBLabelPanel :
	public CCSprite
{
public:
	HBLabelPanel(int w, int region = 4);
	~HBLabelPanel(void);

	void addSpriteWithData(CCDictionary* data);
	void addLabelTTFWithData(CCDictionary* data);
//	void addLabelPanelWithData(CCDictionary* data);

	void addSprite(const char* path,int offsetY = 0,bool bolCenter = false);
	void addLabelTTF(const char* str,int fontSize,ccColor3B color,int offsetY,bool bolCenter = false);

	//增加一个面板用来对label进行自动分行
	void addSplitLabelTTF(const char* str,int fontSize,ccColor3B color,int offsetY,int w,int rowSpace);

	void addSpriteWithSprite(CCSprite* sprite,int offsetX = 0,int offsetY = 0);
	void addLabelTTFWithLabel(CCLabelTTF* label,int offsetX = 0,int offsetY = 0,bool bolCenter = false);
	void addSpace(int n);
	void addLine();
	void addLine(ccColor3B color,int lineNum = 0);
	void addEnter();

	int getWidth();
	int getHeight();
	
private:

	void addLabelTTFWithStr(const char* str,int fontSize,ccColor3B color,int offsetY,bool bolCenter = false);

	void centerCurrentLine();

	void nextLine(bool bolchange);

	int maxWidth;   //每一行的最大宽度

	int labeTegion;

	int currenWidth;
	int currenHight;

	int maxHeight;   //记录当前行的最大高度
	vector<CCNode*> currentNodeList;
};
