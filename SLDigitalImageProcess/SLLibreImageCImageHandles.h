#ifndef __SLCIMAGEHANDLES__
#define __SLCIMAGEHANDLES__
#pragma once

#include "SLCImageProcess.h"

namespace sldip
{
	enum SLLibreColorType : unsigned;
	class SLImageParam;
	class SLLibreImage;

	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage);
	void DuplicateImage(const SLLibreImage& srcImage, CImage& dstImage);

	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath);
	SLLibreColorType GetImage2DColorType(const CImage& image);
	SLImageParam GetImageParam(CImage& image);

}; // end of namespace sldip

#endif // __SLCIMAGEHANDLES__