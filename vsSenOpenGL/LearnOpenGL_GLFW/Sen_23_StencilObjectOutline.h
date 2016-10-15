#pragma once

#ifndef __Sen_23_StencilObjectOutline__
#define __Sen_23_StencilObjectOutline__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_23_StencilObjectOutline :	public SenFreeSpaceAbstract
{
public:
	Sen_23_StencilObjectOutline();
	virtual ~Sen_23_StencilObjectOutline();

protected:
	void initGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	Sen_Cube *similarCube;
	GLuint cubeTexture, floorTexture;

	glm::vec3 firstCubePosition = glm::vec3(-1.0f, 0.0f + SenFREESPACE_ZfightingOffset, -1.0f);
	glm::vec3 secondCubePosition = glm::vec3(2.0f, 0.0f + SenFREESPACE_ZfightingOffset, 0.0f);

};



#endif