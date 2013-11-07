//
//  CFightHeroDataLayerManager.cpp
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#include "CFightHeroDataLayerManager.h"
#include "CFightBufferPngSprite.h"
CFightHeroDataLayerManager::CFightHeroDataLayerManager()
{
    m_nLeftBufferCount=0;
    m_nRightBufferCount=0;
    
}
CFightHeroDataLayerManager::~CFightHeroDataLayerManager()
{
    
}
void CFightHeroDataLayerManager::setCurrentTeamAtkIndex(EN_LEFTTEAMORRIGHTTEAM enTeamPostion, int nLeftAtkIndex,int nRightAtkIndex)
{
    m_enTeamPostion=enTeamPostion;
    m_nLeftAtk=nLeftAtkIndex;
    m_nRightAtk=nRightAtkIndex;
}

void CFightHeroDataLayerManager::initUpdateBuffer(EN_LEFTTEAMORRIGHTTEAM enTeamPostion)
{
   
    if (enTeamPostion ==EN_LEFTTEAM) {
         m_nLeftBufferCount=0;
        for (list<CFightBufferPngValue *>::iterator it=m_listBufferList.begin(); it!=m_listBufferList.end(); it++) {
            if ((*it)->enType==EN_LEFTTEAM && (*it)->nAtkIndex<m_nLeftAtk)
            {
                CFightBufferPngValue *tempdata=(*it);
                removeChildByTag(tempdata->tag,true);
                CC_SAFE_DELETE(tempdata);
                m_listBufferList.erase(it);
            }
            else if((*it)->enType==EN_LEFTTEAM && (*it)->nAtkIndex==m_nLeftAtk)
            {
                CFightBufferPngSprite *spite=CFightBufferPngSprite::Create((*it)->effectid, (*it)->enType, (*it)->nAtkIndex);
                (*it)->tag=(*it)->effectid +1000000;
                addChild(spite,1,(*it)->tag);
                spite->setPosition(getPngFilePostion(EN_LEFTTEAM,m_nLeftBufferCount));
                m_nLeftBufferCount++;
                (*it)->isAddChild=true;

            }
            
        }
        
    }else if(enTeamPostion==EN_RIGHTTEAM)
    {
        m_nRightBufferCount=0;
        for (list<CFightBufferPngValue *>::iterator it=m_listBufferList.begin(); it!=m_listBufferList.end(); it++) {
            if ((*it)->enType==EN_LEFTTEAM && (*it)->nAtkIndex<m_nLeftAtk)
            {
                CFightBufferPngValue *tempdata=(*it);
                removeChildByTag(tempdata->tag,true);
                CC_SAFE_DELETE(tempdata);
                m_listBufferList.erase(it);
            }
            else if((*it)->enType==EN_LEFTTEAM && (*it)->nAtkIndex==m_nLeftAtk)
            {
                CFightBufferPngSprite *spite=CFightBufferPngSprite::Create((*it)->effectid, (*it)->enType, (*it)->nAtkIndex);
                (*it)->tag=(*it)->effectid +1000000;
                addChild(spite,1,(*it)->tag);
                spite->setPosition(getPngFilePostion(EN_LEFTTEAM,m_nLeftBufferCount));
                m_nLeftBufferCount++;
                (*it)->isAddChild=true;
                
            }
            
        }

    }
}


CCPoint CFightHeroDataLayerManager::getPngFilePostion(EN_LEFTTEAMORRIGHTTEAM enTeamPostion,int currPngCount)
{
    if (enTeamPostion==EN_LEFTTEAM) {
        return CCPointMake(100+50*currPngCount, 200);
    }
    else if(enTeamPostion ==EN_RIGHTTEAM)
    {
        return CCPointMake(1000-50*currPngCount, 200);
    }
    return CCPointMake(0, 0);
    
}



void CFightHeroDataLayerManager::appendBuffer(int effectid,int nAtk,EN_LEFTTEAMORRIGHTTEAM enType)
{
    //建立一个数据结构
    CFightBufferPngValue *value=new CFightBufferPngValue ;
    value->effectid=effectid;
    value->nAtkIndex=nAtk;
    value->enType=enType;
    if (enType==EN_LEFTTEAM)
    {
        if (nAtk==m_nLeftAtk)
        {
            //简历 一个addchile
            CFightBufferPngSprite *spite=CFightBufferPngSprite::Create(effectid, enType, nAtk);
            value->tag=effectid+1000000;
            addChild(spite,1,effectid+1000000);
            spite->setPosition(getPngFilePostion(EN_LEFTTEAM,m_nLeftBufferCount));
            m_nLeftBufferCount++;
            value->isAddChild=true;
        }
        else{
            value->isAddChild=false;
        }
    
    }
    else{
        if (nAtk==m_nRightAtk)
        {
            //简历 一个addchile
            CFightBufferPngSprite *spite=CFightBufferPngSprite::Create(effectid, enType, nAtk);
            value->tag=effectid+2000000;
            addChild(spite,1, value->tag);
            spite->setPosition(getPngFilePostion(EN_RIGHTTEAM,m_nRightBufferCount));
            m_nRightBufferCount++;
            value->isAddChild=true;
        }
        else
        {
            value->isAddChild=false;
        }

    }
    this->m_listBufferList.push_back(value);
}

void CFightHeroDataLayerManager::everyHuiheDealWith(int nAtk,EN_LEFTTEAMORRIGHTTEAM enType)
{
    
}