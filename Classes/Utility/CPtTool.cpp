//
//  CPtTool.cpp
//  DemoTest
//
//  Created by phileas on 13-7-15.
//
//
#include <vector>
#include "CPtTool.h"
#include <fstream>
namespace CPtTool
{
    void drag(CCNode* node,CCTouch *pTouch)
    {
        if (node)
        {
            CCPoint delta = pTouch->getDelta();
            CCPoint curPos = node->getPosition();
            curPos= ccpAdd( curPos, delta );
            node->setPosition( curPos );
        }
        
    }
    
    CCNode *whichNode(CCArray *array, CCNode *parent ,CCTouch *pTouch)
    {
        CCPoint  point = parent->convertTouchToNodeSpace(pTouch);
        for(int i =  array->count() -1 ; i >= 0; i--)
        {
            CCNode * tmp = (CCNode *)((CCObject *)array->objectAtIndex(i));
            if (tmp)
            {
                if (tmp->boundingBox().containsPoint(point))
                {
                    return tmp;
                }
            }
        }
        return NULL;
    }
    

    
    int whichNode(CCNode* array[], CCNode *parent ,CCTouch *pTouch, int inCount)
    {
        CCPoint  point = parent->convertTouchToNodeSpace(pTouch);
        for(int i =  inCount -1 ; i >= 0; i--)
        {
            CCNode * tmp = (CCNode *)(array[i]);
            if (tmp)
            {
                if (tmp->boundingBox().containsPoint(point))
                {
                    return i;
                }
            }
        }
        return -1;
        
    }
    
    int insertWhichNode(CCNode* array[], CCNode *parent ,int inCount)
    {
        for (int i = 0; i < inCount; i++)
        {
            if (array[i] == NULL)
            {
                return  i;
            }
        }
        
        return -1;
        
    }
    
    
    void  insertMoveNode(CCNode * array[], CCNode *parent, CCNode *inCard, int inCount)
    {
        
        CCRect rect = inCard->boundingBox();
        
        rect.origin = parent->convertToNodeSpace(rect.origin);
        CCLog("the moveCard: %f , %f", rect.origin.x, rect.origin.y );
        
        int index = 0;
        for (int i = inCount - 1 ;  i >= 0; i--)
        {
            if (array[i] != NULL)
            {
                CCRect tmp = array[i]->boundingBox();
                CCLog("the %d card: %f , %f", i, tmp.origin.x, tmp.origin.y);
                
                if (tmp.intersectsRect(rect))
                {
                    CCLog("insert :%d", i);
                    index++;
                    
                    if (index == 1)
                    {
                        if (isNeedMove(inCard, array[i],  parent, 2))
                        {
                            // move(array, 1, i, 4);
                            if (i-1 >= 0 && array[i-1] == NULL)
                            {
                                move(array, 2, i, 4);
                            }else
                            {
                                move(array, 1, i, 4);
                            }
                            
                        }
                    }
                    else if(index == 2)
                    {
                        
                        if (isNeedMove(inCard, array[i], parent,1))
                        {
                            move(array, 2, i, 4);
                        }
                        
                    }
                }
            }
        }
        
        
        
    }
    
    /*
     * @param inType 1: right, 2: left
     */
    bool isNeedMove(CCNode *moveNode, CCNode * aroundNode, CCNode *parent, int inType )
    {
        CCPoint point = parent->convertToNodeSpace(moveNode->getPosition());
        bool flag = false;
        if (inType == 1 && aroundNode && (point.x-aroundNode->getPositionX()) < 30.0f && (point.x-aroundNode->getPositionX()) >15.0f)
        {
            
            flag = true;
            
        }else if ( inType == 2 && aroundNode && (point.x-  aroundNode->getPositionX()) > -30.0f && (point.x - aroundNode->getPositionX()) < -
                  15.0f)
        {
            flag = true;
        }
        else if(inType == 4 && aroundNode && (point.x-aroundNode->getPositionX()) < 30.0f && (point.x-aroundNode->getPositionX()) >15.0f)
        {
            return true;
            
        }else if ( inType == 3 && aroundNode && (point.x-  aroundNode->getPositionX()) > -30.0f && (point.x - aroundNode->getPositionX()) < -
                  15.0f)
        {
            flag = true;
        }
        
        
        CCLog("the flag: %d", flag);
        
        return flag;
    }
    
    bool isNeedMove(CCNode *moveNode, CCNode * aroundNode, int inType )
    {
        if (moveNode == NULL || aroundNode == NULL)
        {
            return false;
        }
        CCPoint point = moveNode->getPosition();
        point = moveNode->getParent()->convertToWorldSpace(point);
        point = aroundNode->getParent()->convertToNodeSpace(point);
        bool flag = false;
        if (inType == 1 && aroundNode && (point.x-aroundNode->getPositionX()) < 30.0f && (point.x-aroundNode->getPositionX()) >15.0f)
        {
            
            flag = true;
            
        }else if ( inType == 2 && aroundNode && (point.x-  aroundNode->getPositionX()) > -30.0f && (point.x - aroundNode->getPositionX()) < -
                  15.0f)
        {
            flag = true;
        }
        CCLog("the flag: %d", flag);
        
        return flag;
    }
    
    
    /*
     * @param inType 1: right, 2: left
     */
    void moveHalfStep(CCNode *node,int inDirection)
    {
        if (node == NULL)
        {
            return;
        }
        if (inDirection == 2)
        {
            node->setPosition(ccp(node->getPositionX()- node->getContentSize().width/2, node->getPositionY()));
        }else if(inDirection == 1)
        {
            
            node->setPosition(ccp(node->getPositionX()+ node->getContentSize().width/2, node->getPositionY()));
        }
        
    }
    
    
    void move(CCNode *array[] , int inDirection, int index, int inCount)
    {
        if (inDirection == 1)
        {
            if (index +1 < inCount )
            {
                if (array[index+1] == NULL)
                {
                    moveHalfStep(array[index], 1);
                    array[index+1] = array[index];
                    array[index] = NULL;
                }
                
            }
        }
        else if(inDirection == 2)
        {
            if (index -1 >= 0)
            {
                if (array[index-1] == NULL)
                {
                    moveHalfStep(array[index], 2);
                    array[index-1] = array[index];
                    array[index] =NULL;
                }
            }
        }
    }
    
    bool isInNode(CCNode *node, CCPoint & point)
    {
        bool bRet = false;
        if (node)
        {
            CCNode *parent = node->getParent();
            if (parent)
            {
                point = parent->convertToNodeSpace(point);
                if (node->boundingBox().containsPoint(point))
                {
                    bRet = true;
                }
            }
        }
        
        return bRet;
        
    }

    bool isInNode(CCNode *node,CCTouch *pTouch)
    {
        bool bRet = false;
        if (node && node->isVisible())
        {
            CCNode *parent = node->getParent();
            if (parent)
            {
                CCPoint point = pTouch->getLocation();
                point = parent->convertToNodeSpace(point);
                if (node->boundingBox().containsPoint(point))
                {
                    bRet = true;
                }
            }
        }
        
        return bRet;
    }
   bool isInRect(CCNode *node, CCRect inRect, CCTouch *pTouch)
   {
       bool bRet = false;
       if (node && node->isVisible())
       {
           CCNode *parent = node->getParent();
           if (parent)
           {
               CCPoint point = pTouch->getLocation();
               point = parent->convertToNodeSpace(point);
               if (inRect.containsPoint(point))
               {
                   bRet = true;
               }
           }
       }
       
       return bRet;

   }
    
    bool sameColor(const int suit[],  const int len)
    {
        bool bRet = true;
        for (int i = 0; i < len-1; ++i)
        {
            /* code */
            if (suit[i] != suit[i+1])
            {
                /* code */
                bRet = false;
                break;
            }
        }
        return bRet;
    }
    
    int getSuit(const int inSequence[], const int len)
    {
        
        int tmpIndex;
        int sum = 0;
        int suit = 0;
        int sequence[5]= {-1,-1,-1,-1,-1};
        if (len <= 1)
        {
            return suit;
        }
        
        for (int i = 0; i < len; i++)
        {
            sequence[i] = inSequence[i];
            sum += sequence[i];
        }
        
       

        for (int i = 0; i < len; ++i)
        {
            tmpIndex = i;
            for (int j = i+1; j  < len; ++j)
            {
                if (sequence[j] < sequence[tmpIndex])
                {
                    /* code */
                    tmpIndex = j;
                }
            }
            int tmp = sequence[i];
            sequence[i] = sequence[tmpIndex];
            
            sequence[tmpIndex] = tmp;
        }
        
       
        if ((len == 5) &&((sequence[1]- sequence[0])==1) && ((sequence[len-1] - sequence[0]) == 4)&&((sequence[len-1]-sequence[len-2])==1))
        {
            /* code */
            return 5;
        }
        
        std::vector<int> v;
        int count[]={1,1};
        int index = 0;
        for (int i = 1; i < len; i++)
        {
            if (sequence[i-1] == sequence[i])
            {
                count[index]++;
                
            }else
            {
                if (count[index] != 1)
                {
                    index++;
                }
                if (sequence[i]==-1)
                {
                    break;
                }
            }
        }
       
       
        
        int allcount = count[0] + count[1];
        switch (allcount)
        {
            case 3:
                //对子
                suit = 1;
                break;
            case 4:
                if (count[0] == 2)
                {
                    suit = 2;
                    // 两对
                }else
                {
                    suit = 3;
                    // 三条
                }
                break;
            case 5:
                if (count[0]==2 || count[1]==2)
                {
                    suit = 7;
                    //葫芦
                }
                else
                {
                    suit =6;
                    //四条
                }
                break;
            case 6:
                // 五条
                suit = 9;
                break;
            default:
                suit = 0;
                break;
        }
        
        return suit;
    }

    int getSuitResult(const int suit[], const int sequence[], const int len)
    {
        if (len == 1)
        {
            return 0;
        }
        else
        {
            int tmp = getSuit(sequence, len);
            if (len == 5)
            {
                bool flag = sameColor(suit, len);
                if (flag)
                {
                    if (tmp == 5)
                    {
                        return 8;
                    }else if(tmp == 9 || tmp == 7 || tmp == 6)
                    {
                        return  tmp;
                    }else
                    {
                        return 4;
                    }
                }

            }
            return tmp;

           
        }
    }
    
    
    /*
     * @return : 向下取整
     */
    int calulate(int card_base_value, int config_value, int star, float correct_value_1, float star_parameter, float correct_value_2,float correct_parameter)
    {
        int result = (int)(((card_base_value+ config_value*(star+correct_value_1)*star_parameter+ correct_value_2)*correct_parameter));
        CCLog("result: %d", result);
        return result;
    };
    
    int calSupportValue(int base_value,int currentValue, float starParam, float ractor)
    {
        return (int)((base_value+ currentValue*ractor*starParam));
    }
    bool getSuit(const int &number, int &outSuit, int &outSequence)
    {
        int inSuit = number;
        if (inSuit <= 0 || inSuit > 52)
        {
            CCLog("suit: error");
            return false;
        }
        
        
        outSuit = inSuit/13 + (inSuit%13 == 0 ? 0 : 1);
        outSequence = inSuit- (13* (outSuit-1));
        return  true;
        
    }

    bool memoryInfo(vm_statistics_data_t *vmStats)
    {
            mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
            kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)vmStats, &infoCount);
            
            return kernReturn == KERN_SUCCESS;
    }
    void logMemoryInfo() {
        vm_statistics_data_t vmStats;
        
        if (memoryInfo(&vmStats)) {
            CCLog("free: %u\nactive: %u\ninactive: %u\nwire: %u\nzero fill: %u\nreactivations: %u\npageins: %u\npageouts: %u\nfaults: %u\ncow_faults: %u\nlookups: %u\nhits: %u",
                  vmStats.free_count * vm_page_size,
                  vmStats.active_count * vm_page_size,
                  vmStats.inactive_count * vm_page_size,
                  vmStats.wire_count * vm_page_size,
                  vmStats.zero_fill_count * vm_page_size,
                  vmStats.reactivations * vm_page_size,
                  vmStats.pageins * vm_page_size,
                  vmStats.pageouts * vm_page_size,
                  vmStats.faults,
                  vmStats.cow_faults,
                  vmStats.lookups,
                  vmStats.hits
                  );
        }
    }
    
    
    time_t strtotime(char* const date,std::string format)
    {
        struct tm tm;
        strptime(date,format.c_str(), &tm) ;
        time_t ft=mktime(&tm);
        return ft;
    }

    std::string   timetodate(time_t const timer)
    {
        struct tm *l=localtime(&timer);
        
        char buf[128];
        snprintf(buf,sizeof(buf),"%04d-%02d-%02d %02d:%02d:%02d",l->tm_year+1900,l->tm_mon+1,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
            std::string s(buf);
        return s;
        
        
    }
    void getDataShijianChai(time_t  beforetime,time_t now,std::string &str)
    {
        CCLog("beforetime::%s",timetodate(beforetime).c_str());
        CCLog("now::%s",timetodate(now).c_str());
        double interval=difftime(now, beforetime);
         CCLog(" intervale=%f",interval);
        long day=(long)interval/(24*3600);//天
        long hour=(long)interval%(24*3600)/3600;//小时
        long minute=(long)interval%3600/60;//分钟
        //long second=(long)interval%60;//秒
        std::stringstream ss;
        if(day>0)
        {
            ss<<day<<"天前";
        }
        else if (hour>0)
        {
            ss<<hour<<"时前";
        }
        else if (minute)
        {
            ss<<minute<<"分前";
        }
        
        ss>>str;
        
    }
    
    double getDateIntermissionRebackMSC(time_t beforeTime,time_t afterTime)
    {
        return difftime(afterTime,beforeTime);
    }
    std::string  readFileName(const char *filename)
    {
        std::string filpat=CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filename);
        std::ifstream out;
        out.open(filpat.c_str(), std::ios::in);
        static std::string result;
        result.clear();
        std::string line;
        if(!out)
        {
            std::cout<<"aaaa";
        }
        while(!out.eof())
        {
            std::getline(out,line);
            result+=line;
        }
        out.close();
        return result;
    }
    
    
    const char * stringForObjectValue(int seconds)
    {
        static char timerFormat[10];
        
        if (seconds<=0) {
            sprintf(timerFormat, "00:00:00");
        }
        unsigned int tm_hour = seconds/3600;
        unsigned int tm_minute = seconds%3600/60;
        unsigned int tm_second = seconds%60;
        if(tm_hour < 100){
            sprintf(timerFormat, "%02d:%02d:%02d", tm_hour, tm_minute, tm_second);
        }
        else {
            sprintf(timerFormat, "99:99:99");
        }
        
        return timerFormat;
    }
    
    CCScrollView *getScrollWord(const char *inText, CCSize viewSize, ccColor3B wordColor, const char *fontName, int fontSize)
    {
        CCScrollView* scrollView  = CCScrollView::create(viewSize);
        scrollView->setTouchEnabled(true);
        scrollView->setDirection( kCCScrollViewDirectionVertical);
        CCLabelTTF* pLabel = CCLabelTTF::create("", fontName, fontSize);
        pLabel->cocos2d::CCNode::setAnchorPoint(CCPointZero);
        pLabel->setColor(wordColor);
        pLabel->setDimensions(CCSizeMake(viewSize.width, 0));
        pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        pLabel->setString(inText);
        scrollView->setContentSize(pLabel->getContentSize());
        CCSize pSize = pLabel->getContentSize();
        if (viewSize.height > pSize.height)
        {
            scrollView->setTouchEnabled(false);
        }
        CCLog("pSize: %f,%f", pSize.width, pSize.height);
        
        scrollView->addChild(pLabel);
        scrollView->setAnchorPoint(ccp(0,1));
        scrollView->setContentOffset(ccp(0, scrollView->getViewSize().height-pSize.height));
        return scrollView;
    }
    
   int getSellPrice(int baseValue, int levelValue, int starValue)
   {
       return baseValue + (int)(levelValue*42*starValue/5);
   }
    
    int ceil(float inSrc)
    {
        
        int result = inSrc;
        float tmp = (float)result;
        result = tmp == inSrc ? result: result+1;
        return result;
    }

};

