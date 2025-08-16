#pragma once

#include <Asset.h>
#include "UniformBufferObject.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Cicada {

class Shader : public Asset<Shader> {
friend class Asset<Shader>;
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
	[[nodiscard]]
	static GLuint CompileShaderStage(GLenum type, const std::string& shaderSource);
	[[nodiscard]]
	static GLuint Link(GLuint vertProg, GLuint fragProg);

	void CacheSingleUniforms();
	void CacheUniformBlockIndices();

	[[nodiscard]]
	bool GetUniform(std::string_view name, UniformInfo& uniform) const;

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
