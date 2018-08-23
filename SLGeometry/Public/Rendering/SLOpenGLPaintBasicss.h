#ifndef __SLOPENGLPAINTBASICS__
#define __SLOPENGLPAINTBASICS__
#pragma once

//-------------------------- GL Forward Declaration --------------------------------------
extern "C" {
	typedef unsigned int GLuint;
}

namespace sldip
{
	class SLImageParam;
}

namespace slgeom
{
	/// <summary>For Image Rendering, considering that SLOpenGLTextureID is related to GL textureID, 
	///           it is highly recommended to use std::shared_ptr<SLOpenGLTextureID> as member only. </summary>
	class SLOpenGLTextureID
	{
	public:
		~SLOpenGLTextureID(); // call Reset() to free texture memory on GPU
		GLuint GetTextureID() const { return m_TextureID; }
		void SetTextureID(GLuint textureID) { m_TextureID = textureID; }

		const BYTE* GetLinearBufferEntry() const { return m_LinearImageBufferEntry; }
		void SetLinearBufferEntry(const BYTE* bufferEntry) { m_LinearImageBufferEntry = bufferEntry; }

		bool IsReadyForRendering() const;
		void Reset();

	private:
		GLuint m_TextureID = 0; // 0 means Binding-Free for OpenGL driver, i.e., no texture binding for GPU 
		const BYTE* m_LinearImageBufferEntry = nullptr;
	};

	GLuint UploadLinearImageToGPU(const sldip::SLImageParam& textureParam);




}; // End of namespace slgeom

#endif // __SLOPENGLPAINTBASICS__