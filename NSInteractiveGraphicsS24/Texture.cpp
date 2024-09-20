#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& filePath) {
	glGenTextures(1, &m_textureId);

	stbi_set_flip_vertically_on_load(true);
	m_textureData = stbi_load(filePath.c_str(), &m_width, &m_height, &m_numberOfChannels, 0);
	if (m_textureData == nullptr) {
		Util::Log("Failed to load texture data from file: " + filePath);
		UseFallbackData();
		return;
	}

	if (m_numberOfChannels == 3) {
		m_sourceFormat = GL_RGB;
	}

	m_isLoadedFromFile = true;

	Util::Log("Texture data loaded from file: " + filePath);
}

Texture::~Texture() {
	if (m_textureData == nullptr)
		return;
	if (m_isLoadedFromFile) //if the texture was loaded from a file, use stb's special free function
		stbi_image_free((void*)m_textureData);
	else //otherwise delete normally
		delete[] m_textureData;
	m_textureData = nullptr;

	glDeleteTextures(1, &m_textureId);
}

void Texture::UseFallbackData() {

	m_width = 64;
	m_height = 64;

	m_wrapS = GL_REPEAT;
	m_wrapT = GL_REPEAT;
	m_minFilter = GL_NEAREST;
	m_magFilter = GL_NEAREST;

	m_isLoadedFromFile = false;
	m_textureData = new unsigned char[FallbackTexture::DATALEN];
	//TODO: set m_textureData to the address of the root data, no need to copy?
	std::memcpy((void*)m_textureData, FallbackTexture::DATA, FallbackTexture::DATALEN);
}

void Texture::Bind() const {
	glActiveTexture(GL_TEXTURE0 + (GLenum)m_textureUnit);
	glBindTexture(GL_TEXTURE_2D, (GLuint)m_textureId);
}

void Texture::Allocate() {
	glBindTexture(GL_TEXTURE_2D, (GLuint)m_textureId); //select the texture for modification
	//set up the texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
	//send the texture data to the gpu
	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_sourceFormat, GL_UNSIGNED_BYTE, m_textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); //deselect the texture for modification
}
