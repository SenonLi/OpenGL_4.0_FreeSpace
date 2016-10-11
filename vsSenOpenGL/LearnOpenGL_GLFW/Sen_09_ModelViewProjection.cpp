#include "Sen_09_ModelViewProjection.h"


Sen_09_ModelViewProjection::Sen_09_ModelViewProjection()
{
	strWindowName = "Sen Auto CameraView";

	widgetWidth = int(widgetWidth*1.5);
}


Sen_09_ModelViewProjection::~Sen_09_ModelViewProjection()
{
}


void Sen_09_ModelViewProjection::initialGlfwGlewGL()
{
	SenAbstractGLFW::initialGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_09_ModelViewProjection.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_09_ModelViewProjection.frag" },
		{ GL_NONE, NULL }
	};
	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
	//	{ GL_NONE, NULL }
	//};
	programA = LoadShaders(shaders);


	initialVertices();
	initialBackgroundTexture();
	initialNewLayerTexture();

	initModelViewProjection();


	OutputDebugString(" Initial GLFW Texture\n\n");
}

void Sen_09_ModelViewProjection::initialVertices()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions           // Texture Coords
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Front Top Right
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Front Bottom Right
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Front Bottom Left
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Front Top Left 

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // Back Top Right
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Back Bottom Right
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Back Bottom Left
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // Back Top Left 

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // Left Top Right
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Left Bottom Right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Left Bottom Left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Left Top Left 

		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Right Top Right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Right Bottom Right
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Right Bottom Left
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // Right Top Left 

		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Top Top Right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // Top Bottom Right
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // Top Bottom Left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Top Top Left 

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom Top Right
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Bottom Bottom Right
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Bottom Bottom Left
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f // Bottom Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // Front First Triangle
		1, 2, 3, // Front Second Triangle
		4, 5, 7, // Back First Triangle
		5, 6, 7,  // Back Second Triangle
		8, 9, 11, // Left First Triangle
		9, 10, 11, // Left Second Triangle
		12, 13, 15, // Right First Triangle
		13, 14, 15, // Right Second Triangle
		16, 17, 19, // Top First Triangle
		17, 18, 19, // Top Second Triangle
		20, 21, 23, // Bottom First Triangle
		21, 22, 23 // Bottom Second Triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void Sen_09_ModelViewProjection::initialNewLayerTexture()
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
	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/UKY.jpg", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGBA);
	textureImagePtr = SOIL_load_image("./../WatchMe/Images/UKY.jpg", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newLayerWidth, newLayerHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0);// unbind when done

	glGenTextures(1, &thirdLayerTexture);
	glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	textureImagePtr = SOIL_load_image("./../WatchMe/Images/Lau2.jpg", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGBA);

	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/Lau2.jpg", &newLayerWidth, &newLayerHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newLayerWidth, newLayerHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0);// unbind when done
}

void Sen_09_ModelViewProjection::initialBackgroundTexture()
{
	int width, height;
	//textureImagePtr = SOIL_load_image("./LearnOpenGL_GLFW/Images/SenSqaurePortrait.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	textureImagePtr = SOIL_load_image("./../WatchMe/Images/SenSqaurePortrait.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImagePtr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
}

void Sen_09_ModelViewProjection::initModelViewProjection()
{
	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projection = glm::perspective(float(glm::radians(60.0)), (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.1f, 100.0f);
	model = glm::rotate(model, float(glm::radians(-50.0)), glm::vec3(1.0f, 0.0f, 0.0f));
}

void Sen_09_ModelViewProjection::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);

	glBindVertexArray(verArrObjArray[0]);

	GLfloat radius = abs(viewCenter);
	GLfloat camX = float(sin(glfwGetTime()) * radius);
	GLfloat camZ = float(viewCenter + cos(glfwGetTime()) * radius);
	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, viewCenter), glm::vec3(0.0f, 1.0f, 0.0f));

	updateModelViewProjection();
	
	bindBackgroundTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	bindNewLayerTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
	glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));


	//model = glm::translate(model, cubePositions[i]);
	//GLfloat angle = 20.0f * i;
	//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	updateSecondModelViewProjection();
	bindBackgroundTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	bindNewLayerTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
	glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));


	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sen_09_ModelViewProjection::updateModelViewProjection()
{
	// Create transformations
	//model = glm::rotate(model, (GLfloat)glfwGetTime() * float(glm::radians(10.0)), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 identityMatrix;
	model = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, viewCenter));
	model = glm::rotate(model, (GLfloat)glfwGetTime() * float(glm::radians(90.0)), glm::vec3(-1.0f, 1.0f, 1.0f));

	//// Pass them to the shaders
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Sen_09_ModelViewProjection::updateSecondModelViewProjection()
{
	glm::mat4 identityMatrix;
	model = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, viewCenter));
	model = glm::rotate(model, (GLfloat)glfwGetTime() * float(glm::radians(-90.0)), glm::vec3(1.0f, 1.0f, -1.0f));

	//// Pass them to the shaders
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Sen_09_ModelViewProjection::bindNewLayerTexture()
{
	// Bind Texture
	//textureLocation = glGetUniformLocation(programA, "newLayerTexture");
	//glActiveTexture(GL_TEXTURE1);
	//glUniform1i(glGetUniformLocation(programA, "newLayerTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//glEnable(GL_TEXTURE_2D);
}

void Sen_09_ModelViewProjection::bindBackgroundTexture()
{
	// Bind Texture
	textureLocation = glGetUniformLocation(programA, "backgroundTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
	glUniform1f(textureLocation, 0);
}

void Sen_09_ModelViewProjection::finalize(void)
{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	if (glIsTexture(newLayerTexture))			glDeleteTextures(1, &newLayerTexture);
	if (glIsTexture(thirdLayerTexture))			glDeleteTextures(1, &thirdLayerTexture);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}