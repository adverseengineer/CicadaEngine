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
}

void Material::SetInt(std::string_view uniformName, int value) const {
	if (m_shader != nullptr)
		m_shader->SetInt(uniformName, value);
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: material {:?} has no associated shader", m_name);
}

void Material::SetUInt(std::string_view uniformName, unsigned int value) const {
	if (m_shader != nullptr)
		m_shader->SetUInt(uniformName, value);
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: material {:?} has no associated shader", m_name);
}

void Material::SetFloat(std::string_view uniformName, float value) const {
	if (m_shader != nullptr)
		m_shader->SetFloat(uniformName, value);
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: material {:?} has no associated shader", m_name);
}

void Material::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	if (m_shader != nullptr)
		m_shader->SetVec3(uniformName, value);
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: material {:?} has no associated shader", m_name);
}

void Material::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	if (m_shader != nullptr)
		m_shader->SetMat4(uniformName, value);
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: material {:?} has no associated shader", m_name);
}
