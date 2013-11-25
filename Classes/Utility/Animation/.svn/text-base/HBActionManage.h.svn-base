


#pragma once
#include "cocos2d.h"
#include "HBActionAni.h"

using namespace cocos2d;
using namespace std;

typedef float ccTime; 


/************************************************************************/
/* 动画数据类                                                                    
*/
/************************************************************************/

class HBActionData :
	public CCObject
{
public:
	HBActionData(vector<int> l,HBActionAni* actionAni,float time = 0);
	~HBActionData();
	void runWithRoot(CCNode* root);

	HBActionAni* getActionAni();
	float getDelayTime();

	void setIsPlay(bool v);
private:
	vector<int> tagList;
	HBActionAni* actionAni;
	float delayTime;
	bool isPlay;
};

/************************************************************************/
/* 动画播放管理类  
使用一个二维数组存储动画，
*/
/************************************************************************/


class HBActionManage :
	public CCObject
{
public:
	HBActionManage(void);
	~HBActionManage(void);

	static HBActionManage* sharedActionManage(void);
	static void purgeActionManage(void);

	bool init();

	//播放位置往前
	void proceed();
	//把动画加入动画管理器
    void addCurrentAction(CCNode* owner,HBActionAni* ani,float delay);

	void clear();

	void replay();

    bool runActionAni(ccTime dt);

	//当前动画是否播放结束
	bool isCurrentDone();
	//所有动画是否播放结束
	bool isRunOut();

	//设置动画的基本节点
	void setRootNode(CCNode* node);

	int getCurrentPos();

	int getRunningPos();
private:
	static HBActionManage *s_pSharedHBActionManage;

	CCArray* actionArray;
	int currentPos;        //添加动画的位置
	int runningPos;        //播放动画的位置

	float aniTime;         //动画最大时间
	float timeCount;       

	CCNode* rootNode;
};


