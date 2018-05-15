#pragma once

#ifndef __Sen_DigitalImageProcess__
#define __Sen_DigitalImageProcess__

#include "Support/SenAbstractGLFW.h"


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