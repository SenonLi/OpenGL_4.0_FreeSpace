#include "Sen_07_TextureGLFW.h"


Sen_07_TextureGLFW::Sen_07_TextureGLFW()
{
	strWindowName = "Sen GLFW Texture";
}

Sen_07_TextureGLFW::~Sen_07_TextureGLFW()
{
}

void Sen_07_TextureGLFW::paintGL(void)
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

void Sen_07_TextureGLFW::initGlfwGlewGL()
{
	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/awesomeface.png", &widgetWidth, &widgetHeight, 0, SOIL_LOAD_RGB);
	textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/SenPortrait.jpg", &widgetWidth, &widgetHeight, 0, SOIL_LOAD_RGBA);
	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/apple.jpg", &widgetWidth, &widgetHeight, 0, SOIL_LOAD_RGBA);

	SenAbstractGLFW::initGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_07_TextureGLFW.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);
	
	
	initialVertices();
	initialBackgroundTexture();
	initialNewLayerTexture();

	OutputDebugString(" Initial GLFW Texture\n\n");
}

void Sen_07_TextureGLFW::initialVertices()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions        // Colors         // Texture Coords
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top Right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Bottom Left
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f  // Top Left 
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

void Sen_07_TextureGLFW::initialNewLayerTexture()
{
	int newLayerWidth, newLayerHeight;

	glGenTextures(1, &newLayerTexture);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/awesomeface.png", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newLayerWidth, newLayerHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0);// unbind when done
}

void Sen_07_TextureGLFW::initialBackgroundTexture()
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
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widgetWidth, widgetHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImagePtr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widgetWidth, widgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
}

void Sen_07_TextureGLFW::bindNewLayerTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "newLayerTexture");
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(programA, "newLayerTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//glEnable(GL_TEXTURE_2D);
}

void Sen_07_TextureGLFW::bindBackgroundTexture()
{
	// Bind Texture
	textureLocation = glGetUniformLocation(programA, "backgroundTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void Sen_07_TextureGLFW::finalize(void)
{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	if (glIsTexture(newLayerTexture))			glDeleteTextures(1, &newLayerTexture);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}