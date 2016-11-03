#pragma once
#include "LearnOpenGL_GLFW/SenDebugWindowFreeSpace.h"
#include "Guide8th/vmath.h"

using namespace vmath;

class SenShaderTeapotExplosion : public SenDebugWindowFreeSpace
{
public:
	SenShaderTeapotExplosion();
	virtual ~SenShaderTeapotExplosion();

protected:
	void paintScene();
	void initGlfwGlewGL();
	void cleanDebugWindowFreeSpace();

	const vmath::vec3 x_axis = vmath::vec3(1.0f, 0.0f, 0.0f);
	const vmath::vec3 y_axis = vmath::vec3(0.0f, 1.0f, 0.0f);

private:

};

