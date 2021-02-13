#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	enum class SHADERTYPE
	{
		VERTEX = 0,
		FRAGMENT = 1,
		GEOMETRY = 2
	};
	unsigned int shaderProgramId = NULL;
	unsigned int setUpShader(SHADERTYPE type, const char* shaderCode, const std::string& fileName);
	void setUpShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void use();
	const unsigned int getShaderId() const;
	void setBool(const std::string& name, bool val) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float value) const;
	void setMat2(const std::string& name, const glm::mat2& mat);
	void setMat3(const std::string& name, const glm::mat3& mat);
	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec2(const std::string& name, const glm::vec2& val);
	void setVec3(const std::string& name, const glm::vec3& val);
	void setVec4(const std::string& name, const glm::vec4& val);
};

