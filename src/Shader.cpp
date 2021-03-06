#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Utils.h"


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath) {
	if (Utils::checkFile(vertexShaderPath) && Utils::checkFile(fragmentShaderPath)) {
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream fVetexFile;
		std::ifstream fFragmentFile;
		std::ifstream fGeometryFile;
		fVetexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fFragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fGeometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			fVetexFile.open(vertexShaderPath);
			fFragmentFile.open(fragmentShaderPath);
			std::stringstream vertexStream, fragmentStream;
			vertexStream << fVetexFile.rdbuf();
			fragmentStream << fFragmentFile.rdbuf();
			fVetexFile.close();
			fFragmentFile.close();
			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();

			if (geometryShaderPath != nullptr) {
				fGeometryFile.open(geometryShaderPath);
				std::stringstream geometryFileStream;
				geometryFileStream << fGeometryFile.rdbuf();
				fGeometryFile.close();
				geometryCode = geometryFileStream.str();
			}
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<< e.what() << std::endl;
		}

		unsigned int vertexShaderId = setUpShader(SHADERTYPE::VERTEX, vertexCode.c_str(), vertexShaderPath);
		unsigned int fragmentShaderId = setUpShader(SHADERTYPE::FRAGMENT, fragmentCode.c_str(), fragmentShaderPath);
		if (geometryShaderPath != nullptr) {
			unsigned int geometricShaderId = setUpShader(SHADERTYPE::GEOMETRY, geometryCode.c_str(), geometryShaderPath);
			setUpShaderProgram(vertexShaderId, fragmentShaderId, geometricShaderId);
		}
		else {
			setUpShaderProgram(vertexShaderId, fragmentShaderId);
		}
	}
}
Shader::~Shader() {
	glDeleteProgram(shaderProgramId);
}
void Shader::use() {
	glUseProgram(shaderProgramId);
}
const unsigned int Shader::getShaderId() const {
	return shaderProgramId;
}
void Shader::setBool(const std::string& name, bool val) const {
	glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), (int)val);
}
void Shader::setInt(const std::string& name, int val) const {
	glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), val);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderProgramId, name.c_str()), value);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) {
	glUniformMatrix2fv(glGetUniformLocation(shaderProgramId, name.c_str()), GL_FALSE, 1, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) {
	glUniformMatrix3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setVec2(const std::string& name, const glm::vec2& val) {
	glUniform2fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &val[0]);
}
void Shader::setVec3(const std::string& name, const glm::vec3& val) {
	glUniform3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &val[0]);
}
void Shader::setVec4(const std::string& name, const glm::vec4& val) {
	glUniform4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &val[0]);
}
unsigned int Shader::setUpShader(SHADERTYPE type, const char* shaderCode, const std::string& fileName) {
	unsigned int shaderId = glCreateShader((GLenum)type);
	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);
	int shaderStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus != GL_TRUE) {
		char log[1024];
		glGetShaderInfoLog(shaderId, 1024, NULL, log);
		std::cout << "ERROR::SHADER::" << fileName << "::COMPILATION_FAILED\n" << log << std::endl;
	}
	return shaderId;
}
void Shader::setUpShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int geometricShader) {
	shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
	if (geometricShader != -1) {
		glAttachShader(shaderProgramId, geometricShader);
	}
	glLinkProgram(shaderProgramId);

	int programLinkStatus;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &programLinkStatus);
	if (programLinkStatus != GL_TRUE) {
		char log[1024];
		glGetProgramInfoLog(shaderProgramId, 1024, NULL, log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometricShader != -1) {
		glDeleteShader(geometricShader);
	}
}
