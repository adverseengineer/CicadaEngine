#pragma once

#include "BaseTexture.h"

using namespace Cicada;

BaseTexture::BaseTexture() {
	glGenTextures(1, &m_texId);
}

BaseTexture::~BaseTexture() {
	//TODO:
}

