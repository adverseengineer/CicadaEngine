#pragma once
#include "texture.h"
#include <glad/glad.h> 
#include <string>
#include <unordered_map>
#include <vector>

struct VertexAttribute {
	unsigned int index;
	unsigned int numberOfComponents;
	int type;
	int isNormalized;
	unsigned int bytesToNext;
	void* byteOffset;
};

class VertexBuffer {
protected:
	unsigned int numberOfElementsPerVertex;
	unsigned int vboId;
	int primitiveType;
	std::vector<float> vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;

public:
	VertexBuffer(unsigned int numElementsPerVertex = 3);
	~VertexBuffer();

	inline std::size_t GetNumberOfVertices() const { return vertexData.size(); }

	inline int GetPrimitiveType() const { return primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->primitiveType = primitiveType; }
	
	void Select() const;
	void Deselect() const;
	void SelectTexture() const;
	void AddVertexData(unsigned int count, ...);
	void StaticAllocate() const;
	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount = 0);
	void SetUpAttributeInterpretration() const;
};
