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
		strRollTexture = "./../WatchMe/Images/SenSqaurePortrait.jpg";
		strYawTexture = "./../WatchMe/Images/uky.jpg";
		strPitchTexture = "./../WatchMe/Images/lau2.jpg";
		cubeWorldSpaceAddr = glm::vec3(0.0f, 0.0f, 0.0f);

		selfSpinAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		scaleRatio = glm::vec3(1.0f, 1.0f, 1.0f);
		selfSpinAngle = 0.0f;
	}
	Sen_Cube(const char* strRoll, const char* strYaw, const char*strPitch, glm::vec3 cubeWorldAddr = glm::vec3(0.0f, 0.0f, -3.0f))
		: strRollTexture(strRoll), strYawTexture(strYaw), strPitchTexture(strPitch), cubeWorldSpaceAddr(cubeWorldAddr), textureImagePtr(NULL)
	{
		;
	}

	virtual ~Sen_Cube()	{
		finalizeCube();
	}

	inline glm::mat4 getCubeModelMatrix() const { return cubeModel; }
	inline GLfloat getCubeLogoSide() const { return sideCubeLogo; }
	inline GLfloat getCubeLogoCenterToBorder() const { return centerToBorderCubeLogo; }
	inline void setCubeWorldAddress(glm::vec3 cubeWorldAddr) { cubeWorldSpaceAddr = cubeWorldAddr; }
	inline void setCubeScaleRatio(glm::vec3 scale) { scaleRatio = scale; }
	inline void setCubeRotation(glm::vec3 spinAxis, GLfloat spinAngle) {
		selfSpinAxis = spinAxis;
		selfSpinAngle = glm::radians(spinAngle);// spinAngle;
	}

	void initialCubeGL(GLboolean highDefinition = GL_FALSE)	{
		initialCubeShaders();
		initialCubeVertices();
		initialCubeTexture(highDefinition);
		//initialCubeModel();
	}

	void initialCubeGL(GLuint newProgram, GLuint rollTexture, GLuint yawTexture, GLuint pitchTexture)	{
		cubeProgram = newProgram;
		rollTextureID = rollTexture;
		yawTextureID = yawTexture;
		pitchTextureID = pitchTexture;
		initialCubeVertices();
	}

	virtual void finalizeCube()	{
		// Properly de-allocate all resources once they've outlived their purpose
		if (glIsTexture(rollTextureID))			glDeleteTextures(1, &rollTextureID);
		if (glIsTexture(yawTextureID))			glDeleteTextures(1, &yawTextureID);
		if (glIsTexture(pitchTextureID))			glDeleteTextures(1, &pitchTextureID);

		if (glIsVertexArray(cubeVertexArrayObject))	glDeleteVertexArrays(1, &cubeVertexArrayObject);
		if (glIsBuffer(cubeVertexBufferObject))		glDeleteBuffers(1, &cubeVertexBufferObject);
		if (glIsBuffer(cubeVertexIndicesObject))		glDeleteBuffers(1, &cubeVertexIndicesObject);

		if (glIsProgram(cubeProgram))			glDeleteProgram(cubeProgram);
	}

	void changeNewLinkedCubeProgram(GLuint newProgram)	{
		cubeProgram = newProgram;
	}

	void changeNewUploadedCubeTexture(GLuint rollTexture, GLuint yawTexture, GLuint pitchTexture)	{
		if (glIsTexture(rollTextureID))			glDeleteTextures(1, &rollTextureID);
		if (glIsTexture(yawTextureID))			glDeleteTextures(1, &yawTextureID);
		if (glIsTexture(pitchTextureID))			glDeleteTextures(1, &pitchTextureID);

		rollTextureID = rollTexture;
		yawTextureID = yawTexture;
		pitchTextureID = pitchTexture;
	}

	void paintSenLogoCube(GLfloat widthRatio = 1.0f, GLfloat heightRatio = 1.0f)	{

		GLfloat widthMax = widthRatio;
		GLfloat heightMax = heightRatio;

		logoCubeCameraView = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		logoCubeProjection = glm::ortho(-widthMax, widthMax, -heightMax, heightMax, 0.1f, 100.0f);

		glm::mat4 identityMatrix;
		logoCubeModel = glm::translate(identityMatrix, glm::vec3(widthMax - centerToBorderCubeLogo, heightMax - centerToBorderCubeLogo, -1.1f));
		logoCubeModel = glm::scale(logoCubeModel, glm::vec3(sideCubeLogo));
		logoCubeModel = glm::rotate(logoCubeModel, GLfloat(glfwGetTime() * 1.5 * glm::radians(90.0)), glm::vec3(-1.0f, 1.0f, 1.0f));

		glUseProgram(cubeProgram);
		glBindVertexArray(cubeVertexArrayObject);

		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(logoCubeModel));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(logoCubeCameraView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(logoCubeProjection));
		//glActiveTexture(GL_TEXTURE0);

		//glBindTexture(GL_TEXTURE_2D, rollTextureID);
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		//glBindTexture(GL_TEXTURE_2D, yawTextureID);
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
		//glBindTexture(GL_TEXTURE_2D, pitchTextureID);
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

		//glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glUseProgram(0);

		paintCubeFaces();
	}

	void paintCube(glm::mat4 &projection, glm::mat4 &view, float spinSpeedRate = 0.0f, glm::vec3 vecSpinAxis = glm::vec3(0.0f, 1.0f, 0.0f))	{

		glUseProgram(cubeProgram);
		glBindVertexArray(cubeVertexArrayObject);

		updateCubeModel(cubeWorldSpaceAddr, spinSpeedRate, vecSpinAxis);

		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		paintCubeFaces();
	}

protected:
	GLfloat sideCubeLogo = 0.16f;
	GLfloat centerToBorderCubeLogo = sideCubeLogo * 2.0 / 2.0;
	
	glm::vec3 cubeWorldSpaceAddr, scaleRatio;
	glm::mat4 cubeModel, logoCubeCameraView, logoCubeModel, logoCubeProjection;
	GLuint cubeProgram;
	glm::vec3 selfSpinAxis;
	GLfloat selfSpinAngle;

	unsigned char* textureImagePtr;
	const char *strRollTexture, *strYawTexture, *strPitchTexture;
	GLuint rollTextureID, yawTextureID, pitchTextureID;
	GLuint cubeVertexArrayObject, cubeVertexBufferObject, cubeVertexIndicesObject;

	void initialCubeModel()	{
		glm::mat4 identityMatrix;
		cubeModel = glm::translate(identityMatrix, cubeWorldSpaceAddr);

		selfSpinAngle = float(glm::radians(-70.0));
		selfSpinAxis = glm::vec3(-1.0f, 1.0f, 1.0f);

		cubeModel = glm::rotate(cubeModel, selfSpinAngle, selfSpinAxis);
	}

	void updateCubeModel(glm::vec3 &worldSpaceAddress, float spinSpeedRate = 0.0f, glm::vec3 vecSpinAxis = glm::vec3(0.0f, 1.0f, 0.0f))	{
		glm::mat4 identityMatrix;
		cubeModel = glm::translate(identityMatrix, worldSpaceAddress);
		cubeModel = glm::scale(cubeModel, scaleRatio);

		if (spinSpeedRate != 0.0)	{

			selfSpinAngle = GLfloat(glfwGetTime() * spinSpeedRate * glm::radians(90.0));
			selfSpinAxis = vecSpinAxis;
		}

		cubeModel = glm::rotate(cubeModel, selfSpinAngle, selfSpinAxis);
	}

	void initialCubeTexture(GLboolean highDefinition = GL_FALSE)	{
		uploadTexture(strRollTexture, rollTextureID, highDefinition);
		uploadTexture(strYawTexture, yawTextureID, highDefinition);
		uploadTexture(strPitchTexture, pitchTextureID, highDefinition);
	}

	void initialCubeVertices()	{
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


		glGenVertexArrays(1, &cubeVertexArrayObject);
		glGenBuffers(1, &cubeVertexBufferObject);
		glGenBuffers(1, &cubeVertexIndicesObject);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

		glBindVertexArray(cubeVertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVertexIndicesObject);
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
			"#version 330 core\n"
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
			"#version 330 core\n"
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
	void paintCubeFaces()	{
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, rollTextureID);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, yawTextureID);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(12 * sizeof(GLuint)));
		glBindTexture(GL_TEXTURE_2D, pitchTextureID);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const GLint *)(24 * sizeof(GLuint)));

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}

	void uploadTexture(const char* &textureAddressPointer, GLuint &textureID, GLboolean highDefinition = GL_FALSE)	{
		int width, height;
		textureImagePtr = SOIL_load_image(textureAddressPointer, &width, &height, 0, SOIL_LOAD_RGBA);

		// Load and create a defaultTextureID 
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		//// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, highDefinition ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load textureImage, create defaultTextureID and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImagePtr);
		if (!highDefinition) glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(textureImagePtr);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
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