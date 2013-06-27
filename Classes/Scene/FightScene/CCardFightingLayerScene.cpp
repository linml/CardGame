//
//  CCardFightingLayerScene.cpp
//  91.cube
//
//  Created by linminglu on 13-6-27.
//
//

#include "CCardFightingLayerScene.h"
#include "gamePlayer.h"

CCardFightingLayerScene::CCardFightingLayerScene()
{
    
}

CCardFightingLayerScene::~CCardFightingLayerScene()
{
    
}

bool CCardFightingLayerScene::init()
{
    return true;
}

void CCardFightingLayerScene::fSchudelUpdate(float t)
{
    
}

void CCardFightingLayerScene::createFightCard()
{
    for (int i=0; i<SinglePlayer::instance()->m_hashmapFight.size();i++)
    {
        m_vFightingCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapFight[i]));
    }
    ccar4
    
    
}

void CCardFightingLayerScene::createMonsterCard()
{
    for (int i=0; i<SinglePlayer::instance()->m_hashmapMonster.size(); i++) {
        m_vMonsterCard.push_back(new CFightCard(SinglePlayer::instance()->m_hashmapMonster[i]));
    }
}