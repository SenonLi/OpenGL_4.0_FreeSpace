#include "../../stdafx.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include "../../Public/Rendering/SLTexture2D_Renderer.h"
#include "../../Public/Paintables/SLLibreImagePaintable.h"
#include "LoadShaders.h"

namespace slgeom
{
	SLTexture2D_Renderer::SLTexture2D_Renderer()
	{
		InitShadersAndVertices();
	}
	
	SLTexture2D_Renderer::~SLTexture2D_Renderer()
	{
		ResetShadersAndVertices();
	}

	void SLTexture2D_Renderer::InitShadersAndVertices()
	{
		// Prepare ShaderProgram
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, m_Texture2DShaderVert.c_str() },
			{ GL_FRAGMENT_SHADER, m_Texture2DShaderFrag.c_str() },
			{ GL_NONE, NULL }
		};
		m_TextureShaderProgram = LoadShaders(shaders);
		assert(m_TextureShaderProgram); // Failed if assert

		// Prepare Vertices
		InitVertexArrayObject();
	}

	void SLTexture2D_Renderer::InitVertexArrayObject()
	{
		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			// OpenGL Positions        // Image Coords
			 1.0f,    1.0f,   0.0f,     1.0f, 0.0f, // Top Right
			 1.0f,   -1.0f,   0.0f,     1.0f, 1.0f, // Bottom Right
			-1.0f,   -1.0f,   0.0f,     0.0f, 1.0f, // Bottom Left
			-1.0f,    1.0f,   0.0f,     0.0f, 0.0f  // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};

		glGenVertexArrays(1, &m_VertexArrayObject);
		glGenBuffers(1, &m_VertexBufferObject);
		glGenBuffers(1, &m_IndicesBufferObject);
		glBindVertexArray(m_VertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
		glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	}

	void SLTexture2D_Renderer::ResetShadersAndVertices()
	{
		if (glIsVertexArray(m_VertexArrayObject))
			glDeleteVertexArrays(1, &m_VertexArrayObject);
		if (glIsBuffer(m_VertexBufferObject))
			glDeleteBuffers(1, &m_VertexBufferObject);
		if (glIsBuffer(m_IndicesBufferObject))
			glDeleteBuffers(1, &m_IndicesBufferObject);

		if (glIsProgram(m_TextureShaderProgram))
			glDeleteProgram(m_TextureShaderProgram);
	}

	/// <summary>Upload image to GPU, and output GLTextureID with Initialed TextureID </summary>
	/// <param name="libreImagePaintable">Contains LibreImage to upload onto GPU [IN];
	///   along with m_GLTextureID [OUT] waiting to be updated, the new created textureID from GPU, corresponding CPU image bufferEntry [IN/OUT]</param>
	/// <remark>LibreImage image data may change frequently before being painted;
	/// to make sure we never miss new imageData uploading, and never over uploading the same image data, check before uploading !! </remark>
	void SLTexture2D_Renderer::UploadLinearImageToGPU(SLLibreImagePaintable& libreImagePaintable) const
	{
		// Make sure image data is not over up-loaded, nor miss uploaded
		if (libreImagePaintable.IsUploadToGPUNecessary())
		{
			libreImagePaintable.InitialGLTextureID();
			GLuint textureID = slgeom::UploadLinearImageToGPU(libreImagePaintable.GetImageParam());
			libreImagePaintable.SetGLTextureID(textureID, libreImagePaintable.GetBufferEntry());
		}
	}

	void SLTexture2D_Renderer::PaintTexture2D(GLuint textureID) const
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// Bind ShaderProgram
		glUseProgram(m_TextureShaderProgram);

		// Bind target Texture
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(m_TextureShaderProgram, "textureName"), 1);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Bind VertexArrayObject
		glBindVertexArray(m_VertexArrayObject);

		// Paint Texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Reset GL status
		glBindVertexArray(0);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}// End of namespace slgeom
