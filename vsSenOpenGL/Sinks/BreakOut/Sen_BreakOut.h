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
	void daltaTimeUpdate(GLfloat deltaTime);


	void initShaderPrograms();
	void initVertexAttributes();
	void initTextures();

private:
	// Background Variables
	GLuint backgroundVAO, backgroundVBO, backgroundTexture;
	void initBackgroundVertices();
	void paintBackground();


	// PlayerBoard Variables
	GLuint playerBoardVAO, playerBoardVBO, playerBoardTexture;
	GLfloat playerBoardWidth;
	glm::vec2 playerBoardPosition;
	const GLfloat PLAYER_VELOCITY = (1.0f);
	
	void initPlayerVertices();
	void paintPlayer();
};



#endif // __Sen_BreakOut__