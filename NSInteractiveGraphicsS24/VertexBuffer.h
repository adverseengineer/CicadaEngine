#pragma once
#include "texture.h"
#include <glad/glad.h> 
#include <string>
#include <unordered_map>
#include <vector>

struct VertexAttribute {
	unsigned int m_index;
	unsigned int m_numComponents;
	int m_type;
	int m_isNormalized;
	unsigned int m_bytesToNext;
	void* m_byteOffset;
};

class VertexBuffer {
protected:
	unsigned int m_numElemsPerVert;
	unsigned int m_vboId;
	int m_primitiveType;
	std::vector<float> m_vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;

public:
	VertexBuffer(unsigned int numElementsPerVertex = 3);
	~VertexBuffer();

	inline std::size_t GetNumberOfVertices() const { return m_vertexData.size(); }

	inline int GetPrimitiveType() const { return m_primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->m_primitiveType = primitiveType; }

	void Bind() const;
	void Unbind() const;
	void AddVertexData(unsigned int count, ...);
	void StaticAllocate() const;
	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount = 0);
	void SetUpAttributeInterpretration() const;
};
