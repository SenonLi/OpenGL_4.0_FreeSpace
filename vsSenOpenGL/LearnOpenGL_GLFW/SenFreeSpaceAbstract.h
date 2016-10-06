#pragma once

#ifndef __SenFreeSpaceAbstract__
#define __SenFreeSpaceAbstract__

#include "SenAbstractGLFW.h"

#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SenFreeSpacePieces/Sen_Cube.h"

class SenFreeSpaceAbstract : public SenAbstractGLFW
{
public:
	SenFreeSpaceAbstract();
	virtual ~SenFreeSpaceAbstract();


protected:
	void paintGL(void);
	void initialGlfwGlewGL();
	void finalize();

	glm::mat4 model, view, projection;

	Sen_Cube backgroundCube;

	float viewCenter = -3.0f;
};

#endif // __SenFreeSpaceAbstract__