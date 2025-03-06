#include "core/Material.h"

using namespace Cicada;

Material::Material(std::string_view materialName, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture) :
	ManagedObject(materialName), m_shader(shader), m_texture(texture) {
}

void Material::Bind() const {
	assert(m_shader != nullptr);
	assert(m_texture != nullptr);

	m_shader->Bind();
	m_texture->Bind();
	m_texture->SelectForRendering(); //why do i need to do this? what is the difference from just calling Bind()?
}

void Material::SetInt(std::string_view uniformName, int value) const {
	assert(m_shader != nullptr);
	m_shader->SetInt(uniformName, value);
}

void Material::SetUInt(std::string_view uniformName, unsigned int value) const {
	assert(m_shader != nullptr);
	m_shader->SetUInt(uniformName, value);
}

void Material::SetFloat(std::string_view uniformName, float value) const {
	assert(m_shader != nullptr);
	m_shader->SetFloat(uniformName, value);
}

void Material::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	assert(m_shader != nullptr);
	m_shader->SetVec3(uniformName, value);
}

void Material::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	assert(m_shader != nullptr);
	m_shader->SetMat4(uniformName, value);
}
