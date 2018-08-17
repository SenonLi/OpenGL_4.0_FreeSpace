#ifndef __SLDIGITALIMAGEPROCESS__
#define __SLDIGITALIMAGEPROCESS__
#pragma once

#include "SLImageParam.h"
#include "SLLibreImage.h"

#include <algorithm>

#if !defined(min) || !defined(max)
#define min std::min
#define max std::max
#endif
/// <remarks> atlimage.h is the Header of Windows GDI: Graphics Device Interface 
/// Add for CImage, to load and process image.
/// In GDI, all DIBs are bottom-up. DIB: Device-Independent Bitmap </remarks>
#include<atlimage.h>
// Undef Windows min and max macros so they don't clash with std::min and std::max
#undef min
#undef max

namespace sldip
{
	const int BIT_NUM_IN_ONE_BYTE = 8;
	const int CPU_TOTAL_GRAY_LEVEL = 256;
	const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK = 0x00;
	const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE = 0xFF;
	const float VALID_HISTOGRAM_FLOOR_RATIO = 0.125f;
	const float VALID_HISTOGRAM_CEIL_RATIO = 0.875f;

	class SLImageParam;
	class SLLibreImage;

	enum SLAdaptiveThresholdingType
	{
		ADAPTIVE_THRESH_MEAN,
		ADAPTIVE_THRESH_GAUSSIAN,
	};

	SLImageParam GetImageParam(const SLLibreImage& image);
	SLImageParam LoadImageParam(SLLibreImage& targetImageLoader, const TCHAR* filePath);

	void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType);
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType);


	void UploadLinearImageToGPU(SLImageParam& textureParam);

	// !!! SLImageParam is not supposed to be edited!! Change it to SLLibreImage!
	void HistorgramEqualization(SLImageParam& textureParam);
	void AdaptiveThresholding(SLImageParam& textureParam, SLAdaptiveThresholdingType filterType);
	void MeanFilterBlur(SLImageParam& textureParam);
	void GaussianFilterBlur(SLImageParam& textureParam);

}; // end of namespace DIP

#endif // __SLDIGITALIMAGEPROCESS__