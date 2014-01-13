//
//  PtHttpURL.h
//  91.cube
//
//  Created by xianbei1987 on 13-7-25.
//
//

#ifndef _1_cube_PtHttpURL_h
#define _1_cube_PtHttpURL_h

#include "gamePlayer.h"
#include "gameConfig.h"

//服务器地址
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if defined CUBE_91
#if defined DEBUG
#define STR_URL_ADRESS "http://cube.games.com/"
#else
#define STR_URL_ADRESS "http://cube.test.games.com/"
#endif
#endif
#endif

#define STR_USER_SIG SinglePlayer::instance()->getUserSig()
#define URL_FACTORY(ADR,UID) (g_serverUrl+"/"+string(ADR)+string(SinglePlayer::instance()->getUserId())).c_str()

//登录
#define STR_URL_LOGIN               "http://passport.games.com/gateway.php?m=Platform&a=login"//STR_URL_ADRESS"gateway.php?m=Platform&a=login"
//注册
#define STR_URL_REGISTER            "http://passport.games.com/gateway.php?m=Platform&a=register"//STR_URL_ADRESS"gateway.php?m=Platform&a=register"
//服务器列表
#define STR_URL_SERVER_LIST         (string("http://passport.games.com/gateway.php?m=Platform&a=selectServer&puid=")+string(SinglePlayer::instance()->getUserId())).c_str()
//注册子服务器
#define STR_URL_REGISTER_SERVER     (string("http://passport.games.com/gateway.php?m=Platform&a=addServerId&puid=")+string(SinglePlayer::instance()->getUserId())).c_str()
//登录服务器
#define STR_URL_SERVER_LOGIN(UID)   URL_FACTORY("api.php?m=GameBegin&a=init",UID)
//公告
#define STR_URL_NOTICE(UID)         URL_FACTORY("api.php?m=Resource&a=notice",UID)

//卡包
#define STR_URL_GET_ITEM(UID)       URL_FACTORY("api.php?m=Card&a=getCardItem&uid=",UID)
//获取阵容
#define STR_URL_GET_TEAM(UID)       URL_FACTORY("api.php?m=Card&a=getCardTeam&uid=",UID)
//更新阵容
#define STR_URL_CLEAR_TEAM(UID)     URL_FACTORY("api.php?m=Card&a=clearCardTeam&uid=",UID)
//保存阵容
#define STR_URL_SAVE_TEAM(UID)      URL_FACTORY("api.php?m=Card&a=saveCardTeam&uid=",UID)
//获取卡包
#define STR_URL_GET_CARBAG(UID)     URL_FACTORY("api.php?m=Card&a=getCardItem&uid=",UID)

//卡牌强化
#define STR_URL_ADVANCE_CARD(UID)   URL_FACTORY("api.php?m=Card&a=cardUpGrade&uid=",UID)
//卡牌进化
#define STR_URL_UPGRADE_CARD(UID)   URL_FACTORY("api.php?m=Card&a=cardUpGrade&uid=",UID)

//卡牌出售

#define STR_URL_SELL_CARD(UID)  URL_FACTORY("api.php?m=CardItem&a=sellCardItems&uid=",UID)



//战斗前选择阵容
//调试的时候需要在api 测试的时候需要添加oliver/CubeSERVER/trunk/web/
#define STR_URL_CHOOSE_TEAM(UID)    URL_FACTORY("api.php?m=Fight&a=getTeamInfo&uid=",UID)

//战斗结束校验
#define STR_URL_FIGHT(UID)          URL_FACTORY("api.php?m=Fight&a=fight&uid=",UID)

//获取道具背包
#define STR_URL_ADD_GRID(UID)       URL_FACTORY("api.php?m=bag&a=unlock&uid=",UID)


//删除和使用道具
#define STR_URL_DELETE_PROP(UID)    URL_FACTORY("api.php?m=Item&a=deleteItem&uid=",UID)
#define STR_URL_USE_GRID(UID)       URL_FACTORY("api.php?m=Item&a=useItem&uid=",UID)


//获取邮件
#define STR_URL_EMAIL(UID)           URL_FACTORY("api.php?m=Email&a=getEmail&uid=",UID)
//修改邮件未读或者已读表示
#define STR_URL_EMAILREADSTATUS(UID) URL_FACTORY("api.php?m=Inbox&a=updateInboxStatus&uid=",UID)

#define STR_URL_EMAILGETITEMS(UID)  URL_FACTORY("api.php?m=Inbox&a=getItems&uid=",UID)

//玩家升级
#define STR_URL_LEVEL_UP(UID)       URL_FACTORY("api.php?m=GameBegin&a=checkLevelUp&uid=",UID)


//进入迷宫（既选中小节后进入）
#define STR_URL_GOSECTION(UID)      URL_FACTORY("api.php?m=Part&a=getPartEvent&uid=",UID) 

//保存进度:
#define STR_URL_SAVE_PROGRESS(UID)  URL_FACTORY("api.php?m=Part&a=recordPart&uid=",UID)

//获取具体事件
#define STR_URL_NEXT_EVENT(UID)     URL_FACTORY("api.php?m=Part&a=startEvent&uid=",UID)

//结束事件：
#define STR_URL_FINISH_EVENT(UID)   URL_FACTORY("api.php?m=Part&a=finishEvent&uid=",UID)
//系统初始化

#define STR_URL_GAMEINIT(UID)       URL_FACTORY("api.php?m=GameBegin&a=init&uid=",UID)
//完成任务求验证的HTTP
#define STR_URL_COMPLATETASK(UID)   URL_FACTORY("api.php?m=Task&a=completeTask&uid=",UID)
//本地读取任务后需要告诉服务端当前本地接的任务
#define STR_URL_ADDTASK(UID)        URL_FACTORY("api.php?m=Task&a=addTask&uid=",UID)

//定时更新数据请求
#define STR_URL_UPDATEAPHP(UID)     URL_FACTORY("api.php?m=GameBegin&a=cron&uid=",UID)

//公告请求
#define STR_URL_GONGGAOQINGQIU(UID)  URL_FACTORY("api.php?m=GameBegin&a=notice&uid=",UID)

//IAP请求
#define STR_URL_IAPBUY(UID)         URL_FACTORY("api.php?m=recharge&a=checkRecharge&uid=",UID)

//获取商城里面商店限量的物品已经购买的数量
#define STR_URL_GETLIMITITEMS(UID)  URL_FACTORY("api.php?m=shop&a=getLimitItems&uid=",UID)

//获取购买物品的信息结果
#define STR_URL_GETSHOPBUY(UID)     URL_FACTORY("api.php?m=shop&a=buy&uid=",UID)

//获取扭蛋的时间
#define STR_URL_GETCARDRANDOM(UID)  URL_FACTORY("api.php?m=Card&a=getCardRandom&uid=",UID)
//获取扭蛋卡牌
#define STR_URL_CARDRANDOM(UID)     URL_FACTORY("api.php?m=Card&a=cardRandom&uid=",UID)
//设置准备大战的阵容
//设置战队
#define STR_URL_SETFIGHTTEAM(UID)   URL_FACTORY("api.php?m=Fight&a=setTeam&uid=",UID)

//去除Buffer
#define STR_URL_CANCELBUFFER(UID)   URL_FACTORY("api.php?m=Item&a=deleteChapterBuff&uid=",UID)
//使用特殊道具
#define STR_URL_USESPEACIALPROP(UID) URL_FACTORY("api.php?m=Item&a=useSpecialItem&uid=",UID)

//获取活动
#define STR_URL_ACTIVITY(UID)       URL_FACTORY("api.php?m=Activity&a=getNotice&uid=",UID)
//图鉴借口
#define STR_URL_ATLAS(UID)          URL_FACTORY("api.php?m=atlas&a=atlas&uid=",UID)

//获得活动中的奖品
#define STR_URL_GETREWARDITEM(UID)  URL_FACTORY("api.php?m=Activity&a=getActivityItem&uid=",UID)

//获得活动副本的信息
#define STR_URL_GETACTIVITY(UID) URL_FACTORY("api.php?m=Part&a=getActivityPart&uid=",UID)
//获得神力的请求
#define STR_URL_HOSPITALACTITY(UID) URL_FACTORY("api.php?m=clinic&a=cure&uid=",UID)
//获得神力的剩余时间
#define STR_URL_GPLEVELTIME(UID)  URL_FACTORY("api.php?m=clinic&a=clinic&uid=",UID)
//获取好友的信息列表
#define STR_URL_GETFRIEND(UID)    URL_FACTORY("api.php?m=Friend&a=getFriendList&uid=",UID)
//添加一个Uid的好友
#define STR_URL_SEARCHFRIEND(UID)  URL_FACTORY("api.php?m=Friend&a=searchFriend&uid=",UID)
//赞美一个好友
#define STR_URL_GETFRIENDLY(UID)   URL_FACTORY("api.php?m=Friend&a=addFriendly&uid=",UID)
//删除一个好友
#define STR_URL_DELFRIEND(UID)    URL_FACTORY("api.php?m=Friend&a=delFriend&uid=",UID)


//获得申请列表
#define STR_URL_GETAPPLYLIST(UID)  URL_FACTORY("api.php?m=Friend&a=getApplyList&uid=",UID)

//获得感恩列表
#define STR_URL_GETPRIASELIST(UID) URL_FACTORY("api.php?m=Friend&a=getThankList&uid=",UID)

//响应好友申请
#define STR_URL_HANDLERAPPLY(UID)  URL_FACTORY("api.php?m=Friend&a=addFriend&uid=",UID)

//对好友进行感恩
#define STR_URL_GANEN(UID)         URL_FACTORY("api.php?m=Friend&a=thankFriend&uid=",UID)

//获取战力排行榜
#define STR_URL_GETFIGHTRANK(UID)  URL_FACTORY("api.php?m=charts&a=fightPow&uid=",UID)

//获取等级排行榜
#define STR_URL_GETLEVELRANK(UID)  URL_FACTORY("api.php?m=charts&a=level&uid=",UID)
//充值排行榜
#define STR_URL_GETCASHRANK(UID)   URL_FACTORY("api.php?m=charts&a=cash&uid=",UID)
//签到的日期
#define STR_URL_QIANDAO(UID)       URL_FACTORY("api.php?m=GameBegin&a=loginReward&uid=",UID)

//获得排名奖励
#define STR_URL_GETRANKREWARD(UID)  URL_FACTORY("api.php?m=charts&a=rewardCharts&uid=",UID)

//获取已经领取的奖励
#define STR_URL_GETYIJINGLINGQU(UID)   URL_FACTORY("api.php?m=Pvp&a=pvpTask&uid=",UID)

//点击领取PVP任务奖励
#define STR_URL_GETLINGQUPVPJIANGLI(UID) URL_FACTORY("api.php?m=Pvp&a=getPvpTaskReward&uid=",UID)


//魂殿
//合成
#define STR_URL_COMPOSEKAHUN(UID) URL_FACTORY("api.php?m=palaceSoul&a=mergeCard&uid=",UID)
//分解
#define STR_URL_DECOMPOSEKAHUN(UID) URL_FACTORY("api.php?m=palaceSoul&a=splitCard&uid=",UID)

//PVP
//PVP排行榜主界面信息请求:
#define STR_URL_GETPVPMAINUIINFO(UID) URL_FACTORY("api.php?m=Pvp&a=Arena&uid=",UID)
//PVP搜索对手
#define STR_URL_GETPVPSEARCHDUISHOU(UID)  URL_FACTORY("api.php?m=Pvp&a=searchPlayer&uid=" ,UID)

//PVP获取战报
#define STR_URL_GETPVPZHANBAO(UID)   URL_FACTORY("api.php?m=Pvp&a=getPvpReport&uid=",UID)

#endif
