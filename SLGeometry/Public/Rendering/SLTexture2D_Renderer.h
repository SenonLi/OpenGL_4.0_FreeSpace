#ifndef __SLTEXTURE2D_RENDERER__
#define __SLTEXTURE2D_RENDERER__
#pragma once

#include "../../Private/Rendering/SLAbstractRenderer.h"

#include "SLOpenGLPaintBasicss.h" // For SLOpenGLTextureID
#include <memory>                 // For SLOpenGLTextureID actual saving, instancing m_GLTextureIDSet
#include <unordered_set>          // For SLOpenGLTextureID actual saving, instancing m_GLTextureIDSet

namespace slgeom
{
	class SLLibreImagePaintable;

	/// <summary>Contains Texture2D Shaders            --   will be from a series of classes,
	///                   Texture2D Vertices Infos     --   will be from a series of classes.
	///
	///       This Renderer will do OpenGL shader and vertices initialization in the constructor;
	///  considering that OpenGL may not be initialed, the constructor may fail when one uses member instancing,
	///  it is highly recommended that one use unique_ptr<Texture2DRenderer> as a member instead of instancing member directly. </summary>
	class SLTexture2D_Renderer : public SLAbstractRenderer
	{
	public:
		SLTexture2D_Renderer();
		virtual ~SLTexture2D_Renderer();

		void UploadLinearImageToGPU(SLLibreImagePaintable& libreImagePaintable);
		void PaintTexture2D(GLuint textureID) const;

	private:
		std::unordered_set<std::shared_ptr<SLOpenGLTextureID>> m_GLTextureIDSet;

		GLuint m_VertexArrayObject = 0;
		GLuint m_VertexBufferObject = 0;
		GLuint m_IndicesBufferObject = 0;
		GLuint m_TextureShaderProgram = 0;

		// Relative Address start from the project file .proj where instance SLTexture2D_Renderer, which is now in folder SLUnit_VisualTest
		const std::string m_Texture2DShaderVert = "../../SLGeometry/Private/Shaders/SLTexture2DCoords.vert";
		const std::string m_Texture2DShaderFrag = "../../SLGeometry/Private/Shaders/SLTexture2DCoords.frag";

		void InitVertexArrayObject();
		void InitShadersAndVertices();
		void ResetShadersAndVertices();
	};

}// End of namespace slgeom

#endif // __SLTEXTURE2D_RENDERER__