#pragma once

#ifndef __Sen_30_GeometryShow__
#define __Sen_30_GeometryShow__

#include "Sen_09_ModelViewProjection.h"

#include "SenAbstractGLFW.h"

#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sen_10_Camera : public Sen_09_ModelViewProjection
{
public:
	Sen_10_Camera();
	virtual ~Sen_10_Camera();

protected:
	void paintGL(void);
	//void initialGlfwGlewGL();
	//void finalize();


};


#endif
