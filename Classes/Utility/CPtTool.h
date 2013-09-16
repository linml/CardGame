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
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
#include <mach/mach.h>

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
    
    int calulate(int card_base_value, int config_value, int star, float correct_value_1, float star_parameter, float correct_value_2,float correct_parameter);
    int calSupportValue(int base_value, int currentValue, float ractor);
    bool getSuit(const int &number, int &outSuit, int &outSequence);
    bool memoryInfo(vm_statistics_data_t *vmStats) ;
    
    void logMemoryInfo() ;
    time_t strtotime(char* const date,  std::string str="%Y%m%d%H%M%S");
    std::string   timetodate(time_t const timer);
    double getDateIntermissionRebackMSC(time_t beforeTime,time_t afterTime);
    void getDataShijianChai(time_t befortime,time_t befortimeE,std::string &str);
    std::string  readFileName(const char *filename);
    
    CCScrollView *getScrollWord(const char *inText, CCSize viewSize, ccColor3B wordColor, const char *fontName, int fontSize);
    
};


#endif
