#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

namespace Mesh {
	struct VertexInfo
	{
		glm::vec3 vertices;
		glm::vec3 normal;
		glm::vec3 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitTangent;
	};

	struct TextureInfo
	{
		unsigned int id;
		std::string type;
		std::string path;
	};
	class MeshConfig
	{
	private:
		unsigned int m_VAO, m_VBO, m_EBO;
		std::vector<VertexInfo> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<TextureInfo> m_textures;
	public:
		MeshConfig(
			std::vector<VertexInfo> vertices,
			std::vector<unsigned int> indices,
			std::vector<TextureInfo> textures
		);
		~MeshConfig();
		void drawMesh(Shader &shader);
		const unsigned int getVAO() const;
	private:
		void setupMesh();
	};
}
