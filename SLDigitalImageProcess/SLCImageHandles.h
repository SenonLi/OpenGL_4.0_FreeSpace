#ifndef __SLCIMAGEHANDLES__
#define __SLCIMAGEHANDLES__
#pragma once

/// <remark>Whichever class that will use SLCImageHandles will have to include CImage header, thus we put the #include below in SLCImageHandles.h file. </remark>
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
	enum SLImageColorType;
	class SLImageParam;
	class SLLibreImage;

	void DuplicateImage(const CImage& srcImage, CImage& dstImage);
	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage);
	void DuplicateImage(const SLLibreImage& srcImage, CImage& dstImage);

	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath);
	SLImageColorType GetImage2DColorType(const CImage& image);
	SLImageParam GetImageParam(CImage& image);

}; // end of namespace DIP

#endif // __SLCIMAGEHANDLES__