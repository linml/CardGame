//
//  gameStruct.h
//  91.cube
//
//  Created by xianbei1987 on 13-5-23.
//
//

#ifndef _1_cube_gameStruct_h
#define _1_cube_gameStruct_h

#include "CPtSectionConfigData.h"
#include <vector>
#include <map>


enum EN_FACETO
{
    EN_FACETO_PZ = 0,
    EN_FACETO_NZ,
    EN_FACETO_NX,
    EN_FACETO_PX,
    EN_FACETO_PY,
    EN_FACETO_NY
};

enum EN_TURNTO
{
    EN_TURNTO_LEFT = 0,
    EN_TURNTO_RIGHT,
    EN_TURNTO_UP,
    EN_TURNTO_DOWN,
    EN_TURNTO_IN
};

enum EN_COORDINATE
{
    EN_COORDINATE_X = 0,
    EN_COORDINATE_Y,
    EN_COORDINATE_Z
};

enum EN_VAL_TURNTO {
    EN_VAL_TURNTO_BASE = 500,
    EN_VAL_TURNTO_PZ = 504,
    EN_VAL_TURNTO_NZ = 496,
    EN_VAL_TURNTO_PX = 501,
    EN_VAL_TURNTO_NX = 499,
    EN_VAL_TURNTO_PY = 502,
    EN_VAL_TURNTO_NY = 498
};


struct STC_HOUSEPOS{
    int x;
    int y;
    int z;
};

typedef STC_HOUSEPOS TP_Orientations;

/*
 * emialId: 邮件ID
 * props: 该邮件所拥有的道具：key：道具ID， value：道具数目
 */

struct EMAIL_DATA
{
    int emailId;
    std::map<int, int> props;
    
};

#define EMPTY_EVENT -1

struct SECTION_DATA
{
    CPtSection *sectionInfo;
    int currentStep;
    int eventId[3]={EMPTY_EVENT,EMPTY_EVENT,EMPTY_EVENT};
};




#endif
