#ifndef __SLTEST_TEXTURE__
#define __SLTEST_TEXTURE__
#pragma once

#include "Support/SenAbstractGLFW.h"
#include "SLImageParam.h"
#include "SLLibreImage.h"

class SLTest_Texture : public SenAbstractGLFW
{
public:
	SLTest_Texture();
	virtual ~SLTest_Texture() {};

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	void initialVertices();
	void initialBackgroundTexture();
	void bindBackgroundTexture();

	void LoadLibreImage();

	unsigned char* textureImagePtr = nullptr;
	GLuint newLayerTexture;
	//----------------------------- Temporary --------------------
	const TCHAR* m_ImagePath = nullptr;
	sldip::SLImageParam m_ImageParam{};
	sldip::SLLibreImage m_LibreImage{};

	sldip::SLLibreImage m_BackUp_LibreImage{};
};

#endif __SLTEST_TEXTURE__