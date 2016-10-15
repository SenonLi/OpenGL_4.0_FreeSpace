#pragma once

#ifndef __Sen_07_TextureGLFW__
#define __Sen_07_TextureGLFW__

#include "Support/SenAbstractGLFW.h"

#include <soil/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sen_07_TextureGLFW :	public SenAbstractGLFW
{
public:
	Sen_07_TextureGLFW();
	virtual ~Sen_07_TextureGLFW();

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