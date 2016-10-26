#pragma once

#ifndef __Sen_47_BlendSprite__
#define __Sen_47_BlendSprite__

#include "Sinks/BreakOut/Sen_2D_GrameFrame.h"

class Sen_47_BlendSprite :	public Sen_2D_GrameFrame
{
public:
	Sen_47_BlendSprite();
	virtual ~Sen_47_BlendSprite();

protected:
	void clean_2D_GrameFrame();
	//void daltaTimeUpdate(GLfloat deltaTime);
	void init2DGamePaceGL();
	void paint2DGameSpaceGL();

private:
	void initVertexAttributes();
	
	GLuint spriteVAO, spriteVBO, spriteTexture;
	glm::vec3 spriteRenderColor;
};



#endif // __Sen_47_BlendSprite__