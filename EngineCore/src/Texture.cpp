#pragma once

#include "Texture.h"

using namespace Cicada;

Texture::Texture(GLenum texType) : m_texType(texType) {
	glGenTextures(1, &m_texId);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_texId);
}

void Texture::Bind() const {
	glBindTexture(m_texType, m_texId);
}

void Texture::Unbind() const {
	glBindTexture(m_texType, 0);
}

void Texture::AttachTexUnit(GLenum texUnit) const {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(m_texType, m_texId);
}

void Texture::SetSourceFormat(SourceFormat format) {
	//TODO:
}

void Texture::SetInternalFormat(InternalFormat format) {
	//TODO:
}

void Texture::SetMinFilter(MinFilterMode mode) {
	//TODO:
}

void Texture::SetMagFilter(MagFilterMode mode) {
	//TODO:
}

void Texture::SetWrapS(WrapMode mode) {
	//TODO:
}

void Texture::SetWrapT(WrapMode mode) {
	//TODO:
}

void Texture::SetWrapR(WrapMode mode) {
	//TODO:
}
