#include "Sen_430_DebugOutput.h"


Sen_430_DebugOutput::Sen_430_DebugOutput()
{
	strWindowName = "Sen_430 Debug Output Test";

}

Sen_430_DebugOutput::~Sen_430_DebugOutput()
{
}



void Sen_430_DebugOutput::initGlfwGlewGL()
{
	SenFreeSpaceAbstract::initGlfwGlewGL();

	glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);



	// configure 3D cube
	GLfloat vertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right         
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		// front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		// left face
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, -1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f, -0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, -0.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, // top-right
		// right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		// top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right     
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f  // bottom-left        
	};

	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(verArrObjArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	// load cube texture
	int width, height;
	textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/SenSqaurePortrait.jpg", &width, &height, 0, SOIL_LOAD_RGBA);



	// Load and create a defaultTextureID 
	glGenTextures(1, &defaultTextureID);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object

	// Set the defaultTextureID wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set defaultTextureID filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load textureImage, create defaultTextureID and generate mipmaps
	glTexImage2D(GL_FRAMEBUFFER, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
	SOIL_free_image_data(textureImagePtr);

	SenGL_CheckError();

	// set up projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_WidgetWidth / (float)m_WidgetHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(glGetUniformLocation(programA, "backgroundTexture"), 0);

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_430_DebugOutput::paintFreeSpaceGL(void)
{
	glUseProgram(programA);

	GLfloat rotationSpeed = 10.0f;
	GLfloat angle = (float)glfwGetTime() * rotationSpeed;
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3(0.0, 0.0f, -2.5));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));

	//// set up projection matrix
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_WidgetWidth / (float)m_WidgetHeight, 0.1f, 100.0f);
	//glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniform1i(glGetUniformLocation(programA, "tex"), 0);


	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
	glBindVertexArray(verArrObjArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}


void Sen_430_DebugOutput::cleanFreeSpace(void)	{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	//if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}