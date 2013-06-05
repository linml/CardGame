//
//  DragonClanCardSprite.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "DragonClanCardSprite.h"
CDragonClanCardSprite::CDragonClanCardSprite():CCardSprite()
{
    this->m_cardData.m_eCardKind=EN_CARDKIND_DROGANCLAN;
}

CDragonClanCardSprite::~CDragonClanCardSprite()
{
    
}
string  CDragonClanCardSprite::getName()
{
    return  "dragon";
}