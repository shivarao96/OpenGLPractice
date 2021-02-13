#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include "Mesh.h"

class Model
{
private:
	std::string m_directory;
	std::vector<Mesh::MeshConfig*> m_meshes;
	std::vector<Mesh::TextureInfo> m_textureLoaded;
public:
	Model(const char* modelPath);
	~Model();
	void drawModel(Shader &shader);
private:
	void loadModel(const std::string modelPath);
	void processNode(
		aiNode* node, 
		const aiScene* scene
	);
	Mesh::MeshConfig processMesh(
		aiMesh* mesh,
		const aiScene* scene
	);
	std::vector<Mesh::TextureInfo> loadMaterialTextures(
		aiMaterial* mat,
		aiTextureType type,
		std::string typeName
	);
	Texture::TextureHandler* textureFromFile(
		const char* fileName, 
		bool flipVerticallyImage
	);
};

