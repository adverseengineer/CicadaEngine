#pragma once
#include <glad/glad.h> 
#include <string>
#include <vector>

class IndexBuffer {
protected:
	unsigned int iboId;
	std::vector<unsigned short> indexData;

public:
	IndexBuffer();
	~IndexBuffer();

	inline std::size_t GetCount() const { return indexData.size(); }
	void Select() const;
	void Deselect() const;
	void StaticAllocate() const;
	void AddIndexData(unsigned int count, ...);
};
