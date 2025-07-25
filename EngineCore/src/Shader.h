#pragma once

#include "ManagedObject.h"
#include "UniformBufferObject.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>


namespace Cicada {

class Shader : public ManagedObject<Shader> {
friend class ManagedObject<Shader>;
friend class Material;
private:

	struct UniformInfo {
		GLenum type;
		GLint location;
	};

	std::unordered_map<std::string, UniformInfo> m_uniformCache;
	std::unordered_map<std::string, GLuint> m_uniformBlockIndexCache;

	GLuint m_shaderProg = 0;

	Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath);
public:
	~Shader();

	//prohibit copying and assigning shaders, since they manage a GPU resource
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	//TODO: consider whether i need a move constructor

private:
	static unsigned int CompileStage(GLenum type, const std::string& shaderSource);
	static unsigned int Link(GLuint vertProg, GLuint fragProg);

	void CacheSingleUniforms();
	void CacheUniformBlockIndices();
	std::optional<UniformInfo> GetUniform(std::string_view name) const;

public:
	void Bind() const;
	static void Unbind();
	inline GLuint GetShaderProg() const { return m_shaderProg; }

	void AttachUBO(std::string_view blockName, const UniformBufferObject& ubo, GLintptr offset = 0) const;

	void SetInt(std::string_view uniformName, int value) const;
	void SetUint(std::string_view uniformName, unsigned int value) const;
	void SetFloat(std::string_view uniformName, float value) const;
	void SetVec3(std::string_view uniformName, const glm::vec3& value) const;
	void SetMat4(std::string_view uniformName, const glm::mat4& value) const;
};

}
