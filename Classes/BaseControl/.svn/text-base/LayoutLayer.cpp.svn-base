//
//  LayoutLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-6.
//
//

#include "LayoutLayer.h"
#include "PtMapUtility.h"
#include "string.h"

LayoutLayer::LayoutLayer()
{
}

LayoutLayer::~LayoutLayer()
{
    
}

LayoutLayer* LayoutLayer::create()
{
    LayoutLayer* layoutLayer = new LayoutLayer();
    layoutLayer->autorelease();
    return layoutLayer;
}


bool LayoutLayer::initWithFile(CCNode *inParent, const char *pFileName)
{
    m_pParent = inParent;
    memset(m_pFileName, 0, sizeof(char)*300);
    sprintf(m_pFileName, "%s", pFileName);
   
    bool bRet = false;
    do {
        CC_BREAK_IF(!inParent);
        CC_BREAK_IF(!pFileName);
        PtMapUtility::addChildFromScript(inParent, pFileName);
        bRet = true;
    } while (0);
    
    return bRet;
}

CCNode* LayoutLayer::getElementByTags(const int *inArray, const int len)
{
    
    CCNode *element = m_pParent;
    
    for(int i =0; i < len; i++)
    {
        if (element == NULL) {
            break;
        }
        element =  element->getChildByTag(inArray[i]);
    }
    return element;
}

CCNode* LayoutLayer::getElementByTags(vector<int> &inTags)
{
    CCNode *element = m_pParent;
    int len = inTags.size();
    
    for(int i =0; i < len; i++)
    {
        if (element == NULL) {
            break;
        }
        element =  element->getChildByTag(inTags[i]);
    }
    return element;
    
}
CCNode* LayoutLayer::getElementByTags(const string &inTags)
{
    vector<string> tmp = GameTools::splitString(inTags.c_str(), ",");
    vector<int> tags;
    for(int i =0; i < tmp.size(); i++)
    {
        tags.push_back(atoi(tmp[i].c_str()));
    }
    return getElementByTags(tags);
}


void LayoutLayer::getTouchRects(vector<TouchRect>& outTouches)
{
    string tmp = m_pFileName;
    PtMapUtility::addTouchRectFromScript(tmp, m_pParent, &outTouches);
}




