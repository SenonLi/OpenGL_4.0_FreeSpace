#include "stdafx.h"
#include "SLDigitalImageProcess.h"

#include "SLImageObject.h"

void SLDigitalImageProcess::PreImageProcess()
{
	//const TCHAR* imagePath = _T("../WatchMe/Images/Einstein.jpg");
	textureImagePtr = SOIL_load_image("../WatchMe/Images/Einstein.jpg", &m_WidgetWidth, &m_WidgetHeight, 0, SOIL_LOAD_RGBA);
	//m_TargetImage.Load(imagePath);
	//m_WidgetWidth = m_TargetImage.GetWidth();
	//m_WidgetHeight = m_TargetImage.GetHeight();
	//textureImagePtr = (unsigned char*)m_TargetImage.GetBits();
	sldip::HistorgramEqualization(textureImagePtr, m_WidgetWidth, m_WidgetHeight, 4);

}

namespace sldip
{
	/// <summary>Read picture file from Disk, and upload to GPU [IN/OUT]</summary>
	/// <remakrs>CImage can process *.bmp, *.png or *.jpg </remakrs>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <param name="imageObject">Save image information + buffer pointer [OUT]</param>
	/// <returns>TextureID of uploaded image </returns>
	GLint UploadImageToGPUFromDisk(const TCHAR* filePath, SLImageObject& imageObject)
	{	
		assert(filePath && _tcsclen(filePath) != 0);

		CImage image;
		image.Load(filePath);



	}


	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <param name="image">Beginning address of image buffer [IN/OUT]</param>
	/// <param name="channels">RGBA, or RGB, or R</param>
	void HistorgramEqualization(unsigned char* image, int imageWidth, int imageHeight, int channels)
	{
		assert(image);
		int cdf[256];
		std::memset(cdf, 0, sizeof(cdf));

		// Get Cumulative Distributive Function
		for (int row = 0; row < imageHeight; ++row)
		{
			for (int column = 0; column < imageWidth; ++column)
			{
				int pixelGrayLevel = image[(row * imageWidth + column) * channels]; // grayLevel == pixel intensity == Red
				cdf[pixelGrayLevel] += 1;
			}
		}
		// find valid min and max of grayLevel
		int minValidGrayLevel = 0;
		int maxValidGrayLevel = 256;
		int sumCDF = 0;
		float totalPixel = imageWidth * imageHeight * 1.0;
		for (int grayLevel = 0; grayLevel < 256; ++grayLevel)
		{
			sumCDF += cdf[grayLevel];
			float lambda = sumCDF / totalPixel;// (imageWidth * imageHeight);
			if (lambda < VALID_HISTOGRAM_FLOOR_RATIO)
				minValidGrayLevel = grayLevel;
			else if (lambda < VALID_HISTOGRAM_CEIL_RATIO)
				maxValidGrayLevel = grayLevel;
		}
		// re-scale image
		for (int row = 0; row < imageHeight; ++row)
			for (int column = 0; column < imageWidth; ++column)
			{
				switch (channels)
				{
				case 3:
				{
					// ImagePixel on CPU consists of RGBA 4 chanels, each represented by 1 unsigned byte under range 0~255
					int pixelIndex = row * imageWidth + column;
					for (int i = 0; i < channels; ++i)
						image[pixelIndex * channels + i] = 0x00;
				}
				break;
				case 4:
				{
					// ImagePixel on CPU consists of RGBA 4 chanels, each represented by 1 unsigned byte under range 0~255
					int pixelIndex = row * imageWidth + column;
					unsigned int oldIntensity = image[pixelIndex * channels];
					unsigned int newIntensity = 0;
					if (oldIntensity <= minValidGrayLevel)
						newIntensity = 0;
					else if (oldIntensity > maxValidGrayLevel)
						newIntensity = 255;
					else
					{
						newIntensity = 255 * (oldIntensity - minValidGrayLevel) / (maxValidGrayLevel - minValidGrayLevel);
					}
					for (int i = 0; i < channels; ++i)
						image[pixelIndex * channels + i] = newIntensity;
				}
				break;
				default:
					assert(false);
				}
			}
	}
} // end of namespace sldip

SLDigitalImageProcess::SLDigitalImageProcess()
{
	strWindowName = "Sen Digital Image Process";
}

SLDigitalImageProcess::~SLDigitalImageProcess()
{
}

void SLDigitalImageProcess::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);

	// Draw Background Texture
	bindBackgroundTexture();
	bindNewLayerTexture();

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

	glBindTexture(GL_TEXTURE_2D, 0);
}

void SLDigitalImageProcess::initGlfwGlewGL()
{
	// Get image size as window size
	//textureImagePtr = SOIL_load_image("../WatchMe/Images/poor_3.bmp", &m_WidgetWidth, &m_WidgetHeight, 0, SOIL_LOAD_RGBA);
	PreImageProcess();
	// Then initial window size
	SenAbstractGLFW::initGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "../vsSenOpenGL/LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.vert" },
		{ GL_FRAGMENT_SHADER, "../vsSenOpenGL/LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);
	
	initialVertices();
	initialBackgroundTexture();
	initialNewLayerTexture();

	SLOutputDebugString(_T(" Initial GLFW Texture\n\n"));
}

void SLDigitalImageProcess::initialVertices()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions             // Colors               // Texture Coords
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

void SLDigitalImageProcess::initialNewLayerTexture()
{
	//int newLayerWidth, newLayerHeight;

	//glGenTextures(1, &newLayerTexture);
	//glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//// Set our texture parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//// Set texture filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// Load, create texture and generate mipmaps
	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/awesomeface.png", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newLayerWidth, newLayerHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImagePtr);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(textureImagePtr);
	//glBindTexture(GL_TEXTURE_2D, 0);// unbind when done


	glGenTextures(1, &newLayerTexture);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps


	gli::texture Texture = gli::load("../vsSenOpenGL/LearnOpenGL_GLFW/Images/pattern_02_bc2.ktx");

	assert(!Texture.empty());

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());
	bool a = gli::is_compressed(Texture.format());
	bool b = Target == gli::TARGET_2D;
	glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
	glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
	glTexStorage2D(Target, static_cast<GLint>(Texture.levels()), Format.Internal, Texture.extent().x, Texture.extent().y);
	for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glm::tvec3<GLsizei> Extent(Texture.extent(Level));
		glCompressedTexSubImage2D(
			Target, static_cast<GLint>(Level), 0, 0, Extent.x, Extent.y,
			Format.Internal, static_cast<GLsizei>(Texture.size(Level)), Texture.data(0, 0, Level));
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);// unbind when done
}

void SLDigitalImageProcess::initialBackgroundTexture()
{
	// Load and create a defaultTextureID 
	glGenTextures(1, &defaultTextureID);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
													//// Set the defaultTextureID wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set defaultTextureID filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load textureImage, create defaultTextureID and generate mipmaps
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_WidgetWidth, m_WidgetHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImagePtr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WidgetWidth, m_WidgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
}

void SLDigitalImageProcess::bindNewLayerTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "newLayerTexture");
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(programA, "newLayerTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//glEnable(GL_TEXTURE_2D);
}

void SLDigitalImageProcess::bindBackgroundTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "backgroundTexture");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(programA, "backgroundTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void SLDigitalImageProcess::finalize(void)
{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	if (glIsTexture(newLayerTexture))			glDeleteTextures(1, &newLayerTexture);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}
