//
//  SEveryATKData.cpp
//  91.cube
//
//  Created by linminglu on 13-7-30.
//
//

#include "SEveryATKData.h"

//#define AAAAFOROSMACHINE

SEveryATKData::SEveryATKData()
{
    for(int i=0;i<2;i++)
    {
        data[i]=NULL;
    }
}
SEveryATKData::~SEveryATKData()
{
    for(int i=0;i<2;i++)
    {
        if(data[i])
        {
            delete data[i];
            data[i]=NULL;
        }
    }
}