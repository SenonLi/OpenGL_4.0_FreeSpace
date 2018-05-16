#pragma once

#ifndef __Sen_DigitalImageProcess__
#define __Sen_DigitalImageProcess__

#include "Support/SenAbstractGLFW.h"

namespace SLDIP
{
static void HistorgramEqualization(unsigned char* image, int imageWidth, int imageHeight, int channels);
static const int CPU_TOTAL_GRAY_LEVEL = 256;
static const float VALID_HISTOGRAM_FLOOR_RATIO = 0.025f;
static const float VALID_HISTOGRAM_CEIL_RATIO = 0.975f;

}; // end of namespace DIP

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