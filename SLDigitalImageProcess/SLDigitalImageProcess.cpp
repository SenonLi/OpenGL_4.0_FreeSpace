#include "stdafx.h"
#include "SLDigitalImageProcess.h"

#include "SLImageParam.h"

void SLDigitalImageProcess::PreImageProcess()
{
	m_ImagePath = _T("../WatchMe/Images/Einstein.jpg");
	std::wstring saveFolderPath = _T("C:/Developer/Processed Images/");

	// Get basic image info
	m_ImageParam = sldip::LoadImageParam(m_ImageLoader, m_ImagePath);
	m_WidgetWidth = m_ImageParam.Width();
	m_WidgetHeight = m_ImageParam.Height();

	// Process Images
	sldip::SaveToImageFile(m_ImageLoader, saveFolderPath, _T("Origin.png"), Gdiplus::ImageFormatPNG);
	sldip::HistorgramEqualization(m_ImageParam);
	sldip::SaveToImageFile(m_ImageLoader, saveFolderPath, _T("HistogramEqualization.png"), Gdiplus::ImageFormatPNG);
}

void SLDigitalImageProcess::initialBackgroundTexture()
{
	// Then upload to GPU and update the newly generated TextureID into textureParam
	UploadLinearImageToGPU(m_ImageParam);

	defaultTextureID = m_ImageParam.TextureID();
}

namespace sldip
{
	/// <summary>Save CImage To Disk Image File </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& filePath, GUID imageType)
	{
		HRESULT result = imageLoader.Save(filePath.c_str(), imageType);
		assert(SUCCEEDED(result));
	}
	void SaveToImageFile(const CImage& imageLoader, const std::wstring& folderPath, const std::wstring& fileName, GUID imageType)
	{
		SaveToImageFile(imageLoader, (folderPath + fileName), imageType);
	}

	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remarks> Will be called by UploadImageToGPUFromDisk, should be put infront of it to avoid pre-declaration
	///               Designed to upload CPU-Processed image, which has to be linear     </remarks>
	/// <param name="textureParam">Offer image information; Get generated TextureID [IN/OUT]</param>
	void UploadLinearImageToGPU(SLImageParam& textureParam)
	{
		assert(textureParam.LinearBufferEntry());

		// Generate and Bind TextureID
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		textureParam.SetTextureID(textureID);

		// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set defaultTextureID filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load textureImage, create defaultTextureID and generate mipmaps
		switch (textureParam.ColorType())
		{
		case SLImageColorType::ColorGray:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureParam.Width(), textureParam.Height(), 0, GL_RED, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case SLImageColorType::ColorRGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureParam.Width(), textureParam.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		case SLImageColorType::ColorRGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParam.Width(), textureParam.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureParam.LinearBufferEntry());
			break;
		default:
			assert(false);// Doesn't support the others ColorType yet
		}

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WidgetWidth, m_WidgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);  // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
		assert(textureParam.TextureID()); // New generated TextureID should not be 0
	}

	/// <summary>Read picture file from Disk, and return SLImageParam </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information, except Mapped GPU TextureID  </returns>
	SLImageParam& LoadImageParam(CImage& imageLoader, const TCHAR* filePath)
	{
		assert(filePath && _tcsclen(filePath) != 0);
		// assert(glew already initialed); // GLEW doesn't require UI, should be added in sldip
		if (!imageLoader.IsNull())
			imageLoader.Destroy();
		imageLoader.Load(filePath);
		assert(!imageLoader.IsNull()); // failed to load image file

		SLImageParam textureParam;
		textureParam.SetWidth(imageLoader.GetWidth());
		textureParam.SetHeight(imageLoader.GetHeight());
		textureParam.SetPitch(imageLoader.GetPitch());
		textureParam.SetImageColorType(imageLoader.GetBPP() / BIT_NUM_IN_ONE_BYTE);

		// CImage belongs to Windows GDI library, in which all DIBs (Device-Independent Bitmap) are bottom-up.
		// GetBits() will return pixel address of (0,0) instead of the first byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, here we need to move pointer up [Height() - 1] levels.
		BYTE* firstPixelAddress = static_cast<BYTE*>(imageLoader.GetBits());
		BYTE* bufferEntry = firstPixelAddress + (textureParam.Height() - 1) * textureParam.Pitch();
		textureParam.SetLinearBufferEntry(bufferEntry);

		return textureParam;
	}

	/// <summary>Read picture file from Disk, and upload to GPU </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information with Mapped GPU TextureID  </returns>
	SLImageParam& UploadImageToGPUFromDisk(CImage& imageLoader, const TCHAR* filePath)
	{
		// Get basic image info
		SLImageParam textureParam = LoadImageParam(imageLoader, filePath);
		// Then upload to GPU and update the newly generated TextureID into textureParam
		UploadLinearImageToGPU(textureParam);

		return textureParam;
	}

	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <param name="image">Beginning address of image buffer [IN/OUT]</param>
	/// <param name="channels">RGBA, or RGB, or R</param>
	void HistorgramEqualization(const SLImageParam& textureParam)
	{
		BYTE* imageBufferEntry = textureParam.LinearBufferEntry();
		assert(imageBufferEntry);
		int imageWidth = textureParam.Width();
		int imageHeight = textureParam.Height();
		int imageChannels = textureParam.ChannelNumber();
		int imageAbsPitch = textureParam.PitchAbsolute();

		int cdf[CPU_TOTAL_GRAY_LEVEL]; // Array of Cumulative Distributive Function at each gray level
		std::memset(cdf, 0, sizeof(cdf));

		// Get Cumulative Distributive Function
		for (int row = 0; row < imageHeight; ++row)	{
			for (int column = 0; column < imageWidth; ++column)	{
				int pixelGrayLevel = imageBufferEntry[row * imageAbsPitch + column * imageChannels]; // grayLevel == pixel intensity == Red
				cdf[pixelGrayLevel] += 1;
			}
		}
		// find valid min and max of grayLevel
		int minValidGrayLevel = 0;
		int maxValidGrayLevel = CPU_TOTAL_GRAY_LEVEL;
		int sumCDF = 0;
		float totalPixel = imageWidth * imageHeight * 1.0f;
		for (int grayLevel = 0; grayLevel < CPU_TOTAL_GRAY_LEVEL; ++grayLevel)
		{
			sumCDF += cdf[grayLevel];
			float lambda = sumCDF / totalPixel;
			if      (lambda < VALID_HISTOGRAM_FLOOR_RATIO)          minValidGrayLevel = grayLevel;
			else if (lambda < VALID_HISTOGRAM_CEIL_RATIO)           maxValidGrayLevel = grayLevel;
		}
		// Re-scale imageBufferEntry
		for (int row = 0; row < imageHeight; ++row) {
			for (int column = 0; column < imageWidth; ++column)	{
					// ImagePixel on CPU consists of RGBA 4 chanels, each represented by 1 unsigned byte under range 0~255
					int curPixelEntry = row * imageAbsPitch + column * imageChannels;
					unsigned int oldIntensity = imageBufferEntry[curPixelEntry];
					unsigned int newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;

					// Calculate New gray-scaled Color Intensity
					if (oldIntensity <= minValidGrayLevel) {
						newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;
					}else if (oldIntensity > maxValidGrayLevel) {
						newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE;
					}else {
						newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE * (oldIntensity - minValidGrayLevel) / (maxValidGrayLevel - minValidGrayLevel);
					}

					// Assign New gray-scaled Color Intensity
					for (int i = 0; i < imageChannels; ++i) {
						imageBufferEntry[curPixelEntry + i] = newIntensity;
					}
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	glDisable(GL_BLEND);
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
		1.0f,    1.0f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, 1.0f, // Top Right
		1.0f,   -1.0f,   0.0f,   0.0f,   1.0f,   0.0f,   1.0f, 0.0f, // Bottom Right
		-1.0f,  -1.0f,   0.0f,   0.0f,   0.0f,   1.0f,   0.0f, 0.0f, // Bottom Left
		-1.0f,   1.0f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f, 1.0f  // Top Left 
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

void SLDigitalImageProcess::bindNewLayerTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "newLayerTexture");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(programA, "newLayerTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//glEnable(GL_TEXTURE_2D);
}

void SLDigitalImageProcess::bindBackgroundTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "backgroundTexture");
	glActiveTexture(GL_TEXTURE1);
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
