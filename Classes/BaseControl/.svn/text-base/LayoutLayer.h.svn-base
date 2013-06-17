//
//  LayoutLayer.h
//  91.cube
//
//  Created by phileas on 13-6-6.
//
//

#ifndef ___1_cube__LayoutLayer__
#define ___1_cube__LayoutLayer__

#include "cocos2d.h"
#include "Utility.h"

USING_NS_CC;

class LayoutLayer : public CCObject {
    
public:
    static LayoutLayer *create();
    
public:
    virtual bool initWithFile(CCNode* inParent, const char * pFileName);
    virtual ~LayoutLayer();
    
public:
    
    LayoutLayer();
    CCNode* getElementByTags(const int *inArray, const int len);
    CCNode* getElementByTags(vector<int> &inTags);
    CCNode* getElementByTags(const string &inTags);
    
    void getTouchRects(vector<TouchRect> &outTouches);
    
protected:
    CCNode *m_pParent;
    char m_pFileName[300];
};


#endif /* defined(___1_cube__LayoutLayer__) */
