#pragma once
#ifndef __Sen_30_GeometryShow__
#define __Sen_30_GeometryShow__

#include "SenAbstractGLFW.h"
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sen_10_Camera :	public SenAbstractGLFW
{
public:
	Sen_10_Camera();
	virtual ~Sen_10_Camera();

protected:
	void paintGL(void);
	void initialGlfwGlewGL();
	void finalize();

	void initialVertices();
	void initialBackgroundTexture();


	void initModelViewProjection();
	void updateModelViewProjection();

	glm::mat4 model, view, projection;

	unsigned char* textureImage;

	GLuint newLayerTexture, thirdLayerTexture;

};


#endif
