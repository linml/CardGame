//
//  gameCardProperty.cpp
//  91.cube
//
//  Created by linminglu on 13-5-27.
//
//

#include "gameCardProperty.h"
#include "Craftsmanship.h"

#define DELETE_SAFT(POINT) \
{ \
if(POINT) \
{ \
delete POINT;  POINT=NULL ; \
}\
}

CGameCardProperty::~CGameCardProperty()
{
    DELETE_SAFT(m_pAdvocacy);
    DELETE_SAFT(m_pBasicSkill);
    DELETE_SAFT(m_pDeadSkill);
    DELETE_SAFT(m_pIncreaseSkill);
}