#pragma once
#include "Util.h"
#include <string>

class TextFile {
private:
	std::string contents;

public:
	TextFile() = default;
	~TextFile() = default;

	inline const std::string& GetContents() const { return contents; }
	bool ReadAllLines(const std::string& path);
};
