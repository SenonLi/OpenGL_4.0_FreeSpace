#pragma once

#ifndef __Sen_Cube__
#define __Sen_Cube__

#include <GL/glew.h>
#include <soil/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sen_Cube
{
public:
	Sen_Cube() : textureImagePtr(NULL), selfSpinAngle(0.0f)
	{
		strRollTexture = "./LearnOpenGL_GLFW/Images/SenSqaurePortrait.jpg";
		strYawTexture = "./LearnOpenGL_GLFW/Images/uky.jpg";
		strPitchTexture = "./LearnOpenGL_GLFW/Images/lau2.jpg";
		cubeWorldSpaceAddr = glm::vec3(0.0f, 0.0f, 0.0f);

		selfSpinAxis = glm::vec3(-1.0f, 1.0f, 1.0f);
		scaleRatio = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	Sen_Cube(const char* strRoll, const char* strYaw, const char*strPitch, glm::vec3 cubeWorldAddr = glm::vec3(0.0f, 0.0f, -3.0f))
		: strRollTexture(strRoll), strYawTexture(strYaw), strPitchTexture(strPitch), cubeWorldSpaceAddr(cubeWorldAddr), textureImagePtr(NULL)
	{ ; }
	
	virtual ~Sen_Cube()	{ 
		finalize(); 
	}

	inline glm::mat4 getCubeModelMatrix() { return cubeModel; }
	inline void setCubeWorldAddress(glm::vec3 cubeWorldAddr) { cubeWorldSpaceAddr = cubeWorldAddr; }
	inline void setCubeScaleRatio(glm::vec3 scale) { scaleRatio = scale; }

	virtual void initialCube()	{
		initialCubeShaders();
		initialVertices();
		initialTexture();
		initialCubeModel();
	}

	virtual void finalize()	{
		// Properly de-allocate all resources once they've outlived their purpose
		if (glIsTexture(rollTexture))			glDeleteTextures(1, &rollTexture);
		if (glIsTexture(yawTexture))			glDeleteTextures(1, &yawTexture);
		if (glIsTexture(pitchTexture))			glDeleteTextures(1, &pitchTexture);

		if (glIsVertexArray(verArrayObject))	glDeleteVertexArrays(1, &verArrayObject);
		if (glIsBuffer(verBufferObject))		glDeleteBuffers(1, &verBufferObject);
		if (glIsBuffer(verIndicesObject))		glDeleteBuffers(1, &verIndicesObject);

		if (glIsProgram(cubeProgram))			glDeleteProgram(cubeProgram);
	}

	void paintSenLogoCube(GLfloat widthRatio, GLfloat heightRatio)	{

		GLfloat widthMax = widthRatio;
		GLfloat heightMax = heightRatio;

		logoCubeCameraView = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		logoCubeProjection = glm::ortho(-widthMax, widthMax, -heightMax, heightMax, 0.1f, 100.0f);

		glm::mat4 identityMatrix;
		logoCubeModel = glm::translate(identityMatrix, glm::vec3(0.8f * widthMax, 0.8f * heightMax, -18.0f));
		logoCubeModel = glm::scale(logoCubeModel, glm::vec3(0.16f, 0.16f, 0.16f));
		logoCubeModel = glm::rotate(logoCubeModel, GLfloat(glfwGetTime() * 1.5 * glm::radians(90.0)), glm::vec3(-1.0f, 1.0f, 1.0f));

		glUseProgram(cubeProgram);
		glBindVertexArray(verArrayObject);

		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(logoCubeModel));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(logoCubeCameraView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(logoCubeProjection));
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, rollTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, yawTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
		glBindTexture(GL_TEXTURE_2D, pitchTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	void paintCube(glm::mat4 &projection, glm::mat4 &view, float spinSpeedRate = 1.5, glm::vec3 vecSpinAxis = glm::vec3(-1.0f, 1.0f, 1.0f))	{

		glUseProgram(cubeProgram);
		glBindVertexArray(verArrayObject);

		updateCubeModel(cubeWorldSpaceAddr, spinSpeedRate, vecSpinAxis);

		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, rollTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, yawTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
		glBindTexture(GL_TEXTURE_2D, pitchTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}

protected:
	glm::vec3 cubeWorldSpaceAddr, scaleRatio;
	glm::mat4 cubeModel, logoCubeCameraView, logoCubeModel, logoCubeProjection;
	GLuint cubeProgram;
	glm::vec3 selfSpinAxis;
	GLfloat selfSpinAngle;

	unsigned char* textureImagePtr;
	const char *strRollTexture, *strYawTexture, *strPitchTexture;
	GLuint rollTexture, yawTexture, pitchTexture;
	GLuint verArrayObject, verBufferObject, verIndicesObject;

	void initialCubeModel()	{
		glm::mat4 identityMatrix;
		cubeModel = glm::translate(identityMatrix, cubeWorldSpaceAddr);

		selfSpinAngle = float(glm::radians(-70.0));
		selfSpinAxis = glm::vec3(-1.0f, 1.0f, 1.0f);

		cubeModel = glm::rotate(cubeModel, selfSpinAngle, selfSpinAxis);
	}

	void updateCubeModel(glm::vec3 &worldSpaceAddress, float spinSpeedRate = 0.0, glm::vec3 vecSpinAxis = glm::vec3(-1.0f, 1.0f, 1.0f))	{
		//if (spinSpeedRate != 0.0)	{
			glm::mat4 identityMatrix;
			cubeModel = glm::translate(identityMatrix, worldSpaceAddress);

			cubeModel = glm::scale(cubeModel, scaleRatio);

			if (spinSpeedRate != 0.0)	selfSpinAngle = GLfloat(glfwGetTime() * spinSpeedRate * glm::radians(90.0));
			selfSpinAxis = vecSpinAxis;

			cubeModel = glm::rotate(cubeModel, selfSpinAngle, selfSpinAxis);
		//}
	}

	void initialTexture()	{
		uploadTexture(strRollTexture, rollTexture);
		uploadTexture(strYawTexture, yawTexture);
		uploadTexture(strPitchTexture, pitchTexture);
	}

	void initialVertices()	{
		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			// Positions           // Texture Coords
			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Front Top Right
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Front Bottom Right
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Front Bottom Left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Front Top Left 

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // Back Top Right
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Back Bottom Right
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Back Bottom Left
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // Back Top Left 

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // Left Top Right
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Left Bottom Right
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Left Bottom Left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Left Top Left 

			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Right Top Right
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Right Bottom Right
			0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Right Bottom Left
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // Right Top Left 

			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Top Top Right
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // Top Bottom Right
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // Top Bottom Left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Top Top Left 

			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom Top Right
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Bottom Bottom Right
			0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Bottom Bottom Left
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f // Bottom Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // Front First Triangle
			1, 2, 3, // Front Second Triangle
			4, 5, 7, // Back First Triangle
			5, 6, 7,  // Back Second Triangle
			8, 9, 11, // Left First Triangle
			9, 10, 11, // Left Second Triangle
			12, 13, 15, // Right First Triangle
			13, 14, 15, // Right Second Triangle
			16, 17, 19, // Top First Triangle
			17, 18, 19, // Top Second Triangle
			20, 21, 23, // Bottom First Triangle
			21, 22, 23 // Bottom Second Triangle
		};


		glGenVertexArrays(1, &verArrayObject);
		glGenBuffers(1, &verBufferObject);
		glGenBuffers(1, &verIndicesObject);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

		glBindVertexArray(verArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, verBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
		glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	}

	void initialCubeShaders()	{
		cubeProgram = glCreateProgram();

		const char cubeVertexShader[] =
			"#version 400 core\n"
			"\n"
			"layout (location = 0) in vec3 vertexCoord;\n"
			"layout (location = 1) in vec2 textureCoord;\n"
			"\n"
			"out vec2 fragInterpTextureCoord;\n"
			"\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = projection * view * model * vec4(vertexCoord, 1.0f);\n"
			"    fragInterpTextureCoord = textureCoord;\n"
			"}\n"
			;

		const char cubeFragmentShader[] =
			"#version 400 core\n"
			"\n"
			"in vec2 fragInterpTextureCoord;\n"
			"out vec4 fragColor;\n"
			"\n"
			"uniform sampler2D backgroundTexture;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    fragColor = texture(backgroundTexture, fragInterpTextureCoord);\n"
			"}\n"
			;

		vglAttachShaderSource(cubeProgram, GL_VERTEX_SHADER, cubeVertexShader);
		vglAttachShaderSource(cubeProgram, GL_FRAGMENT_SHADER, cubeFragmentShader);

		glLinkProgram(cubeProgram);
	}

private:
	void uploadTexture(const char* &textureAddressPointer, GLuint &texture)	{
		int width, height;
		textureImagePtr = SOIL_load_image(textureAddressPointer, &width, &height, 0, SOIL_LOAD_RGBA);

		// Load and create a defaultTexture 
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTexture object
		//// Set the defaultTexture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTexture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load textureImage, create defaultTexture and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(textureImagePtr);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTexture when done, so we won't accidentily mess up our defaultTexture.
	}

	void vglAttachShaderSource(GLuint prog, GLenum type, const char * source)
	{
		GLuint sh;

		sh = glCreateShader(type);
		glShaderSource(sh, 1, &source, NULL);
		glCompileShader(sh);
		char buffer[4096];
		glGetShaderInfoLog(sh, sizeof(buffer), NULL, buffer);
		glAttachShader(prog, sh);
		glDeleteShader(sh);
	}
};

#endif // __Sen_Cube__