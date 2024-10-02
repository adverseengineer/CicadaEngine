#pragma once

#include "texture.h"
#include <cstdarg>
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

class Mesh {
private:
	unsigned int m_vaoId = 0;
	unsigned int m_vboId = 0;
	unsigned int m_iboId = 0;

	unsigned int m_numElemsPerVert;
	int m_primitiveType;
	std::vector<float> m_vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;

	std::vector<unsigned short> m_indexData;

public:
	Mesh(unsigned int numElemsPerVert = 3);
	~Mesh();

	Mesh(const Mesh&) = delete; //disallow copy constructing
	Mesh& operator=(const Mesh&) = delete; //disallow copy assignment

	void Bind() const;
	void Unbind() const;

	void Setup() const;
	bool LoadObj(const std::string& objPath);
	void Upload() const;

	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount);
	void AddVertexData(unsigned int count, ...);
	void AddIndexData(unsigned int count, ...);

	inline size_t VertexCount() const { return m_vertexData.size(); }
	inline size_t IndexCount() const { return m_indexData.size(); }

	inline int GetPrimitiveType() const { return m_primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->m_primitiveType = primitiveType; }
};
