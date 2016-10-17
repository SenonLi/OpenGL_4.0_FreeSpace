#pragma once

#ifndef __Sen_24_BlendingTest__
#define __Sen_24_BlendingTest__

#include "LearnOpenGL_GLFW/SenDebugWindowFreeSpace.h"

#include <map>

class Sen_24_BlendingTest :	public SenDebugWindowFreeSpace
{
public:
	Sen_24_BlendingTest();
	virtual ~Sen_24_BlendingTest();

protected:
	void initDebugWindowProgram();

	void initGlfwGlewGL();
	void paintScene();
	void cleanDebugWindowFreeSpace();


	Sen_Cube *similarCube, *rearCube;
	GLuint cubeTexture, floorTexture;

	glm::vec3 firstCubePosition = glm::vec3(-3.5f, 0.0f + SenFREESPACE_ZfightingOffset, -4.0f);
	glm::vec3 secondCubePosition = glm::vec3(2.0f, 0.0f + SenFREESPACE_ZfightingOffset, -3.0f);
	glm::vec3 rearCubePosition = glm::vec3(-1.5f, 0.42f + SenFREESPACE_ZfightingOffset, 3.5f);
	
	GLuint grassAlphaDiscardVAO, transparentWindowVAO;
	GLuint grassAlphaDiscardVBO, transparentWindowVBO;
	GLuint grassAlphaDiscardTexture, transparentWindowTexture;

	glm::vec3 firstWindowPosition = firstCubePosition + glm::vec3(0.501f, 0.0f, -0.5f);
	glm::vec3 secondWindowPosition = secondCubePosition + glm::vec3(-0.501f, 0.0f, -0.5f);

	std::vector<glm::vec3> grassAddressVector;
	std::vector<glm::vec3> frontWindowAddressVector;
	std::vector<glm::vec3> sideWindowAddressVector;

private:
	void initBlendingProgram();
	void paintDebugWindowFrameBufferGL();

	void initVertexAttributes();
	void initTextures();
	void initGrassWindowAddress();

	void paintFloor();
	void paintGrassAlphaDiscard();
	void paintCubes();
	void paintFrontAndSideTransparentWindows(); // Need to be the last for transparency painting
};


#endif  // __Sen_24_BlendingTest__