//
//  CStructGameActionData.h
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#ifndef ___1_cube__CStructGameActionData__
#define ___1_cube__CStructGameActionData__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CStructGameActionData
{
public:
    CStructGameActionData();
    ~CStructGameActionData();
    
    
public:
    void setDataValue(CCDictionary *dict);
    
    
public:
	/*
	"board_id": "2",
		   "name": "登陆送礼",
		   "show_type": "1",
		   "is_open": "1",
		   "pard_id": "0",
		   "pic": "0",
		   "show_time": "2013年11月1日至2013年11月5日",
		   "item_1": "900004",
		   "item_tips_1": "送你个东西，扩展卡包用的",
		   "item_2": "901033",
		   "item_tips_2": "双倍经验卡，让你飞速升级",
		   "item_3": "901034",
		   "item_tips_3": "双倍金币卡，让你腰缠万贯",
		   "content": "登陆有送礼包好吗"
	*/
    CC_SYNTHESIZE(int,  	m_nBoard_id,    ActionID);
    CC_SYNTHESIZE(string,   m_sName,  ActionName);
    CC_SYNTHESIZE(int , 	m_nShow_type,ActionShowType);
    CC_SYNTHESIZE(int , 	m_nPard_id,ActionPartId); //如果对应的showtype的id 是5的话 那么pardid对应的是副本的ID
    CC_SYNTHESIZE(string ,  m_nPic,ActionPic);
    CC_SYNTHESIZE(string ,  m_sShow_time,ActionShowTime);
    CC_SYNTHESIZE(int , 	m_nItem_1,ActionItemId1);
    CC_SYNTHESIZE(string ,  m_sItem_tips_1,ActionItemTip1);
    CC_SYNTHESIZE(int , 	m_nItem_2,ActionItemId2);
    CC_SYNTHESIZE(string ,  m_sItem_tips_2,ActionItemTip2);
    CC_SYNTHESIZE(int , 	m_nItem_3,ActionItemId3);
    CC_SYNTHESIZE(string ,  m_sItem_tips_3,ActionItemTip3);
    CC_SYNTHESIZE(string ,	m_sContent,Content);
};

#endif /* defined(___1_cube__CStructGameActionData__) */
