#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const std::string Texture::s_fallbackPath = "fallback.png";

Texture::Texture(const std::string& filePath) {
	
	glGenTextures(1, &m_textureId);

	stbi_set_flip_vertically_on_load(true);
	m_textureData = stbi_load(filePath.c_str(), &m_width, &m_height, &m_numChannels, 0);
	if (m_textureData == nullptr) {
		Util::Log("Unable to load texture data from file \"" + filePath + "\", resorting to fallback");
		m_isFallback = true;
		m_textureData = stbi_load(s_fallbackPath.c_str(), &m_width, &m_height, &m_numChannels, 0);
		assert(m_textureData != nullptr, "unable to load fallback texture");
	}

	if (m_numChannels == 3) {
		m_sourceFormat = Format::RGB;
	}

	m_isLoadedFromFile = true;
	if (!m_isFallback)
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

void Texture::Bind() const {
	glActiveTexture(GL_TEXTURE0 + (GLenum)m_textureUnit);
	glBindTexture(GL_TEXTURE_2D, (GLuint)m_textureId);
}

void Texture::Allocate() {
	glBindTexture(GL_TEXTURE_2D, (GLuint)m_textureId); //select the texture for modification
	//set up the texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) m_magFilter);
	//send the texture data to the gpu
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint) m_internalFormat, m_width, m_height, 0, (GLint) m_sourceFormat, GL_UNSIGNED_BYTE, m_textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); //deselect the texture for modification
}
