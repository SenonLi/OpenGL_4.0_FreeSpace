#include "../../stdafx.h"
#include "SLShaderProgram.h"
#include "../Resources/SLGLShadersResource.h"

#include "glad/glad.h" // Support basic OpenGL

#include <cstdlib>
#include <iostream>
#include <memory>          // For unique_ptr

namespace slgeom
{
	/// <summary>Loads the program from the embedded resources, using the resource ID</summary>
	/// <param name="sourceID">The resouce ID</param>
	/// <returns>The program source, or empty on error (an assert will be triggered)</returns>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	static std::unique_ptr<std::string> LoadGLShaderFromResources(int resourceID)
	{
		LPCWSTR resourceIDName = MAKEINTRESOURCE(resourceID);
		assert(resourceIDName);

		// If this parameter is NULL, GetModuleHandle returns a handle to the file used to create the calling process (.exe file).
		HMODULE callingExeHandle = ::GetModuleHandle(nullptr);
		assert(callingExeHandle);

		LPCWSTR resourceTypeID = MAKEINTRESOURCE(SLRC_TYPEID_GL_SHADER);
		HRSRC resourceInfoHandle = ::FindResource(callingExeHandle, resourceIDName, resourceTypeID);
		assert(resourceInfoHandle);

		HGLOBAL resouceDataHandle = ::LoadResource(callingExeHandle, resourceInfoHandle);
		assert(resouceDataHandle);

		size_t resouceSize = ::SizeofResource(callingExeHandle, resourceInfoHandle);
		const char* resourceData = static_cast<const char*>(::LockResource(resouceDataHandle));
		assert(resourceData);
		std::unique_ptr<std::string> resourceString = std::make_unique<std::string>(resourceData, resouceSize);

		return std::move(resourceString);
	}

	//===================================================================================================
	//---------------------------------------------------------------------------------------------------
	// This function below is C code, we should use std::vector to replace pointer shaders, and replace 
	// loop while (entry->type != GL_NONE)
	GLuint CreatePipelineShaderProgram(PipelineShaders* shaders)
	{
		if (shaders == NULL) { return 0; }

		GLuint program = glCreateProgram();

		PipelineShaders* entry = shaders;
		while (entry->type != GL_NONE) {
			GLuint shader = glCreateShader(entry->type);
			entry->shader = shader;

			std::unique_ptr<std::string> shaderSource = LoadGLShaderFromResources(entry->resourceID);
			if (shaderSource == NULL) {
				for (entry = shaders; entry->type != GL_NONE; ++entry) {
					glDeleteShader(entry->shader);
					entry->shader = 0;
				}

				return 0;
			}

			const char* source = shaderSource->c_str();

			glShaderSource(shader, 1, &source, NULL);

			glCompileShader(shader);

			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
#ifdef _DEBUG
				GLsizei len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				GLchar* log = new GLchar[len + 1];
				glGetShaderInfoLog(shader, len, &len, log);
				std::cerr << "Shader compilation failed: " << log << std::endl;
				delete[] log;
#endif // #ifdef _DEBUG

				return 0;
			}

			glAttachShader(program, shader);

			++entry;
		}

		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked) {

#ifdef _DEBUG
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			std::cerr << "Shader linking failed: " << log << std::endl;
			delete[] log;
#endif // #ifdef _DEBUG

			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}

		return program;
	}




}//End of namespace slgeom
