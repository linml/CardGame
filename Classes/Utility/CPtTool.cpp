//
//  CPtTool.cpp
//  DemoTest
//
//  Created by phileas on 13-7-15.
//
//

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
            CCNode * tmp = dynamic_cast<CCNode *>((CCObject *)array->objectAtIndex(i));
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
    
    //CCNode *whichNode(CCNode* array[], CCNode *parent ,CCTouch *pTouch, int inCount)
    //{
    //    CCPoint  point = parent->convertTouchToNodeSpace(pTouch);
    //    for(int i =  inCount -1 ; i >= 0; i--)
    //    {
    //        CCNode * tmp = dynamic_cast<CCNode *>(array[i]);
    //        if (tmp)
    //        {
    //            if (tmp->boundingBox().containsPoint(point))
    //            {
    //                return tmp;
    //            }
    //        }
    //    }
    //    return NULL;
    //
    //}
    
    int whichNode(CCNode* array[], CCNode *parent ,CCTouch *pTouch, int inCount)
    {
        CCPoint  point = parent->convertTouchToNodeSpace(pTouch);
        for(int i =  inCount -1 ; i >= 0; i--)
        {
            CCNode * tmp = dynamic_cast<CCNode *>(array[i]);
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

};

