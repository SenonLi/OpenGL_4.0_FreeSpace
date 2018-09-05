#include "../../stdafx.h"
#include "../../Public/Rendering/SLOpenGLPaintBasicss.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include "SLImageParam.h"
#include "SLLibreImage.h"
#include "StaticConstBasics\SLGeneralImageBasics.h"

namespace slgeom
{
	int SLOpenGLTextureID::m_InstanceCounter = 0; // Declaration of private static counter

	SLOpenGLTextureID::~SLOpenGLTextureID()
	{
		Reset();
	}
	void SLOpenGLTextureID::Reset()
	{
		if (m_LinearImageBufferEntry != nullptr)
		{
			// A valid OpenGL context is required !
			assert(wglGetCurrentContext());

			// m_TextureID must be initialed along with m_ImageBufferEntry
			assert(glIsTexture(m_TextureID));
			glDeleteTextures(1, &m_TextureID);
			m_TextureID = 0;
			m_LinearImageBufferEntry = nullptr;

			m_InstanceCounter--;
			assert(m_InstanceCounter >= 0);
			std::wstringstream outputMessage;
			outputMessage << _T("SLOpenGLTextureID::Reset(), There exist now \t") << m_InstanceCounter << _T(" \t SLOpenGLTextureID alive.\n");
			SLOutputDebugString(outputMessage.str().c_str());
		}
	}

	void SLOpenGLTextureID::SetTextureID(GLuint textureID) {
		// textureID is expected to be successfully initialed
		assert(textureID);
		m_TextureID = textureID;

		m_InstanceCounter++;
		std::wstringstream outputMessage;
		outputMessage << _T("SLOpenGLTextureID::SetTextureID(), There exist now \t") << m_InstanceCounter << _T(" \t SLOpenGLTextureID alive.\n");
		SLOutputDebugString(outputMessage.str().c_str());
	}

	void SLOpenGLTextureID::SetLinearBufferEntry(const BYTE* bufferEntry) {
		// bufferEntry is expected to be initialed
		assert(bufferEntry);
		m_LinearImageBufferEntry = bufferEntry;
	}

	bool SLOpenGLTextureID::IsReadyForRendering() const
	{
		bool isReady = false;
		if (m_LinearImageBufferEntry) {
			assert(m_TextureID);// m_TextureID should not be zero
			isReady = true;
		}else {
			assert(m_TextureID == 0); // m_TextureID should be zero
			isReady = false;
		}
		return isReady;
	}


	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remarks> Will be called by UploadImageToGPUFromDisk, should be put infront of it to avoid pre-declaration
	///               Designed to upload CPU-Processed image, which has to be linear     </remarks>
	/// <param name="textureParam">Offer image information; Get generated TextureID [IN/OUT]</param>
	/// <return>TextureID of textureParam </return>
	GLuint UploadLinearImageToGPU(const sldip::SLImageParam& textureParam)
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
			glPixelStorei(GL_UNPACK_ALIGNMENT, slutil::SL_DEFAULT_STORAGE_PACKING_AlignmentSize);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			glPixelStorei(GL_PACK_ALIGNMENT, slutil::SL_MIN_STORAGE_PACKING_AlignmentSize);
			break;
		case sldip::SLLibreColorType::LibreColorRGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case sldip::SLLibreColorType::LibreColorBGR:
			glPixelStorei(GL_UNPACK_ALIGNMENT, slutil::SL_DEFAULT_STORAGE_PACKING_AlignmentSize);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			glPixelStorei(GL_PACK_ALIGNMENT, slutil::SL_MIN_STORAGE_PACKING_AlignmentSize);
			break;
		case sldip::SLLibreColorType::LibreColorBGRA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		default:
			assert(false);// Doesn't support the others ColorType yet
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);  // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
		assert(textureID);
		return textureID;
	} // End of UploadLinearImageToGPU



} // End of namespace slgeom
