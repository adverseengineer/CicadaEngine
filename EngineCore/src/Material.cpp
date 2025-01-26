#include "Material.h"

void Material::GetUniforms() {

	if (!m_shader)
		return;

	for (const auto& uniformInfo : m_shader->m_UniformInfoCache) {
		m_params.emplace(uniformInfo.first, MaterialParam(uniformInfo.second, 0.0f));
	}
}

void Material::SendParams() const {
	
	for (const auto& [name, param] : m_params)
		m_shader->SendUniform(name, param.m_value);
}

//TODO: maybe need a SetParam method?

void Material::DBG_ShowInfo() const {
	for (const auto& [name, data] : m_params) {
		Log::Writef(
			LogEntry::Severity::Info,
			"{:s}: (type = {:s}, location = {:d})",
			name,
			Log::GLTypeToStr(data.m_info.type),
			data.m_info.location
		);
	}
}
