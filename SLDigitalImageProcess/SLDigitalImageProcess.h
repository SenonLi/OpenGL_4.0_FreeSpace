#ifndef __SLDIGITALIMAGEPROCESS__
#define __SLDIGITALIMAGEPROCESS__
#pragma once

#include "Support/SenAbstractGLFW.h"

namespace sldip
{
	class SLImageParam;

	static const int CPU_TOTAL_GRAY_LEVEL = 256;
	static const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK = 0x00;
	static const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE = 0xFF;
	static const float VALID_HISTOGRAM_FLOOR_RATIO = 0.125f;
	static const float VALID_HISTOGRAM_CEIL_RATIO = 0.875f;

	static void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType);
	static void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType);

	static SLImageParam& LoadImageParam(CImage& imageLoader, const TCHAR* filePath);
	static void UploadLinearImageToGPU(SLImageParam& textureParam);

	static SLImageParam& UploadImageToGPUFromDisk(CImage& imageLoader, const TCHAR* filePath);
	static void HistorgramEqualization(const SLImageParam& textureParam);


}; // end of namespace DIP

#include "SLImageParam.h"

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
	//----------------------------- Temporary --------------------
	const TCHAR* m_ImagePath = nullptr;
	CImage m_ImageLoader;
	sldip::SLImageParam m_ImageParam;
};

#endif __SLDIGITALIMAGEPROCESS__