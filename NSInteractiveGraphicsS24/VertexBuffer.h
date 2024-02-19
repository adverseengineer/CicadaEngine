#pragma once
#include <glad/glad.h> 
#include <vector>
#include <unordered_map>
#include <string>

#include "texture.h"

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
	unsigned int numberOfVertices;
	unsigned int vboId;
	int primitiveType;
	std::vector<float> vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;
	int textureUnit;
	std::shared_ptr<Texture> texturePtr;

public:
	VertexBuffer(unsigned int numElementsPerVertex = 3);
	~VertexBuffer();

	inline void Select() { glBindBuffer(GL_ARRAY_BUFFER, vboId); }
	inline void Deselect() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	inline unsigned int GetNumberOfVertices() const { return numberOfVertices; }
	inline int GetPrimitiveType() const { return primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->primitiveType = primitiveType; }
	inline int GetTextureUnit(void) const { return textureUnit; }
	inline void SetTextureUnit(int textureUnit) { this->textureUnit = textureUnit; }
	inline std::shared_ptr<Texture> GetTexturePtr(void) const { return texturePtr; }
	inline void SetTexturePtr(std::shared_ptr<Texture> texturePtr) { this->texturePtr = texturePtr; }
	inline bool HasTexture(void) const { return texturePtr != nullptr; }
	
	void SelectTexture(void) const;
	void AddVertexData(unsigned int count, ...);
	void StaticAllocate();
	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount = 0);
	void SetUpAttributeInterpretration();
};
