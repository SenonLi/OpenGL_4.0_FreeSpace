#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/types.h>


struct VertexStruct {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct TextureStruct {
	GLuint id;
	string type;//material
	aiString path;
};

class SenMeshStruct {
public:
	SenMeshStruct(vector<VertexStruct> vertices, vector<GLuint> indices, vector<TextureStruct> textures)
	{
		this->meshVerticesVector = vertices;
		this->meshIndicesVector = indices;
		this->meshTexturesVector = textures;

		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->initialMeshGL();
	}

	// Initializes all the buffer objects/arrays
	void initialMeshGL()
	{
		initialMeshVertices();
	}

	// Render the mesh
	virtual void paintMesh(GLuint &program)
	{
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->meshTexturesVector.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->meshTexturesVector[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, this->meshTexturesVector[i].id);
		}

		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(program, "material.shininess"), 16.0f);

		// paintMesh mesh
		glBindVertexArray(this->meshVertexArrayObject);
		glDrawElements(GL_TRIANGLES, this->meshIndicesVector.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->meshTexturesVector.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//glUseProgram(0);
	}

	virtual void finalizeMesh()	{
		for (GLuint i = 0; i < this->meshTexturesVector.size(); i++)
		{
			if (glIsTexture(this->meshTexturesVector[i].id))			
				glDeleteTextures(1, &this->meshTexturesVector[i].id);
		}

		if (glIsVertexArray(meshVertexArrayObject))	glDeleteVertexArrays(1, &meshVertexArrayObject);
		if (glIsBuffer(meshVertexBufferObject))		glDeleteBuffers(1, &meshVertexBufferObject);
		if (glIsBuffer(meshIndicesBufferObject))		glDeleteBuffers(1, &meshIndicesBufferObject);

		//if (glIsProgram(cubeProgram))			glDeleteProgram(cubeProgram);
	}

protected:
	GLuint meshProgram;
	glm::mat4 meshCameraView, meshModel, meshProjection;
	GLuint meshVertexArrayObject, meshVertexBufferObject, meshIndicesBufferObject;
	/*  SenMeshStruct Data  */
	vector<VertexStruct> meshVerticesVector;
	vector<GLuint> meshIndicesVector;
	vector<TextureStruct> meshTexturesVector;

	void initialMeshVertices()	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->meshVertexArrayObject);
		glGenBuffers(1, &this->meshVertexBufferObject);
		glGenBuffers(1, &this->meshIndicesBufferObject);

		glBindVertexArray(this->meshVertexArrayObject);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->meshVertexBufferObject);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->meshVerticesVector.size() * sizeof(VertexStruct), &this->meshVerticesVector[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshIndicesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshIndicesVector.size() * sizeof(GLuint), &this->meshIndicesVector[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// VertexStruct Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (GLvoid*)0);
		// VertexStruct Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (GLvoid*)offsetof(VertexStruct, Normal));
		// VertexStruct TextureStruct Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (GLvoid*)offsetof(VertexStruct, TexCoords));

		glBindVertexArray(0);
	}

};


