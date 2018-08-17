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
	class SLImageParam;
	class SLLibreImage;

	void DuplicateImage(const SLImageParam& srcImageParam, CImage& destImage);
	void DuplicateImage(const CImage& srcImage, CImage& dstImage);
	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage);

	SLImageParam GetImageParam(CImage& image);
	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath);

	//void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType);
	//void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType);

}; // end of namespace DIP

#endif // __SLCIMAGEHANDLES__