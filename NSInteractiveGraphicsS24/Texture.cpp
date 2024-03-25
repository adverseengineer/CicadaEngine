#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	textureData = nullptr;
	glGenTextures(1, &textureId);
}

Texture::~Texture() {
	CleanUp();
	glDeleteTextures(1, &textureId);
}

void Texture::CleanUp() {
	if (textureData == nullptr)
		return;
	if (!isLoadedFromFile)
		delete[] textureData;
	else
		stbi_image_free(textureData);
	textureData = nullptr;
}

void Texture::SetUpFallbackTexture() {
	SetDimensions(64, 64);
	SetTextureData(64 * 64 * 4, FALLBACK_DATA);
	SetWrapS(GL_REPEAT);
	SetWrapT(GL_REPEAT);
	SetMagFilter(GL_NEAREST);
	char addrBuf[18];
	snprintf(addrBuf, 18, "0x%p", FALLBACK_DATA);
	Util::Log("Loaded fallback data from: " + std::string(addrBuf));
}

void Texture::LoadTextureDataFromFile(const std::string& filepath) {

	CleanUp();
	int width, height;
	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(filepath.c_str(), &width, &height, &numberOfChannels, 0);
	if (textureData == nullptr) {
		Util::Log("Failed to load texture data from file: " + filepath);
		SetUpFallbackTexture();
		return;
	}
	
	this->width = width;
	this->height = height;
	if (numberOfChannels == 3) {
		sourceFormat = GL_RGB;
	}
	isLoadedFromFile = true;
	Util::Log("Texture data loaded from file: " + filepath);
}

void Texture::SetTextureData(unsigned int count, const unsigned char* data) {
	CleanUp();
	textureData = new unsigned char[count];
	std::memcpy(textureData, data, count);
	isLoadedFromFile = false;
	char addrBuf[18];
	snprintf(addrBuf, 18, "0x%p", data);
	Util::Log("Texture data loaded from memory: " + std::string(addrBuf));
}

void Texture::SelectToChange() const {
	glBindTexture(type, textureId);
}

void Texture::Deselect() const {
	glBindTexture(type, 0);
}

void Texture::SelectToRender(int textureUnit) const {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(type, textureId);
}

void Texture::Allocate() {
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
