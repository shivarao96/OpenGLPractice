#pragma once
#include <string>

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
	bool checkShaderFile(const char* file); // @TODO::move such functions in utility class
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void use();
	void setBool(const std::string& name, bool val) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float value) const;
};

