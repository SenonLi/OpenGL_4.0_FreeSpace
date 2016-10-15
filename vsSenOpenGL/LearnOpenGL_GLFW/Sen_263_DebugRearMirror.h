#pragma once

#ifndef __Sen_24_BlendingTest__
#define __Sen_24_BlendingTest__

#include "LearnOpenGL_GLFW/SenDebugWindowFreeSpace.h"

class Sen_263_DebugRearMirror : public SenDebugWindowFreeSpace
{
public:
	Sen_263_DebugRearMirror();
	virtual ~Sen_263_DebugRearMirror();

protected:
	void initDebugWindowProgram();

	void initGlfwGlewGL();
	void paintScene();
	void cleanDebugWindowFreeSpace();


	Sen_Cube *similarCube, *rearCube;
	GLuint cubeTexture, floorTexture;

	glm::vec3 firstCubePosition = glm::vec3(-3.5f, 0.0f + SenFREESPACE_ZfightingOffset, -4.0f);
	glm::vec3 secondCubePosition = glm::vec3(2.0f, 0.0f + SenFREESPACE_ZfightingOffset, -3.0f);
	glm::vec3 rearCubePosition = glm::vec3(-1.5f, 0.42f + SenFREESPACE_ZfightingOffset, 3.5f);

private:
	void init_3D_TextureMapProgram();
	void initVertexAttributes();
	void initTextures();
};


#endif  // __Sen_24_BlendingTest__