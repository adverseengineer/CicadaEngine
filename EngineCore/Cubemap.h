#pragma once

#pragma once

#include "Texture.h"
#include "Util.h"

namespace Cicada {

class Cubemap : public Texture, public Asset<Cubemap> {
friend class Asset<Cubemap>;
private:
	const static std::string s_fallbackPath;

	GLsizei m_faceWidth;
	GLsizei m_faceHeight;

public:

	Cubemap(std::string_view name, std::string_view filePath);
	~Cubemap();

	//disallow copying a Texture, because it manages the lifetime of a resource
	//TODO: allow copying, but generate a new GPU-side ID for the new texture to manage
	//Texture2D(const Texture2D&) = delete;
	//Texture2D& operator=(const Texture2D&) = delete;
	//TODO: add back the copy constructor and assignment operator = delete statements

	void Upload() const override;
};

}