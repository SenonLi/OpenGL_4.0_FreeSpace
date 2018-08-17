#include "stdafx.h"

#include "SLCImageHandles.h"
#include "SLDigitalImageProcess.h" // For basic slgip static const definition

#include "SLImageParam.h"
#include "SLLibreImage.h"

namespace sldip
{
	void DuplicateImage(const SLImageParam& srcImageParam, CImage& destImage)
	{
		if (!destImage.IsNull())     destImage.Destroy();

		// Deplicate from srcImage to destImage
		// ...

		assert(!destImage.IsNull());
	};// End of DuplicateImage(const SLImageParam& srcParam, CImage& destImage)

	void DuplicateImage(const CImage& srcImage, CImage& dstImage)
	{
		assert(!srcImage.IsNull());
		if (srcImage == dstImage)
			return;

		int srcBitsCount = srcImage.GetBPP();
		int srcWidth = srcImage.GetWidth();
		int srcHeight = srcImage.GetHeight();

		if (!dstImage.IsNull())     dstImage.Destroy();
		if (srcBitsCount == 32)     dstImage.Create(srcWidth, srcHeight, srcBitsCount, CImage::createAlphaChannel);
		else                        dstImage.Create(srcWidth, srcHeight, srcBitsCount, 0);

		if (srcImage.IsIndexed())	{
			int nColors = srcImage.GetMaxColorTableEntries();
			if (nColors > 0)	{
				std::vector<RGBQUAD> rgbColors(nColors);
				srcImage.GetColorTable(0, nColors, rgbColors.data());
				dstImage.SetColorTable(0, nColors, rgbColors.data());
			}
		}

		HDC deepCopyHDC = dstImage.GetDC();
		srcImage.BitBlt(deepCopyHDC, 0, 0, SRCCOPY);
		dstImage.ReleaseDC();
	};// End of DuplicateImage(const CImage& srcImage, CImage& destImage)

	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage)
	{
		assert(!srcImage.IsNull());
		dstImage.CreateLibreImage(srcImage.GetWidth(), srcImage.GetHeight(), srcImage.GetBPP() / sldip::BIT_NUM_IN_ONE_BYTE);
		assert(!dstImage.IsNull());

		int srcPitch = srcImage.GetPitch();
		int dstPitch = dstImage.GetPitch();

		const BYTE* srcTopLineDataEntry = static_cast<const BYTE*>(srcImage.GetBits());
		BYTE* dstDataEntry = dstImage.GetBufferEntry();

		// Attension Senon!!!
		for (unsigned int row = 0; row < dstImage.GetHeight(); row++)
		{
			// Attention!! :  Must use int for BYTE* address seeking !
			// For Example:   BYTE* bufferEntry = topLinePixelAddress + static_cast<int>(image.GetHeight() - 1) * image.GetPitch();
			memcpy(dstDataEntry + static_cast<int>( row * dstPitch), srcTopLineDataEntry + static_cast<int>(row * srcPitch), std::abs(srcPitch));
		}
	}

	/// <summary>Extract basic ImageParam from Full-Loaded CImage</summary>
	/// <param name="image">Full-Loaded CImage, whose buffer cannot be empty [IN]</param>
	/// <returns>basic ImageParam of the input image, whose LinearBufferEntry cannot be nullptr </returns>
	SLImageParam GetImageParam(CImage& image)
	{
		assert(!image.IsNull());

		// CImage belongs to Windows GDI library, in which all DIBs (Device-Independent Bitmap) are bottom-up.
		// GetBits() will return pixel address of (0,0) instead of the first byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, here we need to move pointer up [Height() - 1] levels.
		BYTE* srcTopLineDataEntry = static_cast<BYTE*>( image.GetBits() );
		// Attention!! :  Must use int for BYTE* address seeking !
		// actualBufferEntry here from CImage is the BottomeLine FirstPixel Address
		BYTE* actualBufferEntry = srcTopLineDataEntry + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch();

		SLImageParam imageParam(
			static_cast<unsigned int>(image.GetWidth()),
			static_cast<unsigned int>(image.GetHeight()),
			image.GetPitch(),
			SLImageParam::GetImage2DColorType(image.GetBPP() / BIT_NUM_IN_ONE_BYTE),
			actualBufferEntry
		);

		return imageParam;
	}

	/// <summary>Read picture file from Disk, and return SLImageParam </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information, except Mapped GPU TextureID  </returns>
	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath)
	{
		assert(filePath && _tcsclen(filePath) != 0);
		if (!imageLoader.IsNull())
			imageLoader.Destroy();
		imageLoader.Load(filePath);
		assert(!imageLoader.IsNull()); // failed to load image file or Param

		return GetImageParam(imageLoader);
	}




	///// <summary>Save CImage To Disk Image File </summary>
	///// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	///// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	///// <param name="filePath">picture filePath + fileName</param>
	//void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType)
	//{
	//	HRESULT result = imageLoader.Save(filePath.c_str(), imageType);
	//	assert(SUCCEEDED(result));
	//}
	//void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType)
	//{
	//	SaveToImageFile(imageLoader, (folderPath + fileName), imageType);
	//}


} // End of namespace sldip
