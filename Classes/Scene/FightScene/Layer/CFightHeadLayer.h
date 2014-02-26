//
//  CFightHeadLayer.h
//  91.cube
//
//  Created by linminglu on 14-2-26.
//
//

#ifndef ___1_cube__CFightHeadLayer__
#define ___1_cube__CFightHeadLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CFightCard;
class CGameCardFactory;
class CFightHeadLayer :public CCLayer
{
public:
    CFightHeadLayer();
    ~CFightHeadLayer();
    static CFightHeadLayer *create(CFightCard *card,int totalHp,int totalEngry);
    void setHpValue(int hp);
    void setEngry(int engry);
    void setValue(CFightCard *card,int totalHp,int totalEngry);
    bool init(CFightCard *card,int totalHp,int totalEngry);
private:
    CGameCardFactory *m_tempCardFactory;
};
#endif /* defined(___1_cube__CFightHeadLayer__) */
