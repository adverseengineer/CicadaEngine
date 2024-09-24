#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct Mesh {

	VertexBuffer m_vBuf;
	IndexBuffer m_iBuf;

	Mesh() : m_vBuf(), m_iBuf() {
	}

	inline Mesh(unsigned int numElemsPerVertex = 3) : m_vBuf(numElemsPerVertex), m_iBuf() {
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	inline VertexBuffer& GetVertexBuffer() { return m_vBuf; }
	inline IndexBuffer& GetIndexBuffer() { return m_iBuf; }

	inline bool HasIndices() const { return m_iBuf.GetCount() != 0; }
};
