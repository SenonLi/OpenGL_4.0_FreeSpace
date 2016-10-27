#pragma once

#ifndef __Sen_BreakOut__
#define __Sen_BreakOut__

#include "Sinks/BreakOut/Sen_2D_GrameSpace.h"

class Sen_2D_BlockBrick	
{
public:
	Sen_2D_BlockBrick() : brickPosition(glm::vec2(0.0f)), brickSize(glm::vec2(0.0f))
		, brickColor(glm::vec3(1.0f)), isSolid(GL_FALSE), isDestroyed(GL_FALSE)
	{}
	Sen_2D_BlockBrick(glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLboolean solid = GL_FALSE) 
		: brickPosition(pos), brickSize(size)
		, brickColor(color), isSolid(solid), isDestroyed(GL_FALSE)
	{}

private:
	glm::vec2 brickPosition;
	glm::vec2 brickSize;
	glm::vec3 brickColor;
	GLboolean isSolid;
	GLboolean isDestroyed;
};

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

	// Bricks Variables
	GLuint blockTexture, solidBlockTexture;
};



#endif // __Sen_BreakOut__