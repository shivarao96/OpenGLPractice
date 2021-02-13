#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stb_image.h>


namespace Texture {
	class TextureHandler
	{
	private:
		unsigned int imageId;
	public:
		TextureHandler(const char* fileName, bool flipVerticallyImage, bool clampToEdge = false);
		~TextureHandler();
		const unsigned int getTextureId() const;
	};

	class CubeTextureHandler {
	private:
		std::vector<std::string> m_Faces = {
			"right",
			"left",
			"top",
			"bottom",
			"front",
			"back"
		};
		unsigned int m_cubeTextureId;
	public:
		CubeTextureHandler(const char* skyBoxFolderPath);
		~CubeTextureHandler();
		const unsigned int getCubeTextureId() const;
	};
}

