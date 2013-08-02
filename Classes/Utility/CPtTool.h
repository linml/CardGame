//
//  CPtTool.h
//  91.cube
//
//  Created by phileas on 13-7-14.
//
//

#ifndef ___1_cube__CPtTool__
#define ___1_cube__CPtTool__

#include "cocos2d.h"
using namespace cocos2d;


namespace CPtTool
{
    void drag(CCNode* node,CCTouch *pTouch);
    CCNode *whichNode(CCArray *array, CCNode *parent ,CCTouch *pTouch);
    int whichNode(CCNode* array[], CCNode *parent ,CCTouch *pTouch, int inCount);
    int insertWhichNode(CCNode* array[], CCNode *parent, int inCount);
    bool isNeedMove(CCNode *moveNode, CCNode * aroundNode, CCNode *parent,int inType );
    bool isNeedMove(CCNode *moveNode, CCNode * aroundNode, int inType );
    void moveHalfStep(CCNode *node,int inDirection);
    void move(CCNode *array[] , int inDirection, int index, int inCount);
    void  insertMoveNode(CCNode * array[], CCNode *parent, CCNode *inCard, int inCount);
    bool isInNode(CCNode *node, CCPoint & point);
    bool isInNode(CCNode *node,CCTouch *pTouch);
    
    bool sameColor(const int suit[],  const int len);
    int getSuit(const int sequence[], const int len);
    int getSuitResult(const int suit[], const int sequence[], const int len);
    
    int calulate(int card_base_value, int config_value, int star, int correct_value_1, int star_parameter, int correct_value_2,int correct_parameter);
    int calSupportValue(int base_value, int currentValue, float ractor);

};


#endif
