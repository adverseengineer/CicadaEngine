#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(void) {
	textureData = nullptr;
	glGenTextures(1, &textureId);
}

Texture::~Texture(void) {
	CleanUp();
	glDeleteTextures(1, &textureId);
}

void Texture::CleanUp(void) {
	if (textureData == nullptr)
		return;
	if (!isLoadedFromFile)
		delete[] textureData;
	else
		stbi_image_free(textureData);
	textureData = nullptr;
}

void Texture::SetTextureData(unsigned int count, unsigned char* data) {
	CleanUp();
	textureData = new unsigned char[count];
	std::memcpy(textureData, data, count);
	isLoadedFromFile = false;
	Util::Log("Texture data loaded");
}

void Texture::SetDimensions(unsigned int width, unsigned int height) {

	this->width = width;
	this->height = height;
}

void Texture::SetWrapS(unsigned int wrapS) {
	this->wrapS = wrapS;
}

void Texture::SetWrapT(unsigned int wrapT) {
	this->wrapT = wrapT;
}

void Texture::SetMagFilter(unsigned int magFilter) {
	this->magFilter = magFilter;
}

void Texture::SetMinFilter(unsigned int minFilter) {
	this->minFilter = minFilter;
}

void Texture::SelectToChange(void) const {
	glBindTexture(type, textureId);
}

void Texture::Deselect(void) const {
	glBindTexture(type, 0);
}

void Texture::SelectToRender(int textureUnit) const {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(type, textureId);
}

void Texture::Allocate(void) {
	SelectToChange();
	//set up the texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	//send the texture data to the gpu
	glTexImage2D(type, 0, internalFormat, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, textureData);
	CleanUp();
	glGenerateMipmap(GL_TEXTURE_2D);
	Deselect();
}

void Texture::LoadTextureDataFromFile(const std::string& filepath) {

	CleanUp();
	int width, height;
	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(filepath.c_str(), &width, &height, &numberOfChannels, 0);
	this->width = width;
	this->height = height;
	if (numberOfChannels == 3) {
		sourceFormat = GL_RGB;
	}
	isLoadedFromFile = true;
}
