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
	glm::vec2 playerBoardPosition;
	GLfloat playerBoardWidth;
	const GLfloat PLAYER_VELOCITY = (1.0f);
	const GLfloat originalPlayerBOARDWIDTH = 1.8 / 4.0;
	const GLfloat originalPlayerBOARDHEIGHT = 1.2 / 30.0;
	const glm::vec2	originalPlayerBoardPOSITION = glm::vec2(0.0f, originalPlayerBOARDHEIGHT / 2.0 - 1.0);

	void initPlayerVertices();
	void paintPlayer();

	//*************************************************************************************
	//*************************************************************************************
	GLuint blendUnitOneSquareProgram, unitOneSquareVAO, unitOneSquareVBO;
	void initUnitOneSquareVertices();
	
	// *********  Bricks Variables + Level Distribution *****************************
	GLuint breakOutLevel;
	std::vector<Sen_BreakOutMap> brickMapsVector;

	GLfloat cubeLogoSquareAreaSide;
	GLuint blockTexture, solidBlockTexture;

	void paintBricksMap();

	// ************ Ball Variables **************************************************
	glm::vec2 ballVelocity, ballPosition;
	GLboolean isBallStuckOnPlayerBoard;
	GLuint ballProgram, ballTexture;
	GLfloat ballSpinAngleDegree, ballSpinSpeed;

	void normalizeAngleDegree(GLfloat &angle)	{
		while (angle < 0)	angle += 360.0f;
		while (angle > 360)	angle -= 360.0f;
	}

	const GLfloat ballSquareSIDE = 0.16f;
	const GLfloat ballRADIUS = 0.93f * 0.5f * ballSquareSIDE;

	GLfloat speedRatio = 7.0f;
	const GLfloat originalBallSpinSpeed = speedRatio * 60.0f;
	const glm::vec2 originalBallVELOCITY = speedRatio * glm::vec2(0.1f, 0.1f);
	const glm::vec2	originalBallPOSITION = originalPlayerBoardPOSITION + glm::vec2(0.0f, ballRADIUS + originalPlayerBOARDHEIGHT / 2.0);

	void initBallVariables();
	void paintBall();
};



#endif // __Sen_BreakOut__