#include "Sen_21_AssimpMeshModel.h"


Sen_21_AssimpMeshModel::Sen_21_AssimpMeshModel()
	:nanoSuitModel(NULL)
{
	strWindowName = "Sen_21 NanoSuit in FreeSpace";

	camera.Position = glm::vec3(0.0f, 0.0f, 1.0f);

	//nanoSuitModel = new SenMeshLinkModel("./LearnOpenGL_GLFW/NanoSuit/nanosuit.obj");
	nanoSuitModel = new SenMeshLinkModel("C:/OpenGL/SenOpenGLusr/FreeSpaceModelCollection/NanoSuit/nanosuit.obj");
}


Sen_21_AssimpMeshModel::~Sen_21_AssimpMeshModel()
{
}


void Sen_21_AssimpMeshModel::initialGlfwGlewGL()
{
	SenFreeSpaceAbstract::initialGlfwGlewGL();
	
	//glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./SenFreeSpacePieces/Shaders/Sen_AssimpNanoSuit.vert" },
		{ GL_FRAGMENT_SHADER, "./SenFreeSpacePieces/Shaders/Sen_AssimpNanoSuit.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);

	nanoSuitModel->initialMeshLinkModelGL();


	OutputDebugString(" Sen_21_AssimpMeshModel Initial \n\n");
}

void Sen_21_AssimpMeshModel::paintFreeSpaceGL(void)
{
	// There might be more uniforms, 
	// due to which the glUseProgram must be called in the control widget (Here, not in the MeshLinkModel)
	glUseProgram(programA); 

	// Draw the loaded model
	glm::mat4 identity;
	model = glm::translate(identity, glm::vec3(0.0f, -2.0f, -3.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down

	nanoSuitModel->paintMeshLinkModel(programA, view, model, projection);

}


void Sen_21_AssimpMeshModel::cleanFreeSpace(void)
{
	if (nanoSuitModel)	{
		nanoSuitModel->finilizeMeshLinkModel();
		delete nanoSuitModel;
	}

	if (glIsProgram(programA))		glDeleteProgram(programA);
}
