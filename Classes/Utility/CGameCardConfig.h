//
//  CGameCardConfig.h
//  91.cube
//
//  Created by phileas on 14-1-2.
//
//

#ifndef _1_cube_CGameCardConfig_h
#define _1_cube_CGameCardConfig_h
#include<string>

extern const std::string g_mapImagesPath;

const std::string g_cardImagePath = g_mapImagesPath + "card/";          //存放卡牌资源目录
const std::string g_cardHeadPath = g_cardImagePath + "head/";           //存放卡牌头像资源目录
const std::string g_cardResourcePath = g_cardImagePath + "resource/";   //存放小卡人物资源目录
const std::string g_cardRolePath = g_cardImagePath + "role/";           //存放大卡人物资源目录

#endif
