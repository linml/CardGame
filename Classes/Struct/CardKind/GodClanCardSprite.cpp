    //
//  GodClanCardSprite.cpp
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#include "GodClanCardSprite.h"
#include "Utility.h"
#include "gameConfig.h"
#include "gameTools.h"
#include "WordCache.h"

CGodClanCardSprite::CGodClanCardSprite():CCardSprite()
{
    this->m_cardData.m_eCardKind=EN_CARDKIND_GODCLAN;
}

CGodClanCardSprite::~CGodClanCardSprite()
{
    
}
string CGodClanCardSprite::getName()
{
    CCDictionary* data = WordCache::sharedWordCache()->WordByName(string(wordDirPath+"word.plist").c_str());
    
    return string(GameTools::valueForKey("GodClan", data));
}