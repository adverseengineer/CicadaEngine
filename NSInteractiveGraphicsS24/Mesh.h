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
	inline Mesh(unsigned int numElemsPerVert = 3) : m_numElemsPerVert(numElemsPerVert) {
		m_primitiveType = GL_TRIANGLES;
		glGenVertexArrays(1, &m_vaoId);
		glGenBuffers(1, &m_vboId);
		glGenBuffers(1, &m_iboId);
	}

	inline ~Mesh() {
		glDeleteVertexArrays(1, &m_vaoId);
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	inline void BindVAO() const { glBindVertexArray(m_vaoId); }
	inline void UnbindVAO() const { glBindVertexArray(0); }

	inline void BindVBO() const { glBindBuffer(GL_ARRAY_BUFFER, m_vboId); }
	inline void UnbindVBO() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	void BindIBO() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId); }
	void UnbindIBO() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	inline void UploadVBO() const {
		BindVBO();
		unsigned long long bytesToAllocate = m_vertexData.size() * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) bytesToAllocate, (void*) m_vertexData.data(), GL_STATIC_DRAW);
	}

	inline void UploadIBO() const {
		BindIBO();
		unsigned long long bytesToAllocate = m_indexData.size() * sizeof(unsigned short);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) bytesToAllocate, (void*) m_indexData.data(), GL_STATIC_DRAW);
	}

	inline void AddVertexData(unsigned int count, ...) {
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

	inline void AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount) {
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

	inline void SetUpAttributeInterpretration() const {
		for (const auto& item : attributeMap) {
			const VertexAttribute& attr = item.second;
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

	inline size_t GetNumberOfVertices() const { return m_vertexData.size(); }

	inline int GetPrimitiveType() const { return m_primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->m_primitiveType = primitiveType; }

	inline void AddIndexData(unsigned int count, ...) {

		va_list args;
		va_start(args, count);
		while (count-- > 0) //the default is double, so accept as double and then cast to float
			m_indexData.push_back(static_cast<unsigned short>(va_arg(args, int)));

		va_end(args);
	}

	inline size_t IndexCount() const { return m_indexData.size(); }
};
