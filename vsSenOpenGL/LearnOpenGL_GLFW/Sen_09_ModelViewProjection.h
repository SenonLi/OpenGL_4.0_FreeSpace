#pragma once
#ifndef __Sen_09_ModelViewProjection__
#define __Sen_09_ModelViewProjection__

#include "Support/SenAbstractGLFW.h"
#include <soil/SOIL.h>
// GLM Mathematics
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
	void initialGlfwGlewGL();
	void finalize();

	glm::mat4 model, view, projection;


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