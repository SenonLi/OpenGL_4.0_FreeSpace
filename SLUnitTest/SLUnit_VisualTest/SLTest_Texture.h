#ifndef __SLTEST_TEXTURE__
#define __SLTEST_TEXTURE__
#pragma once

#include "Support/SenAbstractGLFW.h"
#include "SLImageParam.h"

#include <algorithm>
#if !defined(min) || !defined(max)
#define min std::min
#define max std::max
#endif

#include <atlimage.h> // For CImage

// Undef Windows min and max macros so they don't clash with std::min and std::max
#undef min
#undef max

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
	CImage m_ImageLoader;
	sldip::SLImageParam m_ImageParam;
};

#endif __SLTEST_TEXTURE__