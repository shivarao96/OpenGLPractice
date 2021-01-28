#include "Shader.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {	
	if (checkShaderFile(vertexShaderPath) && checkShaderFile(fragmentShaderPath)) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream fVetexFile;
		std::ifstream fFragmentFile;
		fVetexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fFragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<< e.what() << std::endl;
		}

		unsigned int vertexShaderId = setUpShader(SHADERTYPE::VERTEX, vertexCode.c_str(), vertexShaderPath);
		unsigned int fragmentShaderId = setUpShader(SHADERTYPE::FRAGMENT, fragmentCode.c_str(), fragmentShaderPath);
		setUpShaderProgram(vertexShaderId, fragmentShaderId);
	}
}

Shader::~Shader() {
	glDeleteProgram(shaderProgramId);
}

void Shader::use() {
	glUseProgram(shaderProgramId);
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
void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::setUpShader(SHADERTYPE type, const char* shaderCode, const std::string& fileName) {
	GLenum shaderType = (type == SHADERTYPE::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	unsigned int shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);
	int shaderStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus != GL_TRUE) {
		char log[1024];
		glGetShaderInfoLog(shaderId, 1024, NULL, log);
		std::cout << "ERROR::SHADER::"<< fileName <<"::COMPILATION_FAILED\n" << log << std::endl;
	}
	return shaderId;
}
void Shader::setUpShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
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
}

bool Shader::checkShaderFile(const char* file) { // @TODO::move such functions in utility class
	struct stat buffer;
	if (stat(file, &buffer) != 0) {
		std::cout << "File doesn't exist, please provide proper file path::" << file << std::endl;
		return false;
	}
	return true;
}