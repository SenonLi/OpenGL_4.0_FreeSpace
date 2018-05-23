#pragma once

#ifndef __Sen_BreakOut__
#define __Sen_BreakOut__

#include "Sinks/BreakOut/Sen_2D_GrameSpace.h"
#include <vector>

class Sen_2D_BlockBrick	
{
public:
	Sen_2D_BlockBrick() : brickPosition(glm::vec2(0.0f)), brickSize(glm::vec2(0.0f))
		, brickColor(glm::vec3(1.0f)), isSolid(GL_FALSE), isBrickNotDestroyed(GL_TRUE), brickLife(0)
	{}
	Sen_2D_BlockBrick(glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLuint life = 0, GLboolean solid = GL_FALSE) 
		: brickPosition(pos), brickSize(size), brickLife(life)
		, brickColor(color), isSolid(solid), isBrickNotDestroyed(GL_TRUE)
	{}

	inline GLuint getBrickLife() const { return brickLife; }
	inline glm::vec2 getBrickPosition() const { return brickPosition; }
	inline glm::vec2 getBrickSize() const { return brickSize; }
	inline glm::vec3 getBrickColor() const { return brickColor; }
	inline GLboolean getBrickIsSolidStatus() const { return isSolid; }
	inline GLboolean getBrickNotDestroyedStatus() const { return isBrickNotDestroyed; }
	inline void setBrickNotDestroyStatus(const GLboolean isNotDestroyed) { isBrickNotDestroyed = isNotDestroyed; }
	
	inline void setBrickPosition(const glm::vec2 position) { brickPosition = position; }
	inline void setBrickSize(const glm::vec2 size) { brickSize = size; }

	GLboolean destroyedAfterHitBrick()	{
		if (brickLife > 0) brickLife -= 1;
		if (!brickLife) {
			setBrickNotDestroyStatus(GL_FALSE); return GL_TRUE;
		}
		return GL_FALSE;
	}

private:
	glm::vec2 brickPosition;
	glm::vec2 brickSize;
	glm::vec3 brickColor;
	GLuint brickLife;
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

	inline GLuint getUnsolidBricksNum() const { return unsolidBricksNum; }

private:
	GLuint unsolidBricksNum;
};
//***********************************************************
//***********************************************************
//***********************************************************


// ********** Change when Better Collision Detection ***********************
#include <tuple>
// Represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT__,
	DOWN,
	LEFT__
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// ********** Change when Better Collision Detection ***********************

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

	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);

private:
	// *********  Background Variables ********************************************
	GLuint backgroundVAO, backgroundVBO, backgroundTexture;

	void initBackgroundVertices();
	void paintBackground();
	void init2DMapInfo(std::vector<std::vector<GLuint>> &map2DBrickTypesinfo, GLuint breakOutLevel);

	void paintFontsHints();

	// *********  PlayerBoard Variables ********************************************
	GLuint playerBoardVAO, playerBoardVBO, playerBoardTexture;
	glm::vec2 playerBoardPosition;
	GLfloat playerBoardWidth;
	const GLfloat PLAYER_VELOCITY = (1.0f);
	const GLfloat originalPlayerBOARDWIDTH = 1.8f / 4.0f;
	const GLfloat originalPlayerBOARDHEIGHT = 1.2f / 30.0f;
	const glm::vec2	originalPlayerBoardPOSITION = glm::vec2(0.0f, originalPlayerBOARDHEIGHT / 2.0f - 1.0f);

	void initPlayerVertices();
	void paintPlayer();

	//*************************************************************************************
	//*************************************************************************************
	GLuint blendUnitOneSquareProgram, unitOneSquareVAO, unitOneSquareVBO;
	void initUnitOneSquareVertices();
	
	// *********  Bricks Variables + Level Distribution *****************************
	GLboolean gameActive, currLevelFinished;
	GLuint breakOutLevel, currLevelUnsolidBrickNum;
	std::vector<Sen_BreakOutMap> brickMapsVector;
	std::vector<GLuint> mapUnsolidBrickNumVector;

	GLfloat cubeLogoSquareAreaSide;
	GLuint blockTexture, solidBlockTexture;

	void paintBricksMap();
	void trigerNextLevel();
	// ************ Ball Variables **************************************************
	glm::vec2 ballVelocity, ballPosition;
	GLboolean isBallStuckOnPlayerBoard;
	GLuint ballProgram, ballTexture;
	GLfloat ballSpinAngleDegree, ballSpinSpeed;

	void normalizeAngleDegree(GLfloat &angle)	{
		while (angle < 0)	angle += 360.0f;
		while (angle > 360)	angle -= 360.0f;
	}

	const GLfloat ballSquareSIDE = 0.139f;
	const GLfloat ballRADIUS = 0.93f * 0.5f * ballSquareSIDE;

	GLfloat speedRatio = 4.0f;//7.0f;
	const GLfloat originalBallSpinSpeed = speedRatio * 80.0f;
	const glm::vec2 originalBallVELOCITY = speedRatio * glm::vec2(0.23f, 0.2f);
	const glm::vec2	originalBallPOSITION = originalPlayerBoardPOSITION + glm::vec2(0.0f, ballRADIUS + originalPlayerBOARDHEIGHT / 2.0);

	void initBallVariables();
	void paintBall();

	// ************* Collision Detection *******************************************
	// AABB - AABB collision
	GLboolean checkBrickBallSquareCollision(const Sen_2D_BlockBrick &brick);
	GLboolean checkBrickBallCircleCollision(const Sen_2D_BlockBrick &brick);
	void bricksCollisionKill();




	// ************** Change when Better Collision Detection **************************
	Collision returnBrickBallCircleCollision(const Sen_2D_BlockBrick &brick);
	Direction VectorDirection(glm::vec2 target);
	void bricksCollisionKill_resolution();


	// ************** Change when Better Collision Detection **************************
};



#endif // __Sen_BreakOut__