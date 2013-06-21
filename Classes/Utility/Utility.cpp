#include "Utility.h"
#include "gameTools.h"
//#include "CCFileUtils.h" 
#include "stdlib.h"
#include "WordCache.h"
#include "gameConfig.h"
#include "HBActionAni.h"
#include "HBActionAniCache.h"

namespace Utility {    
    
    void addTouchRect(CCRect rect,int touch_tag,CCNode* node,vector<TouchRect>& vTouchRect)
    {
        TouchRect touchRect(rect,touch_tag,node);
        vTouchRect.push_back(touchRect);
    }
    
    void setTouchRect(CCRect rect,int touch_tag,vector<TouchRect>& vTouchRect)
    {
        for (int i = 0; i < vTouchRect.size(); i++) {
            TouchRect& touchRect = vTouchRect[i];
            if (touchRect.tag == touch_tag) {
                touchRect.rect = rect;
                break;
            }
        }
    }
    
    void addTouchRect(int touch_tag,CCNode* node,vector<TouchRect>& vTouchRect)
    {
        CCRect rect = getNodeTouchRect(node);
        addTouchRect(rect,touch_tag,node,vTouchRect);
    }
    
    void addTouchRect(int touch_tag,CCNode* fatherNode,const string& sTag,vector<TouchRect>& vTouchRect)
    {
        CCNode* node = getNodeByTag(fatherNode, sTag);
        addTouchRect(touch_tag,node,vTouchRect);
    }
    
    //删除触摸精灵
    void removeTouchRect(int touch_tag, vector<TouchRect>& vTouchRect) {
        bool isRepeat = true;
        do {
            isRepeat = false;
            for (int i = 0; i < vTouchRect.size(); i++) {
                TouchRect touchRect = vTouchRect.at(i);
                if (touchRect.tag == touch_tag) {
                    isRepeat = true;
                    vTouchRect.erase(vTouchRect.begin() + i);
                    break;
                }
            }
        } while (isRepeat);
    }
    
    CCNode* getNodeByTag(CCNode* father, const vector<int> vTag, int index) {
        if(!father)
            return NULL;
        CCNode* node = (CCNode*) father->getChildByTag(vTag[index]);
        
        if (index < vTag.size() - 1) {
            return getNodeByTag(node, vTag, index + 1);
        } else {
            return node;
        }
    }
    
    CCNode* getNodeByTag(CCNode* father, const string& sTag, int index)
    {
        vector<int> vTag = transToIntVector(splitString(sTag.c_str(), ","));
        
        return getNodeByTag(father, vTag, 0);
    }
    
    CCRect getNodeTouchRect(CCNode* node)
    {
        CCRect rect;
        rect.origin = node->convertToWorldSpace(ccp(0,0));
        rect.size = node->getContentSize();
        float fScaleX = getParentScaleX(node);
        float fScaleY = getParentScaleY(node);
        rect.size.width *= fScaleX;
        rect.size.height *= fScaleY;
        return rect;
    }
    
    //取出上层节点缩放值
    float getParentScaleX(CCNode* node)
    {
        if (node != NULL)
        {
            CCNode* parent = node->getParent();
            if(parent != NULL && node->getScaleX() == 1.0)
            {
                return getParentScaleX(parent);
            }
            else
            {
                return node->getScaleX();
            }
        }
        return 1.0;
    }
    
    float getParentScaleY(CCNode* node)
    {
        if (node != NULL) {
            CCNode* parent = node->getParent();
            if(parent != NULL && node->getScaleY() == 1.0)
            {
                return getParentScaleY(parent);
            }
            else
            {
                return node->getScaleY();
            }
        }
        return 1.0;
    }
    
    CCPoint getPositionByTag(CCNode* father, CCPoint pos, const vector<int> vTag, int index) {
        CCNode* node = (CCNode*) father->getChildByTag(vTag[index]);
        
        pos = ccp(pos.x + node->getPosition().x,pos.y + node->getPosition().y);
        if (index < vTag.size() - 1) {
            return getPositionByTag(node, pos, vTag, index + 1);
        } else {
            return pos;
        }
    }
    
    CCPoint getPositionByTag(CCNode* father, CCPoint pos, const string& sTag,int index)
    {
        vector<int> vTag = transToIntVector(splitString(sTag.c_str(), ","));
        return getPositionByTag(father, pos, vTag, 0);
    }
    
    //迭代取目标tag在屏幕上的rect
    CCRect getNodeRectInScreen(CCNode* father, const vector<int> vTag, int index) {
        CCRect rect;
        CCNode* node = (CCNode*) father->getChildByTag(vTag[index]);
        if (index < vTag.size() - 1) {
            return getNodeRectInScreen(node, vTag, index + 1);
        } else {
            rect.origin = node->getPosition();
            rect.size = node->getContentSize();
            return rect;
        }
        
    }
    
    CCRect getNodeRectInScreen(CCNode* node, CCPoint pos, const vector<int> vTag, int index) {
        //bq modify
        CCNode* child = getNodeByTag(node, vTag, 0);
        CCRect rect = getNodeTouchRect(child);
        return rect;
    }
    
    CCRect getNodeRectInScreen(CCNode* node, CCPoint pos, const string& sTag, int index) {
        vector<int> vTag = transToIntVector(splitString(sTag.c_str(), ","));        
        return getNodeRectInScreen(node, pos, vTag, 0);
    }
    
    //生成文字
    string getWordWithFile(const string& file, const char* str) {
        //        file = WordPath + file;
        //先寻找缓冲
        CCDictionary* data = WordCache::sharedWordCache()->WordByName(file.c_str());
        if(!data) {
            string fileName = g_wordDirPath + file;
            std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName.c_str());
            CCLOG("word path = %s",m_sPlistFile.c_str());
            data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
            //添加缓冲
            WordCache::sharedWordCache()->addWord(data, file.c_str());
            CCLOG("新文字");
        }
        
        string word = string(valueForKey(str, data));
        return word;
    }
    
    string getWordWithFile(const char* str) {

        return getWordWithFile(g_wordFilePath, str);
    }
    
    int runPtActionScript(CCNode* node, const char* filePtah, int tag)
    {
        CCSprite* spr = CCSprite::create();
        spr->setTag(tag);
        spr->setPosition(ccp(0,0));
        HBActionAni* ani = HBActionAniCache::sharedActionAniCache()->addActionAniWithFile(CSTR_FILEPTAH(g_ActionFilePath,filePtah));
        node->addChild(spr);
        ani->runAnimationBy(spr);
        return 0;
        
    }
    
    int stopPtActionScript(CCNode* node,int tag)
    {
        CCNode* childNode = node->getChildByTag(tag);
        childNode->stopAllActions();
        childNode->removeFromParentAndCleanup(true);
        return 0;
    }
    
    int addPtActionScript(const char* filePtah)
    {
        HBActionAniCache::sharedActionAniCache()->addActionAniWithFile(CSTR_FILEPTAH(g_ActionFilePath,filePtah));
        return 0;
    }

    
}


TouchRect::TouchRect()
{
	this->rect = CCRectMake(0, 0, 0, 0);
	this->tag = -1;
	this->node = NULL;
    this->call_back = "";
    this->nextPath = "";
}

TouchRect::TouchRect(CCRect rect, int tag, CCNode* node,string callback)
{
	this->rect = rect;
	this->tag = tag;
	this->node = node;
    this->call_back = callback;
    this->nextPath = "";
    //    CCLOG("callback %s",callback.c_str());
}

//解析callback
vector<int>  TouchRect::getCallBackType()
{
    vector<int> vType;
    //    int type = CALL_BACK_TYPE_NULL;
    
    vector<string> vSType = splitArgString(call_back.c_str(), ";");
    for (int i = 0; i < vSType.size(); i++)
    {
        
        string sType = getSubStr_endStr(vSType[i], ":");
        int type = CALL_BACK_TYPE_NULL;
        if(sType == "close")
        {
            type = CALL_BACK_TYPE_CLOSE;
        }
        else if (sType == "pass")
        {
            string content = getSubStr(vSType[i], ":").c_str();
             if (content == "0")
            {
                type = CALL_BACK_TYPE_PASS_NOTCLOSE;
            }
            else
            {
                type = CALL_BACK_TYPE_PASS;
            }
        }
        else if (sType == "script")
        {
            vector<string> content = splitString(getSubStr(vSType[i], ":").c_str(), ",");
            if (content[1] == "0")
            {
                type = CALL_BACK_TYPE_SCRIPT_MODIFY;
            }
            else if (content[1] == "1")
            {
                type = CALL_BACK_TYPE_SCRIPT_NEW;
            }
            nextPath = content[0];
        }
        
        vType.push_back(type);
        
    }
    
    return vType;
}

string TouchRect::getNextScriptPath()
{
    return nextPath;
}

int TouchRect::SearchTouchTag(CCPoint pos, vector<TouchRect>& vTouchRect, CCSprite** btnSprite)
{
    int touch_tag = -1;
    for (int i = 0;  i < vTouchRect.size(); i++)
    {
        
        CCRect rect = vTouchRect[i].rect;
        if (rect.containsPoint(pos))
        {
            touch_tag = vTouchRect[i].tag;
            if(btnSprite)
            {
                *btnSprite = (CCSprite*)vTouchRect[i].node;
            }
            break;
        }
    }
    
    return touch_tag;

}
void TouchRect::AppendCurrTouchTag(int touchTag, vector<TouchRect>& vTouchRect,  vector<TouchRect*> &vCurrTouch)
{
    if(touchTag==-1)
    {
        return;
    }
    else
    {
        for ( int i =0 ; i<vTouchRect.size(); i++)
        {
            if (vTouchRect[i].tag==touchTag)
            {
                vCurrTouch.push_back(&vTouchRect[i]);
                break;
            }
        }
    }
    
}
