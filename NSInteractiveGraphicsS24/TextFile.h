#pragma once

#include <string>

#include "BaseObject.h"

class TextFile : BaseObject
{
private:
	std::string contents;

public:
	TextFile(void) = default;
	~TextFile(void) = default;

	inline const std::string& GetContents(void) const { return contents; }
	bool ReadAllLines(const std::string& path);
};
