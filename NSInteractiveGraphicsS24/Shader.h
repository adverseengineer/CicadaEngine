#pragma once
#include "Util.h"
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader {
protected:
	std::string vertexSource;
	std::string fragmentSource;
	unsigned int shaderProgram;
	std::unordered_map<std::string, unsigned int> uniformMap;

public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader() = default;

	inline const std::string& GetVertexSource() const { return vertexSource; }
	inline const std::string& GetFragmentSource() const { return fragmentSource; }
	inline unsigned int GetShaderProgram() const { return shaderProgram; }
	inline const bool IsCreated() const { return shaderProgram != 0; }
	
	void AddUniform(const std::string& uniformName);
	void SendMat4Uniform(const std::string& uniformName, const glm::mat4& mat);
	void SendIntUniform(const std::string& uniformName, int value);

private:
	void Init();
	unsigned int CompileShaderSource(int type, const std::string& shaderSource);
	void CreateShaderProgram();
};
