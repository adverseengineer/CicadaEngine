#include "VertexBuffer.h"
#include <cstdarg>

//creates a new VertexBuffer with the specified number of elements per vertex
VertexBuffer::VertexBuffer(unsigned int numElemsPerVert) {
	m_numElemsPerVert = numElemsPerVert;
	m_primitiveType = GL_TRIANGLES;
	glGenBuffers(1, &m_vboId);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_vboId);
}

void VertexBuffer::Select() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
}

void VertexBuffer::Deselect() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::AddVertexData(unsigned int count, ...) {
	if (count != m_numElemsPerVert)
		throw "Invalid vertex data count!";

	va_list args;
	va_start(args, count);
	while (count > 0) {
		//the default is double, so accept as double and then cast to float
		m_vertexData.push_back(static_cast<float>(va_arg(args, double)));
		count--;
	}
	va_end(args);
}

void VertexBuffer::StaticAllocate() const {
	GLsizeiptr bytesToAllocate = m_vertexData.size() * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, bytesToAllocate, (void*)m_vertexData.data(), GL_STATIC_DRAW);
}

void VertexBuffer::AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount) {
	unsigned int vertexSizeInBytes = sizeof(float) * m_numElemsPerVert;
	unsigned int bytesToNext = vertexSizeInBytes;
	unsigned long long offsetBytes = sizeof(float) * offsetCount;
	VertexAttribute attr = {
		index,
		numberOfElements,
		GL_FLOAT,
		GL_FALSE,
		bytesToNext,
		(void*)offsetBytes
	};
	attributeMap[name] = attr;
}

void VertexBuffer::SetUpAttributeInterpretration() const {
	for (const auto& item : attributeMap) {
		const auto& attr = item.second;
		glEnableVertexAttribArray(attr.m_index);
		glVertexAttribPointer(
			attr.m_index,
			attr.m_numComponents,
			attr.m_type,
			attr.m_isNormalized,
			attr.m_bytesToNext,
			attr.m_byteOffset
		);
	}
}
