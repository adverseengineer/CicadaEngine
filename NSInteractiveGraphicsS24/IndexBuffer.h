#pragma once
#include <glad/glad.h> 
#include <string>
#include <vector>

class IndexBuffer {
protected:
	unsigned int m_iboId;
	std::vector<unsigned short> m_indexData;

public:
	IndexBuffer();
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	inline size_t GetCount() const { return m_indexData.size(); }
	void Bind() const;
	void Unbind() const;
	void StaticAllocate() const;
	void AddIndexData(unsigned int count, ...);
};
