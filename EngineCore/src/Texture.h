#pragma once

#include "ManagedObject.h"
#include <cstdint>
#include <glad/glad.h>

namespace Cicada {

class Texture {
public:

	enum class WrapMode : int {
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum class FilterMode : int {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	//TODO: sort these out better
	enum class Format : int {
		//color formats
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,
		SRGB8 = GL_SRGB8,
		SRGB8_Alpha8 = GL_SRGB8_ALPHA8,

		//depth and stencil formats
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent32F = GL_DEPTH_COMPONENT32F,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		Depth32FStencil8 = GL_DEPTH32F_STENCIL8,

		//compressed formats
		CompressedRGB = GL_COMPRESSED_RGB,
		CompressedRGBA = GL_COMPRESSED_RGBA,
		CompressedSRGB = GL_COMPRESSED_SRGB,
		CompressedSRGBAlpha = GL_COMPRESSED_SRGB_ALPHA
	};

protected:
	const uint8_t* m_textureData = nullptr;
	bool m_isLoadedFromFile = false;
	bool m_isFallback = false;

	unsigned int m_texId;
	unsigned int m_texUnit = 0;

	Format m_internalFormat = Format::RGBA;
	Format m_sourceFormat = Format::RGBA;

	FilterMode m_minFilter = FilterMode::Nearest;
	FilterMode m_magFilter = FilterMode::Nearest;

	WrapMode m_wrapU = WrapMode::Repeat;
	WrapMode m_wrapV = WrapMode::Repeat;
	
public:
	Texture();
	~Texture();

	virtual void Upload() const = 0;
};

}
