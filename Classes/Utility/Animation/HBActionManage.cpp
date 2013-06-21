#include "HBActionManage.h"


HBActionManage* HBActionManage::s_pSharedHBActionManage = NULL;

HBActionManage* HBActionManage::sharedActionManage(void)
{
	if (! s_pSharedHBActionManage)
	{
		s_pSharedHBActionManage = new HBActionManage();
		s_pSharedHBActionManage->init();
	}

	return s_pSharedHBActionManage;
}

void HBActionManage::purgeActionManage(void)
{
	CC_SAFE_RELEASE_NULL(s_pSharedHBActionManage);
}

bool HBActionManage::init()
{
	actionArray = CCArray::array();
	actionArray->retain();
	currentPos = -1;
	runningPos = -1;
	timeCount = 0;
	aniTime = 0;
	return true;
}

HBActionManage::HBActionManage(void)
{

}

HBActionManage::~HBActionManage(void)
{
    //bq modify
	CC_SAFE_RELEASE_NULL(actionArray);
}

void HBActionManage::proceed()
{
	currentPos++;
	actionArray->addObject(CCArray::array());
}

void HBActionManage::addCurrentAction(CCNode* owner,HBActionAni* ani,float delay)
{
	if (!owner)
	{
		
		return;
	}

	vector<int> tagList;
	
	while(owner != rootNode)
	{
		tagList.push_back(owner->getTag());
		owner = owner->getParent();
		if (!owner)
		{
			CCLOG("HBActionManage:node error,is not rootNode's children");
			return;
		}
	}
	
	

	if (currentPos == -1)
	{
		proceed();
	}
	
	HBActionData* data = new HBActionData(tagList,ani,delay);
	CCArray* a = (CCArray*)actionArray->objectAtIndex(currentPos);
	a->addObject(data);
}

void HBActionManage::clear()
{
	
	actionArray->removeAllObjects();
	
	currentPos = -1;
	runningPos = -1;
	timeCount = 0;
	aniTime = 0;
}

void HBActionManage::replay()
{
	

	for (unsigned int i = 0;i<=runningPos;i++)
	{
		CCArray* data = (CCArray*)actionArray->objectAtIndex(i);
		//运行动画脚本
		for (int j = 0;j<data->count();j++)
		{
			HBActionData* act = (HBActionData*)data->objectAtIndex(j);
			act->setIsPlay(false);
		}
	}
	
	runningPos = -1;
	aniTime = 0;
	timeCount = 0;
}

bool HBActionManage::runActionAni(ccTime dt)
{
	if (!isRunOut())
	{
		if (isCurrentDone())
		{
			aniTime = 0;
			timeCount = 0;
			runningPos++;
			CCArray* data = (CCArray*)actionArray->objectAtIndex(runningPos);
			//计算最大动画时间
			for (int i = 0;i<data->count();i++)
			{
				HBActionData* act = (HBActionData*)data->objectAtIndex(i);
				aniTime = max(aniTime,act->getDelayTime()+act->getActionAni()->getDuration());
			}
		}
		else
		{
			timeCount+=dt;
			CCArray* data = (CCArray*)actionArray->objectAtIndex(runningPos);
			//运行动画脚本
			for (int i = 0;i<data->count();i++)
			{
				HBActionData* act = (HBActionData*)data->objectAtIndex(i);
				if (timeCount>=act->getDelayTime())
				{
					act->runWithRoot(rootNode);
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool HBActionManage::isCurrentDone()
{
	if (runningPos == -1)
	{
		return true;
	}
	else
	{
		if (timeCount>aniTime)
		{
			return true;
		}
	}
	return false;
}

bool HBActionManage::isRunOut()
{
	if (runningPos == currentPos)
	{
		if (isCurrentDone())
		{
			return true;
		}
	}
	return false;
}

void HBActionManage::setRootNode(CCNode* node)
{
	rootNode = node;
}

int HBActionManage::getRunningPos()
{
	return runningPos;
}

int HBActionManage::getCurrentPos()
{
	return currentPos;
}



//
HBActionData::HBActionData(vector<int> l,HBActionAni* actionAni,float time)
{
	tagList = l;
	this->actionAni = actionAni;
	if (this->actionAni)
	{
		this->actionAni->retain();
	}
	delayTime = time;
	isPlay = false;
	autorelease();
}

HBActionData::~HBActionData()
{
	CC_SAFE_RELEASE(actionAni);
}

void HBActionData::runWithRoot(CCNode* root)
{
	for (int i = tagList.size()-1;i>=0;i--)
	{
		root = root->getChildByTag(tagList[i]);
	}
	
	if (!isPlay)
	{
		isPlay = true;
		actionAni->runAnimationBy(root);
	}
	
}

void HBActionData::setIsPlay(bool v)
{
	isPlay = v;
}

HBActionAni* HBActionData::getActionAni()
{
	return actionAni;
}

float HBActionData::getDelayTime()
{
	return delayTime;
}

