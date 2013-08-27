//
//  CGameEmailData.h
//  91.cube
//
//  Created by linminglu on 13-8-26.
//
//

#ifndef ___1_cube__CGameEmailData__
#define ___1_cube__CGameEmailData__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
using namespace  std;
using namespace cocos2d;
//mail_id,mail_type,mail_status,FROM_UID,UID,item_id

/*
 `status` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0 未读 1 已读',
 `item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '奖励包ID',
 `time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
 PRIMARY KEY (`msg_id`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='邮件';
 Jason_Lin(Jason_Lin) 09:55:15
 CREATE TABLE `cube_inbox` (
 `msg_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
 `uid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '接收者ID',
 `from_uid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '发送者ID',
 `msg_type` int(100) unsigned NOT NULL DEFAULT '0' COMMENT '对应TYPE类型的配制ID，TYPE类型为0 则MSG_TYPE取至系统奖励配置表，1 为背包满的表，目前只有一条信息，不建表',
 `type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0 系统奖励 1 背包满格临时放置',
 */
class CGameEmailData
{
public:
    CGameEmailData();
    ~CGameEmailData();
    bool isOutActiveTime();   //!<   判断日期 过期问题
    void decodeJsonData(std::string str);
protected:
    CC_SYNTHESIZE(int ,m_iGameEmailItemid, GameEmailItemid);
    CC_SYNTHESIZE(int ,m_iGameEmailStatus, GameEmailStatus);
    CC_SYNTHESIZE(int ,m_iGameEmailMsgId, GameEmailMsgId);
    CC_SYNTHESIZE(int ,m_iGameEmailUid , GameEmailUid);
    CC_SYNTHESIZE(int ,m_iGameEmailFromUid ,GameEmailFromUid);
    CC_SYNTHESIZE(int ,m_iGameEmailType ,GameEmailType);
    CC_SYNTHESIZE(int ,m_iGameEmailTime , GameEmailTime); //在这个时间之前是有效的
    CC_SYNTHESIZE(std::string, m_sGameEmailTitle, GameEmailTitle);
    CC_SYNTHESIZE(std::string , m_sGameEmailContent, GameEmailContent);
    CC_SYNTHESIZE(int ,m_iGameEmailCoins ,GameEmailCoins); //!< 钱币
    CC_SYNTHESIZE(int ,m_iGameEmailExp ,GameEmailExp);     //!< 经验
public:
    map<int, int>m_mapDataProp;  //
protected:
   
};

#endif /* defined(___1_cube__CGameEmailData__) */
