#pragma once

#ifndef __Sen_BreakOut__
#define __Sen_BreakOut__

#include "Sinks/BreakOut/Sen_2D_GrameSpace.h"
#include <vector>

class Sen_2D_BlockBrick	
{
public:
	Sen_2D_BlockBrick() : brickPosition(glm::vec2(0.0f)), brickSize(glm::vec2(0.0f))
		, brickColor(glm::vec3(1.0f)), isSolid(GL_FALSE), isBrickNotDestroyed(GL_TRUE)
	{}
	Sen_2D_BlockBrick(glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLboolean solid = GL_FALSE) 
		: brickPosition(pos), brickSize(size)
		, brickColor(color), isSolid(solid), isBrickNotDestroyed(GL_TRUE)
	{}

	inline glm::vec2 getBrickPosition() const { return brickPosition; }
	inline glm::vec2 getBrickSize() const { return brickSize; }
	inline glm::vec3 getBrickColor() const { return brickColor; }
	inline GLboolean getBrickSolidStatus() const { return isSolid; }
	inline GLboolean getBrickDestroyStatus() const { return isBrickNotDestroyed; }
	inline void setBrickDestroyStatus(const GLboolean isNotDestroyed) { isBrickNotDestroyed = isNotDestroyed; }
	
private:
	glm::vec2 brickPosition;
	glm::vec2 brickSize;
	glm::vec3 brickColor;
	GLboolean isSolid;
	GLboolean isBrickNotDestroyed;
};

//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
class Sen_BreakOutMap
{
public:
	Sen_BreakOutMap(){}
	explicit Sen_BreakOutMap(GLfloat spareSide, std::vector<std::vector<GLuint>> &map2DBrickTypesinfo);

	std::vector<Sen_2D_BlockBrick> bricksVector;
};


//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
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
	void initBrickMapsVector();

private:
	// *********  Background Variables ********************************************
	GLuint backgroundVAO, backgroundVBO, backgroundTexture;

	void initBackgroundVertices();
	void paintBackground();
	void init2DMapInfo(std::vector<std::vector<GLuint>> &map2DBrickTypesinfo, GLuint breakOutLevel);

	// *********  PlayerBoard Variables ********************************************
	GLuint playerBoardVAO, playerBoardVBO, playerBoardTexture;
	GLfloat playerBoardWidth;
	glm::vec2 playerBoardPosition;
	const GLfloat PLAYER_VELOCITY = (1.0f);
	
	void initPlayerVertices();
	void paintPlayer();

	// *********  Bricks Variables + Level Distribution *****************************
	GLuint breakOutLevel;
	std::vector<Sen_BreakOutMap> brickMapsVector;

	GLfloat cubeLogoSquareAreaSide;
	GLuint blockTexture, solidBlockTexture;
	GLuint blendUnitOneSquareProgram, unitOneSquareVAO, unitOneSquareVBO;
	void initUnitOneSquareVertices();
	void paintBricksMap();
};



#endif // __Sen_BreakOut__