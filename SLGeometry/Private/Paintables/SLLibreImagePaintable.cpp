#include "../../stdafx.h"
#include "../../Public/Paintables/SLLibreImagePaintable.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include "../../Public/Rendering/SLOpenGLPaintBasicss.h"

namespace slgeom
{

	SLLibreImagePaintable::SLLibreImagePaintable(const SLLibreImage& libreImage)
		: SLLibreImage(libreImage)
	{
		assert(!libreImage.IsNull()); // For Paintable creation,  libreImage should not be empty
	};

	/// <remark>Reset both SLLibreImage and m_GLTextureID, unload </remark>
	void SLLibreImagePaintable::Reset()
	{
		if (!IsNull())
		{
			SLLibreImage::Reset();
			m_GLTextureID.reset();
		}
	}

	/// <summary>Will automatically reset m_GLTextureID no matter it was already initialed or not </summary>
	/// <remark>Call this function if and only if it is necessary (and ready) to re-upload image to GPU </remark>
	void SLLibreImagePaintable::InitialGLTextureID()
	{
		m_GLTextureID = std::make_shared<SLOpenGLTextureID>();
	}

	//===================================================================================================================================
	//-----------------------------------------------------------------------------------------------------------------------------------
	bool SLLibreImagePaintable::IsUploadToGPUNecessary() const
	{
		bool isNeccessary = false;

		if (m_GLTextureID == nullptr || m_GLTextureID->GetLinearBufferEntry() != this->GetBufferEntry())
			isNeccessary = true;
		else
			assert(m_GLTextureID->IsReadyForRendering());//, L"Invalid GLTextureID, code must be reviewed!!"

		return isNeccessary;
	}


	/// <summary> Set new created textureID along with corresponding CPU linearImageBufferEntry  </summary>
	/// <remark>This public function is supposed to be called right after the image data uploading to GPU in a textureRenderer </remark>
	void SLLibreImagePaintable::SetGLTextureID(GLuint textureID, const BYTE* linearImageBufferEntry)
	{
		// m_GLTextureID should be just initialed, but has nothing
		assert(m_GLTextureID && m_GLTextureID->GetLinearBufferEntry() == nullptr && m_GLTextureID->GetTextureID() == 0);
		assert(textureID && linearImageBufferEntry); // L"Invalid the input parameters !!"

		m_GLTextureID->SetTextureID(textureID);
		m_GLTextureID->SetLinearBufferEntry(linearImageBufferEntry);
	}


	//===================================================================================================================================
	//-----------------------------------------------------------------------------------------------------------------------------------
	GLuint SLLibreImagePaintable::GetTextureID() const
	{
		return m_GLTextureID->GetTextureID();
	}



} // End of namespace slgeom