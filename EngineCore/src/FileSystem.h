#pragma once
#include "Logger.h"
#include <fstream>

namespace Cicada {

class FileSystem {
public:
	//TODO: consider making return an optional
	inline static bool ReadFileToString(std::string_view path, std::string& content) {
		std::ifstream file(path.data());
		if (!file.is_open()) {
			Logger::Writef(LogEntry::Level::Error, "Could not open file: {:?}", path);
			return false;
		}
		Logger::Writef(LogEntry::Level::Info, "Successfully read file {:?}", path);
		content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		return true;
	}

	//TODO: implement this once its needed
	inline static bool WriteStringToFile(std::string_view path, std::string& content) {
		throw std::runtime_error("not implemented");
	}
};

}