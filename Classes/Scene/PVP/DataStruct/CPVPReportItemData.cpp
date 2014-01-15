//
//  CPVPReportItemData.cpp
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#include "CPVPReportItemData.h"
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

CPVPReportItemData::CPVPReportItemData()
{
    
}

CPVPReportItemData::~CPVPReportItemData()
{
    DELETE_POINT_VECTOR(m_vCardList,vector<CFightCard*>,CFightCard );
}
