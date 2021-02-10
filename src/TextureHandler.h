#pragma once
#include <string>
#include <iostream>
#include <stb_image.h>

class TextureHandler
{
private:
	unsigned int imageId;
public:
	TextureHandler(const char* fileName, bool flipVerticallyImage, bool clampToEdge = false);
	~TextureHandler();
	const unsigned int getTextureId() const;
};

