#pragma once

#include "Texture.h"

using namespace Cicada;

Texture::Texture() {
	glGenTextures(1, &m_texId);
}

Texture::~Texture() {
	//TODO:
}

