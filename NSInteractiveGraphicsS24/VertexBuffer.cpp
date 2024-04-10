#include "VertexBuffer.h"
#include <cstdarg>

//creates a new VertexBuffer with the specified number of elements per vertex
VertexBuffer::VertexBuffer(unsigned int numElementsPerVertex) {
	numberOfElementsPerVertex = numElementsPerVertex;
	primitiveType = GL_TRIANGLES;
	textureUnit = 0;
	texture = nullptr;
	glGenBuffers(1, &vboId);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &vboId);
}

void VertexBuffer::Select() const {
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

void VertexBuffer::Deselect() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SelectTexture(void) const {
	if (texture != nullptr)
		texture->SelectToRender(textureUnit);
}

void VertexBuffer::AddVertexData(unsigned int count, ...) {
	if (count != numberOfElementsPerVertex)
		throw "Invalid vertex data count!";

	va_list args;
	va_start(args, count);
	while(count > 0) {
		//the default is double, so accept as double and then cast to float
		vertexData.push_back(static_cast<float>(va_arg(args, double)));
		count--;
	}
	va_end(args);
}

void VertexBuffer::StaticAllocate() const {
	unsigned long long bytesToAllocate = vertexData.size() * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, bytesToAllocate, vertexData.data(), GL_STATIC_DRAW);
	if(texture != nullptr)
		texture->Allocate();
}

void VertexBuffer::AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount) {
	unsigned int vertexSizeInBytes = sizeof(float) * numberOfElementsPerVertex;
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
	for (const std::pair<const std::string, VertexAttribute>& item : attributeMap) {
		const auto& attr = item.second;
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(
			attr.index,
			attr.numberOfComponents,
			attr.type,
			attr.isNormalized,
			attr.bytesToNext,
			attr.byteOffset
		);
	}
}
