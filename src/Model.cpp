#include "Model.h"
#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

Model::Model(const char* modelPath) {
	this->loadModel(modelPath);
}

Model::~Model() {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
	}
	for (unsigned int i = 0; i < m_textureLoaded.size(); i++) {
		delete m_textureLoaded[i].texture;
	}
}

void Model::drawModel(Shader& shader) {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		m_meshes[i]->drawMesh(shader);
	}
}

void Model::loadModel(const std::string modelPath) {
	Assimp::Importer modelImport;
	const aiScene* scene = modelImport.ReadFile(
		modelPath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | 
		aiProcess_CalcTangentSpace
	);
	if (
		!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode
		) {
		std::cout << "ERROR::ASSIMP::" << modelImport.GetErrorString() << std::endl;
		return;
	}
	this->m_directory = modelPath.substr(0, modelPath.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(new Mesh::MeshConfig(processMesh(mesh, scene)));
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh::MeshConfig Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Mesh::VertexInfo> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::TextureInfo> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Mesh::VertexInfo v;
		v.vertices = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		if (mesh->HasNormals()) {
			v.normal = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}
		if (mesh->mTextureCoords[0]) {
			v.texCoords = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
			v.tangent = glm::vec3(
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z
			);
			v.bitTangent = glm::vec3(
				mesh->mBitangents[i].x,
				mesh->mBitangents[i].y,
				mesh->mBitangents[i].z
			);
		}
		else {
			v.texCoords = glm::vec2(0.0f);
		}
		vertices.push_back(v);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Mesh::TextureInfo> diffuse = loadMaterialTextures(
			material, 
			aiTextureType_DIFFUSE, 
			"texture_diffuse"
		);
		textures.insert(
			textures.end(), 
			diffuse.begin(), 
			diffuse.end()
		);

		std::vector<Mesh::TextureInfo> specular = loadMaterialTextures(
			material,
			aiTextureType_SPECULAR,
			"texture_specular"
		);
		textures.insert(
			textures.end(),
			specular.begin(),
			specular.end()
		);

		std::vector<Mesh::TextureInfo> normals = loadMaterialTextures(
			material,
			aiTextureType_HEIGHT,
			"texture_normal"
		);
		textures.insert(
			textures.end(),
			normals.begin(),
			normals.end()
		);

		std::vector<Mesh::TextureInfo> height = loadMaterialTextures(
			material,
			aiTextureType_AMBIENT,
			"texture_height"
		);
		textures.insert(
			textures.end(),
			height.begin(),
			height.end()
		);
	//}

	return Mesh::MeshConfig(vertices, indices, textures);
}

std::vector<Mesh::TextureInfo> Model::loadMaterialTextures(
	aiMaterial* mat,
	aiTextureType type,
	std::string typeName
) {
	std::vector<Mesh::TextureInfo> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_textureLoaded.size(); j++) {
			if (std::strcmp(m_textureLoaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(m_textureLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Mesh::TextureInfo texture;
			texture.texture = textureFromFile(str.C_Str(), false);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_textureLoaded.push_back(texture);
		}
	}
	return textures;
}

Texture::TextureHandler* Model::textureFromFile(const char* fileN, bool flipVerticallyImage) {
	std::string filename = std::string(fileN);
	filename = m_directory + '/' + filename;
	return new Texture::TextureHandler(filename.c_str(), false);
}