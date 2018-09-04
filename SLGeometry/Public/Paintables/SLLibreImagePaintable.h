#ifndef __SLLIBREIMAGEPAINTABLE__
#define __SLLIBREIMAGEPAINTABLE__
#pragma once

#include "SLLibreImage.h"

extern "C" {
	typedef unsigned int GLuint;
}

namespace sldip
{
	class SLImageParam;
}

namespace slgeom
{
	class SLOpenGLTextureID;

	/// <summary>SLLibreImage is designed only for dip, saving image data;
	///          SLLibreImagePaintable is designed for Graphic Painting;
	///         and all related shared textureID will be saved in Texture2D_Renderer </summary>
	class SLLibreImagePaintable : public sldip::SLLibreImage
	{
	public:
		SLLibreImagePaintable() = default; // Default constructor for member instancing
		explicit SLLibreImagePaintable(const SLLibreImage& libreImage); // Always Create SLLibreImage first, then create SLLibreImagePaintable
		void Reset() override;

		bool IsGLTextureID_Empty() const;
		bool IsGLTextureID_OutDated() const;

		void SetGLTextureID(const std::weak_ptr<SLOpenGLTextureID>& glTextureID);
		const std::weak_ptr<SLOpenGLTextureID>&  GetGLTextureID()  const;

		GLuint GetTextureID() const;

	private:
		/// <summary>Shared TextureID along with corresponding LibreImageBufferEntry </summary>
		/// <remark>  Considering that image data is shared, and multiple SLLibreImagePaintable with same image content 
		///      should should also share textureID, so that the same image will be uploaded to GPU for only once;
		///    1. To avoid multiple GPU uploading, we use unordered_set<shared_ptr<SLOpenGLTextureID>> to save actual GLTextureID s;
		///    2. To bind textureID with corresponding image, we save weak_ptr<SLOpenGLTextureID> in SLLibreImagePaintable here;
		///    3. As long as the SLTexture2D_Renderer is alive, textureID would be alive; 
		///         we just need to destroy the SLTexture2D_Renderer before the Graphic Context shutdown;
		///    4. To pair GLTextureID, you should 
		///         a. Upload image to GPU under the help of SLTexture2D_Renderer;
		///         b. Initial a shared_ptr<SLOpenGLTextureID> and insert it into the SLTexture2D_Renderer unordered_set;
		///         c. Call SetGLTextureID to set the weak_ptr<SLOpenGLTextureID>.                                         </remark>
		std::weak_ptr<SLOpenGLTextureID> m_GLTextureID;

	};



} // End of namespace slgeom

#endif // __SLLIBREIMAGEPAINTABLE__
