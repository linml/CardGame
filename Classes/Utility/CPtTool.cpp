//
//  CPtTool.cpp
//  DemoTest
//
//  Created by phileas on 13-7-15.
//
//
#include <vector>
#include "CPtTool.h"
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
                            CCLog("right");
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
                            CCLog("left");
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
        CCPoint point = moveNode->getPosition();
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
        if (node)
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
        int sequence[5]= {-1};
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
        
       
        if ((sum == len*(sequence[0]+sequence[len-1])/2) && ((sequence[1] - sequence[0]) == 1) && len == 5)
        {
            /* code */
            return 5;
        }
        
        int count[2]={1,1};
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
            }
        }
        if (sequence[0]==sequence[len-1])
        {
            count[0]++;
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
                }else
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
     * @return : 向上取整
     */
    int calulate(int card_base_value, int config_value, int star, int correct_value_1, int star_parameter, int correct_value_2,int correct_parameter)
    {
        return (int)(((card_base_value+ config_value*(star+correct_value_1)*star_parameter+ correct_value_2)*correct_parameter)+1.5)-1;
    };
    
    int calSupportValue(int base_value,int currentValue, float ractor)
    {
        return (int)(((base_value+ currentValue*ractor))+1.5)-1;
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
   
};

