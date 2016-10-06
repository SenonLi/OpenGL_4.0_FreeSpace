#include "Sen_10_Camera.h"


Sen_10_Camera::Sen_10_Camera()
{
	strWindowName = "Sen GLFW CameraModel";

}


Sen_10_Camera::~Sen_10_Camera()
{
}


void Sen_10_Camera::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);




	//updateModelViewProjection();

	//// Draw Background Texture
	//bindBackgroundTexture();
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//bindNewLayerTexture();
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));

	//glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

	//glBindVertexArray(0);

	//glBindTexture(GL_TEXTURE_2D, 0);




	glBindVertexArray(verArrObjArray[0]);

	updateModelViewProjection();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glUniform1i(glGetUniformLocation(programA, "ourTexture1"), 0);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

	//// Activate shader
	//ourShader.Use();

	// Create transformations
	glm::mat4 identityMatrix;
	model = glm::rotate(identityMatrix, (GLfloat)glfwGetTime() * float(glm::radians(50.0)), glm::vec3(-1.0f, 1.0f, 1.0f));

	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projection = glm::perspective(45.0f, (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.1f, 100.0f);


	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(programA, "model");
	GLint viewLoc = glGetUniformLocation(programA, "view");
	GLint projLoc = glGetUniformLocation(programA, "projection");
	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	//updateModelViewProjection();

	// Draw container
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glUniform1i(glGetUniformLocation(programA, "ourTexture1"), 0);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));

	glUniform1i(glGetUniformLocation(programA, "ourTexture2"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, defaultTexture);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);

}

void Sen_10_Camera::finalize(void)
{
	glDeleteVertexArrays(1, verArrObjArray);
	glDeleteBuffers(1, verBufferObjArray);
}


void Sen_10_Camera::initModelViewProjection()
{
	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projection = glm::perspective(float(glm::radians(60.0)), (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.0f, 100.0f);

	model = glm::rotate(model, float(glm::radians(-45.0)), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));
}

void Sen_10_Camera::updateModelViewProjection()
{
	// Create transformations
	//model = glm::rotate(model, (GLfloat)glfwGetTime() * float(glm::radians(10.0)), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 identityMatrix;
	model = glm::rotate(identityMatrix, (GLfloat)glfwGetTime() * float(glm::radians(50.0)), glm::vec3(-1.0f, 1.0f, 1.0f));

	// Pass them to the shaders
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Sen_10_Camera::initialGlfwGlewGL()
{
	textureImage = SOIL_load_image("./LearnOpenGL_GLFW/Images/SenSqaurePortrait.jpg", &widgetWidth, &widgetHeight, 0, SOIL_LOAD_RGBA);

	SenAbstractGLFW::initialGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_10_Camera.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_10_Camera.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);

	initialVertices();

	initialBackgroundTexture();


	int width, height;
	unsigned char* image;

	// ===================
	// Texture 2
	// ===================
	glGenTextures(1, &newLayerTexture);
	glBindTexture(GL_TEXTURE_2D, newLayerTexture);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = SOIL_load_image("./LearnOpenGL_GLFW/Images/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	initModelViewProjection();

	OutputDebugString(" Initial GLFW Texture\n\n");
}

void Sen_10_Camera::initialBackgroundTexture()
{
	// Load and create a defaultTexture 
	glGenTextures(1, &defaultTexture);
	glBindTexture(GL_TEXTURE_2D, defaultTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTexture object
	//// Set the defaultTexture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTexture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set defaultTexture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load textureImage, create defaultTexture and generate mipmaps
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widgetWidth, widgetHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widgetWidth, widgetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImage);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTexture when done, so we won't accidentily mess up our defaultTexture.
}

void Sen_10_Camera::initialVertices()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions           // Texture Coords
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Front Top Right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Front Bottom Right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Front Bottom Left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Front Top Left 

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // Back Top Right
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Back Bottom Right
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Back Bottom Left
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // Back Top Left 

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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

