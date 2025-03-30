#pragma once

#include "ManagedObject.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace Cicada {

class Shader : public ManagedObject<Shader> {
friend class ManagedObject<Shader>;
private:

	struct UniformInfo {
		unsigned int type;
		int location;
	};

	static std::unordered_map<std::string, std::weak_ptr<Shader>> s_instances;

	std::unordered_map<std::string, UniformInfo> m_UniformInfoCache;

	unsigned int m_shaderProg = 0;

	Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath);
public:
	~Shader();

	//prohibit copying and assigning shaders, since they manage a GPU resource
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	//TODO: consider whether i need a move constructor

private:
	static unsigned int CompileStage(unsigned int type, const std::string& shaderSource);
	static unsigned int Link(unsigned int vertProg, unsigned int fragProg);

	void QueryUniforms();
	std::optional<UniformInfo> GetUniform(std::string_view name) const;

public:
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetShaderProg() const { return m_shaderProg; }

	//void AttachUniformBlock(std::string_view blockName, unsigned int bindingPoint) const;

	void SetInt(std::string_view uniformName, int value) const;
	void SetUInt(std::string_view uniformName, unsigned int value) const;
	void SetFloat(std::string_view uniformName, float value) const;
	void SetVec3(std::string_view uniformName, const glm::vec3& value) const;
	void SetMat4(std::string_view uniformName, const glm::mat4& value) const;
	
	inline void DBG_ShowInfo() const {
		for (const auto& [name, info] : m_UniformInfoCache) {
			Log::Info(
				"Uniform: {:?} (type = {:d}, location = {:d})",
				name,
				info.type,
				info.location
			);
		}
	}
};

}
