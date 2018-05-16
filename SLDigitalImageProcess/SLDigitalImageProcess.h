#ifndef __SLDIGITALIMAGEPROCESS__
#define __SLDIGITALIMAGEPROCESS__
#pragma once

#include "Support/SenAbstractGLFW.h"

namespace sldip
{
	class SLImageObject
	{
	public:
		inline int Width()      const { return m_Width; }
		inline int Height()     const { return m_Height; }
		inline int Channels()	const { return m_Channels; }
	private:
		int m_Width = 0;
		int m_Height = 0;
		int m_Channels = 0;
	};


	static void UploadImageToGPU(const TCHAR* filePath);

	static void HistorgramEqualization(unsigned char* image, int imageWidth, int imageHeight, int channels);

	static const int CPU_TOTAL_GRAY_LEVEL = 256;
	static const float VALID_HISTOGRAM_FLOOR_RATIO = 0.025f;
	static const float VALID_HISTOGRAM_CEIL_RATIO = 0.975f;

}; // end of namespace DIP

class SLDigitalImageProcess : public SenAbstractGLFW
{
public:
	SLDigitalImageProcess();
	virtual ~SLDigitalImageProcess();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	void initialVertices();
	void initialBackgroundTexture();
	void initialNewLayerTexture();
	void bindBackgroundTexture();
	void bindNewLayerTexture();

	void PreImageProcess();

	unsigned char* textureImagePtr = nullptr;
	GLuint newLayerTexture;
	CImage m_TargetImage;
};

#endif __SLDIGITALIMAGEPROCESS__