#pragma once
#include "Util.h"
#include <glad/glad.h>
#include <unordered_map>

typedef unsigned char byte;

namespace FallbackTexture {
	constexpr size_t WIDTH = 64;
	constexpr size_t HEIGHT = 64;
	constexpr size_t DATALEN = WIDTH * HEIGHT * 4;
	extern const byte DATA[DATALEN];
}

class Texture {
private:
	const byte* m_textureData = nullptr;
	bool m_isLoadedFromFile;

	unsigned int m_textureId;
	int m_width;
	int m_height;
	int m_numberOfChannels;
	int m_internalFormat = GL_RGB8;
	unsigned int m_sourceFormat = GL_RGBA;
	unsigned int m_wrapS = GL_REPEAT;
	unsigned int m_wrapT = GL_REPEAT;
	unsigned int m_minFilter = GL_NEAREST;
	unsigned int m_magFilter = GL_NEAREST;

	void UseFallbackData();

public:
	Texture(const std::string& filePath);
	~Texture();

	//disallow copying a Texture, because it manages the lifetime of a resource
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Bind(unsigned int textureUnit = 0) const;
	void Allocate();
};
