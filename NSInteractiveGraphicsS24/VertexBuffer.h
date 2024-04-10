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
	int textureUnit;
	std::shared_ptr<Texture> texture;

public:
	VertexBuffer(unsigned int numElementsPerVertex = 3);
	~VertexBuffer();

	inline unsigned int GetNumberOfVertices() const { return vertexData.size(); }

	inline int GetPrimitiveType() const { return primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->primitiveType = primitiveType; }
	
	inline int GetTextureUnit() const { return textureUnit; }
	inline void SetTextureUnit(int textureUnit) { this->textureUnit = textureUnit; }
	
	inline const std::shared_ptr<Texture>& GetTexture() const { return texture; }
	inline void SetTexture(const std::shared_ptr<Texture>& texture) { this->texture = texture; }
	inline bool HasTexture() const { return texture != nullptr; }

	void Select() const;
	void Deselect() const;
	void SelectTexture() const;
	void AddVertexData(unsigned int count, ...);
	void StaticAllocate() const;
	void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount = 0);
	void SetUpAttributeInterpretration() const;
};
