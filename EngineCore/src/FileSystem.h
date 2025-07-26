#pragma once
#include "Log.h"
#include <fstream>

namespace Cicada {

class FileSystem {
public:

	inline static std::string ReadEntireFile(std::string_view path) {
		std::ifstream file(path.data());
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	//TODO: consider making return an optional
	inline static bool ReadFileToString(std::string_view path, std::string& content) {
		std::ifstream file(path.data());
		if (!file.is_open()) {
			LOG_ERROR("Could not open file: {:?}", path);
			return false;
		}
		LOG_INFO("Successfully read file {:?}", path);
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