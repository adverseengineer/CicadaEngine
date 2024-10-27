#pragma once
#include "Camera.h"
#include "Util.h"

#include <glm/glm.hpp>
#include <unordered_map>

struct UniformInfo {
	unsigned int type;
	int location;
};

class Shader {
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	inline unsigned int GetShaderProg() const {
		return m_shaderProg;
	}

	void Bind() const;
	void Unbind() const;

	void AttachUniformBlock(const std::string& blockName, unsigned int bindingPoint) const;

	void SendUniform(const std::string& name, int value) const;
	void SendUniform(const std::string& name, unsigned int value) const;
	void SendUniform(const std::string& name, float value) const;
	void SendUniform(const std::string& name, const glm::vec3& value) const;
	void SendUniform(const std::string& name, const glm::mat4& value) const;

	void DBG_ShowInfo() const;

private:
	std::unordered_map<std::string, UniformInfo> m_UniformInfoCache;
	unsigned int m_shaderProg = 0;

	static unsigned int CompileShader(unsigned int type, const std::string& shaderSource);
	void Link(const std::string& vertexSource, const std::string& fragmentSource);

	bool GetUniform(const std::string& name, UniformInfo& info) const;
	void Reflect();
};
