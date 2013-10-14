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
#define URL_FACTORY(ADR,UID) (string(STR_URL_ADRESS)+string(ADR)+string(SinglePlayer::instance()->getUserId())).c_str()

//登录
#define STR_URL_LOGIN               "http://passport.games.com/gateway.php?m=Platform&a=login"//STR_URL_ADRESS"gateway.php?m=Platform&a=login"
//注册
#define STR_URL_REGISTER            "http://passport.games.com/gateway.php?m=Platform&a=register"//STR_URL_ADRESS"gateway.php?m=Platform&a=register"
//服务器列表
#define STR_URL_SERVER_LIST(UID)    URL_FACTORY("gateway.php?m=Platform&a=selectServer",UID)
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
#define STR_URL_BAG_PROP(UID)       URL_FACTORY("api.php?m=Item&a=getItem&uid=",UID)
#define STR_URL_ADD_GRID(UID)       URL_FACTORY("api.php?m=bag&a=unlock&uid=",UID)


//删除和使用道具
#define STR_URL_DELETE_PROP(UID)    URL_FACTORY("api.php?m=Item&a=deleteItem&uid=",UID)
#define STR_URL_USE_GRID(UID)       URL_FACTORY("api.php?m=Item&a=useItem&uid=",UID)
//临时获取已开启的格子数:
#define STR_URL_OPENGRID_NUM(UID)  URL_FACTORY("api.php?m=bag&a=maxbag&uid=",UID)



//获取邮件
#define STR_URL_EMAIL(UID)          URL_FACTORY("api.php?m=Email&a=getEmail&uid=",UID) 
//修改邮件未读或者已读表示
#define STR_URL_EMAILREADSTATUS(UID) URL_FACTORY("api.php?m=Inbox&a=updateInboxStatus&uid=",UID)

#define STR_URL_EMAILGETITEMS(UID)  URL_FACTORY("api.php?m=Inbox&a=getItems&uid=",UID)


//进入迷宫（既选中小节后进入）
#define STR_URL_GOSECTION(UID)      URL_FACTORY("api.php?m=Part&a=getPartEvent&uid=",UID) 

//保存进度:
#define STR_URL_SAVE_PROGRESS(UID)   URL_FACTORY("api.php?m=Part&a=recordPart&uid=",UID) 

//获取具体事件
#define STR_URL_NEXT_EVENT(UID)      URL_FACTORY("api.php?m=Part&a=startEvent&uid=",UID) 

//结束事件：
#define STR_URL_FINISH_EVENT(UID)   URL_FACTORY("api.php?m=Part&a=finishEvent&uid=",UID)
//系统初始化

#define STR_URL_GAMEINIT(UID)       URL_FACTORY("api.php?m=GameBegin&a=init&uid=",UID)
//完成任务求验证的HTTP
#define STR_URL_COMPLATETASK(UID)   URL_FACTORY("api.php?m=Task&a=completeTask&uid=",UID)
//本地读取任务后需要告诉服务端当前本地接的任务
#define STR_URL_ADDTASK(UID)        URL_FACTORY("api.php?m=Task&a=addTask&uid=",UID)

//定时更新数据请求
#define STR_URL_UPDATEAPHP(UID)    URL_FACTORY("api.php?m=GameBegin&a=cron&uid=",UID)

#endif
