#ifndef __SLDIGITALIMAGEPROCESS__
#define __SLDIGITALIMAGEPROCESS__
#pragma once

#include "SLImageParam.h"
#include "SLLibreImage.h"

namespace sldip
{
	const int BIT_NUM_IN_ONE_BYTE = 8;
	const int CPU_TOTAL_GRAY_LEVEL = 256;
	const BYTE CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK = 0x00;
	const BYTE CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE = 0xFF;
	const float VALID_HISTOGRAM_FLOOR_RATIO = 0.125f;
	const float VALID_HISTOGRAM_CEIL_RATIO = 0.875f;

	enum SLAdaptiveThresholdingType
	{
		ADAPTIVE_THRESH_MEAN,
		ADAPTIVE_THRESH_GAUSSIAN,
	};

	SLImageParam GetImageParam(const SLLibreImage& image);
	SLImageParam LoadImageParam(SLLibreImage& targetImageLoader, const TCHAR* filePath);

	void GetGrascaledImage(const SLLibreImage& srcImage, SLLibreImage& dstImage);
	void DuplicateImage(const SLLibreImage& srcImage, SLLibreImage& dstImage);

	void SaveToImageFile(const SLLibreImage& srcImage, const std::wstring& filePath, const SLImageFileType& imageType);
	void SaveToImageFile(const SLLibreImage& srcImage, const std::wstring& folderPath, const std::wstring& fileName, const SLImageFileType& imageType);

	void HistorgramEqualization(SLLibreImage& grayScaledImage);
	void AdaptiveThresholding(SLImageParam& textureParam, SLAdaptiveThresholdingType filterType);
	void MeanFilterBlur(SLImageParam& textureParam);
	void GaussianFilterBlur(SLImageParam& textureParam);

}; // end of namespace sldip

#endif // __SLDIGITALIMAGEPROCESS__