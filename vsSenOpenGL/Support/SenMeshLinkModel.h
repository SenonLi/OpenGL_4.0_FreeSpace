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
		:meshLinkModelAddress(modelAddress), modelDirectory(string(""))
	{	;	}

	void initialMeshLinkModelGL()	{
		// program is initialized in the control widget
		// Here is to initial vertices and overlooking textures for Meshes
		this->loadModel(meshLinkModelAddress);
	}

	// Draws the model, and thus all its meshesVector
	void paintMeshLinkModel(GLuint &program, glm::mat4 &view, glm::mat4 &model, glm::mat4 &projection)
	{
		// There might be other uniforms, due to which the glUseProgram must be called in the control widget
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (GLuint i = 0; i < this->meshesVector.size(); i++)
			this->meshesVector[i].paintMesh(program);
	}

	void finilizeMeshLinkModel()	{
		// program is finalized in the control widget
		// Here is to finalize overlooking texutureIDs and Meshes' vertices' attributes.
		for (GLuint i=0; i < meshLinkTotalLoadedTexStructVector.size(); i++)
		{
			if (glIsTexture(meshLinkTotalLoadedTexStructVector[i].texID))			
				glDeleteTextures(1, &meshLinkTotalLoadedTexStructVector[i].texID);
		}

		for (GLuint i = 0; i < this->meshesVector.size(); i++)
			this->meshesVector[i].finalizeMesh();
	}

protected:
	/*  SenMeshLinkModel Data  */
	GLchar* meshLinkModelAddress;
	string modelDirectory;
	vector<SenMeshStruct> meshesVector;
	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<TextureStruct> meshLinkTotalLoadedTexStructVector;

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
		vector<VertexStruct> meshVertexStructVector;
		vector<GLuint> meshIndexVector;
		vector<TextureStruct> meshTextureStructVector;

		// Walk through each meshVertexStruct of the mesh's meshVertexStructVector
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			VertexStruct meshVertexStruct;
			glm::vec3 tmpVerAttriVector; // We declare a placeholder tmpVerAttriVector since assimp uses its own tmpVerAttriVector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			tmpVerAttriVector.x = mesh->mVertices[i].x;
			tmpVerAttriVector.y = mesh->mVertices[i].y;
			tmpVerAttriVector.z = mesh->mVertices[i].z;
			meshVertexStruct.Position = tmpVerAttriVector;
			// Normals
			tmpVerAttriVector.x = mesh->mNormals[i].x;
			tmpVerAttriVector.y = mesh->mNormals[i].y;
			tmpVerAttriVector.z = mesh->mNormals[i].z;
			meshVertexStruct.Normal = tmpVerAttriVector;
			// TextureStruct Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 tmpTexCoordVector;
				// A meshVertexStruct can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a meshVertexStruct can have multiple texture coordinates so we always take the first set (0).
				tmpTexCoordVector.x = mesh->mTextureCoords[0][i].x;
				tmpTexCoordVector.y = mesh->mTextureCoords[0][i].y;
				meshVertexStruct.TexCoords = tmpTexCoordVector;
			}
			else	meshVertexStruct.TexCoords = glm::vec2(0.0f, 0.0f);

			meshVertexStructVector.push_back(meshVertexStruct);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding meshVertexStruct meshIndexVector.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all meshIndexVector of the face and store them in the meshIndexVector vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				meshIndexVector.push_back(face.mIndices[j]);
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
			vector<TextureStruct> diffuseTexStructVector = this->retrieveUploadedMaterialTexStructVector(prtMeshMaterialStruct, aiTextureType_DIFFUSE, "meshDiffuseTexture");
			meshTextureStructVector.insert(meshTextureStructVector.end(), diffuseTexStructVector.begin(), diffuseTexStructVector.end());
			// 2. Specular maps
			vector<TextureStruct> specularTexStructVector = this->retrieveUploadedMaterialTexStructVector(prtMeshMaterialStruct, aiTextureType_SPECULAR, "meshSpecularTexture");
			meshTextureStructVector.insert(meshTextureStructVector.end(), specularTexStructVector.begin(), specularTexStructVector.end());
		}

		// Return a mesh object created from the extracted mesh data
		return SenMeshStruct(meshVertexStructVector, meshIndexVector, meshTextureStructVector);
	}

	// Checks all prtMeshMaterialStruct materialTexStructVector of a given texMaterialType and loads the materialTexStructVector if they're not loaded yet.
	// The required info is returned as a TextureStruct struct.
	vector<TextureStruct> retrieveUploadedMaterialTexStructVector(aiMaterial* prtMeshMaterialStruct, aiTextureType texMaterialType, string texNameInShader)
	{
		vector<TextureStruct> materialTexStructVector;
		for (GLuint i = 0; i < prtMeshMaterialStruct->GetTextureCount(texMaterialType); i++)
		{
			aiString imageNameStruct;
			prtMeshMaterialStruct->GetTexture(texMaterialType, i, &imageNameStruct);
			// Check if textureStruct was loaded before and if so, continue to next iteration: skip loading a new textureStruct
			GLboolean skip = false;
			for (GLuint j = 0; j < meshLinkTotalLoadedTexStructVector.size(); j++)
			{
				if (meshLinkTotalLoadedTexStructVector[j].srcImageName == imageNameStruct)
				{
					materialTexStructVector.push_back(meshLinkTotalLoadedTexStructVector[j]);
					skip = true; // A textureStruct with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If textureStruct hasn't been loaded already, load it
				TextureStruct textureStruct;
				textureStruct.texID = uploadTextureGetID(imageNameStruct.C_Str(), this->modelDirectory);
				textureStruct.inShaderTexTypeName = texNameInShader;
				textureStruct.srcImageName = imageNameStruct;

				materialTexStructVector.push_back(textureStruct);
				// Store it as textureStruct loaded for entire model, to ensure we won't unneceserily load duplicated materialTexStructVector.
				this->meshLinkTotalLoadedTexStructVector.push_back(textureStruct);
			}
		}
		return materialTexStructVector;
	}

	// meshes may share same texture, due to which the process of textureUpload must stay in the MeshLinkModel, instead of Mesh
	GLint uploadTextureGetID(const char* imageName, string modelDirectory)
	{
		//Generate texture ID and load texture data 
		string imageFileName = string(imageName);
		imageFileName = modelDirectory + '/' + imageFileName;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(imageFileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
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