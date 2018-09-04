#ifndef __SLCIMAGEPROCESS__
#define __SLCIMAGEPROCESS__
#pragma once

/// <remark>Whichever class that will use SLCImageProcess.h will have to include CImage header, thus we put the #include below in SLCImageProcess.h file. </remark>
#include <algorithm>
#if !defined(min) || !defined(max)
#define min std::min
#define max std::max
#endif
/// <remarks> atlimage.h is the Header of Windows GDI: Graphics Device Interface 
/// Add for CImage, to load and process image.
/// In GDI, all DIBs are bottom-up. DIB: Device-Independent Bitmap </remarks>
#include <atlimage.h>
// Undef Windows min and max macros so they don't clash with std::min and std::max
#undef min
#undef max

namespace slcimage
{
	void DuplicateImage(const ATL::CImage& srcImage, ATL::CImage& dstImage);

	void StretchImageByWidth(const ATL::CImage& srcImage, int widthInPixel, ATL::CImage& dstImage);
	void Convert8bitTo24Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage);

	/// <remark>CImage::BitBlt won't work with alpha channel, we have to set pixel one by one</remark>
	void Convert8bitTo32Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage);
	void Convert24bitTo32Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage);



}; // End of namespace slcimage

#endif // __SLCIMAGEPROCESS__