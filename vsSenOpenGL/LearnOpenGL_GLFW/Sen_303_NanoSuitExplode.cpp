#include "Sen_303_NanoSuitExplode.h"


Sen_303_NanoSuitExplode::Sen_303_NanoSuitExplode()
	:nanoSuitModel(NULL)
{
	strWindowName = "Sen_303 NanoSuit Explode in FreeSpace";

	camera.Position = glm::vec3(0.0f, 0.0f, 1.0f);

	nanoSuitModel = new SenMeshLinkModel("./../WatchMe/MeshLinkModels/NanoSuit/nanosuit.obj");
	//nanoSuitModel = new SenMeshLinkModel("C:/OpenGL/SenOpenGLusr/FreeSpaceModelCollection/NanoSuit/nanosuit.obj");
}

Sen_303_NanoSuitExplode::~Sen_303_NanoSuitExplode()
{
}



void Sen_303_NanoSuitExplode::initGlfwGlewGL()
{
	SenFreeSpaceAbstract::initGlfwGlewGL();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_303_NanoSuitExplode.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_303_NanoSuitExplode.frag" },
		{ GL_GEOMETRY_SHADER, "./../WatchMe/Shaders/Sen_303_NanoSuitExplode.geom" },
		{ GL_NONE, NULL }
	};
	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_303_NanoSuitExplode.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_303_NanoSuitExplode.frag" },
	//	{ GL_GEOMETRY_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_303_NanoSuitExplode.geom" },
	//	{ GL_NONE, NULL }
	//};

	programA = LoadShaders(shaders);

	nanoSuitModel->initialMeshLinkModelGL();


	OutputDebugString(" Sen_303_NanoSuitExplode Initial \n\n");
}

void Sen_303_NanoSuitExplode::paintFreeSpaceGL(void)
{
	// There might be more uniforms, 
	// due to which the glUseProgram must be called in the control widget (Here, not in the MeshLinkModel)
	glUseProgram(programA);

	// Add time component to geometry shader in the form of a uniform
	glUniform1f(glGetUniformLocation(programA, "time"), glfwGetTime());

	glm::mat4 identity;
	model = glm::translate(identity, glm::vec3(0.0f, -2.0f, -3.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down

	nanoSuitModel->paintMeshLinkModel(programA, view, model, projection);

}


void Sen_303_NanoSuitExplode::cleanFreeSpace(void)
{
	if (nanoSuitModel)	{
		nanoSuitModel->finilizeMeshLinkModel();
		delete nanoSuitModel;
	}

	if (glIsProgram(programA))		glDeleteProgram(programA);
}
