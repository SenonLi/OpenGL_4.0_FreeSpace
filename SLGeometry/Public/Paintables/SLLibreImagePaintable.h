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
	///          SLLibreImagePaintable is designed for Graphic Painting     </summary>
	class SLLibreImagePaintable : public sldip::SLLibreImage
	{
	public:
		SLLibreImagePaintable() {}; // Default constructor for member instancing
		explicit SLLibreImagePaintable(const SLLibreImage& libreImage); // Always Create SLLibreImage first, then create SLLibreImagePaintable
		void Reset() override;

		/// <summary>Will automatically reset m_GLTextureID no matter it was already initialed or not </summary>
		/// <remark>Call this function if and only if it is necessary to re-upload image to GPU </remark>
		void InitialGLTextureID();

		bool IsUploadToGPUNecessary() const;
		void SetGLTextureID(GLuint textureID, const BYTE* linearImageBufferEntry);
		GLuint GetTextureID() const;

	private:
		/// <summary>Shared TextureID along with corresponding LibreImageBufferEntry </summary>
		/// <remark>  Considering that image data is shared, and multiple SLLibreImagePaintable with same image content 
		///      should should also share textureID, so that the same image will be uploaded to GPU for only once;
		///    1. To avoid multiple GPU uploading, we use shared_ptr;
		///    2. To avoid unused shared_ptr initial, call InitialGLTextureID() only when preparing for texture painting </remark>
		std::shared_ptr<SLOpenGLTextureID> m_GLTextureID = nullptr;

	};



} // End of namespace slgeom

#endif // __SLLIBREIMAGEPAINTABLE__
