#ifndef __SLDIGITALIMAGEPROCESS__
#define __SLDIGITALIMAGEPROCESS__
#pragma once

namespace sldip
{
	namespace ATL
	{
		class CImage;
	}
	class SLImageParam;

	const int CPU_TOTAL_GRAY_LEVEL = 256;
	const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK = 0x00;
	const unsigned int CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE = 0xFF;
	const float VALID_HISTOGRAM_FLOOR_RATIO = 0.125f;
	const float VALID_HISTOGRAM_CEIL_RATIO = 0.875f;
	const int BIT_NUM_IN_ONE_BYTE = 8;

	enum SLAdaptiveThresholdingType
	{
		ADAPTIVE_THRESH_MEAN,
		ADAPTIVE_THRESH_GAUSSIAN,
	};

	SLImageParam GetImageParam(CImage& image);

	void DuplicateImage(const SLImageParam& srcImageParam, CImage& destImage);
	void DuplicateImage(const CImage& srcImage, CImage& destImage);

	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath);
	void UploadLinearImageToGPU(SLImageParam& textureParam);

	void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType);
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType);

	SLImageParam UploadImageToGPUFromDisk(CImage& imageLoader, const TCHAR* filePath);
	void HistorgramEqualization(SLImageParam& textureParam);

	void AdaptiveThresholding(SLImageParam& textureParam, SLAdaptiveThresholdingType filterType);
	void MeanFilterBlur(SLImageParam& textureParam);
	void GaussianFilterBlur(SLImageParam& textureParam);


}; // end of namespace DIP

#endif __SLDIGITALIMAGEPROCESS__