#pragma once

#include "Logger.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <functional> //TODO: remove along with DBG functions when the time comes

namespace Cicada {

struct UniformInfo {
	unsigned int type;
	int location;
};

class Shader {
friend class Material;
private:

	static std::unordered_map<std::string, std::weak_ptr<Shader>> s_instances;

	std::string m_name;
	std::unordered_map<std::string, UniformInfo> m_UniformInfoCache;

	std::unordered_map<std::string, UniformInfo> m_objectUniforms;
	std::unordered_map<std::string, UniformInfo> m_materialUniforms;
	unsigned int m_shaderProg = 0;

	Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath);
public:
	~Shader();

	//prohibit copying and assigning shaders, since they manage a GPU resource
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	//TODO: consider whether i need a move constructor

	static std::shared_ptr<Shader> Create(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath);
	static std::shared_ptr<Shader> Get(std::string_view id);
	static void Cleanup();

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
	void SetUint(std::string_view uniformName, unsigned int value) const;
	void SetFloat(std::string_view uniformName, float value) const;
	void SetVec3(std::string_view uniformName, const glm::vec3& value) const;
	void SetMat4(std::string_view uniformName, const glm::mat4& value) const;
	
	inline void DBG_ShowInfo() const {
		for (const auto& [name, info] : m_UniformInfoCache) {
			Logger::Writef(
				LogEntry::Level::Info,
				"Uniform: {:?} (type = {:s}, location = {:d})",
				name,
				Logger::GLTypeToStr(info.type),
				info.location
			);
		}
	}

	inline static void DBG_ForEach(std::function<void(std::shared_ptr<Shader>)> callback) {
		for (auto& [_, shaderPtr] : s_instances) {
			auto temp = shaderPtr.lock();
			callback(temp);
		}
	}
};

}
