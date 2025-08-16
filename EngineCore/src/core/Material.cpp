#include "core/Material.h"

using namespace Cicada;

Material::Material(std::string_view materialName, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture) :
	Asset(materialName), m_shader(shader), m_texture(texture) {
	assert(shader != nullptr);
}

void Material::Bind() const {
	
	if (m_shader == nullptr) return;	
	m_shader->Bind();

	if (m_texture != nullptr) {
		m_texture->AttachTexUnit(0);
	}
}

void Material::SetInt(std::string_view uniformName, int value) const {
	if (m_shader == nullptr) return;
	m_shader->SetInt(uniformName, value);
}

void Material::SetUint(std::string_view uniformName, unsigned int value) const {
	if (m_shader == nullptr) return;
	m_shader->SetUint(uniformName, value);
}

void Material::SetFloat(std::string_view uniformName, float value) const {
	if (m_shader == nullptr) return;
	m_shader->SetFloat(uniformName, value);
}

void Material::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	if (m_shader == nullptr) return;
	m_shader->SetVec3(uniformName, value);
}

void Material::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	if (m_shader == nullptr) return;
	m_shader->SetMat4(uniformName, value);
}
