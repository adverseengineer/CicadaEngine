#pragma once

#include "Texture.h"
#include "Util.h"

namespace Cicada {

class Texture2D : public Texture, public ManagedObject<Texture2D> {
friend class ManagedObject<Texture>;
private:
	const static std::string s_fallbackPath;

	int m_width;
	int m_height;
	int m_numChannels; //TODO: properly use this field with stb_load to detect source format
	//this will require more research into stb. do this tomorrow. EDIT: this was in like december lmao
	
public:

	Texture2D(std::string_view name, std::string_view filePath);
	~Texture2D();

	//disallow copying a Texture, because it manages the lifetime of a resource
	//TODO: allow copying, but generate a new GPU-side ID for the new texture to manage
	//Texture2D(const Texture2D&) = delete;
	//Texture2D& operator=(const Texture2D&) = delete;
	//TODO: add back the copy constructor and assignment operator = delete statements
	
	void Bind();
	void AttachTexUnit(GLenum texUnit) const;

	void Upload() const override;

	void SetInternalFormat(Format format);
	void SetSourceFormat(Format format);

	void SetMinFilter(FilterMode mode);
	void SetMagFilter(FilterMode mode);

	void SetWrapU(WrapMode mode);
	void SetWrapV(WrapMode mode);

};

}