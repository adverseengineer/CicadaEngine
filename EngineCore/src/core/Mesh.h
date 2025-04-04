#pragma once

#include "Texture2D.h"
#include <cstdarg>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>

namespace Cicada {

struct VertexAttribute {
	unsigned int m_index;
	unsigned int m_numComponents;
	int m_type;
	int m_isNormalized;
	unsigned int m_bytesToNext;
	void* m_byteOffset;
};

class Mesh : public ManagedObject<Mesh> {
friend class ManagedObject<Mesh>;
private:
	unsigned int m_vaoId = 0;
	unsigned int m_vboId = 0;
	unsigned int m_eboId = 0;

	unsigned int m_numElemsPerVert = 12;
	int m_primitiveType;
	std::vector<float> m_vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;

	std::vector<unsigned short> m_indexData;

	Mesh(std::string_view name, std::string_view path);

public:
	~Mesh();

	Mesh(const Mesh&) = delete; //disallow copy constructing
	Mesh& operator=(const Mesh&) = delete; //disallow copy assignment

	void Bind() const;
	static void Unbind();

	bool LoadObj(std::string_view objPath);
	void Upload() const;

	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount);
	void AddVertexData(unsigned int count, ...);
	void AddIndexData(unsigned int count, ...);

	inline size_t VertexElemCount() const { return m_vertexData.size(); }
	inline size_t VertexCount() const { return m_vertexData.size() / m_numElemsPerVert; }
	inline size_t IndexElemCount() const { return m_indexData.size(); }
	inline size_t FaceCount() const { return m_indexData.size() / 3; }

	inline int GetPrimitiveType() const { return m_primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->m_primitiveType = primitiveType; }
};

}
