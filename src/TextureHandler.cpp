#include "TextureHandler.h"
#include <glad/glad.h>

//..Texture handler code start
Texture::TextureHandler::TextureHandler(const char* fileName, bool flipVerticallyImage, bool clampToEdge) {
	if (flipVerticallyImage) {
		stbi_set_flip_vertically_on_load(true);
	}
	glGenTextures(1, &imageId);
	glBindTexture(GL_TEXTURE_2D, imageId);

	GLenum param = clampToEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannel;
	unsigned char* imageData = stbi_load(fileName, &width, &height, &nrChannel, 0);

	if (imageData) {
		GLenum format;
		if (nrChannel > 3) {
			format = GL_RGBA;
		}
		else {
			format = GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load image::" << fileName << std::endl;
	}
	stbi_image_free(imageData);
}
Texture::TextureHandler::~TextureHandler() {
	glDeleteTextures(1, &imageId);
}
const unsigned int Texture::TextureHandler::getTextureId() const {
	return imageId;
}
//..Texture handler code end

//..Cube texture handler code start
Texture::CubeTextureHandler::CubeTextureHandler(const char* skyBoxFolderPath) {
	glGenTextures(1, &m_cubeTextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTextureId);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < m_Faces.size(); i++) {
		std::string fileName = skyBoxFolderPath + m_Faces[i] + ".jpg";
		unsigned char* imageData = stbi_load(
			fileName.c_str(),
			&width,
			&height,
			&nrChannels,
			0
		);
		if (imageData) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				imageData
			);
			stbi_image_free(imageData);
		}
		else {
			const std::string info =
				"Failed to load the texture \n"
				"Please check the path \n"
				"IMPORTANT::(current application only accecpts the JPG format file)\n"
				"File path::" + fileName;
			std::cout << info << std::endl;
			stbi_image_free(imageData);
		}
	}
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR
	);
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
	);
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE
	);
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE
	);
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_WRAP_R,
		GL_CLAMP_TO_EDGE
	);
}

Texture::CubeTextureHandler::~CubeTextureHandler() {
	glDeleteTextures(1, &m_cubeTextureId);
}

const unsigned int Texture::CubeTextureHandler::getCubeTextureId() const {
	return m_cubeTextureId;
}
//..Cube texture handler code end
