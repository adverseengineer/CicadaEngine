#pragma once

#include "BaseObject.h"

#include <glad/glad.h>

class Texture : BaseObject {
	
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

	void CleanUp(void);

public:
	Texture(void);
	~Texture(void);

	inline bool IsLoadedFromFile(void) const { return isLoadedFromFile; }
	inline unsigned char* GetTextureData(void) const { return textureData; }
	inline unsigned int GetTextureId(void) const { return textureId; }
	inline int GetNumberOfChannels(void) const { return numberOfChannels; }

	void SetTextureData(unsigned int count, unsigned char* data);
	void SetDimensions(unsigned int width, unsigned int height);
	void SetWrapS(unsigned int wrapS);
	void SetWrapT(unsigned int wrapT);
	void SetMagFilter(unsigned int magFilter);
	void SetMinFilter(unsigned int minFilter);
	void SelectToChange(void) const;
	void Deselect(void) const;
	void SelectToRender(int textureUnit = 0) const;
	void Allocate(void);
	void LoadTextureDataFromFile(const std::string& filepath);
};
