#include "../stdafx.h"
#include "SLTexture2D_Renderer.h"

#include "SLImageParam.h"

namespace slgeom
{
	SLTexture2D_Renderer::SLTexture2D_Renderer()
	{
	}
	
	SLTexture2D_Renderer::~SLTexture2D_Renderer()
	{

		m_TextureID = 0;

	}

	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remarks> Will be called by UploadImageToGPUFromDisk, should be put infront of it to avoid pre-declaration
	///               Designed to upload CPU-Processed image, which has to be linear     </remarks>
	/// <param name="textureParam">Offer image information; Get generated TextureID [IN/OUT]</param>
	/// <return>TextureID of textureParam </return>
	GLuint SLTexture2D_Renderer::UploadLinearImageToGPU(sldip::SLImageParam& textureParam)
	{
		assert(textureParam.LinearBufferEntry());

		// Generate and Bind TextureID
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object

		// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set defaultTextureID filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load textureImage, create defaultTextureID and generate mipmaps
		switch (textureParam.ColorType())
		{
		case sldip::SLLibreColorType::LibreColorGray:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureParam.Width(), textureParam.Height(), 0, GL_RED, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case sldip::SLLibreColorType::LibreColorRGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case sldip::SLLibreColorType::LibreColorRGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case sldip::SLLibreColorType::LibreColorBGR:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case sldip::SLLibreColorType::LibreColorBGRA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		default:
			assert(false);// Doesn't support the others ColorType yet
		}

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WidgetWidth, m_WidgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);  // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
		assert(textureID);
		return textureID;
	} // End of UploadLinearImageToGPU



}// End of namespace slgeom
