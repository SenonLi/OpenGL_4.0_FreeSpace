#pragma once

#ifndef __Sen_10_Camera__
#define __Sen_10_Camera__

#include "Sen_09_ModelViewProjection.h"
#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

#include "SenFreeSpacePieces/Sen_Cube.h"

class Sen_10_Camera : public SenFreeSpaceAbstract
{
public:
	Sen_10_Camera();
	virtual ~Sen_10_Camera();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void cleanFreeSpace();

	Sen_Cube backgroundCube;

	float viewCenter = -3.0f;
};


#endif
