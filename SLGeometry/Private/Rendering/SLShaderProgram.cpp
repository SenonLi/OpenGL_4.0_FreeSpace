#include "../../stdafx.h"
#include "SLShaderProgram.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include <cstdlib>
#include <iostream>

namespace slgeom
{

	static const GLchar* ReadShaderFile(const char* filename)
	{
		//std::Tstring fullPath = FolderUtility::GetInstance()->GetBuildit_Exe_Path() + filename;
		//FILE * infile = _tfsopen(fullPath.c_str(), L"rb", _SH_DENYWR);
		//ASSERT_AND_RETURN(infile, nullptr, L"Invalid shader file, code must be rewviewed!!");

#ifdef WIN32
		FILE* infile;
		fopen_s(&infile, filename, "rb");
#else
		FILE* infile = fopen(filename, "rb");
#endif // WIN32

		if (!infile) {
#ifdef _DEBUG
			std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
			return NULL;
		}

		fseek(infile, 0, SEEK_END);
		size_t len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		GLchar * source = new GLchar[len + 1];

		fread(source, 1, len, infile);
		fclose(infile);

		source[len] = 0;

		return source;
	}


	//===================================================================================================
	//---------------------------------------------------------------------------------------------------
	GLuint CreatePipelineShaderProgram(PipelineShaders* shaders)
	{
		if (shaders == NULL) { return 0; }

		GLuint program = glCreateProgram();

		PipelineShaders* entry = shaders;
		while (entry->type != GL_NONE) {
			GLuint shader = glCreateShader(entry->type);

			entry->shader = shader;

			const GLchar* source = ReadShaderFile(entry->filename);
			if (source == NULL) {
				for (entry = shaders; entry->type != GL_NONE; ++entry) {
					glDeleteShader(entry->shader);
					entry->shader = 0;
				}

				return 0;
			}

			glShaderSource(shader, 1, &source, NULL);
			delete[] source;

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
