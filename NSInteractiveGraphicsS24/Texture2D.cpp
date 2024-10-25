#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const std::string Texture2D::s_fallbackPath = "fallback.png";

Texture2D::Texture2D(const std::string& filePath) : BaseTexture() {
	
	stbi_set_flip_vertically_on_load(true);
	m_textureData = stbi_load(filePath.c_str(), &m_width, &m_height, &m_numChannels, 0);
	if (m_textureData == nullptr) {
		Util::Log("Unable to load texture data from file \"" + filePath + "\", resorting to fallback");
		m_isFallback = true;
		m_textureData = stbi_load(s_fallbackPath.c_str(), &m_width, &m_height, &m_numChannels, 0);
		assert(m_textureData != nullptr);
	}

	//still need to set this flag, because it is used when freeing the memory
	m_isLoadedFromFile = true;
	if (!m_isFallback)
		Util::Log("Texture data loaded from file: " + filePath);
}

Texture2D::~Texture2D() {

	//TODO: move this into the base class destructor
	glDeleteTextures(1, &m_texId);

	if (m_textureData == nullptr)
		return;
	if (m_isLoadedFromFile) //if the texture was loaded from a file, use stb's special free function
		stbi_image_free((void*) m_textureData);
	else //otherwise delete normally
		delete[] m_textureData;
	m_textureData = nullptr;
}

//call this to select the texture for modification
void Texture2D::Bind() {
	glBindTexture(GL_TEXTURE_2D, (GLuint) m_texId);
}

//call this in order to render the texture
void Texture2D::SelectForRendering() {
	glActiveTexture(GL_TEXTURE0 + (GLenum) m_texUnit);
	glBindTexture(GL_TEXTURE_2D, (GLuint)m_texId);
}

//TODO: for tomorrow: add the optimizations to this function too, or consider removing the optimizations bc they suck

void Texture2D::Upload() const {
	glBindTexture(GL_TEXTURE_2D, (GLuint) m_texId); //select the texture for modification
	//set up the texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) m_wrapU);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) m_wrapV);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) m_magFilter);
	//send the texture data to the gpu
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint) m_internalFormat, m_width, m_height, 0, (GLint) m_sourceFormat, GL_UNSIGNED_BYTE, m_textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetWrapU(WrapMode mode) {

	//TODO:
}

void Texture2D::SetWrapV(WrapMode mode) {

	//TODO:
}

void Texture2D::SetInternalFormat(Format format) {

	//TODO:
}

void Texture2D::SetSourceFormat(Format format) {

	//TODO:
}

void Texture2D::SetMinFilter(FilterMode mode) {

	//TODO:
}

void Texture2D::SetMagFilter(FilterMode mode) {

	//TODO:
}
