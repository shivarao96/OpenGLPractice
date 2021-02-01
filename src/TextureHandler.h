#pragma once
#include <string>
#include <iostream>
#include <stb_image.h>

class TextureHandler
{
private:
	unsigned int imageId;
public:
	TextureHandler(const char* fileName, bool flipVerticallyImage);
	~TextureHandler();
	const unsigned int getTextureId() const;
};

