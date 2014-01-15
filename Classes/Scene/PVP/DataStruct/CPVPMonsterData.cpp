//
//  CPVPMonsterData.cpp
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#include "CPVPMonsterData.h"
#include "gameStruct.h"
#include "CCard.h"
#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}

CPVPMonsterData::CPVPMonsterData()
{
    m_pvpUserData=NULL;
    fuchouID=0;
}

CPVPMonsterData::~CPVPMonsterData()
{
    clearFightingCardData();
    CC_SAFE_DELETE(m_pvpUserData);
    CCLog("~CPVPMonsterData");
    
}
void CPVPMonsterData::clearFightingCardData()
{
    DELETE_POINT_VECTOR(m_vCardList,vector<CFightCard*>,CFightCard );
}