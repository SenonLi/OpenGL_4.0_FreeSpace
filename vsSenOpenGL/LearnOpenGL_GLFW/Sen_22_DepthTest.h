#pragma once

#ifndef __Sen_22_DepthTest__
#define __Sen_22_DepthTest__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_22_DepthTest : public SenFreeSpaceAbstract
{
public:
	Sen_22_DepthTest();
	virtual ~Sen_22_DepthTest();

protected:
	void initialGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	Sen_Cube *similarCube;
	GLuint cubeTexture, floorTexture;
};



#endif