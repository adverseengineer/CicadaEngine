#pragma once
#include "Util.h"
#include <glad/glad.h>

class Texture {	
private:
	static const unsigned char FALLBACK_DATA[64*64*4];
protected:
	unsigned char* textureData;
	bool isLoadedFromFile = false;

	unsigned int textureId;
	unsigned int type = GL_TEXTURE_2D;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int internalFormat = GL_RGB8;
	unsigned int sourceFormat = GL_RGBA;
	unsigned int wrapS = GL_REPEAT;
	unsigned int wrapT = GL_REPEAT;
	unsigned int magFilter = GL_NEAREST;
	unsigned int minFilter = GL_NEAREST;
	int numberOfChannels = 4;

	void CleanUp();

public:
	Texture();
	~Texture();

	inline bool IsLoadedFromFile() const { return isLoadedFromFile; }
	inline unsigned char* GetTextureData() const { return textureData; }
	inline unsigned int GetTextureId() const { return textureId; }
	inline int GetNumberOfChannels() const { return numberOfChannels; }

	void SetUpFallbackTexture();
	void LoadTextureDataFromFile(const std::string& filepath);
	void SetTextureData(unsigned int count, const unsigned char* data);
	
	inline void SetWidth(unsigned int width) { this->width = width; }
	inline void SetHeight(unsigned int height) { this->height = height; }
	inline void SetDimensions(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}

	inline void SetWrapS(unsigned int wrapS) { this->wrapS = wrapS; }
	inline void SetWrapT(unsigned int wrapT) { this->wrapT = wrapT; }

	inline void SetMagFilter(unsigned int magFilter) { this->magFilter = magFilter; }
	inline void SetMinFilter(unsigned int minFilter) { this->minFilter = minFilter; }


	void SelectToChange() const;
	void SelectToRender(int textureUnit = 0) const;
	void Deselect() const;
	void Allocate();
};
