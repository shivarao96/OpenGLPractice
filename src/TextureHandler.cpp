#include "TextureHandler.h"
#include <glad/glad.h>

TextureHandler::TextureHandler(const char* fileName, bool flipVerticallyImage) {
	if (flipVerticallyImage) {
		stbi_set_flip_vertically_on_load(true);
	}
	glGenTextures(1, &imageId);
	glBindTexture(GL_TEXTURE_2D, imageId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
TextureHandler::~TextureHandler() {
	std::cout << "Texture Destructor called" << std::endl;
	glDeleteTextures(1, &imageId);
}
const unsigned int TextureHandler::getTextureId() const {
	return imageId;
}