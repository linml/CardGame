//
//  CGlobalUpdateObject.cpp
//  91.cube
//
//  Created by linminglu on 13-10-11.
//
//

#include "CGlobalUpdateObject.h"
#include "gamePlayer.h"

CGlobalUpdateAp::CGlobalUpdateAp(int iValueTag):CGlobalSchedule(iValueTag)
{
    
}

CGlobalUpdateAp::~CGlobalUpdateAp()
{
    
}

void CGlobalUpdateAp::globalUpdate(float t)
{
    SinglePlayer::instance()->sendUpdateAp();

}

CGlobalUpdateGp::CGlobalUpdateGp(int iValueTag):CGlobalSchedule(iValueTag)
{
    
}

CGlobalUpdateGp::~CGlobalUpdateGp()
{
    
}
void  CGlobalUpdateGp::globalUpdate(float t)
{
   SinglePlayer::instance()->sendUpdateGp();
}