#pragma once

#ifndef __SenMeshLinkModel__
#define __SenMeshLinkModel__

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <soil/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SenMeshStruct.h"

class SenMeshLinkModel
{
public:
	/*  Functions   */
	//SenMeshLinkModel()	{ ; }

	// Constructor, expects a filepath to a 3D model.
	SenMeshLinkModel(GLchar* modelAddress)
	{
		this->loadModel(modelAddress);
	}

	// Draws the model, and thus all its meshesVector
	void paintMeshLinkModel(GLuint &program, glm::mat4 &view, glm::mat4 &model, glm::mat4 &projection)
	{
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (GLuint i = 0; i < this->meshesVector.size(); i++)
			this->meshesVector[i].paintMesh(program);
	}

protected:
	/*  SenMeshLinkModel Data  */
	string modelDirectory;
	vector<SenMeshStruct> meshesVector;
	vector<TextureStruct> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

private:
	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshesVector in the meshesVector vector.
	void loadModel(string modelAddress)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(modelAddress, aiProcess_Triangulate | aiProcess_FlipUVs);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Retrieve the modelDirectory modelAddress of the filepath
		this->modelDirectory = modelAddress.substr(0, modelAddress.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshesVector.push_back(this->processMesh(mesh, scene));
		}
		// After we've processed all of the meshesVector (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	SenMeshStruct processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		vector<VertexStruct> vertexStructVector;
		vector<GLuint> indexVector;
		vector<TextureStruct> textureStructVector;

		// Walk through each vertexStruct of the mesh's vertexStructVector
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			VertexStruct vertexStruct;
			glm::vec3 tmpVerAttriVector; // We declare a placeholder tmpVerAttriVector since assimp uses its own tmpVerAttriVector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			tmpVerAttriVector.x = mesh->mVertices[i].x;
			tmpVerAttriVector.y = mesh->mVertices[i].y;
			tmpVerAttriVector.z = mesh->mVertices[i].z;
			vertexStruct.Position = tmpVerAttriVector;
			// Normals
			tmpVerAttriVector.x = mesh->mNormals[i].x;
			tmpVerAttriVector.y = mesh->mNormals[i].y;
			tmpVerAttriVector.z = mesh->mNormals[i].z;
			vertexStruct.Normal = tmpVerAttriVector;
			// TextureStruct Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 tmpTexCoordVector;
				// A vertexStruct can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertexStruct can have multiple texture coordinates so we always take the first set (0).
				tmpTexCoordVector.x = mesh->mTextureCoords[0][i].x;
				tmpTexCoordVector.y = mesh->mTextureCoords[0][i].y;
				vertexStruct.TexCoords = tmpTexCoordVector;
			}
			else	vertexStruct.TexCoords = glm::vec2(0.0f, 0.0f);

			vertexStructVector.push_back(vertexStruct);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertexStruct indexVector.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indexVector of the face and store them in the indexVector vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indexVector.push_back(face.mIndices[j]);
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* prtMeshMaterialStruct = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'meshDiffuseTextureN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: meshDiffuseTextureN
			// Specular: meshSpecularTextureN
			// Normal: meshNormalTextureN

			// 1. Diffuse maps
			vector<TextureStruct> diffuseMaps = this->loadMaterialTextures(prtMeshMaterialStruct, aiTextureType_DIFFUSE, "meshDiffuseTexture");
			textureStructVector.insert(textureStructVector.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			vector<TextureStruct> specularMaps = this->loadMaterialTextures(prtMeshMaterialStruct, aiTextureType_SPECULAR, "meshSpecularTexture");
			textureStructVector.insert(textureStructVector.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return SenMeshStruct(vertexStructVector, indexVector, textureStructVector);
	}

	// Checks all prtMeshMaterialStruct textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a TextureStruct struct.
	vector<TextureStruct> loadMaterialTextures(aiMaterial* prtMeshMaterialStruct, aiTextureType type, string typeName)
	{
		vector<TextureStruct> textures;
		for (GLuint i = 0; i < prtMeshMaterialStruct->GetTextureCount(type); i++)
		{
			aiString str;
			prtMeshMaterialStruct->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				TextureStruct texture;
				texture.id = TextureFromFile(str.C_Str(), this->modelDirectory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}


	GLint TextureFromFile(const char* path, string modelDirectory)
	{
		//Generate texture ID and load texture data 
		string filename = string(path);
		filename = modelDirectory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}

};



#endif // __SenMeshLinkModel__