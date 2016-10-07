#pragma once

#ifndef __Sen_10_Camera__
#define __Sen_10_Camera__

#include "Sen_09_ModelViewProjection.h"

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

#include <soil/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "SenFreeSpacePieces/Sen_Cube.h"

class Sen_10_Camera : public SenFreeSpaceAbstract
{
public:
	Sen_10_Camera();
	virtual ~Sen_10_Camera();

protected:
	void paintGL(void);
	void initialGlfwGlewGL();
	//void finalize();


	Sen_Cube backgroundCube, stillCube;

	float viewCenter = -3.0f;
};


#endif
