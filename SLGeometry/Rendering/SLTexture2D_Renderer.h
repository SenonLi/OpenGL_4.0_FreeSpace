#ifndef __SLTEXTURE2D_RENDERER__
#define __SLTEXTURE2D_RENDERER__
#pragma once

#include "SLAbstractRenderer.h"

namespace sldip
{
	class SLImageParam;
}

namespace slgeom
{
	class SLTexture2D_Renderer : public SLAbstractRenderer
	{
	public:
		SLTexture2D_Renderer();
		virtual ~SLTexture2D_Renderer();

		static GLuint UploadLinearImageToGPU(sldip::SLImageParam& textureParam);


		inline GLuint TextureID() const { return m_TextureID; }

	private:
		GLuint m_TextureID = 0; // 0 means Binding-Free for OpenGL driver, i.e., no texture binding for GPU 


	};

}// End of namespace slgeom

#endif // __SLTEXTURE2D_RENDERER__