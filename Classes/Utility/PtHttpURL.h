//
//  PtHttpURL.h
//  91.cube
//
//  Created by xianbei1987 on 13-7-25.
//
//

#ifndef _1_cube_PtHttpURL_h
#define _1_cube_PtHttpURL_h


//服务器地址
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if defined CUBE_91
#define STR_URL_ADRESS "http://cube.games.com/"
#endif
#endif

#define URL_FACTORY(ADR,UID) (string(STR_URL_ADRESS)+string(ADR)+ConvertToString(UID)).c_str()

//登录
#define STR_URL_LOGIN               STR_URL_ADRESS"gateway.php?m=Platform&a=login"
//注册
#define STR_URL_REGISTER            STR_URL_ADRESS"gateway.php?m=Platform&a=register"
//服务器列表
#define STR_URL_SERVER_LIST(UID)    URL_FACTORY("gateway.php?m=Platform&a=selectServer",UID)
//登录服务器
#define STR_URL_SERVER_LOGIN(UID)   URL_FACTORY("api.php?m=GameBegin&a=init",UID)
//公告
#define STR_URL_NOTICE(UID)         URL_FACTORY("api.php?m=Resource&a=notice",UID)

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

//战斗前选择阵容
#define STR_URL_CHOOSE_TEAM(UID)    URL_FACTORY("api.php?m=Fight&a=getTeamInfo&uid=",UID)
//战斗结束校验
#define STR_URL_FIGHT(UID)          URL_FACTORY("api.php?m=Fight&a=fight&uid=",UID)


#endif
