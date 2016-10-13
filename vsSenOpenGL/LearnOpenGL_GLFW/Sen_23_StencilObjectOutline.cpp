#include "Sen_23_StencilObjectOutline.h"


Sen_23_StencilObjectOutline::Sen_23_StencilObjectOutline()
{
	similarCube = new Sen_Cube();
	strWindowName = "Sen_23 Stencil Test in FreeSpace";
}

Sen_23_StencilObjectOutline::~Sen_23_StencilObjectOutline()
{
}


void Sen_23_StencilObjectOutline::initialGlfwGlewGL()
{
	SenFreeSpaceAbstract::initialGlfwGlewGL();

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.frag" },
	//	{ GL_NONE, NULL }
	//};

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_23_StencilObjectOutline.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_23_StencilObjectOutline.frag" },
		{ GL_NONE, NULL }
	};

	ShaderInfo shadersOutline[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_23_StencilObjectOutline.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_ColorRed.frag" },
		{ GL_NONE, NULL }
	};

	programA = LoadShaders(shaders);
	programB = LoadShaders(shadersOutline);


	GLfloat planeVertices[] = {
		// Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(verArrObjArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/SenGrassGround3.jpg").c_str(), floorTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/SenGrassGround3,jpg").c_str(), floorTexture, std::string("RGB"));

	similarCube->initialCubeGL(programA, cubeTexture, cubeTexture, cubeTexture);

	camera.setCameraViewPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	glEnable(GL_STENCIL_TEST);
	// Check GL_STENCIL_TEST only when needed
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
	// Disable writing to the stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); 

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_23_StencilObjectOutline::paintFreeSpaceGL(void)
{
	// Paint Floor
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	//============      Object Outline        ==============================================================
	// ===== Clear Stencil Buffer as a beginning =========
	glStencilMask(0xFF); // Enable Stencil Writing for clearing
	glClearStencil(0x00);
	glClear(GL_STENCIL_BUFFER_BIT); // Clear Stencil Buffer
	
	// =====   Paint stencil of "one"s        =============
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Enable writing to the stencil buffer

	// Recover program, normal scaleratio Needed
	glUseProgram(programA);
	similarCube->changeNewLinkedCubeProgram(programA);
	similarCube->setCubeScaleRatio(glm::vec3(1.0f, 1.0f, 1.0f));

	similarCube->setCubeWorldAddress(firstCubePosition);
	similarCube->paintCube(projection, view);
	similarCube->setCubeWorldAddress(secondCubePosition);
	similarCube->paintCube(projection, view);

	// =====  Paint Outline based on stencil of "one"s   ====
	glStencilFunc(GL_NOTEQUAL, 0x01, 0xFF); // All fragments should update the
	glStencilMask(0x00); // Disable writing to the stencil buffer
	//glDisable(GL_DEPTH_TEST);

	// set outline program, and enlarged scaleRatio for outline painting
	glUseProgram(programB);
	similarCube->changeNewLinkedCubeProgram(programB);
	similarCube->setCubeScaleRatio(glm::vec3(1.06f, 1.06f, 1.06f));

	similarCube->setCubeWorldAddress(firstCubePosition);
	similarCube->paintCube(projection, view);
	similarCube->setCubeWorldAddress(secondCubePosition);
	similarCube->paintCube(projection, view);

	// =====  "Close stencil test" for other objects   ====
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); // Disable writing to the stencil buffer
	//glEnable(GL_DEPTH_TEST);

	//============   end of   Object Outline        ==============================================================
	
	
	
}


void Sen_23_StencilObjectOutline::cleanFreeSpace(void)	{
	// Clean Cubes
	if (similarCube)	{
		similarCube->finalizeCube();
		delete similarCube;
	}

	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
	if (glIsProgram(programB))				glDeleteProgram(programB);
}