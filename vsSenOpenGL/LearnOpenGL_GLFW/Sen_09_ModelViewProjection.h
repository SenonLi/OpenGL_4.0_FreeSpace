#pragma once
#ifndef __Sen_09_ModelViewProjection__
#define __Sen_09_ModelViewProjection__

#include "Support/SenAbstractGLFW.h"
#include <soil/SOIL.h>
// GLM Mathematics
#define GLM_FORCE_SWIZZLE // Have to add this for new glm version without default structure initialization 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sen_09_ModelViewProjection :	public SenAbstractGLFW
{
public:
	Sen_09_ModelViewProjection();
	virtual ~Sen_09_ModelViewProjection();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	glm::mat4 model{ 1.0f }, view{ 1.0f }, projection{ 1.0f };


	void initialVertices();
	void initialBackgroundTexture();
	void initialNewLayerTexture();
	void bindBackgroundTexture();
	void bindNewLayerTexture();

	void initModelViewProjection();
	void updateModelViewProjection();
	void updateSecondModelViewProjection();

	unsigned char* textureImagePtr;

	GLuint newLayerTexture, thirdLayerTexture;

	float viewCenter = -3.0f;
};


#endif