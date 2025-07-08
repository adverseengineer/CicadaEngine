#pragma once

#include <glad/glad.h>
#include <vector>

class UniformBuffer {

private:
	const size_t m_size;
public: //KILL THIS
	unsigned int m_uboId;
private: //AND THIS
	std::vector<std::byte> m_data; //TODO: stop storing it here once debugging confirms this works

public:
	inline UniformBuffer(size_t size): m_size(size), m_data(size) {
		glGenBuffers(1, &m_uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	
	inline ~UniformBuffer() {
		glDeleteBuffers(1, &m_uboId);
	};

	//copies a portion of the data to main ram and GPU ram
	inline void Write(const void* data, size_t offset, size_t size) {
		//std::memcpy(m_data.data() + offset, (std::byte*) data, size);
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
