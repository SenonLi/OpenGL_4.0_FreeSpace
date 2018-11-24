#include "../../stdafx.h"

#include "glad/glad.h" // Support basic OpenGL

#include "../../Public/Rendering/SLTexture2D_Renderer.h"
#include "../../Public/Paintables/SLLibreImagePaintable.h"
#include "SLShaderProgram.h"
#include "../Resources/SLGLShadersResource.h"

static const int TEXTURE_2D_COORDS_VERT = IDR_SLTEXTURE_2D_COORDS_VERT;
static const int TEXTURE_2D_COORDS_FRAG = IDR_SLTEXTURE_2D_COORDS_FRAG;

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
		PipelineShaders shaders[] = {
			{ GL_VERTEX_SHADER, TEXTURE_2D_COORDS_VERT },
			{ GL_FRAGMENT_SHADER, TEXTURE_2D_COORDS_FRAG },
			{ GL_NONE, NULL }
		};
		m_TextureShaderProgram = CreatePipelineShaderProgram(shaders);
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
	void SLTexture2D_Renderer::UploadLinearImageToGPU(SLLibreImagePaintable& libreImagePaintable)
	{
		// Make sure image data is not over up-loaded, nor miss uploaded
		if ( !libreImagePaintable.IsGLTextureID_Empty() && !libreImagePaintable.IsGLTextureID_OutDated() ) {
			// ImageBuffer Already been uploaded, No need to re-upload
			assert(libreImagePaintable.GetGLTextureID().lock()->IsReadyForRendering());
		}else {
			// Run into here, it is necessary to upload new ImageBuffer and create new GLTextureID anyway
			
			// First, check if there is outdated textureID waiting to be free from GPU
			if (!libreImagePaintable.IsGLTextureID_Empty())	{
				// We should erase the outdated image textureID from GPU
				std::shared_ptr<SLOpenGLTextureID> oldGLTextureID = libreImagePaintable.GetGLTextureID().lock();
				assert(oldGLTextureID && m_GLTextureIDSet.find(oldGLTextureID) != m_GLTextureIDSet.end() );
				m_GLTextureIDSet.erase(oldGLTextureID);
			}

			// Then, Upload the new ImageBuffer upto GPU now
			std::shared_ptr<SLOpenGLTextureID> slGLTextureID = std::make_shared<SLOpenGLTextureID>();
			GLuint textureID = slgeom::UploadLinearImageToGPU(libreImagePaintable.GetImageParam());
			slGLTextureID->SetTextureID(textureID);
			slGLTextureID->SetLinearBufferEntry(libreImagePaintable.GetBufferEntry());

			m_GLTextureIDSet.insert(slGLTextureID);
			libreImagePaintable.SetGLTextureID(std::weak_ptr<SLOpenGLTextureID>(slGLTextureID));
		}
	}// End of UploadLinearImageToGPU(SLLibreImagePaintable& libreImagePaintable)

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
