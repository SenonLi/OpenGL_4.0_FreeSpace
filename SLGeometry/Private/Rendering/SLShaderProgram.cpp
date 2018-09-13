#include "../../stdafx.h"
#include "SLShaderProgram.h"
#include "../Resources/SLGLShadersResource.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include <cstdlib>
#include <iostream>
#include <memory>          // For unique_ptr

namespace slgeom
{

//	static const GLchar* ReadShaderFile(const char* filename)
//	{
//		//std::Tstring fullPath = FolderUtility::GetInstance()->GetBuildit_Exe_Path() + filename;
//		//FILE * infile = _tfsopen(fullPath.c_str(), L"rb", _SH_DENYWR);
//		//ASSERT_AND_RETURN(infile, nullptr, L"Invalid shader file, code must be rewviewed!!");
//
//#ifdef WIN32
//		FILE* infile;
//		fopen_s(&infile, filename, "rb");
//#else
//		FILE* infile = fopen(filename, "rb");
//#endif // WIN32
//
//		if (!infile) {
//#ifdef _DEBUG
//			std::cerr << "Unable to open file '" << filename << "'" << std::endl;
//#endif /* DEBUG */
//			return NULL;
//		}
//
//		fseek(infile, 0, SEEK_END);
//		size_t len = ftell(infile);
//		fseek(infile, 0, SEEK_SET);
//
//		GLchar * source = new GLchar[len + 1];
//
//		fread(source, 1, len, infile);
//		fclose(infile);
//
//		source[len] = 0;
//
//		return source;
//	}

	/// <summary>Loads the program from the embedded resources, using the resource ID</summary>
	/// <param name="sourceID">The resouce ID</param>
	/// <returns>The program source, or empty on error (an assert will be triggered)</returns>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	/// <remark>This function should be put into slutil::FileManager.h !! </remark>
	static std::unique_ptr<std::string> LoadSourceFromResources(int resourceID)
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

			//const GLchar* source = ReadShaderFile(entry->filename);
			//if (source == NULL) {
			//	for (entry = shaders; entry->type != GL_NONE; ++entry) {
			//		glDeleteShader(entry->shader);
			//		entry->shader = 0;
			//	}

			//	return 0;
			//}

			//glShaderSource(shader, 1, &source, NULL);
			//delete[] source;

			std::unique_ptr<std::string> shaderSource = LoadSourceFromResources(entry->resourceID);
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

#ifdef GL_VERSION_4_1
		if (GLEW_VERSION_4_1) {
			// glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
		}
#endif /* GL_VERSION_4_1 */

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
