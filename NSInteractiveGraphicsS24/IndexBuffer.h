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

	inline std::size_t GetCount() const { return m_indexData.size(); }
	void Select() const;
	void Deselect() const;
	void StaticAllocate() const;
	void AddIndexData(unsigned int count, ...);
};
