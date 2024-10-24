#pragma once
#include "BaseTexture.h"
#include "Util.h"

class Texture2D : public BaseTexture {
private:
	const static std::string s_fallbackPath;
	
	const byte* m_textureData = nullptr;
	bool m_isLoadedFromFile = false;
	bool m_isFallback = false;
	
	unsigned int m_texId;
	unsigned int m_texUnit = 0;
	int m_width;
	int m_height;
	int m_numChannels;

	Format m_internalFormat = Format::RGBA;
	Format m_sourceFormat = Format::RGBA;

	WrapMode m_wrapU = WrapMode::Repeat;
	WrapMode m_wrapV = WrapMode::Repeat;

	FilterMode m_minFilter = FilterMode::Nearest;
	FilterMode m_magFilter = FilterMode::Nearest;
	
public:

	Texture2D(const std::string& filePath);
	~Texture2D();

	//disallow copying a Texture, because it manages the lifetime of a resource
	//TODO: allow copying, but generate a new GPU-side ID for the new texture to manage
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	inline unsigned int GetTextureUnit() const { return m_texUnit; }
	inline void SetTextureUnit(unsigned int textureUnit) { m_texUnit = textureUnit; }

	void Bind();

	void SelectForRendering();
	void Upload() const;

	void SetWrapU(WrapMode mode);
	void SetWrapV(WrapMode mode);

	void SetInternalFormat(Format format);
	void SetSourceFormat(Format format);

	void SetMinFilter(FilterMode mode);
	void SetMagFilter(FilterMode mode);

};
