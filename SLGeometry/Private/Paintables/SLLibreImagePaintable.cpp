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

	//===================================================================================================================================
	//-----------------------------------------------------------------------------------------------------------------------------------
	bool SLLibreImagePaintable::IsGLTextureID_Empty() const
	{
		return m_GLTextureID.lock() == nullptr;
	}
	bool SLLibreImagePaintable::IsGLTextureID_OutDated() const
	{
		// Always test SLLibreImagePaintable::IsGLTextureID_Empty before calling this function
		assert(m_GLTextureID.lock());
		return m_GLTextureID.lock()->GetLinearBufferEntry() != this->GetBufferEntry();
	}


	/// <summary> Set new created textureID along with corresponding CPU linearImageBufferEntry  </summary>
	/// <remark>This public function is supposed to be called right after the image data uploading to GPU in a textureRenderer </remark>
	void SLLibreImagePaintable::SetGLTextureID(const std::weak_ptr<SLOpenGLTextureID>& glTextureID)
	{
		// m_GLTextureID at first already been reset anyway, and glTextureID should already be just initialed
		assert(m_GLTextureID.lock() == nullptr);
		assert(glTextureID.lock() && glTextureID.lock()->GetLinearBufferEntry() && glTextureID.lock()->GetTextureID() != 0);

		m_GLTextureID = glTextureID;
	}

	const std::weak_ptr<SLOpenGLTextureID>&  SLLibreImagePaintable::GetGLTextureID()  const
	{
		return m_GLTextureID;
	}

	//===================================================================================================================================
	//-----------------------------------------------------------------------------------------------------------------------------------
	GLuint SLLibreImagePaintable::GetTextureID() const
	{
		assert(m_GLTextureID.lock());
		return m_GLTextureID.lock()->GetTextureID();
	}



} // End of namespace slgeom