//
//  CNodeGoubian.cpp
//  CrossGap
//
//  Created by linminglu on 14-3-20.
//
//

#include "CNodeGoubian.h"
CNodeGoubian::CNodeGoubian()
{
    setShaderProgram(NULL);
}

void CNodeGoubian::setSprite(cocos2d::CCSprite *sprite)
{
    this->m_ChildSprite=sprite;
    addChild(sprite,0);
}
bool CNodeGoubian::initProgram(const char *pszFileName, const char * pszPosFileName )
{
	//
	m_ChildSprite		= NULL;
	m_pFatherNode		= NULL;
	;
	GLchar * fragSource =(GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pszFileName))->getCString();
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
    
	//pProgram->initWithVertexShaderFilename(pszFileName, pszPosFileName);
    
    
	setShaderProgram(pProgram);
	pProgram->release();
    
	CHECK_GL_ERROR_DEBUG();
    
	getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	getShaderProgram()->addAttribute("a_position", 0);
    
	CHECK_GL_ERROR_DEBUG();
    
	getShaderProgram()->link();
    
	CHECK_GL_ERROR_DEBUG();
    
	
	m_attributePosition = glGetAttribLocation(
                                              pProgram->getProgram(), "a_position");
    
	getShaderProgram()->updateUniforms();
    
	CHECK_GL_ERROR_DEBUG();
    
	m_subLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "substract");
	m_blurLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "blurSize");
	m_blurPos = glGetUniformLocation( getShaderProgram()->getProgram(), "a_position");
	CHECK_GL_ERROR_DEBUG();
    
	return true;
}

void CNodeGoubian::setupStencilForClippingOnPlane(GLint plane)
{
	GLint planeMask = 0x1 << plane;
	glStencilMask(planeMask);
	glClearStencil(0x0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glFlush();
	glStencilFunc(GL_NEVER, planeMask, planeMask);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
}

void CNodeGoubian::setupStencilForDrawingOnPlane(GLint plane)
{
	GLint planeMask = 0x1 << plane;
	GLint equalOrLessPlanesMask = planeMask | (planeMask - 1);
	//GLint equalOrLessPlanesMask = planeMask ;
	glStencilFunc(GL_EQUAL, equalOrLessPlanesMask,equalOrLessPlanesMask );
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

