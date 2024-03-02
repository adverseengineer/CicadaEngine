#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) :
	vertexSource(vertexSource),
	fragmentSource(fragmentSource) {
	Init();
	CreateShaderProgram();
}

void Shader::AddUniform(const std::string& uniformName) {
	if(uniformMap.contains(uniformName)) return;
	int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
	uniformMap.emplace(uniformName, uniformLocation);
}

void Shader::SendMat4Uniform(const std::string& uniformName, const glm::mat4& mat) {
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(uniformMap[uniformName], 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SendIntUniform(const std::string& uniformName, int value) {
	glUseProgram(shaderProgram);
	glUniform1i(uniformMap[uniformName], value);
}

void Shader::Init(void) {
	shaderProgram = 0;
}

unsigned int Shader::CompileShaderSource(int type, const std::string& shaderSource) {

	unsigned shaderId = glCreateShader(type);

	//send the vertex shader source code to GL
	//note that std::string's .c_str is NULL character terminated.
	const char* temp = (const char*) shaderSource.c_str();
	glShaderSource(shaderId, 1, &temp, 0);

	glCompileShader(shaderId);

	int isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		int maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(shaderId);

		Util::Log(infoLog);
		return -1;
	}

	Util::Log("Compiled shader type " + std::to_string(type));
	return shaderId;
}

void Shader::CreateShaderProgram(void) {
	unsigned int vertexShader = CompileShaderSource(GL_VERTEX_SHADER, vertexSource);
	if (vertexShader == -1) return;

	unsigned int fragmentShader = CompileShaderSource(GL_FRAGMENT_SHADER, fragmentSource);
	if (fragmentShader == -1) return;

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*) &isLinked);
	if(isLinked == GL_FALSE) {

		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
		Util::Log(infoLog);

		glDeleteProgram(shaderProgram); //we don't need the program anymore
		glDeleteShader(vertexShader); //delete them to avoid memory leaks
		glDeleteShader(fragmentShader);

		return;
	}

	//always detach shaders after a successful link.
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	Util::Log("Linked shader program");
	return;
}
