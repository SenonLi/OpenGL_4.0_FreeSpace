#include "Sen_26_PostProcessing.h"


Sen_26_PostProcessing::Sen_26_PostProcessing()
{
	similarCube = new Sen_Cube();
	strWindowName = "Sen_26 FrameBuffer Post Processing in FreeSpace";

	camera.Position = glm::vec3(-5.1788f, 5.7847f, 6.5843f);
	camera.Front = glm::vec3(0.336f, -0.7513f, -0.568);
	camera.Up = glm::vec3(0.3824f, 0.6600f, -0.6466f);
	camera.Yaw = -59.40f;
	camera.Pitch = -48.700f;
}


Sen_26_PostProcessing::~Sen_26_PostProcessing()
{
}


void Sen_26_PostProcessing::initGlfwGlewGL()
{
	SenFreeSpaceAbstract::initGlfwGlewGL();

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

	ShaderInfo shadersPostProcessing[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_26_PostProcessing.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_26_PostProcessing.frag" },
		{ GL_NONE, NULL }
	};

	programA = LoadShaders(shaders);
	programB = LoadShaders(shadersPostProcessing);


	GLfloat planeVertices[] = {
		// Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};
	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(2, verArrObjArray);
	glGenBuffers(2, verBufferObjArray);

	// =====   Setup floor verArrObj[0] ================
	glBindVertexArray(verArrObjArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 

	// ===== Setup Widget quad verArrObj[1] =================
	glBindVertexArray(verArrObjArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/SenGrassGround3.jpg").c_str(), floorTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/SenGrassGround3,jpg").c_str(), floorTexture, std::string("RGB"));

	similarCube->initialCubeGL(programA, cubeTexture, cubeTexture, cubeTexture);

	// ==== Initial FrameBuffer ==============================================================
	glGenFramebuffers(1, &testFrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, testFrameBufferObject);
	// Generate and Attach color texture to currently bound framebuffer object
	testFrameBufferWidth = m_WidgetWidth;
	testFrameBufferHeight = m_WidgetHeight;

	textureColorBuffer = generateAttachmentTexture(false, false, testFrameBufferWidth, testFrameBufferHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	glGenRenderbuffers(1, &depthStencilRenderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, testFrameBufferWidth, testFrameBufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// make sure frameBuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error::Framebuffer:: Framebuffer is not complete!" << std::endl;
		
	glClearColor((float)0x87 / (float)0xFF, (float)0xCE / float(0xFF), (float)0xEB / float(0xFF), 1.0f);

	// Make sure operations are for the default framebuffer (main screen)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ==== End of  Initial FrameBuffer ==============================================================

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_26_PostProcessing::paintFreeSpaceGL(void)
{
	// ======== Render Customer FrameBuffer =================================================================
	glBindFramebuffer(GL_FRAMEBUFFER, testFrameBufferObject);
	glViewport(0, 0, testFrameBufferWidth, testFrameBufferHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Paint Floor
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	// Recover program, normal scaleratio Needed
	glUseProgram(programA);
	similarCube->changeNewLinkedCubeProgram(programA);
	similarCube->setCubeScaleRatio(glm::vec3(1.0f, 1.0f, 1.0f));

	similarCube->setCubeWorldAddress(firstCubePosition);
	similarCube->paintCube(projection, view);
	similarCube->setCubeWorldAddress(secondCubePosition);
	similarCube->paintCube(projection, view);

	// ======== End of Render Customer FrameBuffer =================================================================

	/////////////////////////////////////////////////////
	// Bind to default framebuffer again and draw the 
	// quad plane with attched screen texture.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_WidgetWidth, m_WidgetHeight);
	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

	// Draw Screen
	glUseProgram(programB);
	glBindVertexArray(verArrObjArray[1]);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}


void Sen_26_PostProcessing::cleanFreeSpace(void)	{
	// Clean Cubes
	if (similarCube)	{
		similarCube->finalizeCube();
		delete similarCube;
	}

	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	
	// finalize FrameBuffer 
	if (glIsTexture(textureColorBuffer))		
		glDeleteTextures(1, &textureColorBuffer);
	if (glIsFramebuffer(testFrameBufferObject)) 
		glDeleteFramebuffers(1, &testFrameBufferObject);
	if (glIsRenderbuffer(depthStencilRenderBufferObject))
		glDeleteRenderbuffers(1, &depthStencilRenderBufferObject);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsVertexArray(verArrObjArray[1]))		
		glDeleteVertexArrays(1, &(verArrObjArray[1]));
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verBufferObjArray[1]))		
		glDeleteBuffers(1, &(verBufferObjArray[1]));

	if (glIsProgram(programA))				glDeleteProgram(programA);
	if (glIsProgram(programB))				glDeleteProgram(programB);
}