#include "Sen_263_DebugRearMirror.h"


Sen_263_DebugRearMirror::Sen_263_DebugRearMirror()
{
	strWindowName = "Sen_263 Debug Mode Rear Mirror in FreeSpace";

	similarCube = new Sen_Cube();
	rearCube = new Sen_Cube();

	camera.Position = glm::vec3(0.0f, 1.0f, 0.0f);
}

Sen_263_DebugRearMirror::~Sen_263_DebugRearMirror()
{
}

void Sen_263_DebugRearMirror::initGlfwGlewGL()
{
	SenDebugWindowFreeSpace::initGlfwGlewGL();

	// Draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_3D_TextureMapProgram();
	initVertexAttributes();
	initTextures();

	similarCube->initialCubeGL(programA, cubeTexture, cubeTexture, cubeTexture);
	rearCube->initialCubeGL();
	rearCube->setCubeRotation(glm::vec3(1.0f, 0.0f, 1.0f), 75.0f);

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_263_DebugRearMirror::paintScene(void)	{
	// Paint Floor
	glUseProgram(programA);
	view = camera.GetViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(programA, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(verArrObjArray[0]);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	// Recover program, normal scaleratio Needed
	glUseProgram(programA);
	similarCube->changeNewLinkedCubeProgram(programA);

	similarCube->setCubeWorldAddress(firstCubePosition);
	similarCube->paintCube(projection, view);
	similarCube->setCubeWorldAddress(secondCubePosition);
	similarCube->paintCube(projection, view);

	rearCube->setCubeWorldAddress(rearCubePosition);
	rearCube->paintCube(projection, view);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_263_DebugRearMirror::cleanDebugWindowFreeSpace(void)	{
	
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

	if (glIsProgram(programA))				glDeleteProgram(programA);
	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
}





void Sen_263_DebugRearMirror::init_3D_TextureMapProgram(){

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.frag" },
	//	{ GL_NONE, NULL }
	//};

	ShaderInfo shaders_3D_TextureMap[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_23_StencilObjectOutline.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_23_StencilObjectOutline.frag" },
		{ GL_NONE, NULL }
	};

	programA = LoadShaders(shaders_3D_TextureMap);

}

void Sen_263_DebugRearMirror::initVertexAttributes(){
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
}

void Sen_263_DebugRearMirror::initTextures(){
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	uploadFreeSpaceTexture(std::string("./../WatchMe/Images/SenGrassGround3.jpg").c_str(), floorTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/container.jpg").c_str(), cubeTexture, std::string("RGB"));
	//uploadFreeSpaceTexture(std::string("./LearnOpenGL_GLFW/Images/SenGrassGround3,jpg").c_str(), floorTexture, std::string("RGB"));
}
