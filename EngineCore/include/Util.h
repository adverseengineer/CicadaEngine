#pragma once

#include "Logger.h"
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace Cicada {

class Util {
public:
	inline static void TrimWhitespace(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}

	//TODO: move to a more fitting place
	//TODO: consider making return an optional
	inline static bool ReadFileToString(const char* path, std::string& content) {
		std::ifstream file(path);
		if (!file.is_open()) {
			Logger::Writef(LogEntry::Level::Error, "Could not open file: {:?}", path);
			return false;
		}
		Logger::Writef(LogEntry::Level::Error, "Successfully read file {:?}", path);
		content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		return true;
	}

	//linear interpolation from one numeric range to another, this takes any numeric type as input
	template <typename T>
	inline static constexpr T NumericMap(T input, T inputMin, T inputMax, T outputMin, T outputMax) {
		static_assert(std::is_arithmetic<T>::value, "type must be numeric");
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (input - inputMin);
	}
};

}
