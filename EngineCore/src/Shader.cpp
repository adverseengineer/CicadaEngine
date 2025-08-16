
#include "FileSystem.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Cicada;

Shader::Shader(std::string_view shaderName, std::string_view vertSourcePath, std::string_view fragSourcePath) : Asset(shaderName) {
	
	static_assert(std::is_same<GLchar, char>::value, "GLchar must be char for this to work safely");

	std::string vertSource, fragSource;
	//TODO: make these return optionals
	FileSystem::ReadFileToString(vertSourcePath, vertSource);
	FileSystem::ReadFileToString(fragSourcePath, fragSource);
	
	GLuint vertProg = CompileShaderStage(GL_VERTEX_SHADER, vertSource);
	GLuint fragProg = CompileShaderStage(GL_FRAGMENT_SHADER, fragSource);
	m_shaderProg = Link(vertProg, fragProg);

	CacheSingleUniforms();
	CacheUniformBlockIndices();
}

Shader::~Shader() {
	glDeleteProgram(m_shaderProg);
}

//given the source code for a vertex or fragment shader, compile it and store it on the GPU
GLuint Shader::CompileShaderStage(GLenum type, const std::string& shaderSource) {

	GLuint shader = glCreateShader(type);

	//send the vertex shader source code to the graphics card and compile it
	const GLchar* temp = static_cast<const GLchar*>(shaderSource.c_str());
	glShaderSource(shader, 1, &temp, nullptr);
	glCompileShader(shader);

	//check if the shader successfully compiled
	GLint isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		
		GLsizei maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//copy openGL's log into an appropriately sized std::string filled with nulls
		std::string errorMsg(maxLength, '\0');
		glGetShaderInfoLog(shader, maxLength, &maxLength, static_cast<GLchar*>(errorMsg.data()));
		LOG_ERROR("{:s}", errorMsg);

		glDeleteShader(shader);
	}

	return shader; //will be zero if there's an issue
}

//given the source for a vertex shader and fragment shader, link them into a shader program
GLuint Shader::Link(GLuint vertProg, GLuint fragProg) {

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertProg);
	glAttachShader(shaderProg, fragProg);
	glLinkProgram(shaderProg);

	GLint isLinked;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		
		GLsizei maxLength = 0;
		glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorMsg(maxLength, '\0');
		glGetProgramInfoLog(shaderProg, maxLength, &maxLength, static_cast<GLchar*>(errorMsg.data()));
		LOG_ERROR("{:s}", errorMsg);

		glDeleteProgram(shaderProg);
	}

	//we don't need these anymore
	glDetachShader(shaderProg, vertProg);
	glDetachShader(shaderProg, fragProg);
	glDeleteShader(vertProg);
	glDeleteShader(fragProg);

	return shaderProg; //will be zero if there's an issue
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
		if (location == -1) continue; //exclude all with loc zero, they're part of a uniform block and are handled separately

		std::string name(nameBuf.get(), nameLen);
		m_uniformCache.emplace(name, UniformInfo{ type, location });
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
bool Shader::GetUniform(std::string_view uniformName, UniformInfo& uniform) const {
	bool found = m_uniformCache.contains(uniformName.data());
	if (found)
		uniform = m_uniformCache.at(uniformName.data());
	return found;
}

void Shader::Bind() const {
	glUseProgram(m_shaderProg);
};

void Shader::Unbind() {
	glUseProgram(0);
}

void Shader::AttachUBO(const std::string_view blockName, const UniformBufferObject& ubo, GLintptr offset) const {
	
	auto it = m_uniformBlockIndexCache.find(blockName.data());
	if (it == m_uniformBlockIndexCache.end()) {
		LOG_ERROR("Error attaching UBO, no such uniform block: {:?}", blockName);
		return;
	}
	
	//bind the uniform block to the binding point, and then bind the UBO to the same block
	//NOTE: if offset is non-zero, we bind less memory, so subtract it from size
	glUniformBlockBinding(m_shaderProg, it->second, ubo.m_bindingPoint);
	glBindBufferRange(GL_UNIFORM_BUFFER, ubo.m_bindingPoint, ubo.m_handle, offset, ubo.m_size - offset);
}

void Shader::SetInt(std::string_view uniformName, int value) const {
	UniformInfo temp;
	if (GetUniform(uniformName, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1i(temp.location, static_cast<GLint>(value));
	}
	else
		LOG_WARN("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetUint(std::string_view uniformName, unsigned int value) const {
	UniformInfo temp;
	if (GetUniform(uniformName, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1ui(temp.location, static_cast<GLuint>(value));
	}
	else
		LOG_WARN("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetFloat(std::string_view uniformName, float value) const {
	UniformInfo temp;
	if (GetUniform(uniformName, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1f(temp.location, static_cast<GLfloat>(value));
	}
	else
		LOG_WARN("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetVec3(std::string_view uniformName, const glm::vec3& value) const {
	UniformInfo temp;
	if (GetUniform(uniformName, temp)) {
		glUseProgram(m_shaderProg);
		glUniform3fv(temp.location, 1, glm::value_ptr(value));
	}
	else
		LOG_WARN("Shader {:?} has no such uniform: {:?}", m_name, uniformName);
}

void Shader::SetMat4(std::string_view uniformName, const glm::mat4& value) const {
	UniformInfo temp;
	if (GetUniform(uniformName, temp)) {
		glUseProgram(m_shaderProg);
		glUniformMatrix4fv(temp.location, 1, GL_FALSE, glm::value_ptr(value));
	}
	else
		LOG_WARN("Shader {:?} has no such uniform : {:?}", m_name, uniformName);
}
