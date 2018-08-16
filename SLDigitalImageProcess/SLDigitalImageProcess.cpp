#include "stdafx.h"
#include "SLDigitalImageProcess.h"

#include "SLImageParam.h"

namespace sldip
{
	void MeanFilterBlur(SLImageParam& textureParam)
	{
		BYTE* imageBufferEntry = textureParam.LinearBufferEntry();
		assert(imageBufferEntry);
		int imageWidth = textureParam.Width();
		int imageHeight = textureParam.Height();
		int imageChannels = textureParam.ChannelNumber();
		int imageAbsPitch = textureParam.PitchAbsolute();
	}// End of MeanFilterBlur

	void GaussianFilterBlur(SLImageParam& textureParam)
	{

	}// End of GaussianFilterBlur

	void DuplicateImage(const SLImageParam& srcImageParam, CImage& destImage)
	{
		if (!destImage.IsNull())     destImage.Destroy();

		// Deplicate from srcImage to destImage
		// ...

		assert(!destImage.IsNull());
	};// End of DuplicateImage(const SLImageParam& srcParam, CImage& destImage)

	void DuplicateImage(const CImage& srcImage, CImage& destImage)
	{
		if (!destImage.IsNull())     destImage.Destroy();
		
		// Deplicate from srcImage to destImage
		// ...

		assert(!destImage.IsNull());
	};// End of DuplicateImage(const CImage& srcImage, CImage& destImage)

	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <param name="textureParam">All basic Image Info; with Beginning address of image buffer [IN/OUT]</param>
	void AdaptiveThresholding(SLImageParam& textureParam, SLAdaptiveThresholdingType filterType)
	{
		// Copy Image for Background Preparation
		CImage backgroundImage;
		sldip::DuplicateImage(textureParam, backgroundImage);

		// Get Image Background
		switch (filterType)
		{
		case SLAdaptiveThresholdingType::ADAPTIVE_THRESH_MEAN:
			MeanFilterBlur(textureParam);
			break;
		case SLAdaptiveThresholdingType::ADAPTIVE_THRESH_GAUSSIAN:
			GaussianFilterBlur(textureParam);
			break;
		default:
			assert(false);
		}


	}// End of AdaptiveThresholding

	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <param name="textureParam">All basic Image Info; with Beginning address of image buffer [IN/OUT]</param>
	void HistorgramEqualization(SLImageParam& textureParam)
	{
		BYTE* imageBufferEntry = textureParam.LinearBufferEntry();
		assert(imageBufferEntry);
		int imageWidth = textureParam.Width();
		int imageHeight = textureParam.Height();
		int imageChannels = textureParam.ChannelNumber();
		int imageAbsPitch = textureParam.PitchAbsolute();

		int cdf[CPU_TOTAL_GRAY_LEVEL]; // Array of Cumulative Distributive Function at each gray level
		std::memset(cdf, 0, sizeof(cdf));

		// Get Cumulative Distributive Function
		for (int row = 0; row < imageHeight; ++row) {
			for (int column = 0; column < imageWidth; ++column) {
				int pixelGrayLevel = imageBufferEntry[row * imageAbsPitch + column * imageChannels]; // grayLevel == pixel intensity == Red
				cdf[pixelGrayLevel] += 1;
			}
		}
		// find valid min and max of grayLevel
		unsigned int minValidGrayLevel = 0;
		unsigned int maxValidGrayLevel = CPU_TOTAL_GRAY_LEVEL;
		int sumCDF = 0;
		float totalPixel = imageWidth * imageHeight * 1.0f;
		for (int grayLevel = 0; grayLevel < CPU_TOTAL_GRAY_LEVEL; ++grayLevel)
		{
			sumCDF += cdf[grayLevel];
			float lambda = sumCDF / totalPixel;
			if (lambda < VALID_HISTOGRAM_FLOOR_RATIO)          minValidGrayLevel = grayLevel;
			else if (lambda < VALID_HISTOGRAM_CEIL_RATIO)           maxValidGrayLevel = grayLevel;
		}
		// Re-scale imageBufferEntry
		for (int row = 0; row < imageHeight; ++row) {
			for (int column = 0; column < imageWidth; ++column) {
				// ImagePixel on CPU consists of RGBA 4 chanels, each represented by 1 unsigned byte under range 0~255
				int curPixelEntry = row * imageAbsPitch + column * imageChannels;
				unsigned int oldIntensity = imageBufferEntry[curPixelEntry];
				unsigned int newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;

				// Calculate New gray-scaled Color Intensity
				if (oldIntensity <= minValidGrayLevel) {
					newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;
				}
				else if (oldIntensity > maxValidGrayLevel) {
					newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE;
				}
				else {
					newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE * (oldIntensity - minValidGrayLevel) / (maxValidGrayLevel - minValidGrayLevel);
				}

				// Assign New gray-scaled Color Intensity
				for (int i = 0; i < imageChannels; ++i) {
					imageBufferEntry[curPixelEntry + i] = newIntensity;
				}
			}
		}
	} // End of HistorgramEqualization

	/// <summary>Save CImage To Disk Image File </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType)
	{
		HRESULT result = imageLoader.Save(filePath.c_str(), imageType);
		assert(SUCCEEDED(result));
	}
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType)
	{
		SaveToImageFile(imageLoader, (folderPath + fileName), imageType);
	}

	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remarks> Will be called by UploadImageToGPUFromDisk, should be put infront of it to avoid pre-declaration
	///               Designed to upload CPU-Processed image, which has to be linear     </remarks>
	/// <param name="textureParam">Offer image information; Get generated TextureID [IN/OUT]</param>
	void UploadLinearImageToGPU(SLImageParam& textureParam)
	{
		assert(textureParam.LinearBufferEntry());

		// Generate and Bind TextureID
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		textureParam.SetTextureID(textureID);

		// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set defaultTextureID filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load textureImage, create defaultTextureID and generate mipmaps
		switch (textureParam.ColorType())
		{
		case SLImageColorType::ColorGray:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureParam.Width(), textureParam.Height(), 0, GL_RED, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case SLImageColorType::ColorRGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case SLImageColorType::ColorRGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		default:
			assert(false);// Doesn't support the others ColorType yet
		}

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WidgetWidth, m_WidgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);  // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
		assert(textureParam.TextureID()); // New generated TextureID should not be 0
	} // End of UploadLinearImageToGPU

	/// <summary>Extract basic ImageParam from Full-Loaded CImage</summary>
	/// <param name="image">Full-Loaded CImage, whose buffer cannot be empty [IN]</param>
	/// <returns>basic ImageParam of the input image, whose LinearBufferEntry cannot be nullptr </returns>
	SLImageParam GetImageParam(CImage& image)
	{
		assert(!image.IsNull());
		SLImageParam imageParam;
		imageParam.SetWidth(image.GetWidth());
		imageParam.SetHeight(image.GetHeight());
		imageParam.SetPitch(image.GetPitch());
		imageParam.SetImageColorType(image.GetBPP() / BIT_NUM_IN_ONE_BYTE);

		// CImage belongs to Windows GDI library, in which all DIBs (Device-Independent Bitmap) are bottom-up.
		// GetBits() will return pixel address of (0,0) instead of the first byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, here we need to move pointer up [Height() - 1] levels.
		BYTE* firstPixelAddress = static_cast<BYTE*>( image.GetBits() );
		BYTE* bufferEntry = firstPixelAddress + (imageParam.Height() - 1) * imageParam.Pitch();
		imageParam.SetLinearBufferEntry(bufferEntry);

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
		// assert(glew already initialed); // GLEW doesn't require UI, should be added in sldip
		if (!imageLoader.IsNull())
			imageLoader.Destroy();
		imageLoader.Load(filePath);
		assert(!imageLoader.IsNull()); // failed to load image file or Param

		return GetImageParam(imageLoader);
	}

	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information with Mapped GPU TextureID  </returns>
	SLImageParam UploadImageToGPUFromDisk(CImage& imageLoader, const TCHAR* filePath)
	{
		// Get basic image info
		SLImageParam textureParam = LoadImageParam(imageLoader, filePath);
		// Then upload to GPU and update the newly generated TextureID into textureParam
		UploadLinearImageToGPU(textureParam);

		return textureParam;
	}

} // End of namespace sldip
