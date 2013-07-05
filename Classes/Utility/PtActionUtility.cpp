//
//  PtActionUtility.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-6-8.
//
//

#include "PtActionUtility.h"
#include "PtMapUtility.h"
#include "gameTools.h"

//格子效果动作完成标记
bool isActionOver = false;
//按钮正在执行动作（不能重复）
bool isButtonActing = false;

namespace PtActionUtility {
    
    //读取精灵动作
    void createActions(CCDictionary* data,vector<ActionData*>* vActions) {
        CCArray* vActionString = (CCArray*) data->objectForKey(string("actions"));
        int i = 0;
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(vActionString,pObj)
        {
            ActionData* actionData = new ActionData;
            char tmp[10]="";            
            sprintf(tmp, "Item %d", i++);
            actionData->actionName = string(tmp);
            string action = ((CCString*)pObj)->m_sString;
            actionData->action = action;
            actionData->actionType = GameTools::getSubStr_endStr(action, "(");
            vActions->push_back(actionData);
        }
    }
    
    //解析字符串-逐层解析 生成最后动作
    void setAction(CCNode* sprite, const string& actionName, const vector<ActionData*>* vActions) {
        //find action
        string sAction = getSActionWithName(actionName, vActions);
        //迭代解析
        CCAction* action = decodeStringToAction(sAction, vActions);
        sprite->runAction(action);
        
    }
    
    //find action by name
    string getSActionWithName(const string& actionName, const vector<ActionData*>* vActions) {
        //    CCLOG(actionName.c_str());
        for (int i = 0; i < vActions->size(); i++) {
            ActionData* actionData = vActions->at(i);
            //        CCLOG(actionData->actionName.c_str());
            if (actionData->actionName == actionName) {
                return actionData->action;
            }
        }
        return "";
    }
    
    //解析字符串-动作
    CCAction* decodeStringToAction(const string& sAction, const vector<ActionData*>* vActions) {
        string actionType = getSubStr_endStr(sAction, "(");
        //    CCLOG(actionType.c_str());
        if (actionType == act_sequence) {
            //取actionName 进一步解析
            vector<string> vActionName = GameTools::splitString(sAction, "(", ")");
            CCArray* seqActionArray = CCArray::createWithCapacity(vActionName.size());
            for (int i = 0; i < vActionName.size(); i++) {
                string action = getSActionWithName(vActionName[i], vActions);
                CCAction* seqAction = decodeStringToAction(action, vActions);
                seqActionArray->addObject(seqAction);
            }
            CCAction* action = CCSequence::create(seqActionArray);
            //        seqActionArray->release();
            return action;
        } else if (actionType == act_spawn) {
            //取actionName 进一步解析
            vector<string> vActionName = splitString(sAction, "(", ")");
            CCArray* seqActionArray = CCArray::createWithCapacity(vActionName.size());
            for (int i = 0; i < vActionName.size(); i++) {
                string action = getSActionWithName(vActionName[i], vActions);
                CCAction* seqAction = decodeStringToAction(action, vActions);
                seqActionArray->addObject(seqAction);
            }
            CCAction* action = CCSpawn::create(seqActionArray);
            //        seqActionArray->release();
            return action;
        } else if (actionType == act_repeat) {
            vector<string> vValue = splitString(sAction, "(", ")");
            //第一个参数是名字 要进一步解析
            string actionName = vValue[0];
            string saction = getSActionWithName(actionName, vActions);
            CCAction* repeatAction = decodeStringToAction(saction, vActions);
            //次数
            int times = atoi(vValue[1].c_str());
            CCActionInterval* action = CCRepeat::create((CCFiniteTimeAction*) repeatAction, times);
            return action;
        } else if (actionType == act_repeatforever) {
            //参数是名字 要进一步解析
            string actionName = getSubStr(sAction, "(", ")");
            string saction = getSActionWithName(actionName, vActions);
            CCAction* repeatAction = decodeStringToAction(saction, vActions);
            CCActionInterval* action = CCRepeatForever::create((CCActionInterval*) repeatAction);
            return action;
        } else if (actionType == act_reverse) {
            //参数是名字 要进一步解析
            string actionName = getSubStr(sAction, "(", ")");
            string saction = getSActionWithName(actionName, vActions);
            CCAction* repeatAction = decodeStringToAction(saction, vActions);
            return ((CCActionInterval*) repeatAction)->reverse();
        } else if (actionType == act_animation) {
            //参数 动画名字
            string aniName = getSubStr(sAction, "(", ")");
            CCAnimation* ani = CCAnimationCache::sharedAnimationCache()->animationByName(aniName.c_str());
            CCAnimate* action = CCAnimate::create(ani);
            return action;
        } else if (actionType == act_hide) {
            CCActionInstant* action = new CCHide();
            action->autorelease();
            return (CCAction*) action;
        } else if (actionType == act_show) {
            CCActionInstant* action = new CCShow();
            action->autorelease();
            return (CCAction*) action;
        } else if (actionType == act_place) {
            CCPoint point = pointFromString(getSubStr(sAction, "(", ")").c_str());
            return CCPlace::create(point);
        } else if (actionType == act_delay) {
            float time = atof(getSubStr(sAction, "(", ")").c_str());
            return CCDelayTime::create(time);
        } else if (actionType == act_move_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            CCPoint desPos = pointFromString(vValue[1]);
            CCActionInterval* action = CCMoveTo::create(time, desPos);
            return action;
        } else if (actionType == act_move_by) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            CCPoint desPos = pointFromString(vValue[1]);
            CCActionInterval* action = CCMoveBy::create(time, desPos);
            return action;
        } else if (actionType == act_rotate_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float angle = atof(vValue[1].c_str());
            CCActionInterval* action = CCRotateTo::create(time, angle);
            return action;
        } else if (actionType == act_rotate_by) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float angle = atof(vValue[1].c_str());
            CCActionInterval* action = CCRotateBy::create(time, angle);
            return action;
        } else if (actionType == act_scale_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            if (vValue.size() == 2) {
                float scale = atof(vValue[1].c_str());
                CCActionInterval* action = CCScaleTo::create(time, scale);
                return action;
            } else {
                float scaleX = atof(vValue[1].c_str());
                float scaleY = atof(vValue[2].c_str());
                CCActionInterval* action = CCScaleTo::create(time, scaleX, scaleY);
                return action;
            }
        } else if (actionType == act_scale_by) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            if (vValue.size() == 2) {
                float scale = atof(vValue[1].c_str());
                CCActionInterval* action = CCScaleBy::create(time, scale);
                return action;
            } else {
                float scaleX = atof(vValue[1].c_str());
                float scaleY = atof(vValue[2].c_str());
                CCActionInterval* action = CCScaleBy::create(time, scaleX, scaleY);
                return action;
            }
        } else if (actionType == act_skew_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float sx = atof(vValue[1].c_str());
            float sy = atof(vValue[2].c_str());
            CCActionInterval* action = CCSkewTo::create(time, sx, sy);
            return action;
        } else if (actionType == act_skew_by) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float sx = atof(vValue[1].c_str());
            float sy = atof(vValue[2].c_str());
            CCActionInterval* action = CCSkewBy::create(time, sx, sy);
            return action;
        } else if (actionType == act_fade_in) {
            float time = atof(getSubStr(sAction, "(", ")").c_str());
            CCActionInterval* action = CCFadeIn::create(time);
            return action;
        } else if (actionType == act_fade_out) {
            float time = atof(getSubStr(sAction, "(", ")").c_str());
            CCActionInterval* action = CCFadeOut::create(time);
            return action;
        } else if (actionType == act_fade_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float opacity = atof(vValue[1].c_str());
            
            CCActionInterval* action = CCFadeTo::create(time, opacity);
            return action;
        } else if (actionType == act_blink) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            float times = atof(vValue[1].c_str());
            return CCBlink::create(time, times);
        } else if (actionType == act_tint_to) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            int red = atoi(vValue[1].c_str());
            int green = atoi(vValue[2].c_str());
            int blue = atoi(vValue[3].c_str());
            return CCTintTo::create(time, red, green, blue);
        } else if (actionType == act_tint_by) {
            vector<string> vValue = splitString(sAction, "(", ")");
            float time = atof(vValue[0].c_str());
            int red = atoi(vValue[1].c_str());
            int green = atoi(vValue[2].c_str());
            int blue = atoi(vValue[3].c_str());
            return CCTintBy::create(time, red, green, blue);
        } else if (actionType == act_over) {
            CCSprite* sprite = new CCSprite();
            sprite->init();
            sprite->autorelease();
            CCCallFunc* action = CCCallFunc::create(sprite, callfunc_selector(ActionCallFun::actionOver));
            return action;
        }
        else if (actionType == act_remove_self) {
            
        }
        return NULL;
    }
    
    //取得脚本在的一个组合动画
    CCAction* getRunActionWithActionFile(const string& file) {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file.c_str());
        CCDictionary* data =CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        CCAction* action = NULL;
        CCArray* vKey = data->allKeys();
        vector<ActionData*> vActions;
        //判断是否要动作
        for (int i = 0; i < vKey->count(); i++) {
            CCString* key = (CCString*)vKey->objectAtIndex(i);
            if (key->m_sString == "actions") {
                //action数据
                createActions(data, &vActions);
                break;
            }
        }
        //判断是否runAction
        for (int i = 0; i < vKey->count(); i++) {
            CCString* key = (CCString*)vKey->objectAtIndex(i);
            if (key->m_sString == "runAction") {
                string runActionName = string(valueForKey("runAction", data));
                //find action
                string sAction = getSActionWithName(runActionName, &vActions);
                //迭代解析
                action = decodeStringToAction(sAction, &vActions);
                break;
            }
        }
        //删除vAction
        for (int i = 0; i < vActions.size(); i++) {
            delete vActions[i];
        }
        
        return action;
        
    }
    
    //载入动画文件，生成动画
    void createAniWithFile(const string& file) {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file.c_str());
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        CCArray* vAniName = data->allKeys();
        vector<string> vAniFrames;
        float delay = 0.1;
        for (int i = 0; i < vAniName->count(); i++) {
            CCString* key = (CCString*)vAniName->objectAtIndex(i);
            vAniFrames.clear();
            //读取帧数组
            CCArray* sAinFrames = (CCArray*)(data->objectForKey(key->m_sString));
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(sAinFrames, pObj)
            {
                std::string frame = ((CCString*)pObj)->m_sString;
                vAniFrames.push_back(frame);
            }
            
            //生成动画
            CCArray* aniFrames =CCArray::createWithCapacity(vAniFrames.size());
            //第一个元素为动画播放延时
            delay = atof(vAniFrames[0].c_str());
            for (int j = 1; j < vAniFrames.size(); j++) {
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vAniFrames[j].c_str());
                aniFrames->addObject(frame);
            }
            CCAnimation* ani = CCAnimation::create(aniFrames, delay);
//            aniFrames->release();
            
            CCAnimationCache::sharedAnimationCache()->addAnimation(ani, key->m_sString.c_str());
            
        }
        
    }
    //载入动画文件，生成动画
    CCAnimation* createAniWithFile(const char* file, const char* aniName, float delay) {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file);
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        CCArray* vAniName = data->allKeys();
        vector<string> vAniFrames;
        for (int i = 0; i < vAniName->count(); i++) {
            CCString* key = (CCString*)vAniName->objectAtIndex(i);
            if (key->m_sString == string(aniName)) {
                //读取帧数组
                CCArray* sAinFrames = (CCArray*)(data->objectForKey(key->m_sString));
                CCObject* pObj = NULL;
                CCARRAY_FOREACH(sAinFrames, pObj)
                {
                    std::string frame = ((CCString*)pObj)->m_sString;
                    vAniFrames.push_back(frame); 
                }
                
                //生成动画
                CCArray* aniFrames = CCArray::createWithCapacity(vAniFrames.size());
                for (int j = 0; j < vAniFrames.size(); j++) {
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vAniFrames[j].c_str());
                    aniFrames->addObject(frame);
                }
                CCAnimation* ani = CCAnimation::create(aniFrames, delay);
//                aniFrames->release();
                
                return ani;
            }
        }
        return NULL;
    }
    
    //读取动作文件(runAction and actions)
    void readSpriteActionFile(const string& file, CCSprite* sprite,const string& key) {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file.c_str());
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        if(key == "") {
            PtMapUtility::setSpriteDatInfo(sprite, data);
        }
        else {
            CCDictionary* value = (CCDictionary*) data->objectForKey(key.c_str());
            PtMapUtility::setSpriteDatInfo(sprite, value);
        }
        
    }
    
}

//标记动作结束
void ActionCallFun::actionOver() {
	isActionOver = true;
}
//删除本身
void ActionCallFun::removeSelf(CCNode* node, void* data) {
	bool cleanup = (void*) data;
	node->removeFromParentAndCleanup(cleanup);
}
//重设置zorder
void  ActionCallFun::reZorder(cocos2d::CCNode *node, void *data)
{
    int zorder=*(int *)data;
    if(node->getParent())
    {
        node->getParent()->reorderChild(node, zorder);
    }
}
//按钮执行动作结束
void ActionCallFun::btnActOver()
{
    isButtonActing = false;
}