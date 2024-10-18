
#include "Shader.h"

#include <glad/glad.h>
#include <glm/ext.hpp>
#include <gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
	Link(vertexSource, fragmentSource);
	Reflect();
}

Shader::~Shader() {
	glDeleteProgram(m_shaderProg);
}

void Shader::Bind() const {
	glUseProgram(m_shaderProg);
};

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::AttachUniformBlock(const std::string& blockName, unsigned int bindingPoint) const {
	GLuint blockIndex = glGetUniformBlockIndex(m_shaderProg, blockName.c_str());
	glUniformBlockBinding(m_shaderProg, blockIndex, bindingPoint);
}

//send a single signed integer
void Shader::SendUniform(const std::string& name, int value) const {
	UniformInfo temp;
	if (GetUniform(name, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1i(temp.location, value);
	}
	else
		Util::Log("Warning: shader " + std::to_string(m_shaderProg) + " has no such uniform \"" + name + "\"");
}

//send a single unsigned integer
void Shader::SendUniform(const std::string& name, unsigned int value) const {
	UniformInfo temp;
	if (GetUniform(name, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1ui(temp.location, value);
	}
	else
		Util::Log("Warning: shader " + std::to_string(m_shaderProg) + " has no such uniform \"" + name + "\"");
}

//send a single float
void Shader::SendUniform(const std::string& name, float value) const {
	UniformInfo temp;
	if (GetUniform(name, temp)) {
		glUseProgram(m_shaderProg);
		glUniform1f(temp.location, value);
	}
	else
		Util::Log("Warning: shader " + std::to_string(m_shaderProg) + " has no such uniform \"" + name + "\"");
}

//send a glm vector of dimension three
void Shader::SendUniform(const std::string& name, const glm::vec3& value) const {
	UniformInfo temp;
	if (GetUniform(name, temp)) {
		glUseProgram(m_shaderProg);
		glUniform3fv(temp.location, 1, glm::value_ptr(value));
	}
	else {
		Util::Log("Warning: shader " + std::to_string(m_shaderProg) + " has no such uniform \"" + name + "\"");
		for (const auto& uniform : m_UniformInfoCache) {
			Util::Log(uniform.first);
		}
	}
}

//send a 4x4 glm matrix
void Shader::SendUniform(const std::string& name, const glm::mat4& value) const {
	UniformInfo temp;
	if (GetUniform(name, temp)) {
		glUseProgram(m_shaderProg);
		glUniformMatrix4fv(temp.location, 1, GL_FALSE, glm::value_ptr(value));
	}
	else
		Util::Log("Warning: shader " + std::to_string(m_shaderProg) + " has no such uniform \"" + name + "\"");
}

//given the source code for a vertex or fragment shader, compile it and store it on the GPU
unsigned int Shader::CompileShader(unsigned int type, const std::string& shaderSource) {

	GLuint shaderId = glCreateShader((GLenum)type);

	//send the vertex shader source code to GL
	//note that std::string's .c_str is NULL character terminated.
	const GLchar* temp = (const GLchar*)shaderSource.c_str();
	glShaderSource(shaderId, 1, &temp, (const GLint*) nullptr);
	glCompileShader(shaderId);

	//check if the shader successfully compiled
	GLint isCompiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {

		//get the max length of a log entry
		GLint maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//copy GL's log into a char vector
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, (GLsizei)maxLength, (GLsizei*)&maxLength, (GLchar*)&infoLog[0]);
		Util::Log(infoLog);

		//we don't need the shader anymore.
		glDeleteShader(shaderId);

		return -1;
	}

	if (type == GL_VERTEX_SHADER)
		Util::Log("Compiled vertex shader");
	else if (type == GL_FRAGMENT_SHADER)
		Util::Log("Compiled fragment shader");

	return shaderId;
}

//given the source for a vertex shader and fragment shader, link them into a shader program
void Shader::Link(const std::string& vertexSource, const std::string& fragmentSource) {

	GLuint vertShaderId;
	if ((vertShaderId = (GLuint)CompileShader(GL_VERTEX_SHADER, vertexSource)) == 0) return;

	GLuint fragShaderId;
	if ((fragShaderId = (GLuint)CompileShader(GL_FRAGMENT_SHADER, fragmentSource)) == 0) return;

	GLuint tempProg = glCreateProgram();
	glAttachShader(tempProg, vertShaderId);
	glAttachShader(tempProg, fragShaderId);
	glLinkProgram(tempProg);

	GLint isLinked;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {

		GLint maxLength = 0;
		glGetProgramiv(m_shaderProg, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(m_shaderProg, maxLength, &maxLength, (GLchar*)&infoLog[0]);
		Util::Log(infoLog);

		glDeleteProgram(m_shaderProg); //delete the bad program
	}

	glDetachShader(m_shaderProg, vertShaderId);
	glDetachShader(m_shaderProg, fragShaderId);
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

	m_shaderProg = tempProg;
	Util::Log("Linked shader program");
}

//fetches info about a shader uniform into a UniformInfo struct and returns whether or not the uniform was found
bool Shader::GetUniform(const std::string& name, UniformInfo& info) const {
	bool uniformIsPresent = m_UniformInfoCache.count(name) > 0;
	if (uniformIsPresent)
		info = m_UniformInfoCache.at(name);
	return uniformIsPresent;
}

//queries openGL to tell us all the uniforms for a compiled and linked shader program, and stores the info
void Shader::Reflect() {
	GLint numUniforms = 0;
	glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORMS, &numUniforms);

	for (GLint i = 0; i < numUniforms; i++) {
		GLint size;
		GLenum type;
		char name[256];
		glGetActiveUniform(m_shaderProg, (GLint)i, sizeof(name), nullptr, &size, &type, name);
		GLint location = glGetUniformLocation(m_shaderProg, name);

		m_UniformInfoCache[name] = { type, location };
	}
}

void Shader::DBG_ShowInfo() const {

	for (const auto& [name, info] : m_UniformInfoCache) {
		Util::Log(
			name +
			": (type = " + std::to_string(info.type) +
			", location = " + std::to_string(info.location) + ")"
		);
	}
}