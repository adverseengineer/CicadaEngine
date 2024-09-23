#include "IndexBuffer.h"
#include <cstdarg>

//creates a new IndexBuffer with the specified number of elements per vertex
IndexBuffer::IndexBuffer() {
	glGenBuffers(1, &m_iboId);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_iboId);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::StaticAllocate() const {
	unsigned long long bytesToAllocate = m_indexData.size() * sizeof(unsigned short);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesToAllocate, m_indexData.data(), GL_STATIC_DRAW);
}

void IndexBuffer::AddIndexData(unsigned int count, ...) {

	va_list args;
	va_start(args, count);
	while (count-- > 0) //the default is double, so accept as double and then cast to float
		m_indexData.push_back(static_cast<unsigned short>(va_arg(args, int)));

	va_end(args);
}
