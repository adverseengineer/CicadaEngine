#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct Mesh {

	VertexBuffer m_vBuf;
	IndexBuffer m_iBuf;

	unsigned int m_vaoId = 0;

	inline Mesh(unsigned int numElemsPerVertex = 3) : m_vBuf(numElemsPerVertex), m_iBuf() {
		glGenVertexArrays(1, &m_vaoId);
	}

	inline ~Mesh() {
		glDeleteVertexArrays(1, &m_vaoId);
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	inline void Bind() { glBindVertexArray(m_vaoId); }
	inline void Unbind() { glBindVertexArray(0); }

	inline VertexBuffer& GetVertexBuffer() { return m_vBuf; }
	inline IndexBuffer& GetIndexBuffer() { return m_iBuf; }

	inline bool HasIndices() const { return m_iBuf.GetCount() != 0; }
};
