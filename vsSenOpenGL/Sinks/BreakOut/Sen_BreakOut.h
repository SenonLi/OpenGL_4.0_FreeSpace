#pragma once

#ifndef __Sen_BreakOut__
#define __Sen_BreakOut__

#include "Sinks/BreakOut/Sen_2D_GrameSpace.h"

class Sen_BreakOut : public Sen_2D_GrameSpace
{
public:
	Sen_BreakOut();
	virtual ~Sen_BreakOut();

protected:
	void clean_2D_GrameFrame();
	void init2DGamePaceGL();
	void paint2DGameSpaceGL();

private:
	GLuint backgroundVAO, backgroundVBO, backgroundTexture;
	GLuint playerVAO, playerVBO, playerTexture;

	void initShaderPrograms();
	void initVertexAttributes();
	void initTextures();

	void paintBackground();
};



#endif // __Sen_BreakOut__