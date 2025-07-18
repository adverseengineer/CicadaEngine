#pragma once

#include <glad/glad.h>

namespace Cicada {

class BufferObject {

protected:
	GLuint m_handle;
	GLenum m_type;
	GLsizeiptr m_size = 0;

public:
	//creates a buffer, but no data store for it
	BufferObject(GLenum type) : m_type(type) {
		glGenBuffers(1, &m_handle);
	}

	//creates a buffer, and initializes it with data if data isn't nullptr
	BufferObject(GLenum type, GLenum usage, GLsizeiptr size, void* data = nullptr) : m_type(type), m_size(size) {
		glGenBuffers(1, &m_handle);
		glBindBuffer(m_type, m_handle);
		glBufferData(m_type, m_size, data, usage);
	}

	~BufferObject() {
		glDeleteBuffers(1, &m_handle);
	}

	void Bind() const {
		glBindBuffer(m_type, m_handle);
	}

	void Unbind() const {
		glBindBuffer(m_type, 0);
	}

	//sets up the data store for this buffer. if data isn't nullptr, also copies in initial data	
	void Setup(GLenum usage, GLsizeiptr size, void* data = nullptr) {
		m_size = size;
		glBufferData(m_type, m_size, data, usage);
	}

	//binds the buffer writes at the specified offset a number of bytes from data
	void Write(GLsizeiptr offset, GLsizeiptr size, void* data) const {
		glBindBuffer(m_type, m_handle);
		glBufferSubData(m_type, offset, size, data);
	}
};

}
