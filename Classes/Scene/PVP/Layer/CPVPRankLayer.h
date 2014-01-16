//
//  CPVPRankLayer.h
//  91.cube
//
//  Created by phileas on 14-1-14.
//
//

#ifndef ___1_cube__CPVPRankLayer__
#define ___1_cube__CPVPRankLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "PVPDataManager.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define PVPRANKQUITBTN_TAG 20

class CPVPRankLayer : public CCLayer, public CCTableViewDelegate
{
public:
    CREATE_FUNC(CPVPRankLayer);
    CPVPRankLayer();
    virtual ~CPVPRankLayer();
    virtual bool init();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view){};
    void scrollViewDidZoom(CCScrollView* view){};
    
protected:
    void initCPVPRankLayer();
    CCNode *createRankItem(const PVPRankData * inData);
    CCNode *createPVPRankList(const PVPRankData *inDataes, int inSize);
    
    void callBack();
    
    void onClickQuit();
    
    
protected:
    int m_nTouchTag;
    CCNode *m_pQuitBtn;
    CCNode *m_pBg;
    PVPRankData *m_pAllRankData;
    int m_nRankCount;
};
#endif /* defined(___1_cube__CPVPRankLayer__) */
