#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Cicada;

const std::string Texture2D::s_fallbackPath = "res/tex/fallback.png";

Texture2D::Texture2D(std::string_view name, std::string_view filePath) : Texture(GL_TEXTURE_2D), Asset(name) {
	
	stbi_set_flip_vertically_on_load(true);
	m_textureData = stbi_load(filePath.data(), &m_width, &m_height, &m_numChannels, 0);
	if (m_textureData == nullptr) {
		Log::Error("Unable to load texture data from file {:?}, resorting to fallback", filePath);
		m_isFallback = true;
		m_textureData = stbi_load(s_fallbackPath.c_str(), &m_width, &m_height, &m_numChannels, 0);
		assert(m_textureData != nullptr);
	}

	//still need to set this flag, because it is used when freeing the memory
	m_isLoadedFromFile = true;
	if (!m_isFallback)
		Log::Info("Texture data loaded from file: {:?}", filePath);

	//TODO: is this okay to do here?
	Upload();
}

Texture2D::~Texture2D() {

	if (m_textureData == nullptr)
		return;
	if (m_isLoadedFromFile) //if the texture was loaded from a file, use stb's special free function
		stbi_image_free((void*) m_textureData);
	else //otherwise delete normally
		delete[] m_textureData;
	m_textureData = nullptr;
}

void Texture2D::Upload() const {
	glBindTexture(GL_TEXTURE_2D, m_texId); //select the texture for modification
	//set up the texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int) m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int) m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int) m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int) m_magFilter);
	//send the texture data to the gpu
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(m_internalFormat), m_width, m_height, 0, static_cast<GLenum>(m_sourceFormat), GL_UNSIGNED_BYTE, m_textureData);
	//generate mipmaps //TODO: research this more to decide if it needs to be its own function
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
