#include "Sen_10_Camera.h"


Sen_10_Camera::Sen_10_Camera()
{
	strWindowName = "Sen GLFW CameraModel";

}


Sen_10_Camera::~Sen_10_Camera()
{
}


void Sen_10_Camera::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);

	glBindVertexArray(verArrObjArray[0]);

	GLfloat radius = abs(viewCenter);
	GLfloat camX = float(sin(glfwGetTime()) * radius);
	GLfloat camZ = float(viewCenter + cos(glfwGetTime()) * radius);
	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, viewCenter), glm::vec3(0.0f, 1.0f, 0.0f));

	updateModelViewProjection();

	bindBackgroundTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	bindNewLayerTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
	glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));


	//model = glm::translate(model, cubePositions[i]);
	//GLfloat angle = 20.0f * i;
	//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	updateSecondModelViewProjection();
	bindBackgroundTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	bindNewLayerTexture();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
	glBindTexture(GL_TEXTURE_2D, thirdLayerTexture);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));






	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
