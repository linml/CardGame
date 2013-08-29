//
//  CPtSliderWidget.cpp
//  91.cube
//
//  Created by phileas on 13-8-29.
//
//

#include "CPtSliderWidget.h"

CPtSliderWidget* CPtSliderWidget::create(CCSprite * backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite)
{
    CPtSliderWidget *pRet = new CPtSliderWidget();
    pRet->initWithSprites(backgroundSprite, pogressSprite, thumbSprite);
    pRet->autorelease();
    return pRet;
}



bool CPtSliderWidget::touchBegan(CCTouch* touch, CCEvent* pEvent)
{
   
    return  CCControlSlider::ccTouchBegan(touch, pEvent);
}
bool CPtSliderWidget::touchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
//    {
//        CCControlSlider::ccTouchEnded(pTouch, pEvent);
//        return false;
//    }
    CCControlSlider::ccTouchMoved(pTouch, pEvent);
    return true;
}
void CPtSliderWidget::touchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
  
     CCControlSlider::ccTouchEnded(pTouch, pEvent);

}