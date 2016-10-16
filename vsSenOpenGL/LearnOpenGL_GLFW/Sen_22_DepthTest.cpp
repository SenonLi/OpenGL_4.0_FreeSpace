#include "Sen_22_DepthTest.h"


Sen_22_DepthTest::Sen_22_DepthTest()
{
	similarCube = new Sen_Cube();
	strWindowName = "Sen_22 Depth Test in FreeSpace";

	camera.Position = glm::vec3(2.3746f, 2.5661f, 2.6288f);
	camera.Front = glm::vec3(-0.1743f, -0.7083f, -0.6840f);
	camera.Up = glm::vec3(-0.1750f, 0.7059f, -0.6864f);
	camera.Yaw = -104.30f;
	camera.Pitch = -45.10f;
}


Sen_22_DepthTest::~Sen_22_DepthTest()
{
}



void Sen_22_DepthTest::initGlfwGlewGL()
{
	SenFreeSpaceAbstract::initGlfwGlewGL();

	// Setup some OpenGL options
	glDepthFunc(GL_LESS);// By Default

	//glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_22_DepthTest.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_22_DepthTest.frag" },
		{ GL_NONE, NULL }
	};
	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.frag" },
	//	{ GL_NONE, NULL }
	//};
	programA = LoadShaders(shaders);

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

	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/marble.jpg").c_str(), cubeTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/metal.png").c_str(), floorTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/marble.jpg").c_str(), cubeTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/metal.png").c_str(), floorTexture, std::string("RGB"));

	similarCube->initialCubeGL(programA, cubeTexture, cubeTexture, cubeTexture);

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_22_DepthTest::paintFreeSpaceGL(void)
{
	similarCube->setCubeWorldAddress(glm::vec3(-1.0f, 0.0f, -1.0f));
	similarCube->paintCube(projection, view);

	similarCube->setCubeWorldAddress(glm::vec3(2.0f, 0.0f, 0.0f));
	similarCube->paintCube(projection, view);

	// Floor
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void Sen_22_DepthTest::cleanFreeSpace(void)	{
	// Clean Cubes
	if (similarCube)	{
		similarCube->finalizeCube();
		delete similarCube;
	}

	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}