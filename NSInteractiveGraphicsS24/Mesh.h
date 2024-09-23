#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct Mesh {

	VertexBuffer m_vBuf;
	IndexBuffer m_iBuf;

	Mesh(const VertexBuffer& vBuf) : m_vBuf(vBuf), m_ibuf() {
	}

	Mesh(const VertexBuffer& vBuf, const IndexBuffer& iBuf) : m_vBuf(vBuf), m_iBuf(iBuf) {
	}

	inline bool IsIndexed() const { return m_iBuf.Count() != 0; }
};
