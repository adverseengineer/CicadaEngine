
#include "FileSystem.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Cicada;

Shader::Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath) : ManagedObject(shaderName) {
	
	static_assert(std::is_same<GLchar, char>::value, "GLchar must be char for this to work safely");

	std::string vertSource, fragSource;
	//TODO: make these return optionals
	FileSystem::ReadFileToString(vertSourcePath, vertSource);
	FileSystem::ReadFileToString(fragSourcePath, fragSource);
	
	unsigned int vertProg = CompileStage(GL_VERTEX_SHADER, vertSource);
	unsigned int fragProg = CompileStage(GL_FRAGMENT_SHADER, fragSource);
	m_shaderProg = Link(vertProg, fragProg);

	CacheSingleUniforms();
	CacheUniformBlockIndices();
}

Shader::~Shader() {
	glDeleteProgram(m_shaderProg);
}

//given the source code for a vertex or fragment shader, compile it and store it on the GPU
GLuint Shader::CompileStage(GLenum type, const std::string& shaderSource) {

	GLuint shaderId = glCreateShader(type);

	//send the vertex shader source code to the graphics card and compile it
	const GLchar* temp = static_cast<const GLchar*>(shaderSource.c_str());
	glShaderSource(shaderId, 1, &temp, nullptr);
	glCompileShader(shaderId);

	//check if the shader successfully compiled
	GLint isCompiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_TRUE)
		return shaderId;

	//edge case: it didn't
	GLsizei maxLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	//copy GL's log into an appropriately sized std::string filled with null
	std::string errorMsg(maxLength, '\0');
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, static_cast<GLchar*>(errorMsg.data()));

	//we don't need the shader anymore
	glDeleteShader(shaderId);

	Log::Error("{:s}", errorMsg);
}

//given the source for a vertex shader and fragment shader, link them into a shader program
GLuint Shader::Link(GLuint vertProg, GLuint fragProg) {

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertProg);
	glAttachShader(shaderProg, fragProg);
	glLinkProgram(shaderProg);

	GLint isLinked;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_TRUE) {
		glDetachShader(shaderProg, vertProg);
		glDetachShader(shaderProg, fragProg);
		glDeleteShader(vertProg);
		glDeleteShader(fragProg);

		return shaderProg;
	}

	GLsizei maxLength = 0;
	glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &maxLength);

	std::string errorMsg(maxLength, '\0');
	glGetProgramInfoLog(shaderProg, maxLength, &maxLength, static_cast<GLchar*>(errorMsg.data()));
	glDeleteProgram(shaderProg); //delete the bad program
	
	Log::Error("{:s}", errorMsg);
}

//queries openGL to tell us all the uniforms for a compiled and linked shader program, and stores the info
void Shader::CacheSingleUniforms() {

	//fetch the number of uniforms and the length of the longest uniform name
	GLint numUniforms, uniformNameMaxChars;
	glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORMS, &numUniforms);
	glGetProgramInterfaceiv(m_shaderProg, GL_UNIFORM, GL_MAX_NAME_LENGTH, &uniformNameMaxChars);
		
	//TODO: i'm tired of fucking with this, so i'm leaving it like this for now.
	//i wanted to make it use a std::string, but that doesn't seem to want to work.
	std::unique_ptr<GLchar> nameBuf = std::unique_ptr<GLchar>(new GLchar[uniformNameMaxChars]);
	for (GLint i = 0; i < numUniforms; i++) {
		GLint size;
		GLenum type;
		GLsizei nameLen;
		std::memset(nameBuf.get(), '\0', uniformNameMaxChars);
		glGetActiveUniform(m_shaderProg, i, uniformNameMaxChars, &nameLen, &size, &type, nameBuf.get());
		GLint location = glGetUniformLocation(m_shaderProg, nameBuf.get());

		if (location != -1) { //exclude all with location zero, they are part of a uniform block and get handled separately
			std::string name(nameBuf.get(), nameLen);
			m_singleUniformCache.emplace(name, UniformInfo{ type, location });
		}
	}
}

void Shader::CacheUniformBlockIndices() {
	
	//fetch the number of uniform blocks and the length of the longest uniform block name
	GLint numUniformBlocks, uniformBlockNameMaxChars;
	glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
	glGetProgramInterfaceiv(m_shaderProg, GL_UNIFORM_BLOCK, GL_MAX_NAME_LENGTH, &uniformBlockNameMaxChars);

	std::unique_ptr<GLchar> nameBuf = std::unique_ptr<GLchar>(new GLchar[uniformBlockNameMaxChars]);
	for (GLint i = 0; i < numUniformBlocks; i++) {
		
		//fetch the name and name length of the uniform block at this index
		GLsizei nameLen = 0;
		glGetActiveUniformBlockName(m_shaderProg, i, uniformBlockNameMaxChars, &nameLen, nameBuf.get());

		//get the index of the uniform block by that name and cache both
		GLuint index = glGetUniformBlockIndex(m_shaderProg, nameBuf.get());
		std::string name(nameBuf.get(), nameLen);
		m_uniformBlockIndexCache.emplace(name, index);
	}
}

//fetches info about a shader uniform into a UniformInfo struct and returns whether or not the uniform was found
std::optional<Shader::UniformInfo> Shader::GetUniform(std::string_view uniformName) const {
	if (m_singleUniformCache.contains(uniformName.data()))
		return m_singleUniformCache.at(uniformName.data());
	else
		return std::nullopt;
}

void Shader::Bind() const {
	glUseProgram(m_shaderProg);
};

void Shader::Unbind() {
	glUseProgram(0);
}

void Shader::AttachUniformBlock(const std::string_view blockName, unsigned int bindingPoint) const {
	
	auto it = m_uniformBlockIndexCache.find(blockName.data());
	if (it == m_uniformBlockIndexCache.end()) {
		Log::Error("No such uniform block: {:?}", blockName);
		return;
	}
	else
		glUniformBlockBinding(m_shaderProg, it->second, bindingPoint);
}

void Shader::SetInt(std::string_view uniformName, int value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1i(temp.value().location, static_cast<GLint>(value));
	}
	else
		Log::Warn("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetUInt(std::string_view uniformName, unsigned int value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1ui(temp.value().location, static_cast<GLuint>(value));
	}
	else
		Log::Warn("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetFloat(std::string_view uniformName, float value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform1f(temp.value().location, static_cast<GLfloat>(value));
	}
	else
		Log::Warn("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniform3fv(temp.value().location, 1, glm::value_ptr(value));
	}
	else
		Log::Warn("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	std::optional<UniformInfo> temp = GetUniform(uniformName);
	if (temp.has_value()) {
		glUseProgram(m_shaderProg);
		glUniformMatrix4fv(temp.value().location, 1, GL_FALSE, glm::value_ptr(value));
	}
	else
		Log::Warn("Shader {:?} has no such uniform : {:?}", m_name, uniformName);
}
