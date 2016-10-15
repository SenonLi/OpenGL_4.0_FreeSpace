#pragma once

#ifndef __Sen_262_RearMirror__
#define __Sen_262_RearMirror__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_262_RearMirror : public SenFreeSpaceAbstract
{
public:
	Sen_262_RearMirror();
	virtual ~Sen_262_RearMirror();

protected:
	void initGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	void paintScene();

	Sen_Cube *similarCube, *rearCube;
	GLuint cubeTexture, floorTexture;
	GLuint mirrorFrameProgram, mirrorFrameVertexArrayID, mirrorFrameVertexBufferObject;

	glm::vec3 firstCubePosition = glm::vec3(-3.5f, 0.0f + SenFREESPACE_ZfightingOffset, -4.0f);
	glm::vec3 secondCubePosition = glm::vec3(2.0f, 0.0f + SenFREESPACE_ZfightingOffset, -3.0f);
	glm::vec3 rearCubePosition = glm::vec3(-1.5f, 0.42f + SenFREESPACE_ZfightingOffset, 3.5f);


	// ==== FrameBuffer ====
	GLuint testFrameBufferObject;
	GLuint textureColorBuffer, depthStencilRenderBufferObject;
};

#endif // __Sen_262_RearMirror__