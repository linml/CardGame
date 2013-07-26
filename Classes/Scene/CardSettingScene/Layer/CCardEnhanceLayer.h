//
//  CCardEnhanceLayer.h
//  91.cube
//
//  Created by phileas on 13-7-23.
//
//

#ifndef ___1_cube__CCardEnhanceLayer__
#define ___1_cube__CCardEnhanceLayer__

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "CGamesCard.h"

#include <vector>

using namespace cocos2d;
using namespace std;

#define MATERIALCARDCOUNT 8
class CCardEnhanceLayer : public CCLayer
{
public:
    CREATE_FUNC(CCardEnhanceLayer);
    
public:
    CCardEnhanceLayer();
    virtual ~CCardEnhanceLayer ();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    
    int getEmptyCardSlide();
    void addCard(const int& inIndex, CGamesCard * inCard);
    void removeCard(const int &inIndex);
protected:
    void initCCardEvolution();
    void handlerTouch(CCTouch * pTouch);
    
    // action:
    void addCardAction(const int& inIndex, CGamesCard * inCard);
    void removeCardAction(const int& inIndex);
    
    void updateEnhance();
    void updateData();
    void updateTexture();
    
protected:
    int m_nTouchTag;
    
    CPtLevelConfigData *m_pLevelConfig;
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    
    CGamesCard * m_pSelectCard;
    CGamesCard* m_pMaterialCards[MATERIALCARDCOUNT];
    
    int m_nAddAtak;
    int m_nAddDef;
    int m_nAddHp;
    int m_nAddRvc;
    
    int m_nCurrentAtk;
    int m_nCurrentDef;
    int m_nCurrentHp;
    int m_nCurrentRv;
    int m_nCurrentExp;
    
    int m_nCurrentLevel;
    int m_nNeedMoney;
};




#endif /* defined(___1_cube__CCardEvolutionLayer__) */
