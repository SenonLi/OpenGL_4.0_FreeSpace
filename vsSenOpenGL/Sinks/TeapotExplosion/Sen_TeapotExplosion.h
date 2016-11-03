#pragma once

#ifndef __Sen_TeapotExplosion__
#define __Sen_TeapotExplosion__

#include "LearnOpenGL_GLFW/Sen_24_BlendingTest.h"
#include "SenFreeSpacePieces/Sen_Teapot.h"


class Sen_TeapotExplosion : public Sen_24_BlendingTest
{
public:
	Sen_TeapotExplosion();
	virtual ~Sen_TeapotExplosion();

protected:
	void initGlfwGlewGL();
	void paintScene();
	void cleanDebugWindowFreeSpace();

private:
	Sen_Teapot firstTeapot;
	GLuint teapotProgram;
};


#endif  // __Sen_TeapotExplosion__