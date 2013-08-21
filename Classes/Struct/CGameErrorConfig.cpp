//
//  CGameErrorConfig.cpp
//  91.cube
//
//  Created by linminglu on 13-8-15.
//
//

#include "CGameErrorConfig.h"

#include "gameConfig.h"


CGameErrorConfig::CGameErrorConfig()
{
    gameErrorMsg.erase(gameErrorMsg.begin(),gameErrorMsg.end());
}

CGameErrorConfig::~CGameErrorConfig()
{
    gameErrorMsg.erase(gameErrorMsg.begin(),gameErrorMsg.end());
}
string CGameErrorConfig::loadErrorMsgByErrorID(int errorId)
{
    string str=gameErrorMsg[errorId];
    if(str.empty())
    {
        return "错误信息没有定义";
    }
    else
    {
        return str;
    }
}

void CGameErrorConfig::init()
{
   gameErrorMsg[ERROR_MSG_CONNECTSERVERERROR]=string("服务端链接失败");
   gameErrorMsg[10001]=string("服务端请求失败");
}