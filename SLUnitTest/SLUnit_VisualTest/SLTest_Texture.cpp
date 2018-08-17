#include "stdafx.h"
#include "SLTest_Texture.h"

#include "SLDigitalImageProcess.h"
#include "SLImageParam.h"

void SLTest_Texture::initialBackgroundTexture()
{
	// Then upload to GPU and update the newly generated TextureID into textureParam
	sldip::UploadLinearImageToGPU(m_ImageParam);

	defaultTextureID = m_ImageParam.TextureID();
}

SLTest_Texture::SLTest_Texture()
{
	strWindowName = "Sen UnitTest_Visual Test Texture ";
}

void SLTest_Texture::paintGL(void)
{
	SenAbstractGLFW::paintGL();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Draw container
	glUseProgram(programA);

	// Draw Background Texture
	bindBackgroundTexture();

	// Create transformations
	glm::mat4 transform;
	//transform = glm::translate(transform, glm::vec3(0.0f, -0.0f, -1.0f));
	transform = glm::rotate(transform, (GLfloat)glfwGetTime() * float(glm::radians(50.0)), glm::vec3(1.0f, 0.0f, 1.0f));

	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(programA, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(verArrObjArray[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SLTest_Texture::LoadLibreImage()
{
	m_ImagePath = _T("../../WatchMe/Images/Einstein 8bit.jpg"); //Einstein 8bit.jpg   Einstein.jpg    grass.png
	std::wstring saveFolderPath = _T("../../../../../Developer/Processed Images/");

	// GLPaint
	m_ImageParam = sldip::LoadImageParam(m_LibreImage, m_ImagePath);
	m_WidgetWidth = m_ImageParam.Width();
	m_WidgetHeight = m_ImageParam.Height();

	// Process Images
	sldip::SaveToImageFile(m_LibreImage, saveFolderPath, _T("Origin.png"), sldip::SLImageFileType::IMAGE_PNG);
	sldip::HistorgramEqualization(m_ImageParam);
	sldip::SaveToImageFile(m_LibreImage, saveFolderPath, _T("HistogramEqualization.png"), sldip::SLImageFileType::IMAGE_PNG);
}

void SLTest_Texture::initGlfwGlewGL()
{
	LoadLibreImage();

	// Then initial window size
	SenAbstractGLFW::initGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "../../vsSenOpenGL/LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.vert" },
		{ GL_FRAGMENT_SHADER, "../../vsSenOpenGL/LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);
	
	initialVertices();
	initialBackgroundTexture();

	SLOutputDebugString(_T(" Initial GLFW Texture\n\n"));
}

void SLTest_Texture::initialVertices()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// OpenGL Positions             // Colors               // Image Coords
		1.0f,    1.0f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, 0.0f, // Top Right
		1.0f,   -1.0f,   0.0f,   0.0f,   1.0f,   0.0f,   1.0f, 1.0f, // Bottom Right
		-1.0f,  -1.0f,   0.0f,   0.0f,   0.0f,   1.0f,   0.0f, 1.0f, // Bottom Left
		-1.0f,   1.0f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f, 0.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);
	glGenBuffers(1, verIndicesObjArray);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(verArrObjArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObjArray[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void SLTest_Texture::bindBackgroundTexture()
{
	// Bind Texture
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(programA, "backgroundTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void SLTest_Texture::finalize(void)
{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	if (glIsTexture(newLayerTexture))			glDeleteTextures(1, &newLayerTexture);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}
