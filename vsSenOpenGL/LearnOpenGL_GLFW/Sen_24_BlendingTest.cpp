#include "Sen_24_BlendingTest.h"


Sen_24_BlendingTest::Sen_24_BlendingTest()
{
	strWindowName = "Sen_24 Blending Test in FreeSpace";

	debugWindowFuntionString = "rear mirror";

	similarCube = new Sen_Cube();
	rearCube = new Sen_Cube();

	camera.Position = glm::vec3(-0.5f, 1.0f, 3.0f);
	grassAddressVector.clear();
}

Sen_24_BlendingTest::~Sen_24_BlendingTest()
{
}



void Sen_24_BlendingTest::initGlfwGlewGL()
{
	glEnable(GL_CULL_FACE);

	SenDebugWindowFreeSpace::initGlfwGlewGL();
	// Draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	initBlendingProgram();

	initVertexAttributes();
	initTextures();

	similarCube->initialCubeGL(programA, cubeTexture, cubeTexture, cubeTexture);
	rearCube->initialCubeGL();
	rearCube->setCubeRotation(glm::vec3(1.0f, 0.0f, 1.0f), 75.0f);


	initGrassWindowAddress();

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_24_BlendingTest::paintScene(void)	{

	paintFloor();
	paintGrassAlphaDiscard();
	paintCubes();
	paintFrontAndSideTransparentWindows();// Need to be the last for transparency painting

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_24_BlendingTest::paintFrontAndSideTransparentWindows(){
	// Paint Window Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(programB);
	glUniformMatrix4fv(glGetUniformLocation(programB, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(programB, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glBindTexture(GL_TEXTURE_2D, transparentWindowTexture);

	glBindVertexArray(transparentWindowVAO);
	for (GLuint i = 0; i < sideWindowAddressVector.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, sideWindowAddressVector[i]);
		glUniformMatrix4fv(glGetUniformLocation(programB, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	// Paint Sorted Blending Windows
	std::map<GLfloat, glm::vec3> sortedFrontWindowMap;
	for (GLuint i = 0; i < frontWindowAddressVector.size(); i++)
	{
		GLfloat distance = glm::length(camera.Position - frontWindowAddressVector[i]);
		sortedFrontWindowMap[distance] = frontWindowAddressVector[i];
	}

	glBindVertexArray(grassAlphaDiscardVAO);
	for (std::map<float, glm::vec3>::reverse_iterator map = sortedFrontWindowMap.rbegin(); map != sortedFrontWindowMap.rend(); map++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, map->second);
		glUniformMatrix4fv(glGetUniformLocation(programB, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisable(GL_BLEND);
}

void Sen_24_BlendingTest::paintCubes(){
	// Paint Grass Alpha Discard
	glUseProgram(programA);
	view = camera.GetViewMatrix();

	similarCube->changeNewLinkedCubeProgram(programA);

	similarCube->setCubeWorldAddress(firstCubePosition);
	similarCube->paintCube(projection, view);
	similarCube->setCubeWorldAddress(secondCubePosition);
	similarCube->paintCube(projection, view);

	rearCube->setCubeWorldAddress(rearCubePosition);
	rearCube->paintCube(projection, view);
}

void Sen_24_BlendingTest::paintGrassAlphaDiscard(){
	// Paint Grass Alpha Discard
	glUseProgram(programA);
	view = camera.GetViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(grassAlphaDiscardVAO);
	glBindTexture(GL_TEXTURE_2D, grassAlphaDiscardTexture);
	for (GLuint i = 0; i < grassAddressVector.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, grassAddressVector[i]);
		glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_24_BlendingTest::paintFloor(){
	// Paint Floor
	glUseProgram(programA);
	view = camera.GetViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(programA, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(verArrObjArray[0]);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Sen_24_BlendingTest::cleanDebugWindowFreeSpace(void)	{

	// Clean Cubes
	if (similarCube)	{
		similarCube->finalizeCube();
		delete similarCube;
	}
	if (rearCube)	{
		rearCube->finalizeCube();
		delete rearCube;
	}

	// finalize Textures + Vertex Attributes
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);
	if (glIsTexture(grassAlphaDiscardTexture))	glDeleteTextures(1, &grassAlphaDiscardTexture);
	if (glIsTexture(transparentWindowTexture))	glDeleteTextures(1, &transparentWindowTexture);

	if (glIsProgram(programA))				glDeleteProgram(programA);
	if (glIsProgram(programB))				glDeleteProgram(programB);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsVertexArray(grassAlphaDiscardVAO))		glDeleteVertexArrays(1, &grassAlphaDiscardVAO);
	if (glIsVertexArray(transparentWindowVAO))		glDeleteVertexArrays(1, &transparentWindowVAO);

	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(grassAlphaDiscardVBO))		glDeleteBuffers(1, &grassAlphaDiscardVBO);
	if (glIsBuffer(transparentWindowVBO))		glDeleteBuffers(1, &transparentWindowVBO);
}

void Sen_24_BlendingTest::initBlendingProgram(){

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_3D_TextureCoords.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_TextureCoords.frag" },
	//	{ GL_NONE, NULL }
	//};

	ShaderInfo shaders_3D_TextureAlphaDiscard[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_3D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_AlphaDiscardTextureCoords.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders_3D_TextureAlphaDiscard);

	ShaderInfo shaders_3D_TextureCoords[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_3D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoords.frag" },
		{ GL_NONE, NULL }
	};
	programB = LoadShaders(shaders_3D_TextureCoords);
}

void Sen_24_BlendingTest::initGrassWindowAddress(){

	grassAddressVector.push_back(firstCubePosition + glm::vec3(-0.5f, 0.0f, 0.5f));///
	grassAddressVector.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	grassAddressVector.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	grassAddressVector.push_back(secondCubePosition + glm::vec3(-0.5f, 0.0f, 0.5f));///
	grassAddressVector.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));

	frontWindowAddressVector.push_back(glm::vec3(-3.3f, 0.0f, -0.8f));
	frontWindowAddressVector.push_back(glm::vec3(-3.5f, 0.0f, -2.7f));
	frontWindowAddressVector.push_back(glm::vec3(-3.7f, 0.0f, -1.6f));

	sideWindowAddressVector.push_back(firstWindowPosition);
	sideWindowAddressVector.push_back(secondWindowPosition);
}

void Sen_24_BlendingTest::initVertexAttributes(){
	// Note: we set Texture Coords higher than 1, together with GL_REPEAT,
	//		such that texture wrapping mode will cause the floor texture to repeat.
	GLfloat floorVertexAttributes[] = {
		// Positions            // Texture Coords 
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);

	glBindVertexArray(verArrObjArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertexAttributes), floorVertexAttributes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLfloat grassAlphaDiscardVertices[] = {
		// Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.0f, 1.0f, 0.0f
	};

	// Setup transparent Grass Blending plane VAO
	glGenVertexArrays(1, &grassAlphaDiscardVAO);
	glGenBuffers(1, &grassAlphaDiscardVBO);
	glBindVertexArray(grassAlphaDiscardVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassAlphaDiscardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassAlphaDiscardVertices), grassAlphaDiscardVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);


	GLfloat transparentWindowVertices[] = {
		// Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 1.0f, 1.0f, 0.0f
	};

	// Setup transparent Grass Blending plane VAO
	glGenVertexArrays(1, &transparentWindowVAO);
	glGenBuffers(1, &transparentWindowVBO);
	glBindVertexArray(transparentWindowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentWindowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentWindowVertices), transparentWindowVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void Sen_24_BlendingTest::initTextures(){
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/SenGrassGround3.jpg").c_str(), floorTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/grass.png").c_str(), grassAlphaDiscardTexture, std::string("RGBA"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/window.png").c_str(), transparentWindowTexture, std::string("RGBA"));

	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/SenGrassGround3,jpg").c_str(), floorTexture, std::string("RGB"));
}



void Sen_24_BlendingTest::initDebugWindowProgram()
{
	ShaderInfo shadersScreenTexture[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoords.frag" },
		{ GL_NONE, NULL }
	};

	ShaderInfo shadersScreenSingular[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_ColorRed.frag" },
		{ GL_NONE, NULL }
	};

	debugWindowProgram = LoadShaders(shadersScreenTexture);
	debugWindowOutlineProgram = LoadShaders(shadersScreenSingular);
}


void Sen_24_BlendingTest::paintDebugWindowFrameBufferGL()	{

	glBindFramebuffer(GL_FRAMEBUFFER, debugWindowFrameBufferObject);
	glViewport(0, 0, debugWindowFrameBufferWidth, debugWindowFrameBufferHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?

	// Get Rear CameraView
	camera.Front = -camera.Front;

	paintScene();

	camera.Front = -camera.Front; // Recover front CameraView
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_WidgetWidth, m_WidgetHeight);
}
