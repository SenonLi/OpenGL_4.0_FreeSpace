#pragma once

#ifndef __Sen_26_PostProcessing__
#define __Sen_26_PostProcessing__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_26_PostProcessing :	public SenFreeSpaceAbstract
{
public:
	Sen_26_PostProcessing();
	virtual ~Sen_26_PostProcessing();

protected:
	void initGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	Sen_Cube *similarCube;
	GLuint cubeTexture, floorTexture;

	glm::vec3 firstCubePosition = glm::vec3(-1.0f, 0.0f + SenFREESPACE_ZfightingOffset, -1.0f);
	glm::vec3 secondCubePosition = glm::vec3(2.0f, 0.0f + SenFREESPACE_ZfightingOffset, 0.0f);


	// ==== FrameBuffer ====
	GLuint testFrameBufferObject;
	GLuint textureColorBuffer, depthStencilRenderBufferObject;



};



#endif