#pragma once

#include <glad/glad.h>
#include <vector>

class ShaderDataBuffer {

private:
	const size_t m_size;
	unsigned int m_uboId;
	std::vector<std::byte> m_data; //TODO: stop storing it here once debugging confirms this works

public:
	inline ShaderDataBuffer(size_t size): m_size(size), m_data(size) {
		glGenBuffers(1, &m_uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW); //TODO: add support for usage hint later
	}
	
	inline ~ShaderDataBuffer() {
		glDeleteBuffers(1, &m_uboId);
	};

	//copies the entire data to main ram and GPU ram
	inline void Fill(const void* data, size_t length) { //TODO: give this a more appropriate name
		std::memcpy(m_data.data(), data, length);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
		glBufferData(GL_UNIFORM_BUFFER, length, data, GL_STATIC_DRAW);
	}

	//copies a portion of the data to main ram and GPU ram
	inline void Write(const void* data, size_t offset, size_t size) {
		std::memcpy(m_data.data() + offset, (std::byte*) data, size);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	inline void Bind(unsigned int bindingPoint) const {
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_uboId);
	}

	inline void BindRange(unsigned int bindingPoint, size_t offset, size_t size) const {
		glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_uboId, offset, size);
	}

	inline static void Unbind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};
