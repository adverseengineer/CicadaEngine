
#include <sstream>
#include <fstream>

#include "TextFile.h"
#include "util.h"

bool TextFile::ReadAllLines(const std::string& path) {
	std::stringstream ss;
	std::ifstream fin{};
	fin.open(path.c_str());
	if (fin.fail()) {
		Util::Log("Failed to open " + path);
		return false;
	}
	std::string line;
	size_t numLines;
	for (numLines = 0; !fin.eof(); numLines++) {
		getline(fin, line);
		Util::Trim(line);
		if (line != "")
			ss << line << std::endl;
	}
	Util::Log("Read " + std::to_string(numLines) + " lines from " + path);
	fin.close();
	contents = ss.str();
}
