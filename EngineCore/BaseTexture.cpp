#pragma once

#include "BaseTexture.h"

BaseTexture::BaseTexture() {
	glGenTextures(1, &m_texId);
}

BaseTexture::~BaseTexture() {
	//TODO:
}

