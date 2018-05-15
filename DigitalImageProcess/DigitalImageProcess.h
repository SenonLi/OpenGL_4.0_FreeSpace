#pragma once

#ifndef __Sen_DigitalImageProcess__
#define __Sen_DigitalImageProcess__

#include "Support/SenAbstractGLFW.h"

#include <soil/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DigitalImageProcess : public SenAbstractGLFW
{
public:
	DigitalImageProcess();
	virtual ~DigitalImageProcess();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	void initialVertices();
	void initialBackgroundTexture();
	void initialNewLayerTexture();
	void bindBackgroundTexture();
	void bindNewLayerTexture();

	unsigned char* textureImagePtr;

	GLuint newLayerTexture;
};

#endif 