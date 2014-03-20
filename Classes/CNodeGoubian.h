//
//  CNodeGoubian.h
//  CrossGap
//
//  Created by linminglu on 14-3-20.
//
//

#ifndef __CrossGap__CNodeGoubian__
#define __CrossGap__CNodeGoubian__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CNodeGoubian : public CCSprite 
{
public:
    CNodeGoubian();
    bool initProgram(const char *pszFileName , const char * pszPosFileName );
	void		setupStencilForDrawingOnPlane(GLint plane);
	void		setupStencilForClippingOnPlane(GLint plane);
	CCPoint		blur_;
	GLfloat		sub_[4];
	GLuint		m_blurPos;
	GLuint		m_blurLocation;
	GLuint		m_subLocation;
	GLuint      m_attributePosition;
    void setSprite(CCSprite *sprite);
protected:
	CCNode*		m_pFatherNode;
	CCSprite*	m_ChildSprite;
	CCGLProgram* m_pProgram;
    
};

#endif /* defined(__CrossGap__CNodeGoubian__) */
