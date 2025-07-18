#pragma once

#include <glad/glad.h>
#include <vector>

#include "BufferObject.h"

namespace Cicada {

class UniformBufferObject : public BufferObject {
friend class Shader;

private:
	GLuint m_bindingPoint;
	//static std::bitset<GLuint, bool>
	//TODO: implement automatic binding point selection based on what's available
	//for now, passing bindingpoint as a ctor param works

public:
	UniformBufferObject(GLuint bindingPoint) : BufferObject(GL_UNIFORM_BUFFER), m_bindingPoint(bindingPoint) {}
	UniformBufferObject(GLsizeiptr size, GLuint bindingPoint) : BufferObject(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, size), m_bindingPoint(bindingPoint) {}
};

}