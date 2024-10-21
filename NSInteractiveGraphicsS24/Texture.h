#pragma once
#include "Util.h"
#include <glad/glad.h>
#include <unordered_map>

typedef unsigned char byte;

class Texture {
public:
	enum class WrapMode : GLint {
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum class FilterMode : GLint {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	//TODO: sort these out better
	enum class Format : GLint {
		// Color formats
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,
		SRGB8 = GL_SRGB8,
		SRGB8_Alpha8 = GL_SRGB8_ALPHA8,

		// Depth and stencil formats
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent32F = GL_DEPTH_COMPONENT32F,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		Depth32FStencil8 = GL_DEPTH32F_STENCIL8,

		// Compressed formats
		CompressedRGB = GL_COMPRESSED_RGB,
		CompressedRGBA = GL_COMPRESSED_RGBA,
		CompressedSRGB = GL_COMPRESSED_SRGB,
		CompressedSRGBAlpha = GL_COMPRESSED_SRGB_ALPHA
	};

private:
	const static std::string s_fallbackPath;
	
	const byte* m_textureData = nullptr;
	bool m_isLoadedFromFile = false;
	bool m_isFallback = false;

	unsigned int m_textureId;
	int m_width;
	int m_height;
	int m_numChannels;

	Format m_internalFormat = Format::RGB8;
	Format m_sourceFormat = Format::RGBA;

	WrapMode m_wrapS = WrapMode::Repeat;
	WrapMode m_wrapT = WrapMode::Repeat;

	FilterMode m_minFilter = FilterMode::Nearest;
	FilterMode m_magFilter = FilterMode::Nearest;

	unsigned int m_textureUnit = 0;

public:

	Texture(const std::string& filePath);
	~Texture();

	//disallow copying a Texture, because it manages the lifetime of a resource
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	inline unsigned int GetTextureUnit() const { return m_textureUnit; }
	inline void SetTextureUnit(unsigned int textureUnit) { m_textureUnit = textureUnit; }

	//TODO: add methods to mutate these new enum fields and send the new data to the GPU

	void Bind() const;
	void Allocate();
};
