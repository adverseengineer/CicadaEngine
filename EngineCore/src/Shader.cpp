
#include "FileSystem.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Cicada;

std::unordered_map<std::string, std::weak_ptr<Shader>> Shader::s_instances;

Shader::Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath) : ManagedObject(shaderName) {
	
	std::string vertSource, fragSource;
	//TODO: make these return optionals
	FileSystem::ReadFileToString(vertSourcePath, vertSource);
	FileSystem::ReadFileToString(fragSourcePath, fragSource);
	
	unsigned int vertProg = CompileStage(GL_VERTEX_SHADER, vertSource);
	unsigned int fragProg = CompileStage(GL_FRAGMENT_SHADER, fragSource);
	m_shaderProg = Link(vertProg, fragProg);

	QueryUniforms();
}

Shader::~Shader() {
	glDeleteProgram(m_shaderProg);
	Logger::Log("Destroying shader");
}

//given the source code for a vertex or fragment shader, compile it and store it on the GPU
unsigned int Shader::CompileStage(unsigned int type, const std::string& shaderSource) {

	unsigned int shaderId = glCreateShader(type);

	//send the vertex shader source code to the graphics card and compile it
	const char* temp = shaderSource.c_str();
	glShaderSource(shaderId, 1, &temp, nullptr);
	glCompileShader(shaderId);

	//check if the shader successfully compiled
	int isCompiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_TRUE)
		return shaderId;

	//edge case: it didn't
	int maxLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	//copy GL's log into an appropriately sized std::string filled with null
	std::string errorMsg(maxLength, '\0');
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, errorMsg.data());

	//we don't need the shader anymore
	glDeleteShader(shaderId);

	throw std::runtime_error(errorMsg);
}

//given the source for a vertex shader and fragment shader, link them into a shader program
unsigned int Shader::Link(unsigned int vertProg, unsigned int fragProg) {

	unsigned int shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertProg);
	glAttachShader(shaderProg, fragProg);
	glLinkProgram(shaderProg);

	int isLinked;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_TRUE) {
		glDetachShader(shaderProg, vertProg);
		glDetachShader(shaderProg, fragProg);
		glDeleteShader(vertProg);
		glDeleteShader(fragProg);

		return shaderProg;
	}

	int maxLength = 0;
	glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &maxLength);

	std::string errorMsg(maxLength, '\0');
	glGetProgramInfoLog(shaderProg, maxLength, &maxLength, errorMsg.data());
	glDeleteProgram(shaderProg); //delete the bad program
	throw std::runtime_error(errorMsg);
}

//queries openGL to tell us all the uniforms for a compiled and linked shader program, and stores the info
void Shader::QueryUniforms() {

	constexpr size_t uniformNameMaxChars = 256;

	GLint numUniforms = 0;
	glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORMS, &numUniforms);

	char name[uniformNameMaxChars];
	for (GLint i = 0; i < numUniforms; i++) {
		GLint size;
		GLenum type;
		glGetActiveUniform(m_shaderProg, (GLint)i, sizeof(name), nullptr, &size, &type, name);
		GLint location = glGetUniformLocation(m_shaderProg, name);

		m_UniformInfoCache[name] = { type, location };
	}
}

//fetches info about a shader uniform into a UniformInfo struct and returns whether or not the uniform was found
std::optional<UniformInfo> Shader::GetUniform(std::string_view uniformName) const {
	if (m_UniformInfoCache.contains(uniformName.data()))
		return m_UniformInfoCache.at(uniformName.data());
	else
		return std::nullopt;
}

void Shader::Bind() const {
	glUseProgram(m_shaderProg);
};

void Shader::Unbind() const {
	glUseProgram(0);
}

//void Shader::AttachUniformBlock(const std::string& blockName, unsigned int bindingPoint) const {
//	GLuint blockIndex = glGetUniformBlockIndex(m_shaderProg, blockName.c_str());
//	glUniformBlockBinding(m_shaderProg, blockIndex, bindingPoint);
//}

void Shader::SetInt(std::string_view uniformName, int value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1i(temp.value().location, value);
	}
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetUInt(std::string_view uniformName, unsigned int value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1ui(temp.value().location, value);
	}
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetFloat(std::string_view uniformName, float value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1f(temp.value().location, value);
	}
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform3fv(temp.value().location, 1, glm::value_ptr(value));
	}
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniformMatrix4fv(temp.value().location, 1, GL_FALSE, glm::value_ptr(value));
	}
	else
		Logger::Writef(LogEntry::Level::Warning, "Warning: shader {:?} has no such uniform: {:?}", m_name, uniformName);
}
