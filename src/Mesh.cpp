#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>


Mesh::MeshConfig::MeshConfig(
	std::vector<VertexInfo> vertices,
	std::vector<unsigned int> indices,
	std::vector<TextureInfo> textures
) {
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures;

	this->setupMesh();
}
Mesh::MeshConfig::~MeshConfig() {
	glDeleteVertexArrays(1, &this->m_VAO);
	glDeleteBuffers(1, &this->m_VBO);
	glDeleteBuffers(1, &this->m_EBO);
}
void Mesh::MeshConfig::drawMesh(Shader& shader) {
	unsigned int nrDiffuse = 1;
	unsigned int nrSpecular = 1;
	unsigned int nrNormal = 1;
	unsigned int nrheight = 1;

	for (int i = 0; i < this->m_textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = this->m_textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(nrDiffuse++);
		if(name == "texture_specular")
			number = std::to_string(nrSpecular++);
		if(name == "texture_normal")
			number = std::to_string(nrNormal++);
		if(name == "texture_height")
			number = std::to_string(nrheight++);

		shader.setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].texture->getTextureId());
	}
	glBindVertexArray(this->m_VAO);
	if (m_indices.size()) {
		glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	}
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::MeshConfig::setupMesh() {
	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->m_EBO);

	glBindVertexArray(this->m_VAO);

	glBindBuffer(
		GL_ARRAY_BUFFER,
		this->m_VBO
	);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->m_vertices.size() * sizeof(VertexInfo),
		&m_vertices[0],
		GL_STATIC_DRAW
	);

	if (m_indices.size()) {
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER, 
			this->m_EBO
		);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			this->m_indices.size() * sizeof(unsigned int), 
			&m_indices[0], 
			GL_STATIC_DRAW
		);
	}

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexInfo),
		(void*)0
	);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexInfo),
		(void*)offsetof(VertexInfo, normal)
	);
	// texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexInfo),
		(void*)offsetof(VertexInfo, texCoords)
	);
	// tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexInfo),
		(void*)offsetof(VertexInfo, tangent)
	);
	// bit tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexInfo),
		(void*)offsetof(VertexInfo, bitTangent)
	);

	glBindVertexArray(0);
}

const unsigned int Mesh::MeshConfig::getVAO() const {
	return this->m_VAO;
}